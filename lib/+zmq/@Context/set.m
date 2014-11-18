function set(obj, name, value)
    optName = obj.normalize_const_name(name);
    option = zmq.core.ctx_set(obj.contextPointer, optName, value);
end
