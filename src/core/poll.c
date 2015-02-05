#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <stdlib.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void **sockets = NULL;
    zmq_pollitem_t *items = NULL;
    int nItems, i, coreAPIReturn;
    long *timeout = NULL;
    int *flags = NULL;
    short *ret = NULL;
  
    if (nrhs == 2) {
            nItems = *((int *) int_from_m(plhs[0]));
            sockets = (void **) mxGetData(plhs[1]);
            flags = (int *) int_from_m(plhs[2]);
    } else if (nrhs == 3) {
            nItems = *((int *) int_from_m(plhs[0]));
            sockets = (void **) mxGetData(plhs[1]);
            flags = (int *) int_from_m(plhs[1]);
            timeout = (long *) long_from_m(plhs[2]);
    } else {
            mexErrMsgIdAndTxt("zmq:core:poll:invalidArgs", "Invalid arguments.");
    }

    /* allocate and populate zmq_pollitem_t with everything passed in from matlab */
    items = (zmq_pollitem_t *) calloc((size_t) nItems, sizeof(zmq_pollitem_t));
    if (items == NULL) {
            mexErrMsgIdAndTxt("zmq:core:poll:callocFail","calloc() failed.");
    }
    
    for (i=0; i<nItems; i++) {
            items[i].socket = (void *) (sockets[i]);
            items[i].events = flags[i];
            items[i].revents = 0;
    }

    /* call out to ZMQ to handle the poll */
    if (timeout != NULL) {
            coreAPIReturn = zmq_poll(items, nItems, *timeout);
    } else {
            /* We assume default behaviour if no timeout is given*/
            coreAPIReturn = zmq_poll(items, nItems, 1);
    }
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











