function test_zmq_socket
    ctx = zmq.core.ctx_new();

    % let's just create and destroy a dummy socket
    socket = assert_does_not_throw(@zmq.core.socket, ctx, 'ZMQ_REP');
    response = assert_does_not_throw(@zmq.core.close, socket);
    assert(response == 0, 'status code should be 0, %d given.', response);

    % close session
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end
