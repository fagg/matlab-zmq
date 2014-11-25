#include <mex.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *context;
    void **mexReturn;

    if (nrhs != 0) {
        mexErrMsgIdAndTxt("zmq:core:ctx_new:invalidArgs",
            "Error: No arguments are accepted by this function.");
    }
    /* Set up ZMQ context and check its validity */
    context = zmq_ctx_new();
    if (context == NULL) {
         mexErrMsgIdAndTxt("zmq:core:ctx_new:contextInitFail",
                 "Error: ZMQ context instantiation failed.");
    }

    /* TODO: Refactor this */
    /* Are we on a 32 or 64 bit machine?  */
    if (sizeof(void*) == 4) {
        plhs[0] = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
    } else {
        plhs[0] = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
    }

    mexReturn = (void **) mxGetData(plhs[0]);
    mexReturn[0] = context;
}
