function term(obj)
    zmq.core.ctx_term(obj.contextPointer);
    obj.contextPointer = 0;  % ensure NULL pointer
end
