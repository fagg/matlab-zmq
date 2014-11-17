function test_zmq_socket
    ctx = setup;
    cleanupObj = onCleanup(@() teardown(ctx));

    % let's just create and destroy a dummy socket
    socket = assert_does_not_throw(@zmq_socket, ctx, 'ZMQ_REP');
    response = assert_does_not_throw(@zmq_close, socket);
    assert(response == 0, 'status code should be 0, %d given.', response);
end

function ctx = setup
    ctx = zmq_ctx_new();
end

function teardown(ctx)
    % close session
    zmq_ctx_shutdown(ctx);
    zmq_ctx_term(ctx);
end