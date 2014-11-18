function option = get(obj, name)
    optName = obj.normalize_const_name(name);
    option = zmq.core.ctx_get(obj.contextPointer, optName);
end
