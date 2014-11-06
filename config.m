% Please edit this file with the correct paths for ZMQ instalation.
%
% Examples can be found in files `config_unix.m`, `config_win.m`.
% This file itself shows how to build `matlab-zmq` using a Homebrew
% instalation of ZMQ 4.0.4 for OS-X.

% The filename for the compiled lib (binary file)
ZMQ_COMPILED_LIB = 'libzmq.a';

% Where is the compiled lib placed?
ZMQ_LIB_PATH = '/usr/local/Cellar/zeromq/4.0.4/lib/';

% Where can we find the headers?
ZMQ_INCLUDE_PATH = '/usr/local/Cellar/zeromq/4.0.4/include/';