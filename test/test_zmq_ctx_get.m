function test_zmq_ctx_get
  ctx = zmq_ctx_new();

  % ZMQ_IO_THREADS Default Value: 1
  nthreads = zmq_ctx_get(ctx, 'ZMQ_IO_THREADS');
  assertTrue(nthreads == 1);

  % ZMQ_IPV6 Default Value: 0
  ipv6 = zmq_ctx_get(ctx, 'ZMQ_IPV6');
  assertTrue(ipv6 == 0);

  % ZMQ_MAX_SOCKETS Default Value: 1024
  maxs = zmq_ctx_get(ctx, 'ZMQ_MAX_SOCKETS');
  assertTrue(maxs == 1024);
  % NOTICE:
  % According to documentation (http://api.zeromq.org/4-0:zmq-ctx-set)
  % it should be 1024, but I suspect receiving 1023 is OK...

  zmq_ctx_shutdown(ctx);
  zmq_ctx_term(ctx);
end