#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <stdlib.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *sockets = NULL;
    int *flags = NULL;
    zmq_pollitem_t *items = NULL;
    int nItems, i, coreAPIReturn;
    long timeout = 1;
    short *ret = NULL;
  
    if (nrhs == 2) {
            nItems = (int) mxGetM(plhs[0]);
            sockets = (void *) mxGetData(plhs[0]);
            flags = (int *) mxGetData(plhs[1]);
    } else if (nrhs == 3) {
            nItems = mxGetM(plhs[0]);
            sockets = (void *) mxGetData(plhs[0]);
            flags = (int *) mxGetData(plhs[1]);
            timeout = (long *) int_from_m(plhs[2]);
    } else {
            mexErrMsgIdAndTxt("zmq:core:poll:invalidArgs", "Invalid arguments.");
    }

    items = (zmq_pollitem_t *) malloc(nItems * sizeof(zmq_pollitem_t));
    if (items == NULL) {
            mexErrMsgIdAndTxt("zmq:core:poll:mallocFail","malloc() failed.");
    }
    
    for (i=0; i<nItems; i++) {
            items[i].socket = (void *) sockets[i];
            items[i].events = flags[i];
            items[i].revents = 0;
    }

    coreAPIReturn = zmq_poll(items, nItems, timeout);
    if (coreAPIReturn == -1) {
            free(items);
            handle_error();
    } else {
            ret = mxCalloc(nItems, sizeof(short));
            plhs[0] = mxCreateNumericArray(0, 0, mxUINT16_CLASS, mxREAL);
            for (i=0; i<nItems; i++) {
                    ret[i] = items[i].revents;
            }
            mxSetData(plhs[0], ret); /* don't call free on ret */
            mxSetM(plhs[0], nItems);
            mxSetN(plhs[0], 1);
            free(items);
    }
}











