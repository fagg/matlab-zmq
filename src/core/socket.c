#include <mex.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <zmq.h>

char *get_socket_type(const mxArray *);
void *core_socket(const mxArray *[]);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *coreAPIReturn;
    void **mexReturn;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("zmq:core:socket:invalidArgs",
                "Error: Two arguments are required - context, socket_type.");
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:socket:sockTypeNotString",
                "Error: socket_type is not a string.");
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:socket:sockTypeNotRowVec",
                "Error: socket_type is not a row vector.");
    }

    coreAPIReturn = core_socket(prhs);

    if (sizeof(void *) == 4) {
        plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS, mxREAL);
    } else {
        plhs[0] = mxCreateNumericMatrix(1,1,mxUINT64_CLASS, mxREAL);
    }
    mexReturn = (void **) mxGetData(plhs[0]);
    mexReturn[0] = coreAPIReturn;
}

char *get_socket_type(const mxArray *param)
{
    int optLen;
    char *ret;

    optLen = mxGetM(param) * mxGetN(param) + 1;
    ret = (char *) mxCalloc(optLen, sizeof(char));

    if (mxGetString(param, ret, optLen) != 0) {
        mexErrMsgIdAndTxt("zmq:core:socket:sockTypeCopyFail",
                "Error: Couldn't get socket_type as string.");
    }
    return ret;
}

void *core_socket(const mxArray *params[])
{
    int sockTypeVal;
    char *sockType;
    void **contextPtr;
    void *coreAPIReturn;
    int err;

    sockType = get_socket_type(params[1]);
    contextPtr = (void **) mxGetData(params[0]);

    if (!strcmp(sockType, "ZMQ_REQ"))
        sockTypeVal = ZMQ_REQ;
    else if (!strcmp(sockType, "ZMQ_REP"))
        sockTypeVal = ZMQ_REP;
    else if (!strcmp(sockType, "ZMQ_DEALER"))
        sockTypeVal = ZMQ_DEALER;
    else if (!strcmp(sockType, "ZMQ_ROUTER"))
        sockTypeVal = ZMQ_ROUTER;
    else if (!strcmp(sockType, "ZMQ_PUB"))
        sockTypeVal = ZMQ_PUB;
    else if (!strcmp(sockType, "ZMQ_SUB"))
        sockTypeVal = ZMQ_SUB;
    else if (!strcmp(sockType, "ZMQ_XPUB"))
        sockTypeVal = ZMQ_XPUB;
    else if (!strcmp(sockType, "ZMQ_XSUB"))
        sockTypeVal = ZMQ_XSUB;
    else if (!strcmp(sockType, "ZMQ_PUSH"))
        sockTypeVal = ZMQ_PUSH;
    else if (!strcmp(sockType, "ZMQ_PULL"))
        sockTypeVal = ZMQ_PULL;
    else if (!strcmp(sockType, "ZMQ_PAIR"))
        sockTypeVal = ZMQ_PAIR;
    else if (!strcmp(sockType, "ZMQ_STREAM"))
        sockTypeVal = ZMQ_STREAM;
    else {
        mxFree(sockType);
        mexErrMsgIdAndTxt("zmq:core:socket:unknownSocketType",
                "Error: Unknown socket type.");
    }

    coreAPIReturn = zmq_socket(*contextPtr, sockTypeVal);
    if (coreAPIReturn == NULL) {
        err = errno;
        /* Windows users can have problems with errno, see http://api.zeromq.org/4-0:zmq-errno */
        if (err == 0) err = zmq_errno();

        switch (errno) {
            case EINVAL:
                mexErrMsgIdAndTxt("zmq:core:socket:unknownSocketTypeCore",
                        "Error: Unknown socket type (core)."
                        "\n(original message: %s)", zmq_strerror(err));
                break;
            case EFAULT:
                mexErrMsgIdAndTxt("zmq:core:socket:invalidContext",
                        "Error: Invalid ZMQ context."
                        "\n(original message: %s)", zmq_strerror(err));
                break;
            case EMFILE:
                mexErrMsgIdAndTxt("zmq:core:socket:maxSocketsReached",
                        "Error: Max sockets reached on context."
                        "\n(original message: %s)", zmq_strerror(err));
                break;
            case ETERM:
                mexErrMsgIdAndTxt("zmq:core:socket:contextTerminated",
                        "Error: ZMQ context was terminated."
                        "\n(original message: %s)", zmq_strerror(err));
                break;
            default:
                mexErrMsgIdAndTxt("zmq:core:socket:unknownOops",
                        "Error: Something has gone very, very wrong. Unknown error."
                        "\n(original message: %s)", zmq_strerror(err));
        }
    }
    return coreAPIReturn;
}
