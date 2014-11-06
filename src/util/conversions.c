#include "conversions.h"
#include <mex.h>
#include <zmq.h>

/* -- from C to MATLAB ------------------------------------------------------ */

mxArray* uint64_matrix_to_m(void* handle, size_t m, size_t n) {
    mxArray* ret;
    uint64_t* input;
    uint64_t* output;
    int i, j;

    input = (uint64_t*) handle;

    ret = mxCreateNumericMatrix(m, n, mxUINT64_CLASS, mxREAL);
    output = (uint64_t*) mxGetData(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            output[j*m + i] = (uint64_t) input[i*n + j];
        }
    }

    return ret;
}

mxArray* int64_matrix_to_m(void* handle, size_t m, size_t n) {
    mxArray* ret;
    int64_t* input;
    int64_t* output;
    int i, j;

    input = (int64_t*) handle;

    ret = mxCreateNumericMatrix(m, n, mxINT64_CLASS, mxREAL);
    output = (int64_t*) mxGetData(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            output[j*m + i] = (int64_t) input[i*n + j];
        }
    }

    return ret;
}

mxArray* uint32_matrix_to_m(void* handle, size_t m, size_t n) {
    mxArray* ret;
    uint32_t* input;
    uint32_t* output;
    int i, j;

    input = (uint32_t*) handle;

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
mxArray* int_matrix_to_m(void* handle, size_t m, size_t n) {
    mxArray* ret;
    int* input;
    int32_t* output;
    int i, j;

    input = (int*) handle;

    ret = mxCreateNumericMatrix(m, n, mxINT32_CLASS, mxREAL);
    output = (int32_t*) mxGetData(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            output[j*m + i] = (int32_t) input[i*n + j];
        }
    }

    return ret;
}

mxArray* uint64_to_m(void* handle) {
    return uint64_matrix_to_m(handle, 1, 1);
}

mxArray* int64_to_m(void* handle) {
    return int64_matrix_to_m(handle, 1, 1);
}

mxArray* uint32_to_m(void* handle) {
    return uint32_matrix_to_m(handle, 1, 1);
}

mxArray* int_to_m(void* handle) {
    return int_matrix_to_m(handle, 1, 1);
}

mxArray* str_to_m(void* handle) {
    return mxCreateString((char*) handle);
}

mxArray* pointer_to_m(void* handle) {
    mxArray* ret;
    void** output;

    if (sizeof(void*) == 4) {
        ret = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
    } else {
        ret = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
    }

    output = (void**) mxGetData(ret);
    output[0] = handle;

    return ret;
}


/* -- from MATLAB to C ------------------------------------------------------ */
void* uint64_from_m(const mxArray* param) {
    uint64_t* output = NULL;
    output = (uint64_t*) mxCalloc(1, sizeof(uint64_t));

    if (output == NULL) {
        mexErrMsgIdAndTxt("util:calloc", "Error: Unsuccessful memory allocation.");
    }

    *output = (uint64_t) mxGetScalar(param);

    return (void*) output;
}

void* int64_from_m(const mxArray* param) {
    int64_t* output = NULL;
    output = (int64_t*) mxCalloc(1, sizeof(int64_t));

    if (output == NULL) {
        mexErrMsgIdAndTxt("util:calloc", "Error: Unsuccessful memory allocation.");
    }

    *output = (int64_t) mxGetScalar(param);

    return (void*) output;
}

void* uint32_from_m(const mxArray* param) {
    uint32_t* output = NULL;
    output = (uint32_t*) mxCalloc(1, sizeof(uint32_t));

    if (output == NULL) {
        mexErrMsgIdAndTxt("util:calloc", "Error: Unsuccessful memory allocation.");
    }

    *output = (uint32_t) mxGetScalar(param);

    return (void*) output;
}

void* int_from_m(const mxArray* param) {
    int* output = NULL;
    output = (int*) mxCalloc(1, sizeof(int));

    if (output == NULL) {
        mexErrMsgIdAndTxt("util:calloc", "Error: Unsuccessful memory allocation.");
    }

    *output = (int) mxGetScalar(param);

    return (void*) output;
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
        mexErrMsgIdAndTxt("util:str_from_m",
                "Error: Couldn't get parameter as string.");
    }

    return (void*) str;
}

void* pointer_from_m(const mxArray* param) {
    void** input = NULL;
    void* output = NULL;

    input = (void**) mxGetData(param);
    output = (void*) input[0];

    if (output == NULL) {
        mexErrMsgIdAndTxt("util:pointer_from_m", "Error: Invalid pointer.");
    }

    return output;
}
