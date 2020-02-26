function pub_server(varargin)
    % Super reliable weather information server
    % This weather server will broadcast messages consisting of two integers
    % separated by a space, the first one is the CEP (ZIP brazilian equivalent)
    % and the second is the temperature in celsius

    port = 5552;

    context = zmq.core.ctx_new();
    publisher = zmq.core.socket(context, 'ZMQ_PUB');
    zmq.core.bind(publisher, sprintf('tcp://*:%d', port));

    fprintf('Broadcasting information...\n');
    while (1)
        
        message = sprintf('%d %0.4f ', 10001, 3.21);
        fprintf('Sending %s\n', message);
        zmq.core.send(publisher, uint8(message));
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
