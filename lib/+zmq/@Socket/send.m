function nbytes = send(obj, data, varargin)
    [~, options] = obj.normalize_msg_options(varargin{:});
    nbytes = zmq.core.send(obj.socketPointer, data, options{:});
end