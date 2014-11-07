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
    size_t len = 0;
    int i, rc, flags = 0;

    if (nrhs < 2) {
        mexErrMsgIdAndTxt("zmq:send:invalidArgs",
                "Error: Ate least two arguments are required: socket, message.");
        return;
    }
    /*
      I think it's a good design decision to enforce binary data transmission with
      a uint8 row vector parameter. The reasons are:

      - Since Matlab string encoding can be different from system to system,
        just assuming message type as char string could lead to misrepresentation,
        memory waste or even difficulties of understanding the behavior for both
        zmq_send and zmq_recv.
        (Reported in: https://github.com/yida/msgpack-matlab/blob/master/README.md)

      - In the receiver side there is no way to recover the original message
        shape just having the binary data, this would require a protocol.
        So if we permit multidimensional messages sending, the receiver side
        could not differentiate messages like [1 2; 3 4; 5 6], [1 2 3; 4 5 6],
        [1 2 3 4 5 6], leading the inattentive user to frustration and madness.
     */
    if (mxIsUint8(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:send:invalidMessage",
                "Error: message is not a uint8 vector. "
                "Please consider using `uint8`, `cast` and `typecast` "
                "before send data.");
        return;
    }

    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:send:invalidMessageDimensions",
                "Error: message is not a row vector. "
                "Please consider using `reshape` before send data.");
        return;
    }

    socket = pointer_from_m(prhs[0]);
    if (socket == NULL) return;

    len = mxGetN(prhs[1]);
    /* Let's just copy message and manage memory directly... it's easier */
    buffer = (uint8_t*) uint8_array_from_m(prhs[1], len);
    if (buffer == NULL) return;

    if (nrhs > 2) {
        /* Parse options and store the binary flags */
        for (i = 2; i < nrhs; i++) {
            option = (char*) str_from_m(prhs[i]);

            if (option != NULL) {
                if (!strcmp(option, "ZMQ_DONTWAIT")) {
                    flags |= ZMQ_DONTWAIT;
                } else if (!strcmp(option, "ZMQ_SNDMORE")) {
                    flags |= ZMQ_SNDMORE;
                } else {
                    mexWarnMsgIdAndTxt("zmq:send:invalidOption", "Unknow option '%s'", option);
                }

                mxFree(option);
            }
        }
    }

    rc = zmq_send(socket, buffer, len*sizeof(uint8_t), flags);

    if (rc < 0)
        handle_error();
    else
        plhs[0] = int_to_m((void*) &rc);

    mxFree(buffer);
}
