#ifndef _UTIL_ERRORS_H_
#define _UTIL_ERRORS_H_

typedef struct _error_desc {
  int id;
  const char* name;
} error_desc_t;

void handle_error(void);

#endif
