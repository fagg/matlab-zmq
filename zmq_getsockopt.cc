#include <mex.h>
#include <string.h>
#include <zmq.h>

char *get_option_name(const mxArray *);
void core_getsockopt(void *, char *, void *, size_t *);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 3) {
        mexErrMsgIdAndTxt("zmq:getsockopts:invalidArgs",
                "Error: Two arguments are required: socket, option_name");
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:getsockopts:invalidOptionName",
                "Error: option_name is not a string.");
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:getsockopts:optionNameNotRowVec",
                "Error: option_name is not a row vector.");
    }

    void **socketPtr = (void **) mxGetData(prhs[0]);
    void *coreAPIReturn;
    size_t coreAPIReturnSz;
    char *optionName = get_option_name(prhs[1]);

    core_getsockopt(*socketPtr, optionName, coreAPIReturn, &coreAPIReturnSz);

    /* handle return */
}

char *get_option_name(const mxArray *param)
{
    int optLen = mxGetM(param) * mxGetN(param) + 1;
    char *ret = (char *) mxCalloc(optLen, sizeof(char));
    if (mxGetString(param, ret, optLen) != 0) {
        mexErrMsgIdAndTxt("zmq:getsockopts:optNameCopyFail",
                "Error: Couldn't get option_name as string.");
    }
    return ret;
}

void core_getsockopt(void *socket, char *optionName, void *coreAPIReturn, size_t *coreAPIReturnSz)
{
    int optSel;

    if (!strcmp(optionName, "ZMQ_TYPE"))
        optSel = ZMQ_TYPE;
    else if (!strcmp(optionName, "ZMQ_RCVMORE"))
        optSel = ZMQ_RCVMORE;
    else if (!strcmp(optionName, "ZMQ_SNDHWM"))
        optSel = ZMQ_SNDHWM;
    else if (!strcmp(optionName, "ZMQ_RCVHWM"))
        optSel = ZMQ_RCVHWM;
    else if (!strcmp(optionName, "ZMQ_AFFINITY"))
        optSel = ZMQ_AFFINITY;
    else if (!strcmp(optionName, "ZMQ_IDENTITY"))
        optSel = ZMQ_IDENTITY;
    else if (!strcmp(optionName, "ZMQ_RATE"))
        optSel = ZMQ_RATE;
    else if (!strcmp(optionName, "ZMQ_RECOVERY_IVL"))
        optSel = ZMQ_RECOVERY_IVL;
    else if (!strcmp(optionName, "ZMQ_SNDBUF"))
        optSel = ZMQ_SNDBUF;
    else if (!strcmp(optionName, "ZMQ_RCVBUF"))
        optSel = ZMQ_RCVBUF;
    else if (!strcmp(optionName, "ZMQ_LINGER"))
        optSel = ZMQ_LINGER;
    else if (!strcmp(optionName, "ZMQ_RECONNECT_IVL"))
        optSel = ZMQ_RECONNECT_IVL;
    else if (!strcmp(optionName, "ZMQ_RECONNECT_IVL_MAX"))
        optSel = ZMQ_RECONNECT_IVL_MAX;
    else if (!strcmp(optionName, "ZMQ_BACKLOG"))
        optSel = ZMQ_BACKLOG;
    else if (!strcmp(optionName, "ZMQ_MAXMSGSIZE"))
        optSel = ZMQ_MAXMSGSIZE;
    else if (!strcmp(optionName, "ZMQ_MULTICAST_HOPS"))
        optSel = ZMQ_MULTICAST_HOPS;
    else if (!strcmp(optionName, "ZMQ_RCVTIMEO"))
        optSel = ZMQ_RCVTIMEO;
    else if (!strcmp(optionName, "ZMQ_SNDTIMEO"))
        optSel = ZMQ_SNDTIMEO;
    else if (!strcmp(optionName, "ZMQ_IPV6"))
        optSel = ZMQ_IPV6;
    else if (!strcmp(optionName, "ZMQ_IPV4ONLY"))
        optSel = ZMQ_IPV4ONLY;
    else if (!strcmp(optionName, "ZMQ_IMMEDIATE"))
        optSel = ZMQ_IMMEDIATE;
    else if (!strcmp(optionName, "ZMQ_FD"))
        optSel = ZMQ_FD;
    else if (!strcmp(optionName, "ZMQ_EVENTS"))
        optSel = ZMQ_EVENTS;
    else if (!strcmp(optionName, "ZMQ_LAST_ENDPOINT"))
        optSel = ZMQ_LAST_ENDPOINT;
    else if (!strcmp(optionName, "ZMQ_TCP_KEEPALIVE"))
        optSel = ZMQ_TCP_KEEPALIVE;
    else if (!strcmp(optionName, "ZMQ_TCP_KEEPALIVE_IDLE"))
        optSel = ZMQ_TCP_KEEPALIVE_IDLE;
    else if (!strcmp(optionName, "ZMQ_TCP_KEEPALIVE_INTVL"))
        optSel = ZMQ_TCP_KEEPALIVE_INTVL;
    else if (!strcmp(optionName, "ZMQ_MECHANISM"))
        optSel = ZMQ_MECHANISM;
    else if (!strcmp(optionName, "ZMQ_PLAIN_SERVER"))
        optSel = ZMQ_PLAIN_SERVER;
    else if (!strcmp(optionName, "ZMQ_PLAIN_USERNAME"))
        optSel = ZMQ_PLAIN_USERNAME;
    else if (!strcmp(optionName, "ZMQ_CURVE_PUBLICKEY"))
        optSel = ZMQ_CURVE_PUBLICKEY;
    else if (!strcmp(optionName, "ZMQ_CURVE_SECRETKEY"))
        optSel = ZMQ_CURVE_SECRETKEY;
    else if (!strcmp(optionName, "ZMQ_CURVE_SERVERKEY"))
        optSel = ZMQ_CURVE_SERVERKEY;
    else if (!strcmp(optionName, "ZMQ_ZAP_DOMAIN"))
        optSel = ZMQ_ZAP_DOMAIN;
    else {
        mxFree(optionName);
        mexErrMsgIdAndTxt("zmq:getsockopts:invalidSocketOption",
                "Error: socket_option is invalid.");
    }

    int rc = zmq_getsockopt(socket, optSel, coreAPIReturn, coreAPIReturnSz);

    if (rc < 0) {
        mxFree(optionName);
        switch (errno) {
            case EINVAL:
                mexErrMsgIdAndTxt("zmq:getsockopts:invalidOptsCore",
                        "Error: The core API reports that something has gone wrong.");
            break;
            case ETERM:
                mexErrMsgIdAndTxt("zmq:getsockopts:contextTerm",
                        "Error: The context associated with the socket was terminated.");
            break;
            case ENOTSOCK:
                mexErrMsgIdAndTxt("zmq:getsockopts:invalidSocket",
                        "Error: Invalid socket.");
            break;
            case EINTR:
                mexErrMsgIdAndTxt("zmq:getsockopts:interrupted",
                        "Error: The operation was interrupted by a signal.");
            break;
            default:
                mexErrMsgIdAndTxt("zmq:getsockopts:unknownOops",
                        "Error: Something has gone very, very wrong. Unknown error.");
        }
    } else {
        mxFree(optionName);
    }

}
