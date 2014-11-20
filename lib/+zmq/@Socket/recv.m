function varargout = recv(obj, varargin)
    [buffLen, options] = obj.normalize_msg_options(varargin{:});
    [varargout{1:nargout}] = zmq.core.recv(obj.socketPointer, buffLen, options{:});
end