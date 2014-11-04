#include "conversions.h"
#include <mex.h>
#include <zmq.h>

mxArray* uint64_matrix_to_m(void* handler, size_t m, size_t n) {
    mxArray* ret;
    uint64_t* input;
    uint64_t* output;
    int i, j;

    input = (uint64_t*) handler;

    ret = mxCreateNumericMatrix(m, n, mxUINT64_CLASS, mxREAL);
    output = (uint64_t*) mxGetData(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            output[j*m + i] = (uint64_t) input[i*n + j];
        }
    }

    return ret;
}

mxArray* int64_matrix_to_m(void* handler, size_t m, size_t n) {
    mxArray* ret;
    int64_t* input;
    int64_t* output;
    int i, j;

    input = (int64_t*) handler;

    ret = mxCreateNumericMatrix(m, n, mxINT64_CLASS, mxREAL);
    output = (int64_t*) mxGetData(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            output[j*m + i] = (int64_t) input[i*n + j];
        }
    }

    return ret;
}

mxArray* uint32_matrix_to_m(void* handler, size_t m, size_t n) {
    mxArray* ret;
    uint32_t* input;
    uint32_t* output;
    int i, j;

    input = (uint32_t*) handler;

    ret = mxCreateNumericMatrix(m, n, mxUINT32_CLASS, mxREAL);
    output = (uint32_t*) mxGetData(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            output[j*m + i] = (uint32_t) input[i*n + j];
        }
    }

    return ret;
}

/* TODO: discover the exact size of platform default int */
mxArray* int_matrix_to_m(void* handler, size_t m, size_t n) {
    mxArray* ret;
    int* input;
    int32_t* output;
    int i, j;

    input = (int*) handler;

    ret = mxCreateNumericMatrix(m, n, mxINT32_CLASS, mxREAL);
    output = (int32_t*) mxGetData(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            output[j*m + i] = (int32_t) input[i*n + j];
        }
    }

    return ret;
}

mxArray* uint64_to_m(void* handler) {
    return uint64_matrix_to_m(handler, 1, 1);
}

mxArray* int64_to_m(void* handler) {
    return int64_matrix_to_m(handler, 1, 1);
}

mxArray* uint32_to_m(void* handler) {
    return uint32_matrix_to_m(handler, 1, 1);
}

mxArray* int_to_m(void* handler) {
    return int_matrix_to_m(handler, 1, 1);
}

mxArray* str_to_m(void* handler) {
    return mxCreateString((char*) handler);
}

mxArray* pointer_to_m(void* handler) {
    mxArray* ret;
    void** output;

    if (sizeof(void*) == 4) {
        ret = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
    } else {
        ret = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
    }

    output = (void**) mxGetData(ret);
    output[0] = handler;

    return ret;
}

void* str_from_m(const mxArray* param) {
    char* str = NULL;
    size_t len;

    len = (size_t) mxGetM(param) * mxGetN(param) + 1;
    str = (char*) mxCalloc(len, sizeof(char));

    if (str == NULL) {
        mexErrMsgIdAndTxt("mex:calloc",
                "Error: Unsuccessful memory allocation.");
        return str;
    }

    if (mxGetString(param, str, len) != 0) {
        mexErrMsgIdAndTxt("util:mex2str",
                "Error: Couldn't get parameter as string.");
    }

    return (void*) str;
}

void* pointer_from_m(const mxArray* param) {
    void** input = NULL;
    void* output = NULL;

    input = (void**) mxGetData(param);
    output = (void*) input[0];

    return output;
}