function test_zmq_req_rep
    [ctx, server, client] = setup;
    cleanupObj = onCleanup(@() teardown(ctx, server, client));

    %% client test - request send
    msgSent = uint8('request');
    msgSentSz = length(msgSent);
    rc = assert_does_not_throw(@zmq.core.send, client, msgSent);
    assert(rc == msgSentSz, ...
        'zmq.core.send should return the length of message. Expecting %d, but %d given', ...
         msgSentSz, rc);

    %% server test - request receive
    [msgRecv, msgRecvSz] = assert_does_not_throw(@zmq.core.recv, server);
    assert(msgSentSz == msgRecvSz, ...
        'zmq.core.recv should return the correct length of message. Expecting %d, but %d given', ...
         msgSentSz, msgRecvSz);
    assert(strcmp(char(msgSent), char(msgRecv)), ...
        'zmq.core.recv should return exactly the sent message. Expecting "%s", but "%s" given', ...
         char(msgSent), char(msgRecv));

    %% server test - response send
    msgSent = uint8('response');
    msgSentSz = length(msgSent);
    rc = assert_does_not_throw(@zmq.core.send, server, msgSent);
    assert(rc == msgSentSz, ...
        'zmq.core.send should return the length of message. Expecting %d, but %d given', ...
         msgSentSz, rc);

    %% client test - response receive
    delta = 2; % buffer reduction
    origState = warning; % save for further restoring
    warning('off', 'zmq:core:recv:bufferTooSmall');
    msgRecv = zmq.core.recv(client, msgSentSz-delta, 'ZMQ_DONTWAIT');
    warning(origState);
    assert(strcmp(char(msgSent(1:end-delta)), char(msgRecv)), ...
        'zmq.core.recv should return the sent message truncated. Expecting "%s", but "%s" given', ...
         char(msgSent(1:end-delta)), char(msgRecv));

    %% client test - cannot send 2 messages in a row (unless multipart)
    assert_does_not_throw(@zmq.core.send, client, msgSent);
    assert_throw(@zmq.core.send, client, msgSent);
end

function [ctx, server, client] = setup
    %% open session
    ctx = zmq.core.ctx_new();

    client = zmq.core.socket(ctx, 'ZMQ_REQ');
    zmq.core.connect(client, 'tcp://127.0.0.1:30000');

    server = zmq.core.socket(ctx, 'ZMQ_REP');
    zmq.core.bind(server, 'tcp://127.0.0.1:30000');
end

function teardown(ctx, server, client)
    %% close session
    zmq.core.unbind(server, 'tcp://127.0.0.1:30000');
    zmq.core.close(server);

    zmq.core.disconnect(client, 'tcp://127.0.0.1:30000');
    zmq.core.close(client);

    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
end
