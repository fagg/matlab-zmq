function option = get(obj, name)
    % Normalize 'name' parameter:
    optName = strrep(upper(name), 'ZMQ_', '');
    optName = strcat('ZMQ_', optName);

    option = zmq.core.getsockopt(obj.socketPointer, optName);
end