function unbind(obj, endpoint)
    status = zmq.core.unbind(obj.socketPointer, endpoint);
    if (status == 0)
        % Remove endpoint from the tracked bindings
        % to avoid double cleaning
        index = find(strcmp(obj.bindings, endpoint));
        obj.bindings(index) = [];
    end
end