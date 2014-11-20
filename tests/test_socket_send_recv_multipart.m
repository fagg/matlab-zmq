function test_socket_send_recv_multipart
    [ctx, server, client] = setup;
    cleanupObj = onCleanup(@() cellfun(@(f) f(), ...
        {@() clear('client', 'server'), @() teardown(ctx)}, ...
        'UniformOutput', false));

    %% client test - request send
    msgSent = uint8('request');
    client.send(msgSent, 'sndmore');
    assert_does_not_throw(@client.send, msgSent);

    %% server test - request receive
    msg1 = server.recv;
    rc = server.get('rcvmore');
    assert(rc == 1, 'rcvmore option should be 1, while does not receive all parts in a multipart message');
    msg2 = server.recv;
    rc = server.get('rcvmore');
    assert(rc == 0, 'rcvmore option should be 0, after receive all parts in a multipart message');

    %% -------- multipart methods ----------------------------------------------
    original = uint8(text_fixture('wikipedia.html'));
    assert_does_not_throw(@server.send_multipart, original);  % server send
    received = assert_does_not_throw(@client.recv_multipart); % client recv
    assert(strcmp(char(received), char(original)),...
        ['multipart messages should be transmitted without errors,'...
         ' but received differs from original']);
end

function [ctx, server, client] = setup
    %% open session
    ctx = zmq.core.ctx_new();

    client = zmq.Socket(ctx, 'req');
    client.connect('tcp://127.0.0.1:30000');

    server = zmq.Socket(ctx, 'rep');
    server.bind('tcp://127.0.0.1:30000');
end

function teardown(ctx)
    %% close session
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end