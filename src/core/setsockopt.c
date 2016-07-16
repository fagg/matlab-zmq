#include <util/socket.h>
#include <util/sockopt.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <zmq.h>
#include <string.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void* socket = NULL;
    char* option = NULL;
    void* optValue = NULL;
    size_t optLen;
    const zmq_sockopt_desc_t* optDesc = NULL;
    const zmq_sockopt_type_t* typeDesc = NULL;
    int rc;

    if (nrhs != 3) {
        mexErrMsgIdAndTxt("zmq:core:setsockopt:invalidArgs",
                "Error: Tree arguments are required: socket, option, value");
        return;
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:setsockopt:invalidOptionName",
                "Error: option_name is not a string.");
        return;
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:setsockopt:optionNameNotRowVec",
                "Error: option_name is not a row vector.");
        return;
    }

    /* Discover which option should be used */
    option = (char*) str_from_m(prhs[1]);
    if (option == NULL) return;

    optDesc = find_sockopt_by_name(option);
    if (optDesc == NULL) return;

    typeDesc = find_sockopt_type_by_id(optDesc->typeId);
    if (typeDesc == NULL) return;

    mxFree(option);

    socket = pointer_from_m(prhs[0]);
    if (socket == NULL) return;

    optValue = typeDesc->from_m(prhs[2]);
    if (optValue == NULL) return;

    optLen = typeDesc->maxLen;
    /* Correct len for compound types */
    if (typeDesc->id == SOPT_STRING || typeDesc->id == SOPT_KEY) {
        optLen = mxGetNumberOfElements(prhs[2]);
    }

    rc = zmq_setsockopt(socket, optDesc->id, optValue, optLen);

    if (rc < 0) {
        handle_error();
        mxFree(optValue);
        return;
    }

    plhs[0] = int_to_m((void*) &rc);
    mxFree(optValue);
}
