function test_socket_connect
    [ctx, socket] = setup;
    cleanupObj = onCleanup(@() cellfun(@(f) f(), ...
        {@() clear('socket'), @() teardown(ctx)}, ...
        'UniformOutput', false));

    %% connecting
    assert_throw('EPROTONOSUPPORT', @socket.connect, 'abc://localhost'); % invalid transport
    assert_throw('EINVAL', @socket.connect, 'tcp://localhost');          % port must specified
    assert_does_not_throw(@socket.connect, 'tcp://127.0.0.1:30000');

    %% disconnecting
    assert_throw(@socket.disconnect, 'tcp://127.0.0.1:30103');
    assert_does_not_throw(@socket.disconnect, 'tcp://127.0.0.1:30000');
end

function [ctx, socket] = setup
    % let's just create and destroy a dummy socket
    ctx = zmq.core.ctx_new();
    socket = zmq.Socket(ctx, 'sub');
end

function teardown(ctx)
    % close session
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end
