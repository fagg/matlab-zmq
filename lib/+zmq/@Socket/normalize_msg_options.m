function options = normalize_msg_options(obj, varargin)
    options = cell(1, nargin-1);
    if (nargin > 1)
        n0 = 1;
        if (isnumeric(varargin{1}))
            options{1} = varargin{1};
            n0 = 2;
        end

        for n = n0:nargin-1
            options{n} = obj.normalize_const_name(varargin{n});
        end
    end
end