#include <mex.h>
#include <errno.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void **contextPtr;
    int coreAPIReturn;

    if (nrhs != 1) {
        mexErrMsgIdAndTxt("zmq:core:ctx_term:invalidArgs",
            "Error: Only a context argument is accepted by this function.");
    }
    contextPtr = (void **) mxGetData(prhs[0]);
    coreAPIReturn = zmq_ctx_term(*contextPtr);
    if (coreAPIReturn < 0) {
        switch (errno) {
            case EFAULT:
                mexErrMsgIdAndTxt("zmq:core:ctx_term:invalidContext",
                    "Error: Invalid ZMQ context.");
                break;
            case EINTR:
                mexErrMsgIdAndTxt("zmq:core:ctx_term:termInter",
                        "Error: Termination was interrupted by a signal.");
                break;
            default:
                mexErrMsgIdAndTxt("zmq:core:ctx_term:unknownOops",
                        "Error: Something has gone very, very wrong. Unknown error.");
        }
    }
}
