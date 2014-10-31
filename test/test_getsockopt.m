function test_getsockopt
% matlab-xunit test (see: https://github.com/psexton/matlab-xunit)
  ctx = zmq_ctx_new();
  socket = zmq_socket(ctx, 'ZMQ_REP');

  response = zmq_getsockopt(socket, 'ZMQ_RCVMORE');
  assertFalse(response);

  zmq_ctx_shutdown(ctx);
  zmq_ctx_term(ctx);
end