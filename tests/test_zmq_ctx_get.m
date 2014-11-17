function test_zmq_ctx_get
    ctx = setup;
    cleanupObj = onCleanup(@() teardown(ctx));

    % Once the context is fresh and unused, all the options should remain with the
    % default values.

    % ZMQ_IO_THREADS Default Value: 1
    nthreads = zmq.core.ctx_get(ctx, 'ZMQ_IO_THREADS');
    assert(nthreads == 1, 'ZMQ_IO_THREADS should be 1, %d given.', nthreads);

    % ZMQ_IPV6 Default Value: 0
    ipv6 = zmq.core.ctx_get(ctx, 'ZMQ_IPV6');
    assert(ipv6 == 0, 'ZMQ_IPV6 should be 0, %d given.', ipv6);

    % ZMQ_MAX_SOCKETS Default Value: 1024
    maxs = zmq.core.ctx_get(ctx, 'ZMQ_MAX_SOCKETS');
    assert(maxs == 1024 || maxs == 1023, ...
        'ZMQ_MAX_SOCKETS should be 1024, %d given (I suspect receiving 1023 is OK).', maxs);
    % NOTICE:
    % According to documentation (http://api.zeromq.org/4-0:zmq-ctx-set)
    % it should be 1024, but `zmq.h` defines it as 1023...
end

function ctx = setup
    % let's just create a dummy context
    ctx = zmq.core.ctx_new();
end

function teardown(ctx)
    % close session
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end
