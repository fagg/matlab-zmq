function producer
    % A Push socket will distribute sent messages to its Pull clients evenly
    %
    % Example borrowed from
    % http://learning-0mq-with-pyzmq.readthedocs.org/en/latest/pyzmq/patterns/pushpull.html

    context = zmq_ctx_new();
    socket  = zmq_socket(context, 'ZMQ_PUSH');
    address = 'tcp://127.0.0.1:5557';
    zmq_bind(socket, address);

    % NOICE: Start your result manager and workers before you start your producers
    for num = 1:100
        zmq_send(socket, uint8(num2str(num)));
    end

    zmq_disconnect(socket, address);
    zmq_close(socket);

    zmq_ctx_shutdown(context);
    zmq_ctx_term(context);
end