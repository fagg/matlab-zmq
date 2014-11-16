function test_zmq_bind
    [ctx, socket] = setup;
    cleanupObj = onCleanup(@() teardown(ctx, socket));

    %% binding
    assert_throw('EPROTONOSUPPORT', @zmq_bind, socket, 'abc://localhost'); % invalid transport - TODO sometimes it throws "zmq:bind:unknownOops" ("No such file or directory").
    assert_throw('EINVAL', @zmq_bind, socket, 'tcp://localhost');          % port must specified
    response = assert_does_not_throw(@zmq_bind, socket, 'tcp://127.0.0.1:30000');
    assert(response == 0, 'status code should be 0, %d given.', response);

    %% unbinding
    assert_throw(@zmq_unbind, socket, 'tcp://127.0.0.1:30103');
    response = assert_does_not_throw(@zmq_unbind, socket, 'tcp://127.0.0.1:30000');
    assert(response == 0, 'status code should be 0, %d given.', response);
end

function [ctx, socket] = setup
    % let's just create and destroy a dummy socket
    ctx = zmq_ctx_new();
    socket = zmq_socket(ctx, 'ZMQ_PUB');
end

function teardown(ctx, socket)
    % close session
    zmq_close(socket);
    zmq_ctx_shutdown(ctx);
    zmq_ctx_term(ctx);
end