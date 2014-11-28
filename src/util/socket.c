#include "socket.h"
#include "conversions.h"
#include <mex.h>
#include <zmq.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
/*
  Lookupt table with metadata used to interpret socket types.

  The field `id` is the constant defined in `zmq.h`.
  The field `name` is the string representing this constant.
 */
static const zmq_socket_type_t socketTypeLookup[] = {
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
const zmq_socket_type_t* find_socket_type_by_name(char* type) {
    int i;
    const zmq_socket_type_t* descriptor = NULL;

    /*
      TODO: as `socketTypeLookup` table order can be chosen arbitrarily,
      we can use binary search to speed up this process.
     */
    for (i = 0; socketTypeLookup[i].name != NULL; i++) {
        if (!strcmp(type, socketTypeLookup[i].name)) {
            descriptor = &(socketTypeLookup[i]);
            break;
        }
    }

    if (descriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:core:socket:invalidTypeName",
            "Error: socket type %s is invalid.", type);
    }
    return descriptor;
}

/*
  Find the metadata related to the socket type.

  ## Arguments
    - id: integer constant identifying the type, e.g. ZMQ_REQ (defined in zmq.h)

  ## Return
  Pointer to a struct with metada (`zmq_socket_type_t`)
 */
const zmq_socket_type_t* find_socket_type_by_id(int id) {
    int i;
    const zmq_socket_type_t* descriptor = NULL;

    /*
      TODO: as `socketTypeLookup` table order can be chosen arbitrarily,
      we can use binary search to speed up this process.
     */
    for (i = 0; socketTypeLookup[i].name != NULL; i++) {
        if (id == socketTypeLookup[i].id) {
            descriptor = &(socketTypeLookup[i]);
            break;
        }
    }

    if (descriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:core:socket:invalidTypeId",
            "Error: socket type %d is invalid.", id);
    }
    return descriptor;
}


/* Custom CONSTANT <=> STRING convertions */

mxArray* socktype_to_m(void* handle) {
    mxArray* ret;
    const zmq_socket_type_t* sockType = NULL;

    sockType = find_socket_type_by_id(*((int*) handle));

    if (sockType != NULL) ret = (mxArray*) str_to_m((void*) sockType->name);

    return ret;
}

void* socktype_from_m(const mxArray* param) {
    int* output = NULL;
    char* name = NULL;
    const zmq_socket_type_t* sockType = NULL;

    output = (int*) mxCalloc(1, sizeof(int));

    if (output == NULL) {
        mexErrMsgIdAndTxt("util:calloc", "Error: Unsuccessful memory allocation.");
    }

    name = (char*) str_from_m(param);
    if (name == NULL) return NULL;

    sockType = find_socket_type_by_name(name);
    if (sockType != NULL) *output = sockType->id;

    mxFree(name);

    return (void*) output;
}

/* Common error handler */

void socket_error() {
    int err;

    err = errno;

    /* Windows users can have problems with errno, see http://api.zeromq.org/master:zmq-errno */
    if (err == 0) err = zmq_errno();

    switch (err) {
        case EINVAL:
            mexErrMsgIdAndTxt("zmq:core:socket:invalidOptsCore",
                    "Error: The core API reports that something has gone wrong."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case ETERM:
            mexErrMsgIdAndTxt("zmq:core:socket:contextTerm",
                    "Error: The context associated with the socket was terminated."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case EFAULT:
        case ENOTSOCK:
            mexErrMsgIdAndTxt("zmq:core:socket:invalidSocket",
                    "Error: Invalid socket."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case EINTR:
            mexErrMsgIdAndTxt("zmq:core:socket:interrupted",
                    "Error: The operation was interrupted by a signal."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        default:
            mexErrMsgIdAndTxt("zmq:unknownOops",
                    "Error: Something has gone very, very wrong. Unknown error."
                    "\n(original message: %s)", zmq_strerror(err));
    }
}
