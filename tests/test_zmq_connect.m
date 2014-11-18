function test_zmq_connect
    [ctx, socket] = setup;
    cleanupObj = onCleanup(@() teardown(ctx, socket));
    
    %% connecting
    assert_throw('EPROTONOSUPPORT', @zmq.core.connect, socket, 'abc://localhost'); % invalid transport
    assert_throw('EINVAL', @zmq.core.connect, socket, 'tcp://localhost');          % port must specified
    response = assert_does_not_throw(@zmq.core.connect, socket, 'tcp://127.0.0.1:30000');
    assert(response == 0, 'status code should be 0, %d given.', response);

    %% disconnecting
    assert_throw(@zmq.core.disconnect, socket, 'tcp://127.0.0.1:30103');
    response = assert_does_not_throw(@zmq.core.disconnect, socket, 'tcp://127.0.0.1:30000');
    assert(response == 0, 'status code should be 0, %d given.', response);
end

function [ctx, socket] = setup
    % let's just create and destroy a dummy socket
    ctx = zmq.core.ctx_new();
    socket = zmq.core.socket(ctx, 'ZMQ_SUB');
end

function teardown(ctx, socket)
    % close session
    zmq.core.close(socket);
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end
