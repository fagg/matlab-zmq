% zmq_unbind - Stop accepting connections on a socket from a endpoint
%
% Usage: status = zmq_unbind(socket, endpoint)
%
% Input: socket   - Instantiated ZMQ socket handle (see zmq_socket).
%        endpoint - String consisting of a 'transport://' followed by an 'address'.
%                   (see zmq_bind).
%
% Output: Zero if successful, otherwise -1.
%