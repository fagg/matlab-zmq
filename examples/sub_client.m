function sub_client(varargin)
    % Temperature informant
    %
    % Example borrowed from
    % http://learning-0mq-with-pyzmq.readthedocs.org/en/latest/pyzmq/patterns/pubsub.html
    %
    % This informant will collect 5 temperature updates from a weather server and
    % calculate the average

    port = 5556;
    if (nargin > 0)
        port =  varargin{1};
    end

    if (nargin > 1)
        port1 =  varargin{2};
    end

    % Socket to talk to server
    context = zmq.core.ctx_new();
    socket = zmq.core.socket(context, 'ZMQ_SUB');

    % Subscribe to the first weather server
    fprintf('Collecting updates from weather server...\n');
    address = sprintf('tcp://localhost:%d', port);
    zmq.core.connect(socket, address);

    if (nargin > 1)
        % Subscribe to the second weather server if required
        % This will make the client receive updates from both servers
        address = sprintf('tcp://localhost:%d', port1);
        zmq.core.connect(socket, address1);
    end

    % Subscribe to receive updates from a brasilian CEP
    % This will filter messages thata starts with the required string
    topicfilter = '15200';
    zmq.core.setsockopt(socket, 'ZMQ_SUBSCRIBE', topicfilter);

    % Process 5 updates
    total = 0;
    for update = 1:5
        message = char(zmq.core.recv(socket));
        parts = strsplit(message);
        [topic, data] = parts{:};
        total = total + str2double(data);
        fprintf('%s %s\n', topic, data);
    end

    fprintf('\nAverage temperature for region with CEP starting with ''%s'' was:\n\n%gºC\n', topicfilter, total/update);

    zmq.core.disconnect(socket, address);
    if (nargin > 1)
        zmq.core.disconnect(socket, address1);
    end

    zmq.core.close(socket);

    zmq.core.ctx_shutdown(context);
    zmq.core.ctx_term(context);
end
