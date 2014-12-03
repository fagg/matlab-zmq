#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <stdlib.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *socket = NULL;
    int rc;
    void *retRevents = NULL, *retEvents = NULL;
    zmq_pollitem_t item;

    if (nrhs == 1) {
            socket = pointer_from_m(prhs[0]);
    } else {
            mexErrMsgIdAndTxt("zmq:core:poll:invalidArgs",
                              "Error: Exactly one argument is required.");
    }

    item.socket = (void *) socket;
    
    /* -1 for timeout blocks indefinitely until something happens. */
    rc = zmq_poll(&item, 1, -1);
    if (rc != 1) {
            handle_error();
            return;
    } else {
            retRevents = (void *) &item.revents;
            retEvents = (void *) &item.events;
            plhs[0] = int_to_m(retRevents);
            plhs[1] = int_to_m(retEvents);
            return;
    }
}













