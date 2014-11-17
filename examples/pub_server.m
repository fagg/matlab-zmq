function pub_server(varargin)
    % Super reliable weather information server
    %
    % Example borrowed from
    % http://learning-0mq-with-pyzmq.readthedocs.org/en/latest/pyzmq/patterns/pubsub.html
    %
    % This weather server will broadcast messages consisting of two integers
    % separated by a space, the first one is the CEP (ZIP brazilian equivalent)
    % and the second is the temperature in celsius

    port = 5556;
    if (nargin > 0)
        port =  varargin{1};
    end

    context = zmq.core.ctx_new();
    socket = zmq.core.socket(context, 'ZMQ_PUB');
    zmq.core.bind(socket, sprintf('tcp://*:%d', port));

    fprintf('Broadcasting temperature information...\n');
    while (1)
        topic = randi([15198, 15202], 1, 1); % Choose a brazilian CEP code (first 5 digits)
        data = randi([10, 45]);              % Pick a random temperature
        message = sprintf('%d %d', topic, data);
        fprintf('%s\n', message);
        zmq.core.send(socket, uint8(message));
        delay(1);
    end
end

function delay(seconds)
    % pause the program
    %
    % Aguments:
    %  seconds - delay time in seconds
    tic;
    while toc < seconds
    end
end
