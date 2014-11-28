function test_socket_bind
    [ctx, socket] = setup;
    cleanupObj = onCleanup(@() cellfun(@(f) f(), ...
        {@() clear('socket'), @() teardown(ctx)}, ...
        'UniformOutput', false));

    %% binding
    assert_throw('EPROTONOSUPPORT', @socket.bind, 'abc://localhost'); % invalid transport - TODO sometimes it throws "zmq:core:bind:unknownOops" ("No such file or directory").
    assert_throw('EINVAL', @socket.bind, 'tcp://localhost');          % port must specified
    assert_does_not_throw(@socket.bind, 'tcp://127.0.0.1:30000');

    %% unbinding
    assert_throw(@socket.unbind, 'tcp://127.0.0.1:30103');
    assert_does_not_throw(@socket.unbind, 'tcp://127.0.0.1:30000');

    clear socket;
end

function [ctx, socket] = setup
    % let's just create and destroy a dummy socket
    ctx = zmq.core.ctx_new();
    socket = zmq.Socket(ctx, 'pub');
end

function teardown(ctx)
    % close session
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end
