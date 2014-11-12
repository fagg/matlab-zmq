function collector
    % A collector receive the results sent from workers and aggregate them.
    %
    % Example borrowed from
    % http://learning-0mq-with-pyzmq.readthedocs.org/en/latest/pyzmq/patterns/pushpull.html

    context = zmq_ctx_new();
    socket  = zmq_socket(context, 'ZMQ_PULL');
    address = 'tcp://127.0.0.1:5558';
    zmq_bind(socket, address);

    total = 0;
    for x = 1:50
        result = sscanf(char(zmq_recv(socket)), '%d %d');
        fprintf('data received from consumer#%d: %d\n', result(1), result(2));
        total = total + result(2);
    end

    fprintf('\nTotal: %d\n', total);

    zmq_disconnect(socket, address);
    zmq_close(socket);

    zmq_ctx_shutdown(context);
    zmq_ctx_term(context);
end