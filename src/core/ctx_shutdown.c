#include <mex.h>
#include <errno.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void **contextPtr;
    int coreAPIReturn;

    if (nrhs != 1) {
        mexErrMsgIdAndTxt("zmq:core:ctx_shutdown:invalidArgs",
                "Error: Only a context argument is accepted by this function.");
    }
    contextPtr = (void **) mxGetData(prhs[0]);
    coreAPIReturn = zmq_ctx_shutdown(*contextPtr);
    if (coreAPIReturn < 0) {
        switch (errno) {
            case EFAULT:
                mexErrMsgIdAndTxt("zmq:core:ctx_shutdown:invalidContext",
                        "Error: Invalid ZMQ Context.");
                break;
            default:
                mexErrMsgIdAndTxt("zmq:core:ctx_shutdown:unknownOops",
                        "Error: Something has gone very, very wrong. Unknown error.");
        }
    }
}
