function test_zmq_req_rep
    [ctx, server, client] = setup;
    cleanupObj = onCleanup(@() teardown(ctx, server, client));

    %% client test - request send
    msgSent = uint8('request');
    msgSentSz = length(msgSent);
    rc = assert_does_not_throw(@zmq_send, client, msgSent);
    assert(rc == msgSentSz, ...
        'zmq_send should return the length of message. Expecting %d, but %d given', ...
         msgSentSz, rc);

    %% server test - request receive
    [msgRecv, msgRecvSz] = assert_does_not_throw(@zmq_recv, server);
    assert(msgSentSz == msgRecvSz, ...
        'zmq_recv should return the correct length of message. Expecting %d, but %d given', ...
         msgSentSz, msgRecvSz);
    assert(strcmp(char(msgSent), char(msgRecv)), ...
        'zmq_recv should return exactly the sent message. Expecting "%s", but "%s" given', ...
         char(msgSent), char(msgRecv));

    %% server test - response send
    msgSent = uint8('response');
    msgSentSz = length(msgSent);
    rc = assert_does_not_throw(@zmq_send, server, msgSent);
    assert(rc == msgSentSz, ...
        'zmq_send should return the length of message. Expecting %d, but %d given', ...
         msgSentSz, rc);

    %% client test - response receive
    delta = 2; % buffer reduction
    origState = warning; % save for further restoring
    warning('off', 'zmq:recv:bufferTooSmall');
    msgRecv = zmq_recv(client, msgSentSz-delta, 'ZMQ_DONTWAIT');
    warning(origState);
    assert(strcmp(char(msgSent(1:end-delta)), char(msgRecv)), ...
        'zmq_recv should return the sent message truncated. Expecting "%s", but "%s" given', ...
         char(msgSent(1:end-delta)), char(msgRecv));

    %% client test - cannot send 2 messages in a row (unless multipart)
    assert_does_not_throw(@zmq_send, client, msgSent);
    assert_throw(@zmq_send, client, msgSent);
end

function [ctx, server, client] = setup
    %% open session
    ctx = zmq_ctx_new();

    client = zmq_socket(ctx, 'ZMQ_REQ');
    zmq_connect(client, 'tcp://127.0.0.1:30000');

    server = zmq_socket(ctx, 'ZMQ_REP');
    zmq_bind(server, 'tcp://127.0.0.1:30000');
end

function teardown(ctx, server, client)
    %% close session
    zmq_unbind(server, 'tcp://127.0.0.1:30000');
    zmq_close(server);

    zmq_disconnect(client, 'tcp://127.0.0.1:30000');
    zmq_close(client);

    zmq_ctx_shutdown(ctx);
    zmq_ctx_term(ctx);
end