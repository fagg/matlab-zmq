function test_context_cleanup
    % issues/22
    context = zmq.Context();
    socket = context.socket('ZMQ_PUB');
    clear('context');
    % Assert not block
end