function success = runner(varargin)
  [test_path, ~, ~] = fileparts(mfilename('fullpath'));

  % save current path
  original_path = path;
  addpath(test_path);

  failures = {};
  nfailures = 0;

  if nargin > 0
    tests = varargin;
  else
    try
      tests = cellstr(dir(fullfile(test_path, 'test*.m')));
    catch
      tests = {};
    end
  end

  fprintf('Running tests:\n\n');
  for n = 1:length(tests)
    try
      [~, funcname, ~] = fileparts(tests{n});
      func = str2func(funcname);
      func();
      fprintf('.');
    catch e
      fprintf('F');
      % Check if it is octave (`try...catch` not 100%)
      if exist('OCTAVE_VERSION', 'builtin'); e = lasterror; end
      nfailures = nfailures + 1;
      failures{nfailures} = e;
    end
  end

  fprintf('\n');

  if nfailures > 0
    success = 0;
    reportgen  = @(f) sprintf('%s (line %d):\n\t%s\n', f.stack(1).name, f.stack(1).line, f.message);
    error('\nFailures:\n\n%s\n', strjoin(cellfun(reportgen, failures, 'UniformOutput', false), '\n'));
  else
    success = 1;
    fprintf('\nAll tests passed.\n');
  end

  % restore path
  path(original_path);
end