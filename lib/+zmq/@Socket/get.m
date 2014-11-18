function option = get(obj, name)
    optName = obj.normalize_const_name(name);
    option = zmq.core.getsockopt(obj.socketPointer, optName);
end