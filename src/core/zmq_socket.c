#include <mex.h>
#include <errno.h>
#include <string.h>
#include <zmq.h>

char *get_socket_type(const mxArray *);
void *core_socket(const mxArray *[]);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *coreAPIReturn;
    void **mexReturn;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("zmq:socket:invalidArgs",
                "Error: Two arguments are required - context, socket_type.");
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:socket:sockTypeNotString",
                "Error: socket_type is not a string.");
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:socket:sockTypeNotRowVec",
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
        mexErrMsgIdAndTxt("zmq:socket:sockTypeCopyFail",
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
        mexErrMsgIdAndTxt("zmq:socket:unknownSocketType",
                "Error: Unknown socket type.");
    }

    coreAPIReturn = zmq_socket(*contextPtr, sockTypeVal);
    if (coreAPIReturn == NULL) {
        switch (errno) {
            case EINVAL:
                mexErrMsgIdAndTxt("zmq:socket:unknownSocketTypeCore",
                        "Error: Unknown socket type (core).");
                break;
            case EFAULT:
                mexErrMsgIdAndTxt("zmq:socket:invalidContext",
                        "Error: Invalid ZMQ context.");
                break;
            case EMFILE:
                mexErrMsgIdAndTxt("zmq:socket:maxSocketsReached",
                        "Error: Max sockets reached on context.");
                break;
            case ETERM:
                mexErrMsgIdAndTxt("zmq:socket:contextTerminated",
                        "Error: ZMQ context was terminated.");
        }
    }
    return coreAPIReturn;
}
