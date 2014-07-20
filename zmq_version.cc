#include <mex.h>
#include <zmq.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int minor, major, patch;
    zmq_version(&major, &minor, &patch);
    mexPrintf("ZMQ Version %d.%d.%d\n", major, minor, patch);
}
