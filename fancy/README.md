This is the start of the higher level, ``fancy'' API for ZMQ, which builds upon the
lower level matlab-zmq API.

The proposed design is as follows:

ZMQ -> abstracts and allows easy access to ZMQ contexts.
ZMQSocket -> abstracts and controls socket instances, spawned from ZMQ objects.
ZMQMessage -> abstracts serialization of objects in order to build messages to be
send via ZMQSocket instances. Also used in reverse to obtain information recieved via ZMQ.

Serialization:

JSON


Input welcome: ashton@fagg.id.au
