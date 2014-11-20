function [buffLen, options] = normalize_msg_options(obj, varargin)
    buffLen = obj.defaultBufferLength;
    options = cell(0);
    if (nargin > 1)
        if (isnumeric(varargin{1}))
            options = cell(1, nargin-2);
            buffLen = varargin{1};
            offset = 1;
        else
            options = cell(1, nargin-1);
            offset = 0;
        end

        for n = 1:nargin-offset-1
            options{n} = obj.normalize_const_name(varargin{n+offset});
        end
    end
end