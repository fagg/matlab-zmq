#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <zmq.h>
#include <string.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void* socket = NULL;
    void* buffer = NULL;
    char* option = NULL;
    size_t msgLen = 0;   /* message length */
    size_t bufLen = 255; /* default buffer length */
    size_t* param = NULL;
    int i, flags = 0;

    if (nrhs < 1) {
        mexErrMsgIdAndTxt("zmq:recv:invalidArgs",
                "Error: Ate least one argument is required: socket.");
        return;
    }

    if (nrhs >= 2) {
        if (mxIsNumeric(prhs[1])) {
            i = 2; /* Start to search options form 3th argument */
            param = (size_t*) size_t_from_m(prhs[1]);
            if (param != NULL) {
                bufLen = (size_t) *param;
                mxFree(param);
            } else {
                mexWarnMsgIdAndTxt("zmq:recv:unknowOops", "Unable to convert parameter.");
            }
        } else {
            i = 1; /* Start to search options form 2th argument */
        }

        /* Parse options and store the binary flags */
        for (; i < nrhs; i++) {
            option = (char*) str_from_m(prhs[i]);

            if (option != NULL) {
                if (!strcmp(option, "ZMQ_DONTWAIT")) {
                    flags |= ZMQ_DONTWAIT;
                } else if (!strcmp(option, "ZMQ_SNDMORE")) {
                    flags |= ZMQ_SNDMORE;
                } else {
                    mexWarnMsgIdAndTxt("zmq:recv:invalidOption", "Unknow option '%s'", option);
                }

                mxFree(option);
            }
        }
    }

    socket = pointer_from_m(prhs[0]);
    if (socket == NULL) return;

    buffer = (uint8_t*) mxCalloc(bufLen, sizeof(uint8_t));
    msgLen = zmq_recv(socket, buffer, bufLen*sizeof(uint8_t), flags);

    if (msgLen < 0) {
        handle_error();
    } else {
        if (msgLen > bufLen) {
            mexWarnMsgIdAndTxt("zmq:recv:bufferTooSmall",
                "Message is %d bytes long, but buffer is %d. Truncated.",
                msgLen, bufLen);
            plhs[0] = uint8_array_to_m((void*) buffer, bufLen);
        } else {
            plhs[0] = uint8_array_to_m((void*) buffer, msgLen);
        }

        if (nlhs > 1) {
            plhs[1] = size_t_to_m((void*) &msgLen);
        }
    }

    mxFree(buffer);
}
