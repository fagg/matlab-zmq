function close(obj, endpoint)
    status = zmq.core.close(obj.socketPointer);
    if (status == 0)
        obj.socketPointer = 0; % ensure NULL pointer
    end
end