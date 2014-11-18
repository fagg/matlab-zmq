function option = get(obj, name)
    optName = normalize_const_name(name);
    option = zmq.core.ctx_get(obj.contextPointer, optionStr);
end
