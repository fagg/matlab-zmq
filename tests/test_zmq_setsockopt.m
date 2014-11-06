function test_zmq_setsockopt
  % let's just create a dummy socket
  ctx = zmq_ctx_new();
  socket = zmq_socket(ctx, 'ZMQ_REQ');

  % -- Options not tested here------------------------------------------------- %
  %% Read-Only properties
  % 'ZMQ_TYPE'
  % 'ZMQ_RCVMORE'
  % 'ZMQ_MECHANISM'

  %% Specific options:
  % ZMQ_ROUTER_MANDATORY (w) -> ZMQ_ROUTER
  % ZMQ_PROBE_ROUTER (w) -> ZMQ_ROUTER, ZMQ_DEALER, ZMQ_REQ
  % ZMQ_REQ_CORRELATE (w), ZMQ_REQ_RELAXED (w) -> ZMQ_REQ
  % ZMQ_CONFLATE (w) -> ZMQ_PULL, ZMQ_PUSH, ZMQ_SUB, ZMQ_PUB, ZMQ_DEALER
  % ZMQ_SUBSCRIBE (w), ZMQ_UNSUBSCRIBE (w) -> ZMQ_SUB
  % ZMQ_IDENTITY (rw) -> ZMQ_REQ, ZMQ_REP, ZMQ_ROUTER, ZMQ_DEALER
  % ZMQ_XPUB_VERBOSE (w) -> ZMQ_XPUB
  % -------------------------------------------------------------------------- %

  % Table with examples for test socket options
  % OBS.: all of them are read-write
  common_options = { ...
    {'ZMQ_SNDHWM'              , 0         } , ...
    {'ZMQ_RCVHWM'              , 0         } , ...
    {'ZMQ_AFFINITY'            , 2         } , ...
    {'ZMQ_RATE'                , 10        } , ...
    {'ZMQ_RECOVERY_IVL'        , 1000      } , ...
    {'ZMQ_SNDBUF'              , 2         } , ...
    {'ZMQ_RCVBUF'              , 2         } , ...
    {'ZMQ_LINGER'              , 2         } , ...
    {'ZMQ_RECONNECT_IVL'       , 120       } , ...
    {'ZMQ_RECONNECT_IVL_MAX'   , 150       } , ...
    {'ZMQ_BACKLOG'             , 150       } , ...
    {'ZMQ_MAXMSGSIZE'          , 100       } , ...
    {'ZMQ_MULTICAST_HOPS'      , 5         } , ...
    {'ZMQ_SNDTIMEO'            , 1         } , ...
    {'ZMQ_RCVTIMEO'            , 1         } , ...
    {'ZMQ_IPV6'                , 1         } , ...
    {'ZMQ_IPV4ONLY'            , 0         } , ...
    {'ZMQ_IMMEDIATE'           , 1         },  ...
    {'ZMQ_TCP_KEEPALIVE'       , 1         } , ...
    {'ZMQ_TCP_KEEPALIVE_IDLE'  , 2         } , ...
    {'ZMQ_TCP_KEEPALIVE_CNT'   , 2         } , ...
    {'ZMQ_TCP_KEEPALIVE_INTVL' , 2         } , ...
... %    {'ZMQ_TCP_ACCEPT_FILTER'   , '123'     } , ... % I don't know how it exactly works
    {'ZMQ_PLAIN_SERVER'        , 1         } , ...
    {'ZMQ_PLAIN_USERNAME'      , 'user'    } , ...
    {'ZMQ_PLAIN_PASSWORD'      , 'password'} , ...
    {'ZMQ_CURVE_SERVER'        , 1         }   ...
... %  {'ZMQ_CURVE_PUBLICKEY'     , ''        } , ... % Z85 text generation/prase not implemented yet
... %  {'ZMQ_CURVE_SECRETKEY'     , ''        } , ... % Z85 text generation/prase not implemented yet
... %  {'ZMQ_CURVE_SERVERKEY'     , ''        } , ... % Z85 text generation/prase not implemented yet
  };

  % This loop will test all the socket options against the default values listed
  % above.
  %
  % Once the socket is fresh and unused, all the options should remain with the
  % default values.
  for n = 1:(length(common_options)-1)
    option = common_options{n}{1};
    value = common_options{n}{2};

    try
      response = zmq_setsockopt(socket, option, value);
    catch e
      % Check if it is octave (`try...catch` not 100% > needs workaround)
      if exist('OCTAVE_VERSION', 'builtin'); e = lasterror; end
      assert(isempty(e.message), '%s should not raise this error: "%s".', option, e.message);
    end
    response = zmq_getsockopt(socket, option);

    if ~ischar(value)
      condition = response == value;
      % display
      response = num2str(response);
      value = num2str(value);
    else
      condition = strcmp(value, response);
      % display
      response = ['"' response '"'];
      value = ['"' value '"'];
    end

    assert(condition, '%s should be %s, %s given.', option, value, response);
  end

  % close session
  zmq_ctx_shutdown(ctx);
  % zmq_ctx_term(ctx); % Despite of ZMQ_LINGER being setted to 2, it still blocks...
                       % My suspect: it will only work if socket is bounded
end