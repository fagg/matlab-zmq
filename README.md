matlab-zmq
==========

What's this all about?
----------------------

Quite simply, this will eventually be a complete ZMQ binding for MATLAB. Eventually, this project will include the
following:

+ Mex bindings for libzmq (core ZMQ API).
+ Using said Mex bindings, a "nicer" interface to ZMQ for use with MATLAB.

Getting Started
---------------
+ Make sure you have ZMQ 4.0.4 installed. Change settings in `config.m` to point to your installation.
+ Ensure you have mex configured [[1]](http://www.mathworks.com/help/matlab/matlab_external/what-you-need-to-build-mex-files.html) [[2]](http://www.mathworks.com/support/compilers/R2014b/index.html)
+ Run `make.m`

Can I help?
-----------

Yep -  pull requests, patches, comments, questions, concerns, outright abuse are welcome. I am working on this project
in my free time, which is limited, so if you are interested in helping I welcome your input.

How can I contact you?
-----------------------

ashton@fagg.id.au

Contributors
------------

The following people have made contributions to this library (with my sincere thanks):

- Anderson Bravalheri (@abravalheri - Initial support for Windows)

