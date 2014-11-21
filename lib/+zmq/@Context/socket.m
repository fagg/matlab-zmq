function newSocket = socket(obj, socketType)
    % Spawns a socket from the context

    newSocket = zmq.Socket(obj.contextPointer, socketType);

    % Keep tracking of spawned sockets
    % this is important to the cleanup process
    obj.spawnedSockets{end+1} = newSocket;
end
