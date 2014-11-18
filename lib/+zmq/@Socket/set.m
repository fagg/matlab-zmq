function set(obj, name, value)
    optName = obj.normalize_const_name(name);
    option = zmq.core.setsockopt(obj.socketPointer, optName, value);
end