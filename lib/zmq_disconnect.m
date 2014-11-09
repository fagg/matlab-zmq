% zmq_disconnect - Disconnect a socket from a endpoint
%
% Usage: status = zmq_disconnect(socket, endpoint)
%
% Input: socket   - Instantiated ZMQ socket handle (see zmq_socket).
%        endpoint - String consisting of a 'transport://' followed by an 'address'.
%                   (see zmq_connect).
%
% Output: Zero if successful, otherwise -1.
%