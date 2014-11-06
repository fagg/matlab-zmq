function varargout = assert_does_not_throw(func, varargin)
    % Call function, passing the remaining arguments to it, and
    % check if an error does not occur
    %
    % ## Usage
    % ```matlab
    % assert_does_not_throw(idregex, @func, arg1, arg2, ...);
    %       % Check if an error (whose identifier matches `idregex`)
    %       % does not occur when calling `func(arg1, arg2, ...)`
    %
    % assert_does_not_throw(@func, arg1, arg2, ...);
    % assert_does_not_throw('*', @func, arg1, arg2, ...);
    %       % Check if no error occurs when calling `func(arg1, arg2, ...)`
    % ```
    %
    % ## Return
    %  The same as func(arg1, arg2, ...)

    if isa(func, 'function_handle')
        idregex = '.*';
        args = varargin;
    else
        idregex = func;
        func = varargin{1};
        args = varargin(2:end);
    end

    if strcmp(idregex, '*')
        idregex = '.*';
    end

    try
        if ~nargout
            func(args{:});
        else
            [varargout{1:nargout}] = func(args{:});
        end
    catch e
        % Check if it is octave (`try...catch` not 100% > needs workaround)
        if exist('OCTAVE_VERSION', 'builtin')
            e = lasterror;
        end

        match = regexp(e.identifier, idregex);
        if (~isempty(match))
            error('assert:doesNotThrow', 'should not throw (%s) "%s".', e.identifier, e.message);
        else
            error(e);
        end
    end
end