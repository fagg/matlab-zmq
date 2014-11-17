function close(obj, endpoint)
    status = zmq.core.close(obj.socketPointer);
end