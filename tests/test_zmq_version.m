function test_zmq_version
  v = zmq_version();
  match = regexpi(v, '\d+\.\d+\.\d+');

  assert(~isempty(match), 'zmq_version should return a valid version string, %s given.', v);
end