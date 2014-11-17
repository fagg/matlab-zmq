function producer
    % A Push socket will distribute sent messages to its Pull clients evenly
    %
    % Example borrowed from
    % http://learning-0mq-with-pyzmq.readthedocs.org/en/latest/pyzmq/patterns/pushpull.html

    context = zmq.core.ctx_new();
    socket  = zmq.core.socket(context, 'ZMQ_PUSH');
    address = 'tcp://127.0.0.1:5557';
    zmq.core.bind(socket, address);

    % NOICE: Start your result manager and workers before you start your producers
    for num = 1:100
        zmq.core.send(socket, uint8(num2str(num)));
    end

    zmq.core.disconnect(socket, address);
    zmq.core.close(socket);

    zmq.core.ctx_shutdown(context);
    zmq.core.ctx_term(context);
end
