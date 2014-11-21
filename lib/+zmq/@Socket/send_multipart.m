function send_multipart(obj, message, varargin)
    [buffLen, options] = obj.normalize_msg_options(varargin{:});

    offset = 1;

    L = length(message);  % length of original message
    N = floor(L/buffLen); % number of multipart messages

    for m = 1:N
        part = message(offset:(offset+buffLen-1));
        offset = offset+buffLen;
        obj.send(part, 'sndmore');
    end

    part = message(offset:end);
    obj.send(part);
end