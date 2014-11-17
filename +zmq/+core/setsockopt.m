% zmq_setsockopt - Set ZMQ socket options. Complementary to zmq_getsockopt.
%
% Usage: status = zmq_getsockopt(socket, optionName, optionValue).
%
% Input: socket - Instantiated ZMQ socket handle (see zmq_socket).
%        optionName  - Option string. Please refer to http://api.zeromq.org/master:zmq-setsockopt
%                      for a complete description. Examples:
%                      * ZMQ_TYPE: Retrieve socket type
%                      * ZMQ_BACKLOG: Retrieve maximum length of the queue of outstanding connections
%                      * ZMQ_IPV6: Retrieve IPv6 socket status
%        optionValue - New value for option. Please refer to http://api.zeromq.org/master:zmq-setsockopt
%                      for a complete description.
%
% Output:  Zero if successful, otherwise -1 .
