function test_zmq_bind
    % let's just create and destroy a dummy socket
    ctx = zmq_ctx_new();
    socket = zmq_socket(ctx, 'ZMQ_REP');

    assert_throw('zmq:bind:EPROTONOSUPPORT', @zmq_bind, socket, 'abc://localhost'); % invalid transport
    assert_throw('zmq:bind:EINVAL', @zmq_bind, socket, 'tcp://localhost');          % port must specified
    response = assert_does_not_throw(@zmq_bind, socket, 'tcp://127.0.0.1:30000');

    assert(response == 0, 'status code should be 0, %d given.', response);

    % close session
    zmq_close(socket);
    zmq_ctx_shutdown(ctx);
    zmq_ctx_term(ctx);
end