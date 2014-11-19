function bind(obj, endpoint)
    status = zmq.core.bind(obj.socketPointer, endpoint);
    if (status == 0)
        % Add endpoint to the tracked bindings
        % this is important to the cleanup process
        obj.bindings{end+1} = endpoint;
    end
end