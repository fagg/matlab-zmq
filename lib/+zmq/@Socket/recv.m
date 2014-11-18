function varargout = recv(obj, varargin)
    options = obj.normalize_msg_options(varargin{:});
    [varargout{1:nargout}] = zmq.core.recv(obj.socketPointer, options{:});
end