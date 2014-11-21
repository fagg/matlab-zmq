function message = recv_multipart(obj, varargin)
    message = char(obj.recv_multipart(varargin{:}));
end