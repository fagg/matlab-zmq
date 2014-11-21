function send_string(obj, message, varargin)
    obj.send_multipart(uint8(message), varargin{:});
end