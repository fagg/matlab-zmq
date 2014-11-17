function set(obj, name, value)
    % Normalize 'name' parameter:
    optName = strrep(upper(name), 'ZMQ_', '');
    optName = strcat('ZMQ_', optName);

    option = zmq.core.setsockopt(obj.socketPointer, optName, value);
end