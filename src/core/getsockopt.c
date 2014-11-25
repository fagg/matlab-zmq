#include <util/socket.h>
#include <util/sockopt.h>
#include <util/conversions.h>
#include <util/errors.h>
#include <mex.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void* socket = NULL;
    char* option = NULL;
    void* coreAPIReturn = NULL;
    size_t coreAPIReturnSz;
    const zmq_sockopt_desc_t* optDesc = NULL;
    const zmq_sockopt_type_t* typeDesc = NULL;
    int rc;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("zmq:core:getsockopts:invalidArgs",
                "Error: Two arguments are required: socket, option");
        return;
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:getsockopts:invalidOptionName",
                "Error: option_name is not a string.");
        return;
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:core:getsockopts:optionNameNotRowVec",
                "Error: option_name is not a row vector.");
        return;
    }

    /* Discover which option should be used */
    option = (char*) str_from_m(prhs[1]);
    if (option == NULL) 
        return;

    optDesc = find_sockopt_by_name(option);
    if (optDesc == NULL) 
        return;

    typeDesc = find_sockopt_type_by_id(optDesc->typeId);
    if (typeDesc == NULL) 
        return;

    mxFree(option);

    socket = pointer_from_m(prhs[0]);

    coreAPIReturnSz = typeDesc->maxLen;
    coreAPIReturn = (void*) mxMalloc(coreAPIReturnSz);

    if (coreAPIReturn == NULL) {
        mexErrMsgIdAndTxt("mex:malloc",
                "Error: Unsuccessful memory allocation.");
        return;
    }

    rc = zmq_getsockopt(socket, optDesc->id, coreAPIReturn, &coreAPIReturnSz);

    if (rc < 0) {
        handle_error();
        mxFree(coreAPIReturn);
        return;
    }

    plhs[0] = typeDesc->to_m(coreAPIReturn);
    mxFree(coreAPIReturn);
}
