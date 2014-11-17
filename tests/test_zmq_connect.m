function test_zmq_connect
    [ctx, socket] = setup;
    cleanupObj = onCleanup(@() teardown(ctx, socket));

    %% connecting
    assert_throw('EPROTONOSUPPORT', @zmq_connect, socket, 'abc://localhost'); % invalid transport
    assert_throw('EINVAL', @zmq_connect, socket, 'tcp://localhost');          % port must specified
    response = assert_does_not_throw(@zmq_connect, socket, 'tcp://127.0.0.1:30000');
    assert(response == 0, 'status code should be 0, %d given.', response);

    %% disconnecting
    assert_throw(@zmq_disconnect, socket, 'tcp://127.0.0.1:30103');
    response = assert_does_not_throw(@zmq_disconnect, socket, 'tcp://127.0.0.1:30000');
    assert(response == 0, 'status code should be 0, %d given.', response);
end

function [ctx, socket] = setup
    % let's just create and destroy a dummy socket
    ctx = zmq_ctx_new();
    socket = zmq_socket(ctx, 'ZMQ_SUB');
end

function teardown(ctx, socket)
    % close session
    zmq_close(socket);
    zmq_ctx_shutdown(ctx);
    zmq_ctx_term(ctx);
end