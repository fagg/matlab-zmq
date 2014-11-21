function connect(obj, endpoint)
    status = zmq.core.connect(obj.socketPointer, endpoint);
    if (status == 0)
        % Add endpoint to the tracked connections
        % this is important to the cleanup process
        obj.connections{end+1} = endpoint;
    end
end