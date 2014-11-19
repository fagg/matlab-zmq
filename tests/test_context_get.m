function test_context_get
    context = zmq.Context();
    
    % These are what we expect the values to be
    % nThreads = 1;  ipv6Status = 0; maxSockets = 1023;
  
    ipv6 = context.get('ZMQ_IPV6');
    assert(ipv6 == 0, 'ZMQ_IPV6 should be 0, %d given', ipv6);

    nThreads = context.get('ZMQ_IO_THREADS');
    assert(nThreads == 1, 'ZMQ_IO_THREADS should 1, %d given', nThreads);

    maxSocks = context.get('ZMQ_MAX_SOCKETS');
    assert(maxSocks==1023 || maxSocks == 1024, ...
           'ZMQ_MAX_SOCKETS should be 1024, %d given', maxSocks);
    clear('context');
end
