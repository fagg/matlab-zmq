#include <mex.h>
#include <string.h>
#include <zmq.h>

char *get_option_name(const mxArray *param, int *err)
{
    int optLen;
    char *ret;

    *err = 1;
    optLen = (int) mxGetM(param) * mxGetN(param) + 1;
    ret = (char *) mxCalloc(optLen, sizeof(char));

    if (mxGetString(param, ret, optLen) != 0) {
        mexErrMsgIdAndTxt("zmq:getsockopts:optNameCopyFail",
                "Error: Couldn't get option_name as string.");
    } else {
        *err = 0;
    }

    return ret;
}

int get_option_value(const mxArray *param, int *err) {
    int value;
    char *optionName;

    optionName = get_option_name(param, err);
    if (*err) return -1;

    *err = 1;

    if (!strcmp(optionName, "ZMQ_TYPE")) {
        value = ZMQ_TYPE;
    } else if (!strcmp(optionName, "ZMQ_RCVMORE")) {
        value = ZMQ_RCVMORE;
    } else if (!strcmp(optionName, "ZMQ_SNDHWM")) {
        value = ZMQ_SNDHWM;
    } else if (!strcmp(optionName, "ZMQ_RCVHWM")) {
        value = ZMQ_RCVHWM;
    } else if (!strcmp(optionName, "ZMQ_AFFINITY")) {
        value = ZMQ_AFFINITY;
    } else if (!strcmp(optionName, "ZMQ_IDENTITY")) {
        value = ZMQ_IDENTITY;
    } else if (!strcmp(optionName, "ZMQ_RATE")) {
        value = ZMQ_RATE;
    } else if (!strcmp(optionName, "ZMQ_RECOVERY_IVL")) {
        value = ZMQ_RECOVERY_IVL;
    } else if (!strcmp(optionName, "ZMQ_SNDBUF")) {
        value = ZMQ_SNDBUF;
    } else if (!strcmp(optionName, "ZMQ_RCVBUF")) {
        value = ZMQ_RCVBUF;
    } else if (!strcmp(optionName, "ZMQ_LINGER")) {
        value = ZMQ_LINGER;
    } else if (!strcmp(optionName, "ZMQ_RECONNECT_IVL")) {
        value = ZMQ_RECONNECT_IVL;
    } else if (!strcmp(optionName, "ZMQ_RECONNECT_IVL_MAX")) {
        value = ZMQ_RECONNECT_IVL_MAX;
    } else if (!strcmp(optionName, "ZMQ_BACKLOG")) {
        value = ZMQ_BACKLOG;
    } else if (!strcmp(optionName, "ZMQ_MAXMSGSIZE")) {
        value = ZMQ_MAXMSGSIZE;
    } else if (!strcmp(optionName, "ZMQ_MULTICAST_HOPS")) {
        value = ZMQ_MULTICAST_HOPS;
    } else if (!strcmp(optionName, "ZMQ_RCVTIMEO")) {
        value = ZMQ_RCVTIMEO;
    } else if (!strcmp(optionName, "ZMQ_SNDTIMEO")) {
        value = ZMQ_SNDTIMEO;
    } else if (!strcmp(optionName, "ZMQ_IPV6")) {
        value = ZMQ_IPV6;
    } else if (!strcmp(optionName, "ZMQ_IPV4ONLY")) {
        value = ZMQ_IPV4ONLY;
    } else if (!strcmp(optionName, "ZMQ_IMMEDIATE")) {
        value = ZMQ_IMMEDIATE;
    } else if (!strcmp(optionName, "ZMQ_FD")) {
        value = ZMQ_FD;
    } else if (!strcmp(optionName, "ZMQ_EVENTS")) {
        value = ZMQ_EVENTS;
    } else if (!strcmp(optionName, "ZMQ_LAST_ENDPOINT")) {
        value = ZMQ_LAST_ENDPOINT;
    } else if (!strcmp(optionName, "ZMQ_TCP_KEEPALIVE")) {
        value = ZMQ_TCP_KEEPALIVE;
    } else if (!strcmp(optionName, "ZMQ_TCP_KEEPALIVE_IDLE")) {
        value = ZMQ_TCP_KEEPALIVE_IDLE;
    } else if (!strcmp(optionName, "ZMQ_TCP_KEEPALIVE_INTVL")) {
        value = ZMQ_TCP_KEEPALIVE_INTVL;
    } else if (!strcmp(optionName, "ZMQ_MECHANISM")) {
        value = ZMQ_MECHANISM;
    } else if (!strcmp(optionName, "ZMQ_PLAIN_SERVER")) {
        value = ZMQ_PLAIN_SERVER;
    } else if (!strcmp(optionName, "ZMQ_PLAIN_USERNAME")) {
        value = ZMQ_PLAIN_USERNAME;
    } else if (!strcmp(optionName, "ZMQ_CURVE_PUBLICKEY")) {
        value = ZMQ_CURVE_PUBLICKEY;
    } else if (!strcmp(optionName, "ZMQ_CURVE_SECRETKEY")) {
        value = ZMQ_CURVE_SECRETKEY;
    } else if (!strcmp(optionName, "ZMQ_CURVE_SERVERKEY")) {
        value = ZMQ_CURVE_SERVERKEY;
    } else if (!strcmp(optionName, "ZMQ_ZAP_DOMAIN")) {
        value = ZMQ_ZAP_DOMAIN;
    } else {
        mxFree(optionName);
        mexErrMsgIdAndTxt("zmq:getsockopts:invalidSocketOption",
                "Error: socket_option %s is invalid.", optionName);
    }

    *err = 0;
    return value;
}

void handle_error() {
    int err;

    err = errno;

    /* Windows users can have problems with errno, see http://api.zeromq.org/4-0:zmq-errno */
    if (err == 0) err = zmq_errno();

    switch (err) {
        case EINVAL:
            mexErrMsgIdAndTxt("zmq:getsockopts:invalidOptsCore",
                    "Error: The core API reports that something has gone wrong."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case ETERM:
            mexErrMsgIdAndTxt("zmq:getsockopts:contextTerm",
                    "Error: The context associated with the socket was terminated."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case EFAULT:
        case ENOTSOCK:
            mexErrMsgIdAndTxt("zmq:getsockopts:invalidSocket",
                    "Error: Invalid socket."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case EINTR:
            mexErrMsgIdAndTxt("zmq:getsockopts:interrupted",
                    "Error: The operation was interrupted by a signal."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        default:
            mexErrMsgIdAndTxt("zmq:getsockopts:unknownOops",
                    "Error: Something has gone very, very wrong. Unknown error."
                    "\n(original message: %s)", zmq_strerror(err));
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *socket;
    void **mexParam;
    void *coreAPIReturn;
    size_t coreAPIReturnSz;
    int rc, err, option;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("zmq:getsockopts:invalidArgs",
                "Error: Two arguments are required: socket, option_name");
        return;
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:getsockopts:invalidOptionName",
                "Error: option_name is not a string.");
        return;
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:getsockopts:optionNameNotRowVec",
                "Error: option_name is not a row vector.");
        return;
    }

    mexParam = (void **) mxGetData(prhs[0]);
    socket = mexParam[0];

    option = get_option_value(prhs[1], &err);
    if (err) return;

    rc = zmq_getsockopt(socket, option, coreAPIReturn, &coreAPIReturnSz);
    if (rc < 0) {
        handle_error();
        return;
    }
}