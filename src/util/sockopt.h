#ifndef _UTIL_SOCKOPT_H_
#define _UTIL_SOCKOPT_H_

#include <mex.h>
#include <zmq.h>

enum {
    SOPT_UINT64,
    SOPT_INT64,
    SOPT_UINT32,
    SOPT_INT,
    SOPT_STRING,
    SOPT_KEY,
#if defined _WIN32
    SOPT_SOCKET,
#endif
    SOPT_INVALID
};

/*
  Struct with metadata fields, used to interpret socket options.

  The field `id` is the constant defined in `zmq.h`.
  The field `name` is the string representing this constant.
  The field `type_id` is a constant, describing the type for option value,
  e.g. utin64, string, etc...
 */
typedef struct _sopt_desc {
    int id;
    const char* name;
    int type_id;
} zmq_sockopt_desc_t;

/*
  Struct with metadata fields, used to interpret socket option types.

  The field `id` is a constant, identifying the type.
  The field `maxlen` is the maximum number of bytes that can be used for the
  option.
  The field `to_m` is a function pointer, that can be used to convert from C
  value to MATLAB.
 */
typedef struct _sopt_type_desc {
    int id;
    size_t maxlen;
    mxArray* (*to_m)(void* handler);
} zmq_sockopt_type_t;

const zmq_sockopt_desc_t* sockopt_find_by_name(char* option);
const zmq_sockopt_type_t* sockopt_find_type(int type_id);

#endif
