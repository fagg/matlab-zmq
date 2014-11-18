% Spawns a socket from the context

function newSocket = socket(obj, socketType)
    newSocket = zmq.Socket(obj.contextPointer, socketType);
end
