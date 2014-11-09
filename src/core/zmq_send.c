#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <string.h>
#include <zmq.h>
#include <mex.h>


/* TODO: Clean up, inline declarations. 
 * TODO: Write tests.
 */
void configure_message(const mxArray *, void * , size_t *);
int configure_flag(const mxArray *, int );

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *socket = NULL;
    void *payload = NULL;
    size_t payloadLen = 0;
    int coreAPIReturn, coreAPIOptionFlag = 0;

    /* Configure the message payload */
    configure_message(prhs[1], payload, &payloadLen);

    /* Get the options for sending */
    if (nrhs > 2)
        coreAPIOptionFlag = configure_flag(prhs, nrhs);

    coreAPIReturn = zmq_send(socket, payload, payloadLen * sizeof(uint8), coreAPIOptionFlag);
    if (coreAPIReturn != 0)
        handle_error();
    else
        plhs[0] = int_to_m( (void *) coreAPIReturn);

    mxFree(payload);
}

void configure_message(const mxArray *rawMessage, void *payload, size_t *payloadLen)
{
    /* We don't care what we're sending, just send it in a dumb way and let higher levels
     * deal with typing and reconstruction of dimensions, tensors etc (i.e. serialize with JSON) */
    if (mxGetM(rawMessage) != 1) {
        mexErrMsgIdAndTxt("zmq:send:messageNotRowVec",
                "Error: Message must be a row vector. Flatten before attempting to send.");
    }
    if (mxIsUint8(rawMessage) != 1) {
        mexErrMsgIdAndTxt("zmq:send:messageNotUint8",
                "Error: Message payload must be uint8");
    }

    *payloadLen = mxGetN(rawMessage);

    *payload = (uint8_t *) uint8_array_from_m(rawMessage, *payloadLen);
    if (payload == NULL) {
        mexErrMsgIdAndTxt("zmq:send:messageIsEmpty", "Error: You're trying to send an empty message.");
    }
}

int configure_flag(const mxArray *params, int nParams)
{
    int i = 2, coreAPIOptionFlag = 0;
    char *flagStr = NULL;
    for ( ; i<nParams; i++) {
        flagStr = (char *) str_from_m(params[i]);
        if (flagStr != NULL) {
            if (strcmp(flagStr, "ZMQ_SNDMORE") == 0)
                coreAPIOptionFlag |= ZMQ_SNDMORE;
            else if(strcmp(flagStr, "ZMQ_DONTWAIT") == 0)
                coreAPIOptionFlag |= ZMQ_DONTWAIT;
            else
                mexErrMsgIdAndTxt("zmq:send:invalidFlag", "Error: Unknown flag.");
            mxFree(flagStr);
        }
    }
}
