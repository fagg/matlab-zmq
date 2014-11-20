function test_context_cleanup
    % issues/22
    context = zmq.Context();
    socket = context.socket('ZMQ_PUB');
    clear('context'); % Assert do not block

    context = zmq.Context();
    socket = context.socket('ZMQ_PUB');
    context.delete(); % Assert do not block
    clear('context'); % Assert do not block

    context = zmq.Context();
    socket = context.socket('ZMQ_PUB');
    socket.delete();
    context.delete(); % Assert do not block
    clear('context'); % Assert do not block
end