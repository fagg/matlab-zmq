function test_zmq_version
  v = zmq_version();
  didmatch = regexpi(v, '\d+\.\d+\.\d+');

  assertFalse(isempty(didmatch));
end