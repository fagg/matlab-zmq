function test_getsockopt
% matlab-xunit test (see: https://github.com/psexton/matlab-xunit)
  ctx = zmq_ctx_new();
  socket = zmq_socket(ctx, 'ZMQ_REP');

  response = zmq_getsockopt(socket, 'ZMQ_RCVMORE');
  assertTrue(response == 0);

  zmq_ctx_term(ctx);
  zmq_ctx_shutdown(ctx);
end