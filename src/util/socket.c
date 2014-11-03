#include <mex.h>
#include <zmq.h>

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
