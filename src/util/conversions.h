#ifndef _UTIL_CONVERSIONS_H_
#define _UTIL_CONVERSIONS_H_

#include <mex.h>

mxArray* uint64_matrix_to_m(void* handle, size_t m, size_t n);
mxArray* int64_matrix_to_m(void* handle, size_t m, size_t n);
mxArray* uint32_matrix_to_m(void* handle, size_t m, size_t n);
mxArray* int_matrix_to_m(void* handle, size_t m, size_t n);

mxArray* uint8_array_to_m(void* handle, size_t n);

mxArray* size_t_to_m(void* handle);
mxArray* uint64_to_m(void* handle);
mxArray* int64_to_m(void* handle);
mxArray* uint32_to_m(void* handle);
mxArray* int_to_m(void* handle);
mxArray* str_to_m(void* handle);
mxArray* pointer_to_m(void* handle);

void* uint8_array_from_m(const mxArray* param, size_t n);

void* size_t_from_m(const mxArray* param);
void* uint64_from_m(const mxArray* param);
void* int64_from_m(const mxArray* param);
void* uint32_from_m(const mxArray* param);
void* int_from_m(const mxArray* param);
void* str_from_m(const mxArray* param);
void* pointer_from_m(const mxArray* param);

#endif
