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
    SOPT_SOCKTYPE,
    SOPT_MECHANISM,
#if defined _WIN32
    SOPT_SOCKET,
#endif
    SOPT_INVALID
};

/*
  Struct with metadata fields, used to interpret socket options.

  The field `id` is the constant defined in `zmq.h`.
  The field `name` is the string representing this constant.
  The field `typeId` is a constant, describing the type for option value,
  e.g. utin64, string, etc...
 */
typedef struct _sopt_desc {
    int id;
    const char* name;
    int typeId;
} zmq_sockopt_desc_t;

/*
  Struct with metadata fields, used to interpret socket option types.

  The field `id` is a constant, identifying the type.
  The field `maxLen` is the maximum number of bytes that can be used for the
  option.
  The field `to_m` is a function pointer, that can be used to convert from C
  value to MATLAB.
 */
typedef struct _sopt_type_desc {
    int id;
    size_t maxLen;
    mxArray* (*to_m)(void* handle);
    void* (*from_m)(const mxArray* param);
} zmq_sockopt_type_t;

/*
  Struct with metadata fields, used to interpret security mechanism.

  The field `id` is the constant defined in `zmq.h`.
  The field `name` is the string representing this constant.
 */
typedef struct _sopt_mechanism_desc {
    int id;
    const char* name;
} zmq_sockopt_mechanism_t;

/* Extra conversions CONST => STRING */
const zmq_sockopt_desc_t* find_sockopt_by_name(char* option);
const zmq_sockopt_type_t* find_sockopt_type_by_id(int typeId);

const zmq_sockopt_mechanism_t* find_sockopt_mechanism_by_id(int mechanismId);
const zmq_sockopt_mechanism_t* find_sockopt_mechanism_by_name(char* name);

mxArray* mechanism_to_m(void* handle) ;
void* mechanism_from_m(const mxArray* param);

#endif
