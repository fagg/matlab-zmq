function success = runner(varargin)
    % Run tests for the library
    %
    % ## Arguments
    %   - [...]: variable list of tests.
    %
    % If no argument is provided, all the files `test*.m` under the directory
    % of this file will run.
    %
    % Notice that the files will be considered relative to this file.

    [testPath, ~, ~] = fileparts(mfilename('fullpath'));

    % save current path
    origPath = path;
    addpath(testPath);
    addpath(fullfile(testPath, 'support'));
    cleanupObj = onCleanup(@() path(origPath)); % restore path after finish

    failures = {};
    nfailures = 0;
    nsuccess  = 0;

    % Get the test files
    if (nargin > 0)
        tests = varargin;
    else
        try
            tests = cellstr(ls(fullfile(testPath, 'test*.m')));
        catch
            tests = {};
        end
    end

    fprintf('Running tests:\n\n');
    tic;
    for n = 1:length(tests)
        % Failed tests should throw an exception
        try
            [~, funcname, ~] = fileparts(tests{n});
            func = str2func(funcname);
            func();
            fprintf('.');
            nsuccess = nsuccess + 1;
        catch e
            fprintf('F');
            % Check if it is octave (`try...catch` not 100% > needs workaround)
            if (exist('OCTAVE_VERSION', 'builtin'))
                e = lasterror;
            end
            % Save the failure, in order to posterior display
            nfailures = nfailures + 1;
            failures{nfailures} = e;
        end
    end

    fprintf('\n\n');
    toc;
    fprintf('\n\n');

    if nfailures > 0
        success = 0;
        % Throw all failures as one
        error('Tests: %d passed, %d failed.\n\n%s\n', nsuccess, nfailures, ...
            strjoin(cellfun(@reportgen, failures, 'UniformOutput', false), '\n'));
        % An error will ensure the exit code different from 0 in case of any not-well-succeeded test.
        % This is useful for continuous-integration services.
    else
        success = 1;
        fprintf('All tests passed.\n');
    end
end

function output = reportgen(err)
    stack = find_test(err.stack);
    output = sprintf('%s (line %d):\n\t%s\n', stack.name, stack.line, err.message);
end

function output = find_test(stack)
    n = 1;
    l = length(stack);

    while (isempty(regexp(stack(n).name, '^test', 'once')) && n < l)
        n = n + 1;
    end

    output = stack(n);
end