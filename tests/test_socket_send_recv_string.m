function test_socket_send_recv_string
    [ctx, server, client, encoding] = setup;
    cleanupObj = onCleanup(@() cellfun(@(f) f(), ...
        {@() clear('client', 'server'), @() teardown(ctx, encoding) }, ...
        'UniformOutput', false));

    encodingArray = {'UTF-8', 'ISO-8859-1', 'Shift_JIS', 'latin1', 'windows-1252', encoding};
    for n = 1:length(encodingArray)
        % Test against several encodings
        feature('DefaultCharacterSet', encodingArray{n});

        original = text_fixture('utf8-sampler.html');
        assert_does_not_throw(@client.send_string, original);  % server send
        received = assert_does_not_throw(@server.recv_string); % client recv

        difference = find(original ~= received);
        assert(strcmp(received, original),...
            ['string messages should be transmitted without errors, '...
             'but received differs from original:\n'...
             'ORIGINAL:\n%s\n-------------\n'...
             'RECEIVED:\n%s\n-------------\nEncoding: %s\n'],...
             original(difference), received(difference), encodingArray{n});

        % Restart socket state machine
        server.send(uint8(1));
        client.recv();
    end
end

function [ctx, server, client, encoding] = setup
    %% open session
    ctx = zmq.core.ctx_new();

    client = zmq.Socket(ctx, 'req');
    client.connect('tcp://127.0.0.1:30000');

    server = zmq.Socket(ctx, 'rep');
    server.bind('tcp://127.0.0.1:30000');

    %% save string encoding
    encoding = feature('DefaultCharacterSet');
end

function teardown(ctx, encoding)
    %% close session
    zmq.core.ctx_shutdown(ctx);
    zmq.core.ctx_term(ctx);
    %% restore string encoding
    feature('DefaultCharacterSet', encoding);
end