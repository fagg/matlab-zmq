#include <mex.h>
#include <zmq.h>
#include <stdio.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int minor, major, patch, bufLen = 50;
    char *outputBuf;

    zmq_version(&major, &minor, &patch);

    if (nlhs < 1) {
      mexPrintf("ZMQ Version %d.%d.%d\n", major, minor, patch);
    } else if (nlhs > 1) {
      mexErrMsgTxt("Too many output arguments.");
    } else {
      outputBuf = mxCalloc(bufLen, sizeof(char));
      sprintf(outputBuf, "%d.%d.%d", major, minor, patch);
      plhs[0] = mxCreateString(outputBuf);
    }
}
