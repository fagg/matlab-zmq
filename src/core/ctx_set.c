#include <mex.h>
#include <string.h>
#include <zmq.h>

char *get_option_name(const mxArray *);
void core_ctx_set(const mxArray *[]);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 3) {
        mexErrMsgIdAndTxt("zmq:core:ctx_set:invalidArgs",
                "Error: Need three arguments - context, option_name, option_value.");
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:ctx_set:optionNotString",
                "Error: option_name must be a string.");
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:ctx_set:optionNotRowVec",
                "Error: option_name must be a row vector.");
    }
    if (mxIsNumeric(prhs[2]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:ctx_set:optionNotNumeric",
                "Error: option_value isn't numeric.");
    }
    if (mxGetM(prhs[2]) != 1 || mxGetN(prhs[2]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:ctx_set:optionNotSingleNumeric",
            "Error: option_value can't be an array or matrix.");
    }

    core_ctx_set(prhs);
}

char *get_option_name(const mxArray *param)
{
    int optLen = mxGetM(param) * mxGetN(param) + 1;
    char *ret = (char *) mxCalloc(optLen, sizeof(char));
    if (mxGetString(param, ret, optLen) != 0) {
        mexErrMsgIdAndTxt("zmq:core:ctx_set:optNameCopyFail",
                "Error: Couldn't get option_name as string.");
    }
    return ret;
}

void core_ctx_set(const mxArray *params[])
{
    int ret, optValue;
    char *option = get_option_name(params[1]);
    int optVal = mxGetScalar(params[2]);
    void **contextPtr = (void **) mxGetData(params[0]);

    if (!strcmp(option, "ZMQ_IO_THREADS"))
        ret = zmq_ctx_set(*contextPtr, ZMQ_IO_THREADS, optVal);
    else if (!strcmp(option, "ZMQ_MAX_SOCKETS"))
        ret = zmq_ctx_set(*contextPtr, ZMQ_MAX_SOCKETS, optVal);
    else if (!strcmp(option, "ZMQ_IPV6"))
        ret = zmq_ctx_set(*contextPtr, ZMQ_IPV6, optVal);
    else {
        mxFree(option);
        mexErrMsgIdAndTxt("zmq:core:ctx_set:optNameInvalid",
                "Error: Invalid option_name.");
    }
    if (ret != 0) {
        mxFree(option);
        mexErrMsgIdAndTxt("zmq:core:ctx_set:coreAPISetFailure",
                "Error: Setting option_name to option_value failed.");
    } else {
        mxFree(option);
    }
}
