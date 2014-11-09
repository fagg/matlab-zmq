#ifndef _UTIL_CONVERSIONS_H_
#define _UTIL_CONVERSIONS_H_

#include <mex.h>

mxArray* uint64_matrix_to_m(void* handle, size_t m, size_t n);
mxArray* int64_matrix_to_m(void* handle, size_t m, size_t n);
mxArray* uint32_matrix_to_m(void* handle, size_t m, size_t n);
mxArray* int_matrix_to_m(void* handle, size_t m, size_t n);

mxArray* uint64_to_m(void* handle);
mxArray* int64_to_m(void* handle);
mxArray* uint32_to_m(void* handle);
mxArray* int_to_m(void* handle);
mxArray* str_to_m(void* handle);
mxArray* pointer_to_m(void* handle);

void* str_from_m(const mxArray* param);
void* pointer_from_m(const mxArray* param);

#endif
