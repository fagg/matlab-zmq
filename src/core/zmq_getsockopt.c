#include <util/sockopt.h>
#include <util/socket.h>
#include <util/conversions.h>
#include <mex.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    void* socket = NULL;
    char* option = NULL;
    void* api_return = NULL;
    void** mexParam = NULL;
    const zmq_sockopt_desc_t* optdesc = NULL;
    const zmq_sockopt_type_t* typedesc = NULL;
    size_t api_return_len;
    int rc, err;

    if (nrhs != 2) {
        mexErrMsgIdAndTxt("zmq:getsockopts:invalidArgs",
                "Error: Two arguments are required: socket, option");
        return;
    }
    if (mxIsChar(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:getsockopts:invalidOptionName",
                "Error: option_name is not a string.");
        return;
    }
    if (mxGetM(prhs[1]) != 1) {
        mexErrMsgIdAndTxt("zmq:getsockopts:optionNameNotRowVec",
                "Error: option_name is not a row vector.");
        return;
    }

    /* Discover which option should be used */
    option = (char*) str_from_m(prhs[1]);
    if (option == NULL) return;

    optdesc = sockopt_find_by_name(option);
    if (optdesc == NULL) return;

    typedesc = sockopt_find_type(optdesc->type_id);
    if (typedesc == NULL) return;

    mxFree(option);

    mexParam = (void**) mxGetData(prhs[0]);
    socket = (void*) mexParam[0];

    api_return_len = typedesc->maxlen;
    api_return = (void*) mxMalloc(api_return_len);

    if (api_return == NULL) {
        mexErrMsgIdAndTxt("mex:malloc",
                "Error: Unsuccessful memory allocation.");
        return;
    }

    rc = zmq_getsockopt(socket, optdesc->id, api_return, &api_return_len);

    if (rc < 0) {
        socket_error();
        mxFree(api_return);
        return;
    }

    plhs[0] = typedesc->to_m(api_return);
    mxFree(api_return);
}