function set(obj, name, value)
    optName = obj.normalize_const_name(name);
    zmq.core.ctx_set(obj.contextPointer, optName, value);
end
