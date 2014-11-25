#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <stdlib.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void* socket = NULL;
    int rc;

    if (nrhs != 1) {
        mexErrMsgIdAndTxt("zmq:core:close:invalidArgs",
                "Error: One argument is required: socket");
        return;
    }

    socket = pointer_from_m(prhs[0]);
    if (socket == NULL) return;

    rc = zmq_close(socket);

    if (rc < 0) {
        handle_error();
        return;
    }

    plhs[0] = int_to_m((void*) &rc);
}
