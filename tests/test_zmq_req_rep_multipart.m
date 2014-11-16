function test_zmq_req_rep_multipart
    [ctx, server, client] = setup;
    cleanupObj = onCleanup(@() teardown(ctx, server, client));

    %% client test - request send
    msgSent = uint8('request');
    msgSentSz = length(msgSent);
    zmq_send(client, msgSent, 'ZMQ_SNDMORE');
    assert_does_not_throw(@zmq_send, client, msgSent);

    %% server test - request receive
    msg1 = zmq_recv(server);
    rc = zmq_getsockopt(server, 'ZMQ_RCVMORE');
    assert(rc == 1, 'ZMQ_RCVMORE option should be 1, while does not receive all parts in a multipart message');
    msg2 = zmq_recv(server);
    rc = zmq_getsockopt(server, 'ZMQ_RCVMORE');
    assert(rc == 0, 'ZMQ_RCVMORE option should be 0, after receive all parts in a multipart message');
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