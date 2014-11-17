function disconnect(obj, endpoint)
    status = zmq.core.disconnect(obj.socketPointer, endpoint);
    if (status == 0)
        % Remove endpoint from the tracked connections
        % to avoid double cleaning
        index = find(strcmp(obj.connections, endpoint));
        obj.connections(index) = [];
    end
end