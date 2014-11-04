#include "socket.h"
#include <mex.h>
#include <zmq.h>

/*
  Lookupt table with metadata used to interpret socket types.

  The field `id` is the constant defined in `zmq.h`.
  The field `name` is the string representing this constant.
 */
static const zmq_socket_type_t socket_type_lookup[] = {
    {ZMQ_PAIR   , "ZMQ_PAIR"  } ,
    {ZMQ_PUB    , "ZMQ_PUB"   } ,
    {ZMQ_SUB    , "ZMQ_SUB"   } ,
    {ZMQ_REQ    , "ZMQ_REQ"   } ,
    {ZMQ_REP    , "ZMQ_REP"   } ,
    {ZMQ_DEALER , "ZMQ_DEALER"} ,
    {ZMQ_ROUTER , "ZMQ_ROUTER"} ,
    {ZMQ_PULL   , "ZMQ_PULL"  } ,
    {ZMQ_PUSH   , "ZMQ_PUSH"  } ,
    {ZMQ_XPUB   , "ZMQ_XPUB"  } ,
    {ZMQ_XSUB   , "ZMQ_XSUB"  } ,
    {ZMQ_STREAM , "ZMQ_STREAM"} ,
    {-1         , NULL        }
};

/*
  Find the metadata related to the socket type.

  ## Arguments
    - type: pointer to string, containing the type, e.g. "ZMQ_REQ"

  ## Return
  Pointer to a struct with metada (`zmq_socket_type_t`)
 */
const zmq_socket_type_t* socket_type_find_by_name(char* type) {
    int i;
    const zmq_socket_type_t* decriptor = NULL;

    /*
      TODO: as `socket_type_lookup` table order can be chosen arbitrarily,
      we can use binary search to speed up this process.
     */
    for (i = 0; socket_type_lookup[i].name != NULL; i++) {
        if (!strcmp(type, socket_type_lookup[i].name)) {
            decriptor = &(socket_type_lookup[i]);
            break;
        }
    }

    if (decriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:socket:invalidTypeName",
            "Error: socket type %s is invalid.", type);
    }
    return decriptor;
}

/*
  Find the metadata related to the socket type.

  ## Arguments
    - id: integer constant identifying the type, e.g. ZMQ_REQ (defined in zmq.h)

  ## Return
  Pointer to a struct with metada (`zmq_socket_type_t`)
 */
const zmq_socket_type_t* socket_type_find_by_id(int id) {
    int i;
    const zmq_socket_type_t* decriptor = NULL;

    /*
      TODO: as `socket_type_lookup` table order can be chosen arbitrarily,
      we can use binary search to speed up this process.
     */
    for (i = 0; socket_type_lookup[i].name != NULL; i++) {
        if (id == socket_type_lookup[i].id) {
            decriptor = &(socket_type_lookup[i]);
            break;
        }
    }

    if (decriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:socket:invalidTypeId",
            "Error: socket type %d is invalid.", id);
    }
    return decriptor;
}


/* Custom CONSTANT <=> STRING convertions */

mxArray* socktype_to_m(void* handler) {
    mxArray* ret;
    const zmq_socket_type_t* socktype = NULL;

    socktype = socket_type_find_by_id(*((int*) handler));

    if (socktype != NULL) ret = (mxArray*) str_to_m((void*) socktype->name);

    return ret;
}

void socket_error() {
    int err;

    err = errno;

    /* Windows users can have problems with errno, see http://api.zeromq.org/master:zmq-errno */
    if (err == 0) err = zmq_errno();

    switch (err) {
        case EINVAL:
            mexErrMsgIdAndTxt("zmq:socket:invalidOptsCore",
                    "Error: The core API reports that something has gone wrong."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case ETERM:
            mexErrMsgIdAndTxt("zmq:socket:contextTerm",
                    "Error: The context associated with the socket was terminated."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case EFAULT:
        case ENOTSOCK:
            mexErrMsgIdAndTxt("zmq:socket:invalidSocket",
                    "Error: Invalid socket."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case EINTR:
            mexErrMsgIdAndTxt("zmq:socket:interrupted",
                    "Error: The operation was interrupted by a signal."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        default:
            mexErrMsgIdAndTxt("zmq:getsockopts:unknownOops",
                    "Error: Something has gone very, very wrong. Unknown error."
                    "\n(original message: %s)", zmq_strerror(err));
    }
}
