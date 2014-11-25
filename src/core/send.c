#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <zmq.h>
#include <string.h>


/* TODO: Clean up, inline declarations.
 */
void configure_message(const mxArray *, void **, size_t *);
int configure_flag(const mxArray **, int );

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *socket = NULL;
    void *payload = NULL;
    size_t payloadLen = 0;
    int coreAPIReturn, coreAPIOptionFlag = 0;

    /* Configure the message payload */
    configure_message(prhs[1], &payload, &payloadLen);
    if (payload == NULL)
        return;

    /* Get the options for sending */
    if (nrhs > 2)
        coreAPIOptionFlag = configure_flag(&(prhs[2]), nrhs - 2);

    socket = pointer_from_m(prhs[0]);
    if (socket != NULL) {
        coreAPIReturn = zmq_send(socket, payload, payloadLen * sizeof(uint8_t), coreAPIOptionFlag);
        if (coreAPIReturn < 0)
            handle_error();
        else
            plhs[0] = int_to_m( (void *) &coreAPIReturn);
    } else {
        mexErrMsgIdAndTxt("zmq:core:send:invalidSocket", "Error: Invalid socket.");
    }

    mxFree(payload);
}

void configure_message(const mxArray *rawMessage, void **payload, size_t *payloadLen)
{
    /* We don't care what we're sending, just send it in a dumb way and let higher levels
     * deal with typing and reconstruction of dimensions, tensors etc (i.e. serialize with JSON)
     *
     * Reasons for using uint8:
     * - Reported inconsistencies when using strings due enconding:
     *   https://github.com/yida/msgpack-matlab/blob/master/README.md
     *
     * Reasons for restricting input to a row vector:
     * - Uncertainty when differentiating message shapes,
     *   e. g. [1 2; 3 4; 5 6], [1 2 3; 4 5 6], [1 2 3 4 5 6]. */
    if (mxGetM(rawMessage) != 1) {
        mexErrMsgIdAndTxt("zmq:core:send:messageNotRowVec",
                "Error: Message must be a row vector. Flatten before attempting to send.");
    }
    if (mxIsUint8(rawMessage) != 1) {
        mexErrMsgIdAndTxt("zmq:core:send:messageNotUint8",
                "Error: Message payload must be uint8");
    }

    *payloadLen = mxGetN(rawMessage);

    *payload = uint8_array_from_m(rawMessage, *payloadLen);
    if (payload == NULL) {
        mexErrMsgIdAndTxt("zmq:core:send:messageIsEmpty", "Error: You're trying to send an empty message.");
    }
}

int configure_flag(const mxArray **params, int nParams)
{
    int i, coreAPIOptionFlag = 0;
    char *flagStr = NULL;
    for (i = 0 ; i<nParams; i++) {
        flagStr = (char *) str_from_m(params[i]);
        if (flagStr != NULL) {
            if (strcmp(flagStr, "ZMQ_SNDMORE") == 0)
                coreAPIOptionFlag |= ZMQ_SNDMORE;
            else if(strcmp(flagStr, "ZMQ_DONTWAIT") == 0)
                coreAPIOptionFlag |= ZMQ_DONTWAIT;
            else
                mexErrMsgIdAndTxt("zmq:core:send:invalidFlag", "Error: Unknown flag.");
            mxFree(flagStr);
        }
    }

    return coreAPIOptionFlag;
}
