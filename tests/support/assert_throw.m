function varargout = assert_throw(func, varargin)
    % Call function, passing the remaining arguments to it, and
    % check if a error occurs
    %
    % ## Usage
    % ```matlab
    % assert_throw(idregex, @func, arg1, arg2, ...);
    %       % Check if an error (whose identifier matches `idregex`)
    %       % occurs when calling `func(arg1, arg2, ...)`
    %
    % assert_throw('*', @func, arg1, arg2, ...);
    % assert_throw(@func, arg1, arg2, ...);
    %       % Check if any error occurs when calling `func(arg1, arg2, ...)`
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
        if (isempty(match))
            error('assert:throw', 'should throw "%s". Instead "%s" thrown ("%s").', idregex, ...
                e.identifier, e.message);
        else
            return
        end
    end

    error('assert:throw:nothingThrown', 'should throw "%s".', idregex);
end