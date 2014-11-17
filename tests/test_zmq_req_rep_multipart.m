function test_zmq_req_rep_multipart
    [ctx, server, client] = setup;
    cleanupObj = onCleanup(@() teardown(ctx, server, client));

    %% client test - request send
    msgSent = uint8('request');
    msgSentSz = length(msgSent);
    zmq.core.send(client, msgSent, 'ZMQ_SNDMORE');
    assert_does_not_throw(@zmq.core.send, client, msgSent);

    %% server test - request receive
    msg1 = zmq.core.recv(server);
    rc = zmq.core.getsockopt(server, 'ZMQ_RCVMORE');
    assert(rc == 1, 'ZMQ_RCVMORE option should be 1, while does not receive all parts in a multipart message');
    msg2 = zmq.core.recv(server);
    rc = zmq.core.getsockopt(server, 'ZMQ_RCVMORE');
    assert(rc == 0, 'ZMQ_RCVMORE option should be 0, after receive all parts in a multipart message');
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
