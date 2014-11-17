function test_zmq_bind
    % let's just create and destroy a dummy socket
    ctx = zmq.core.ctx_new();
    socket = zmq.core.socket(ctx, 'ZMQ_PUB');

    %% binding
    assert_throw('EPROTONOSUPPORT', @zmq.core.bind, socket, 'abc://localhost'); % invalid transport
    assert_throw('EINVAL', @zmq.core.bind, socket, 'tcp://localhost');          % port must specified
    response = assert_does_not_throw(@zmq.core.bind, socket, 'tcp://127.0.0.1:30000');
    assert(response == 0, 'status code should be 0, %d given.', response);

    %% unbinding
    assert_throw(@zmq.core.unbind, socket, 'tcp://127.0.0.1:30103');
    response = assert_does_not_throw(@zmq.core.unbind, socket, 'tcp://127.0.0.1:30000');
    assert(response == 0, 'status code should be 0, %d given.', response);

    % close session
    zmq.core.close(socket);
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end
