#include <stdlib.h>
#include <util/socket.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <zmq.h>
#include <string.h>

#define DEFAULT_BUFFER_LENGTH 255

int configure_flag(const mxArray*);
void configure_return(int, mxArray **, int, size_t, void *);
size_t configure_buffer_length(const mxArray*);
void free_at_exit(void);

static void *buffer = NULL;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void *socket = NULL;
    size_t bufLen = DEFAULT_BUFFER_LENGTH;
    int coreAPIReturn = 0;
    int coreAPIOptionFlag = 0;

    /* Require at least one argument */
    if (nrhs < 1) {
        mexErrMsgIdAndTxt("zmq:core:recv:invalidArgs",
            "Error: At least one argument is required: socket.");
        return;
    }

    /* Open the socket */
    socket = pointer_from_m(prhs[0]);
    if (socket == NULL) {
        mexErrMsgIdAndTxt("zmq:core:recv:invalidSocket", "Error: Invalid socket.");
        return;
    }
    
    /* Get the maximum message length if specified */
    if (nrhs >= 2) {
      bufLen = configure_buffer_length(prhs[1]);
    }

    /* Get the options for receiving if specified */
    if (nrhs >= 3) {
        coreAPIOptionFlag = configure_flag(prhs[2]);
    }

    /* Reallocate buffer and call API */
    buffer = (uint8_t*) realloc(buffer, bufLen * sizeof(uint8_t));
    if (buffer == NULL) {
        mexErrMsgIdAndTxt("util:realloc", "Error: Unsuccessful memory allocation.");
        return;
    }
    mexAtExit(free_at_exit);

    coreAPIReturn = zmq_recv(socket, buffer, bufLen * sizeof(uint8_t), coreAPIOptionFlag);

    if (coreAPIReturn < 0) {
        /* Check if error is due to non-blocking with no message */
        if (errno == EAGAIN) {
            /* no error, so return zmq_recv return value */
            plhs[0] = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
            *((int*)mxGetData(plhs[0])) = coreAPIReturn;
        }
        else
            handle_error();
    } else {
        /* Prepare the values that should be returned to MATLAB */
        configure_return(nlhs, plhs, coreAPIReturn, bufLen, buffer);
    }
}

/* Discover which options are passed by the user, and calculate the corresponding flag */
int configure_flag(const mxArray *param)
{
    int coreAPIOptionFlag = 0;
    char *flagStr = NULL;

    flagStr = (char *) str_from_m(param);
    if (flagStr != NULL) {
        if(strcmp(flagStr, "ZMQ_DONTWAIT") == 0)
            coreAPIOptionFlag |= ZMQ_DONTWAIT;
        else
            mexErrMsgIdAndTxt("zmq:core:recv:invalidFlag", "Error: Unknown flag.");
        mxFree(flagStr);
    }

    return coreAPIOptionFlag;
}

/* Check if the message was truncated, advising user and avoiding memory waste
 * while prepare it to return to MATLAB. Also handle the second optional return */
void configure_return(int nlhs, mxArray **plhs, int msgLen, size_t bufLen, void *buf) {
    if (msgLen > bufLen) {
        mexWarnMsgIdAndTxt("zmq:core:recv:bufferTooSmall",
            "Message is %d bytes long, but buffer is %d. Truncated.",
            msgLen, bufLen);
        plhs[0] = uint8_array_to_m((void*) buf, bufLen);
    } else {
        plhs[0] = uint8_array_to_m((void*) buf, msgLen);
    }

    if (nlhs > 1) {
        plhs[1] = int_to_m((void*) &msgLen);
    }
}

/* Check if the param is a number. If so, convert it from MATLAB */
size_t configure_buffer_length(const mxArray *param)
{
    size_t length = 0;
    size_t *input = NULL;

    if (!mxIsNumeric(param)) {
        mexErrMsgIdAndTxt("zmq:core:recv:unknowOops", "Buffer length parameter must be numeric.");
        return 0;
    }
    
    input = (size_t*) size_t_from_m(param);
    if (input == NULL) {
        mexErrMsgIdAndTxt("zmq:core:recv:unknowOops", "Unable to convert parameter.");
        return 0;
    }
    
    length = *input;
    mxFree(input);

    return length;
}

/* Frees the buffer at program exit */
void free_at_exit(void)
{
  free(buffer);
  buffer = NULL;
  return;
}
