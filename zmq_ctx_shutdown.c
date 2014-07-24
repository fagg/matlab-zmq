#include <mex.h>
#include <errno.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs != 1) {
        mexErrMsgIdAndTxt("zmq:ctx_shutdown:invalidArgs",
                "Error: Only a context argument is accepted by this function.");
    }
    void **contextPtr = (void **) mxGetData(prhs[0]);
    int coreAPIReturn = zmq_ctx_shutdown(*contextPtr);
    if (coreAPIReturn < 0) {
        switch (errno) {
            case EFAULT:
                mexErrMsgIdAndTxt("zmq:ctx_shutdown:invalidContext",
                        "Error: Invalid ZMQ Context.");
                break;
            default:
                mexErrMsgIdAndTxt("zmq:ctx_shutdown:unknownOops",
                        "Error: Something has gone very, very wrong. Unknown error.");
        }
    }
}
