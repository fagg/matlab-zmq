function test_socket_send_recv
    [ctx, server, client] = setup;
    cleanupObj = onCleanup(@() cellfun(@(f) f(), ...
        {@() clear('client', 'server'), @() teardown(ctx)}, ...
        'UniformOutput', false));

    %% client test - request send
    msgSent = uint8('request');
    msgSentSz = length(msgSent);
    rc = assert_does_not_throw(@client.send, msgSent);
    assert(rc == msgSentSz, ...
        'socket.send should return the length of message. Expecting %d, but %d given', ...
         msgSentSz, rc);

    %% server test - request receive
    [msgRecv, msgRecvSz] = assert_does_not_throw(@server.recv);
    assert(msgSentSz == msgRecvSz, ...
        'socket.recv should return the correct length of message. Expecting %d, but %d given', ...
         msgSentSz, msgRecvSz);
    assert(strcmp(char(msgSent), char(msgRecv)), ...
        'socket.recv should return exactly the sent message. Expecting "%s", but "%s" given', ...
         char(msgSent), char(msgRecv));

    %% server test - response send
    msgSent = uint8('response');
    msgSentSz = length(msgSent);
    rc = assert_does_not_throw(@server.send, msgSent);
    assert(rc == msgSentSz, ...
        'socket.send should return the length of message. Expecting %d, but %d given', ...
         msgSentSz, rc);

    %% client test - response receive
    delta = 2; % buffer reduction
    origState = warning; % save for further restoring
    warning('off', 'zmq:core:recv:bufferTooSmall');
    msgRecv = client.recv(msgSentSz-delta, 'dontwait');
    warning(origState);
    assert(strcmp(char(msgSent(1:end-delta)), char(msgRecv)), ...
        'socket.recv should return the sent message truncated. Expecting "%s", but "%s" given', ...
         char(msgSent(1:end-delta)), char(msgRecv));

    %% client test - cannot send 2 messages in a row (unless multipart)
    assert_does_not_throw(@client.send, msgSent);
    assert_throw(@client.send, msgSent);
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
