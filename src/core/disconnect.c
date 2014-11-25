#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void* socket = NULL;
    char* endpoint = NULL;
    int rc;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("zmq:core:disconnect:invalidArgs",
                "Error: Two arguments are required: socket, endpoint.");
        return;
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:disconnect:invalidEndpoint",
                "Error: endpoint is not a string.");
        return;
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:disconnect:endpointNotRowVec",
                "Error: endpoint is not a row vector.");
        return;
    }

    socket = pointer_from_m(prhs[0]);
    if (socket == NULL) return;

    endpoint = (char*) str_from_m(prhs[1]);
    if (endpoint == NULL) return;

    rc = zmq_disconnect(socket, endpoint);

    if (rc < 0) handle_error();
    else plhs[0] = int_to_m((void*) &rc);

    mxFree(endpoint);
}
