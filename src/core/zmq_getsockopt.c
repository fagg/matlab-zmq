#include <mex.h>
#include <string.h>
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

typedef struct _sopt_desc {
    int id;
    const char* name;
    int type_id;
} zmq_sockopt_desc_t;

typedef struct _sopt_type_desc {
    int id;
    size_t maxlen;
    mxArray* (*to_m)(void* handler, size_t m, size_t n);
} zmq_sockopt_type_t;

/*
  Information used to construct this table can found in:
  - http://api.zeromq.org/4-0:zmq-getsockopt
  - http://api.zeromq.org/4-0:zmq-setsockopt

  TODO: Maybe is possible to use imatix/gsl in combination with
  https://github.com/zeromq/czmq/blob/master/src/sockopts.xml file, to generate
  this table automatically ...
 */
static const zmq_sockopt_desc_t sockopt_array[] = {
    {ZMQ_TYPE                , "ZMQ_TYPE"                , SOPT_INT   } ,
    {ZMQ_RCVMORE             , "ZMQ_RCVMORE"             , SOPT_INT   } ,
    {ZMQ_SNDHWM              , "ZMQ_SNDHWM"              , SOPT_INT   } ,
    {ZMQ_RCVHWM              , "ZMQ_RCVHWM"              , SOPT_INT   } ,
    {ZMQ_AFFINITY            , "ZMQ_AFFINITY"            , SOPT_UINT64} ,
    {ZMQ_SUBSCRIBE           , "ZMQ_SUBSCRIBE"           , SOPT_STRING} ,
    {ZMQ_UNSUBSCRIBE         , "ZMQ_UNSUBSCRIBE"         , SOPT_STRING} ,
    {ZMQ_IDENTITY            , "ZMQ_IDENTITY"            , SOPT_STRING} ,
    {ZMQ_RATE                , "ZMQ_RATE"                , SOPT_INT   } ,
    {ZMQ_RECOVERY_IVL        , "ZMQ_RECOVERY_IVL"        , SOPT_INT   } ,
    {ZMQ_SNDBUF              , "ZMQ_SNDBUF"              , SOPT_INT   } ,
    {ZMQ_RCVBUF              , "ZMQ_RCVBUF"              , SOPT_INT   } ,
    {ZMQ_LINGER              , "ZMQ_LINGER"              , SOPT_INT   } ,
    {ZMQ_RECONNECT_IVL       , "ZMQ_RECONNECT_IVL"       , SOPT_INT   } ,
    {ZMQ_RECONNECT_IVL_MAX   , "ZMQ_RECONNECT_IVL_MAX"   , SOPT_INT   } ,
    {ZMQ_BACKLOG             , "ZMQ_BACKLOG"             , SOPT_INT   } ,
    {ZMQ_MAXMSGSIZE          , "ZMQ_MAXMSGSIZE"          , SOPT_INT64 } ,
    {ZMQ_MULTICAST_HOPS      , "ZMQ_MULTICAST_HOPS"      , SOPT_INT   } ,
    {ZMQ_RCVTIMEO            , "ZMQ_RCVTIMEO"            , SOPT_INT   } ,
    {ZMQ_SNDTIMEO            , "ZMQ_SNDTIMEO"            , SOPT_INT   } ,
    {ZMQ_IPV6                , "ZMQ_IPV6"                , SOPT_INT   } ,
    {ZMQ_IPV4ONLY            , "ZMQ_IPV4ONLY"            , SOPT_INT   } ,
    {ZMQ_IMMEDIATE           , "ZMQ_IMMEDIATE"           , SOPT_INT   } ,
#if defined _WIN32
/*
 _WIN32 is always defined   , even for 64 bits        ,
 see http://jpassing.com/2011/05/02/ifdef-_win32
 http://msdn.microsoft.com/en-us/library/b0084kay.aspx
*/
    {ZMQ_FD                  , "ZMQ_FD"                  , SOPT_INT   } ,
#else
    {ZMQ_FD                  , "ZMQ_FD"                  , SOPT_SOCKET} ,
#endif
    {ZMQ_EVENTS              , "ZMQ_EVENTS"              , SOPT_INT   } ,
    {ZMQ_LAST_ENDPOINT       , "ZMQ_LAST_ENDPOINT"       , SOPT_STRING} ,
    {ZMQ_ROUTER_MANDATORY    , "ZMQ_ROUTER_MANDATORY"    , SOPT_INT   } ,
    {ZMQ_ROUTER_RAW          , "ZMQ_ROUTER_RAW"          , SOPT_INT   } ,
    {ZMQ_PROBE_ROUTER        , "ZMQ_PROBE_ROUTER"        , SOPT_INT   } ,
    {ZMQ_XPUB_VERBOSE        , "ZMQ_XPUB_VERBOSE"        , SOPT_INT   } ,
    {ZMQ_REQ_CORRELATE       , "ZMQ_REQ_CORRELATE"       , SOPT_INT   } ,
    {ZMQ_REQ_RELAXED         , "ZMQ_REQ_RELAXED"         , SOPT_INT   } ,
    {ZMQ_TCP_KEEPALIVE       , "ZMQ_TCP_KEEPALIVE"       , SOPT_INT   } ,
    {ZMQ_TCP_KEEPALIVE_IDLE  , "ZMQ_TCP_KEEPALIVE_IDLE"  , SOPT_INT   } ,
    {ZMQ_TCP_KEEPALIVE_CNT   , "ZMQ_TCP_KEEPALIVE_CNT"   , SOPT_INT   } ,
    {ZMQ_TCP_KEEPALIVE_INTVL , "ZMQ_TCP_KEEPALIVE_INTVL" , SOPT_INT   } ,
    {ZMQ_TCP_ACCEPT_FILTER   , "ZMQ_TCP_ACCEPT_FILTER"   , SOPT_STRING} ,
    {ZMQ_MECHANISM           , "ZMQ_MECHANISM"           , SOPT_INT   } ,
    {ZMQ_PLAIN_SERVER        , "ZMQ_PLAIN_SERVER"        , SOPT_INT   } ,
    {ZMQ_PLAIN_USERNAME      , "ZMQ_PLAIN_USERNAME"      , SOPT_STRING} ,
    {ZMQ_PLAIN_PASSWORD      , "ZMQ_PLAIN_PASSWORD"      , SOPT_STRING} ,
    {ZMQ_CURVE_SERVER        , "ZMQ_CURVE_SERVER"        , SOPT_INT   } ,
    {ZMQ_CURVE_PUBLICKEY     , "ZMQ_CURVE_PUBLICKEY"     , SOPT_KEY   } ,
    {ZMQ_CURVE_SECRETKEY     , "ZMQ_CURVE_SECRETKEY"     , SOPT_KEY   } ,
    {ZMQ_CURVE_SERVERKEY     , "ZMQ_CURVE_SERVERKEY"     , SOPT_KEY   } ,
    {ZMQ_ZAP_DOMAIN          , "ZMQ_ZAP_DOMAIN"          , SOPT_STRING} ,
    {ZMQ_CONFLATE            , "ZMQ_CONFLATE"            , SOPT_INT   } ,
/* END of array (`name` member of string **MUST** be NULL)*/
    {-1                      , NULL                      , -1         }
};

mxArray* uint64_matrix_to_m(void* handler, size_t m, size_t n) {
    mxArray* ret;
    uint64_t** value;
    void** pointer;
    int i, j;

    value = (uint64_t**) handler;

    ret = mxCreateNumericMatrix(m, n, mxUINT64_CLASS, mxREAL);
    pointer = mxGetPr(ret);

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("m: %d, n: %d, d: %d\n", i, j, *(value+n*j+i));
            *(pointer+n*j+i) = *(value+n*j+i);
        }
    }

    return ret;
}

mxArray* int64_matrix_to_m(void* handler, size_t m, size_t n) {
    mxArray* ret;
    int64_t** value;
    void** pointer;
    int i, j;

    value = (int64_t**) handler;

    ret = mxCreateNumericMatrix(m, n, mxUINT64_CLASS, mxREAL);
    pointer = mxGetPr(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            *(pointer+i+j) = *(value+i+j);
        }
    }

    return ret;
}

mxArray* uint32_matrix_to_m(void* handler, size_t m, size_t n) {
    mxArray* ret;
    uint32_t** value;
    void** pointer;
    int i, j;

    value = (uint32_t**) handler;

    ret = mxCreateNumericMatrix(m, n, mxUINT64_CLASS, mxREAL);
    pointer = mxGetPr(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            *(pointer+i+j) = *(value+i+j);
        }
    }

    return ret;
}

mxArray* int_matrix_to_m(void* handler, size_t m, size_t n) {
    mxArray* ret;
    int** value;
    void** pointer;
    int i, j;

    value = (int**) handler;

    ret = mxCreateNumericMatrix(m, n, mxUINT64_CLASS, mxREAL);
    pointer = mxGetPr(ret);

    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            *(pointer+i+j) = *(value+i+j);
        }
    }

    return ret;
}

mxArray* uint64_to_m(void* handler) {
    return uint64_matrix_to_m(handler, 1, 1);
}

mxArray* int64_to_m(void* handler) {
    return uint64_matrix_to_m(handler, 1, 1);
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

#if defined _WIN32
mxArray* sock_to_m(void* handler) {
    mxArray*  ret;
    void** pointer;

    if (sizeof(void*) == 4) {
        ret = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
    } else {
        ret = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
    }

    pointer = (void**) mxGetData(ret);
    pointer[0] = handler;
}
#endif

/* Reference: https://github.com/zeromq/czmq/blob/master/src/zsockopt.gsl*/
const zmq_sockopt_type_t sockopt_type_array[] = {
    {SOPT_UINT64 , sizeof(uint64_t) , uint64_to_m},
    {SOPT_INT64  , sizeof(int64_t)  , int64_to_m },
    {SOPT_UINT32 , sizeof(uint32_t) , uint32_to_m},
    {SOPT_INT    , sizeof(int)      , int_to_m   },
    {SOPT_STRING , 255*sizeof(char) , str_to_m   },
    {SOPT_KEY    , 41*sizeof(char)  , str_to_m   }, /* Z85 key (40) + terminator (1) */
#if defined _WIN32
    {SOPT_SOCKET , sizeof(SOCKET)   , sock_to_m  },
#endif
    {SOPT_INVALID, 0                , NULL       }
};

char* m_to_str(const mxArray* param) {
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

    return str;
}

const zmq_sockopt_desc_t* sockopt_find_by_name(char* option) {
    int i;
    const zmq_sockopt_desc_t* decriptor = NULL;

    for (i = 0; sockopt_array[i].name != NULL; i++) {
        if (!strcmp(option, sockopt_array[i].name)) {
            decriptor = &(sockopt_array[i]);
            break;
        }
    }

    if (decriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:sockopt:invalidOptionName",
                "Error: socket_option %s is invalid.", option);
    }

    return decriptor;
}

const zmq_sockopt_type_t* sockopt_find_type(int type_id) {
    int i;
    const zmq_sockopt_type_t* decriptor = NULL;

    for (i = 0; sockopt_type_array[i].id != SOPT_INVALID; i++) {
        if (type_id == sockopt_type_array[i].id) {
            decriptor = &(sockopt_type_array[i]);
            break;
        }
    }

    if (decriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:sockopt:invalidOptionTypeId",
                "Error: socket_option type_id %d is invalid.", type_id);
    }

    return decriptor;
}

void socket_error() {
    int err;

    err = errno;

    /* Windows users can have problems with errno, see http://api.zeromq.org/master:zmq-errno */
    if (err == 0) err = zmq_errno();

    switch (err) {
        case EINVAL:
            mexErrMsgIdAndTxt("zmq:socket:invalidOptsCore",
                    "Error: The core API reports that something has gone wrong."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case ETERM:
            mexErrMsgIdAndTxt("zmq:socket:contextTerm",
                    "Error: The context associated with the socket was terminated."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case EFAULT:
        case ENOTSOCK:
            mexErrMsgIdAndTxt("zmq:socket:invalidSocket",
                    "Error: Invalid socket."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        case EINTR:
            mexErrMsgIdAndTxt("zmq:socket:interrupted",
                    "Error: The operation was interrupted by a signal."
                    "\n(original message: %s)", zmq_strerror(err));
        break;
        default:
            mexErrMsgIdAndTxt("zmq:getsockopts:unknownOops",
                    "Error: Something has gone very, very wrong. Unknown error."
                    "\n(original message: %s)", zmq_strerror(err));
    }
}

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

    uint64_t vec[2][2] = {{0,1},{2,3}};

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
    option = m_to_str(prhs[1]);
    if (option == NULL) return;

    optdesc = sockopt_find_by_name(option);
    if (optdesc == NULL) return;

    typedesc = sockopt_find_type(optdesc->type_id);
    if (typedesc == NULL) return;
/*DEBUG*/ printf("(%s, %d):\n\tOption %s, length: %d\n", __FILE__, __LINE__, optdesc->name, typedesc->maxlen); //return;

    mxFree(option);

    mexParam = (void**) mxGetData(prhs[0]);
    socket = mexParam[0];
/*DEBUG*/ printf("(%s, %d):\n\tSocket pointer: %p\n", __FILE__, __LINE__, socket);

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
        return;
    }

    plhs[0] = uint64_matrix_to_m((void*) vec, 2, 2);
}