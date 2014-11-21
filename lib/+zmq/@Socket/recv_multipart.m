function message = recv_multipart(obj, varargin)
    [buffLen, options] = obj.normalize_msg_options(varargin{:});

    message = [];

    keepReceiving = 1;

    while keepReceiving > 0
        part = obj.recv(buffLen, options{:});
        message = [message part];
        keepReceiving = obj.get('rcvmore');
    end
end