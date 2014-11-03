function test_zmq_version
  v = zmq_version();
  didmatch = regexpi(v, '\d+\.\d+\.\d+');

  assert(~isempty(didmatch), 'zmq_version should return a valid version string, %s given.', v);
end