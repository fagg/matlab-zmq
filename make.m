% make.m
%
% Builds core mex API.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% ZMQ CONFIGURATION:

if ispc
% == WINDOWS ==

  % see http://zeromq.org/distro:microsoft-windows for a description of the
  % binaries and SO version compatibility

  % The filename for the compiled lib (without the extension)
  ZMQ_COMPILED_LIB = 'libzmq-v120-mt-4_0_4';

  % Where is the compiled lib placed?
  ZMQ_LIB_PATH = 'C:\Program Files\ZeroMQ 4.0.4\lib\';

  % Where can we find the headers?
  ZMQ_INCLUDE_PATH = 'C:\Program Files\ZeroMQ 4.0.4\include\';
else
% == POSIX ==

  % The filename for the compiled lib (without the extension and the 'lib'
  % prefix)
  ZMQ_COMPILED_LIB = 'zmq';

  % Where is the compiled lib placed?
  ZMQ_LIB_PATH = '/usr/local/Cellar/zeromq/4.0.4/lib/'; % maybe /usr/local/lib/ ?

  % Where can we find the headers?
  ZMQ_INCLUDE_PATH = '/usr/local/Cellar/zeromq/4.0.4/include/'; % maybe /usr/local/include/ ?
end


%% SCRIPT VARS

% --> Windows whitespace normalization :(
if ispc
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
  reduce_path = @(orig_path) system(['for %A in ("', orig_path ,'") do @echo %~sA']);
  [status, ZMQ_INCLUDE_PATH] = reduce_path(ZMQ_INCLUDE_PATH);
  if status; error('mex:compile', 'Unable to recognize path'); end
  ZMQ_INCLUDE_PATH = strtrim(ZMQ_INCLUDE_PATH);
  [status, ZMQ_LIB_PATH] = reduce_path(ZMQ_LIB_PATH);
  if status; error('mex:compile', 'Unable to recognize path'); end
  ZMQ_LIB_PATH = strtrim(ZMQ_LIB_PATH);
end
% <--

ZMQ_COMPILE_FLAGS = sprintf('-I"%s" -L"%s" -l"%s"', ZMQ_INCLUDE_PATH, ZMQ_LIB_PATH, ZMQ_COMPILED_LIB);

[MAKE_DIR, ~, ~] = fileparts(mfilename('fullpath')); % Get the directory of this file (absolute path)
LIB_PATH = fullfile(MAKE_DIR, 'lib');
SRC_PATH = fullfile(MAKE_DIR, 'src', 'core');

%% CORE API FILE LIST
CORE_FILE_LIST = { ...
                fullfile(SRC_PATH, 'zmq_version.c'), ...
                fullfile(SRC_PATH, 'zmq_ctx_new.c'), ...
                fullfile(SRC_PATH, 'zmq_ctx_term.c'), ...
                fullfile(SRC_PATH, 'zmq_ctx_shutdown.c'), ...
                fullfile(SRC_PATH, 'zmq_ctx_get.c'), ...
                fullfile(SRC_PATH, 'zmq_ctx_set.c'), ...
                fullfile(SRC_PATH, 'zmq_socket.c') ...
                };


%% BUILD
% TODO: scape properly the LIB_PATH to avoid whitespace issues.
% Inexplicably just using quotes (`sprintf(' "%s" -outdir "%s"', ...`) does
% not work on Windows, even when there are not whitespaces.

if ispc
  [status, LIB_PATH] = reduce_path(LIB_PATH);
  if status; error('mex:compile', 'Unable to recognize path'); end
  LIB_PATH = strtrim(LIB_PATH);
end

BUILD_FUNCTION = @(file) eval(['mex -largeArrayDims -O ', ZMQ_COMPILE_FLAGS, sprintf(' "%s" -outdir %s', char(file), LIB_PATH)]);
cellfun(BUILD_FUNCTION, CORE_FILE_LIST);