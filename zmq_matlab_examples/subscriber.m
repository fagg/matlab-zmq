%% Matlab Subscribers using ZMQ
%%
 clc
 clear all


%%
port = 5556;

% Socket to talk to server
context = zmq.core.ctx_new();
socket = zmq.core.socket(context, 'ZMQ_SUB');

% Subscribe to the server
fprintf('Collecting updates from server...\n');

address = sprintf('tcp://localhost:%d', port);
zmq.core.connect(socket, address);

topicfilter = '10001';

zmq.core.setsockopt(socket, 'ZMQ_SUBSCRIBE', topicfilter);


%%

for i=1:1500

message=char(zmq.core.recv(socket));
parts=strsplit(message);
[topic, px]=parts{:};

topic=str2double(topic);
Px=str2double(px);

fprintf('Recieved Positions (Px, py, pz) %0.4f Topic %d\n', Px,topic);

end

zmq.core.disconnect(socket, address);

zmq.core.close(socket);

zmq.core.ctx_shutdown(context);
zmq.core.ctx_term(context);

%%