#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <stdlib.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void* socket = NULL;
    long timeout = NULL;
    int rc, revents, events;
    zmq_pollitem_t item;

    if (nrhs == 1) {
            socket = pointer_from_m(prhs[0]);
    } else {
            mexErrMsgIdAndTxt("zmq:core:poll:invalidArgs",
                              "Error: Exactly one argument is required.");
    }

    item.socket = (void *) socket;
    rc = zmq_poll(&item, 1);
    if (rc != 1) {
            handle_error();
            return;
    } else {
            revents = (int) item.revents[0];
            events = (int) item.events[0];
            plhs[0] = int_to_m(revents);
            plhs[1] = int_to_m(events);
            return;
    }
}













