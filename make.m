function success = make(varargin)
  % make.m
  %
  % Tools for building matlab API for ZMQ.
  %
  % ## USAGE:
  %
  % ```matlab
  % >> make % This will build all the core API
  % >> make zmq_version.c zmq_ctx_new.c % This will build only the listed files
  % >> make clean % Remove files produced by compilation process
  % ```
  %
  % ## NOTICE:
  %
  % Before runnig this, make sure to install ZMQ and edit 'config.m' file.
  %
  % Instructions for installing ZMQ: http://zeromq.org/intro:get-the-software.
  %
  % The files `config_win.m` and `config_unix.m` are examples of how to edit
  % `config.m` for different operating systems.
  %
  % The file `config.m` itself shows how to build `matlab-zmq` using a Homebrew
  % instalation of ZMQ 4.0.4 for OS-X.

  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  %% Rules
  if nargin > 0
    switch lower(varargin{1})
      case 'clean'
        success = clean(varargin{2:end});
      case 'test'
        success = run_tests(varargin{2:end});
      case {'build', 'compile'}
        success = build(varargin{2:end});
      otherwise
        success = build(varargin{:});
    end
  else
    success = build;
  end
end

%% Make rules

function success = clean(varargin)
  % Remove the files generated during compilation process.
  %
  % Arguments:
  %  - [...]: Variable list of paths to be deleted, relative to this file.
  %           No recursive glob patterns can be used.
  %
  % If no argument is provided, all '+zmq/+core/*.mex*', '*.o', '*.asv', '*.m~' files
  % will be remvoed.
  %
  % NOTICE: Without arguments, it will purge the created bindings.
  success = false;

  [make_path, ~, ~, ~] = get_paths();
  if nargin > 0
    rubbish = varargin;
  else
    rubbish = {'lib/+zmq/+core/*.mex*', '*.o', '*.asv', '*.m~'};
  end

  for n = 1:length(rubbish)
    pattern = fullfile(make_path, rubbish{n});
    try
      if ~isempty(dir(pattern))
        delete(pattern); 
      end
    end
  end

  success = true;
end

function success = run_tests(varargin)
  % Run tests for the library
  %
  % ## Arguments
  %   - [...]: variable list of tests.
  %
  % If no argument is provided, all the files `test*.m` under `tests` dir will
  % run.
  %
  % Notice that the files will be considered relative to `tests` directory.
  [make_path, ~, ~, test_path] = get_paths;

  % save current path
  original_path = path;
  addpath(test_path);
  % point at the core package. TODO: this could be tidier
  addpath(fullfile(make_path, 'lib'));
  cleanup = onCleanup(@() path(original_path)); % restore path after finish
  success = runner(varargin{:});
end

function success = build(varargin)
  % Build core ZMQ bindings
  %
  % ## Arguments
  %   - [...]: variable list of files to be compiled.
  %
  % If no argument is provided, the list will be loaded from `compile_list.m`
  % file.
  %
  % Notice that the files will be considered relative to `src/core` directory.
  %
  % Instead of a plain string, a cell array of strings can be also used as
  % argument. In this case, the first string is the _main_ file
  % (relative to `src/core` directory), while the others are _dependencies_
  % (relative to `src`).

  [make_path, lib_path, src_path, ~] = get_paths;

  %% ZMQ CONFIGURATION:
  % Try manual config first
  config;

  if (~testzmq(ZMQ_LIB_PATH) || ~testzmq(ZMQ_INCLUDE_PATH))
    % Default fallback
    if (ispc)
      % == WINDOWS ==
      config_win;
    else
      % == POSIX ==
      config_unix;
    end

    if (~testzmq(ZMQ_LIB_PATH) || ~testzmq(ZMQ_INCLUDE_PATH))
      error('make:matlab-zmq:badConfig', ...
        'Could not find ZMQ files, please edit ''config.m'' and try again.');
    end
  end

  %% SCRIPT VARS

  % --> Windows whitespace normalization :(
  orig_zmq_include_path = ZMQ_INCLUDE_PATH;
  orig_zmq_lib_path = ZMQ_LIB_PATH;
  ZMQ_INCLUDE_PATH = reducepath(ZMQ_INCLUDE_PATH);
  ZMQ_LIB_PATH = reducepath(ZMQ_LIB_PATH);
  % <--

  % --> '-l' option: libname normalization
  orig_zmq_lib = ZMQ_COMPILED_LIB;
  ZMQ_COMPILED_LIB = regexprep(orig_zmq_lib, '(^lib)|(\.\w+$)', '');
  % <--

  zmq_compile_flags = { ...
    ['-I' dquote(src_path)], ...
    ['-I' dquote(ZMQ_INCLUDE_PATH)], ...
    ['-L' dquote(ZMQ_LIB_PATH)], ...
    ['-l' dquote(ZMQ_COMPILED_LIB)] ...
  };

  if nargin > 0
    COMPILE_LIST = varargin;
  else
    compile_list;
  end

  lib_path = reducepath(lib_path); % Windows :(

  build_function = @(file) compile(zmq_compile_flags, file, fullfile(lib_path,'+zmq/+core'));
  cellfun(build_function, COMPILE_LIST);

  clean('*.o');

  files = ls(fullfile(fullfile(lib_path, '+zmq/+core'), '*.mex*'));
  if size(files, 1) == length(COMPILE_LIST)
    success = true;
    fprintf('\nSuccesful build for:\n');
  else
    success = false;
    fprintf('\nErrors during build for:\n');
  end
  fprintf(...
    '\tZMQ_INCLUDE_PATH = %s\n\tZMQ_LIB_PATH = %s\n\tZMQ_COMPILED_LIB = %s\n\n', ...
    orig_zmq_include_path, orig_zmq_lib_path, orig_zmq_lib);
end

%% Aux functions

function compile(flags, file, outputdir)
  % Compile files
  [~, ~, src_path, ~] = get_paths;

  deps = {};

  if iscell(file)
    if length(file) > 1
      deps = cellfun(@(dep) dquote(fullfile(src_path, dep)), file(2:end), 'UniformOutput', false);
    end
    file = file{1};
  end

  [~, name, ext] = fileparts(file);
  outputfile = fullfile(outputdir, name);
  file = dquote([fullfile(src_path, 'core', name), ext]);

  fprintf('compile %s\n', file);
  cellfun(@(dep) fprintf('\t- %s\n', dep), deps);

  if isoctave
    mex(flags{:}, deps{:}, file, '-o', dquote(outputfile));
  else
    % TODO: scape properly the `outputfile` to avoid whitespace issues.
    % Inexplicably just using quotes (`sprintf('"%s"', outputfile)` or
    % `['"' outputfile '"']` ) does not work on Windows, even when there are not
    % whitespaces.
    mex('-largeArrayDims', '-O', flags{:}, deps{:}, file, '-output', outputfile);
  end
end

function result = isoctave
  % Check if it is octave
  result = exist('OCTAVE_VERSION', 'builtin');
end

function quoted = dquote(file)
  % Double quote strings
  quoted = ['"' file '"'];
end

function red_path = reducepath(orig_path)
  % Unfortunatelly windows has severe issues with spaces in the path
  % strings, even when we scape them with quotes.
  %
  % Thanks to backward compatibility (dating back to MS-DOS times), it's
  % possible to convert the paths to an alternate ancient short form
  % (http://en.wikipedia.org/wiki/8.3_filename).
  %
  % The black magic necessary to do so was found in:
  % - http://stackoverflow.com/questions/1333589/how-do-i-transform-the-working-directory-into-a-8-3-short-file-name-using-batch
  % - http://stackoverflow.com/questions/10227144/convert-long-filename-to-short-filename-8-3-using-cmd-exe
  %
  % There is an alternate procedure, as documented in:
  % http://www.mathworks.com/matlabcentral/answers/93932-how-can-i-get-the-short-path-for-a-windows-long-path-using-matlab-7-8-r2009a
  %
  % TODO: consider the much nicer second alternative. Is it reliable in a
  % wide range of environments (diferent versions of Windows, even the
  % future ones)?

  if ispc
    [status, red_path] = system(['for %A in ("', orig_path ,'") do @echo %~sA']);
    if status; error('system:reducepath', 'Unable to recognize path'); end
    red_path = strtrim(red_path);
  else
    red_path = orig_path;
  end
end

function response = testzmq(folder)
  % Test if there are any zmq files inside folder

  try
    files = dir(fullfile(folder, '*zmq*'));
  catch
    files = [];
  end

  if ~isempty(files)
    response = 1;
  else
    response = 0;
  end
end

function [make_path, lib_path, src_path, test_path] = get_paths()
  % Return the paths used for this library

  [make_path, ~, ~] = fileparts(mfilename('fullpath'));
  lib_path = fullfile(make_path, 'lib/');
  src_path = fullfile(make_path, 'src');
  test_path = fullfile(make_path, 'tests');
end





