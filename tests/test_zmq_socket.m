function test_zmq_socket
    ctx = setup;
    cleanupObj = onCleanup(@() teardown(ctx));
    
    % let's just create and destroy a dummy socket
    socket = assert_does_not_throw(@zmq.core.socket, ctx, 'ZMQ_REP');
    response = assert_does_not_throw(@zmq.core.close, socket);
    assert(response == 0, 'status code should be 0, %d given.', response);
end

function ctx = setup
    ctx = zmq.core.ctx_new();
end

function teardown(ctx)
    % close session
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end
