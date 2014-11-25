/* zmq_ctx_get.cc - Mex wrapper for zmq_ctx_get(3).
 *
 * Copyright 2014 Ashton Fagg (ashton@fagg.id.au)
 */

#include <mex.h>
#include <string.h>
#include <zmq.h>

char *get_option_name(const mxArray *);
int core_ctx_get(const mxArray *[]);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int coreAPIReturn;
    int *mexReturn;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("zmq:core:ctx_get:invalidArgs",
                "Error: Need two arguments - context and option_name.");
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:ctx_get:optionNotString",
                "Error: option_name must be a string.");
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:ctx_get:optionNotRowVec",
                "Error: option_name must be a row vector.");
    }

    coreAPIReturn = core_ctx_get(prhs);

    if (sizeof(void *) == 4) {
        plhs[0] = mxCreateNumericMatrix(1,1,mxUINT32_CLASS, mxREAL);
    } else {
        plhs[0] = mxCreateNumericMatrix(1,1,mxUINT64_CLASS, mxREAL);
    }
    mexReturn = (int *) mxGetData(plhs[0]);
    *mexReturn = coreAPIReturn;
}

char *get_option_name(const mxArray *param)
{
    int optLen;
    char *ret;

    optLen = mxGetM(param) * mxGetN(param) + 1;
    ret = (char *) mxCalloc(optLen, sizeof(char));

    if (mxGetString(param, ret, optLen) != 0) {
        mexErrMsgIdAndTxt("zmq:core:ctx_get:optNameCopyFail",
                "Error: Couldn't get option_name as string.");
    }
    return ret;
}

int core_ctx_get(const mxArray *params[])
{
    int ret, optSelection;
    char *option;
    void **contextPtr;

    option = get_option_name(params[1]);
    contextPtr = (void **) mxGetData(params[0]);

    if (!strcmp(option, "ZMQ_IO_THREADS"))
        optSelection = ZMQ_IO_THREADS;
    else if(!strcmp(option, "ZMQ_MAX_SOCKETS"))
        optSelection = ZMQ_MAX_SOCKETS;
    else if (!strcmp(option, "ZMQ_IPV6"))
        optSelection = ZMQ_IPV6;
    else {
        mxFree(option);
        mexErrMsgIdAndTxt("zmq:core:ctx_get:optNameInvalid",
                "Error: Invalid option_name.");
    }
    ret = zmq_ctx_get(*contextPtr, optSelection);

    /* Make sure the call returns OK, it should do considering we filter our
     * own option_value values. */
    if (ret < 0) {
        mxFree(option);
        mexErrMsgIdAndTxt("zmq:core:ctx_get:coreAPIInvalidOption",
                "Error: Invalid option_name (core API).");
    }
    mxFree(option);
    return ret;
}
