#include "socket.h"
#include "sockopt.h"
#include "conversions.h"
#include <mex.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

/*
  Lookupt table with metadata used to interpret socket options.

  The field `id` is the constant defined in `zmq.h`.
  The field `name` is the string representing this constant.
  The field `typeId` is a constant, describing the type for option value,
  e.g. utin64, string, etc...

  Information used to construct this table can found in:
  - http://api.zeromq.org/master:zmq-getsockopt
  - http://api.zeromq.org/master:zmq-setsockopt
  - https://github.com/zeromq/czmq/blob/master/src/sockopts.xml

  WARNING: Option type seems to depend on ZMQ version, and documentation is not
  100% reliable.
  TODO:generate this table automatically using iMatix gsl...
 */
static const zmq_sockopt_desc_t sockOptLookup[] = {
    {ZMQ_TYPE                , "ZMQ_TYPE"                , SOPT_SOCKTYPE  } ,
    {ZMQ_RCVMORE             , "ZMQ_RCVMORE"             , SOPT_INT       } ,
    {ZMQ_SNDHWM              , "ZMQ_SNDHWM"              , SOPT_INT       } ,
    {ZMQ_RCVHWM              , "ZMQ_RCVHWM"              , SOPT_INT       } ,
    {ZMQ_AFFINITY            , "ZMQ_AFFINITY"            , SOPT_UINT64    } ,
    {ZMQ_SUBSCRIBE           , "ZMQ_SUBSCRIBE"           , SOPT_STRING    } ,
    {ZMQ_UNSUBSCRIBE         , "ZMQ_UNSUBSCRIBE"         , SOPT_STRING    } ,
    {ZMQ_IDENTITY            , "ZMQ_IDENTITY"            , SOPT_STRING    } ,
    {ZMQ_RATE                , "ZMQ_RATE"                , SOPT_INT       } ,
    {ZMQ_RECOVERY_IVL        , "ZMQ_RECOVERY_IVL"        , SOPT_INT       } ,
    {ZMQ_SNDBUF              , "ZMQ_SNDBUF"              , SOPT_INT       } ,
    {ZMQ_RCVBUF              , "ZMQ_RCVBUF"              , SOPT_INT       } ,
    {ZMQ_LINGER              , "ZMQ_LINGER"              , SOPT_INT       } ,
    {ZMQ_RECONNECT_IVL       , "ZMQ_RECONNECT_IVL"       , SOPT_INT       } ,
    {ZMQ_RECONNECT_IVL_MAX   , "ZMQ_RECONNECT_IVL_MAX"   , SOPT_INT       } ,
    {ZMQ_BACKLOG             , "ZMQ_BACKLOG"             , SOPT_INT       } ,
    {ZMQ_MAXMSGSIZE          , "ZMQ_MAXMSGSIZE"          , SOPT_INT64     } ,
    {ZMQ_MULTICAST_HOPS      , "ZMQ_MULTICAST_HOPS"      , SOPT_INT       } ,
    {ZMQ_RCVTIMEO            , "ZMQ_RCVTIMEO"            , SOPT_INT       } ,
    {ZMQ_SNDTIMEO            , "ZMQ_SNDTIMEO"            , SOPT_INT       } ,
    {ZMQ_IPV6                , "ZMQ_IPV6"                , SOPT_INT       } ,
    {ZMQ_IPV4ONLY            , "ZMQ_IPV4ONLY"            , SOPT_INT       } ,
    {ZMQ_IMMEDIATE           , "ZMQ_IMMEDIATE"           , SOPT_INT       } ,
#if defined _WIN32
/*
  _WIN32 is always defined , even for 64 bits          ,
  see http://jpassing.com/2011/05/02/ifdef-_win32
  http://msdn.microsoft.com/en-us/library/b0084kay.aspx
*/
    {ZMQ_FD                  , "ZMQ_FD"                  , SOPT_SOCKET    } ,
#else
    {ZMQ_FD                  , "ZMQ_FD"                  , SOPT_INT       } ,
#endif
    {ZMQ_EVENTS              , "ZMQ_EVENTS"              , SOPT_INT       } ,
    {ZMQ_LAST_ENDPOINT       , "ZMQ_LAST_ENDPOINT"       , SOPT_STRING    } ,
    {ZMQ_ROUTER_MANDATORY    , "ZMQ_ROUTER_MANDATORY"    , SOPT_INT       } ,
    {ZMQ_ROUTER_RAW          , "ZMQ_ROUTER_RAW"          , SOPT_INT       } ,
    {ZMQ_PROBE_ROUTER        , "ZMQ_PROBE_ROUTER"        , SOPT_INT       } ,
    {ZMQ_XPUB_VERBOSE        , "ZMQ_XPUB_VERBOSE"        , SOPT_INT       } ,
    {ZMQ_REQ_CORRELATE       , "ZMQ_REQ_CORRELATE"       , SOPT_INT       } ,
    {ZMQ_REQ_RELAXED         , "ZMQ_REQ_RELAXED"         , SOPT_INT       } ,
    {ZMQ_TCP_KEEPALIVE       , "ZMQ_TCP_KEEPALIVE"       , SOPT_INT       } ,
    {ZMQ_TCP_KEEPALIVE_IDLE  , "ZMQ_TCP_KEEPALIVE_IDLE"  , SOPT_INT       } ,
    {ZMQ_TCP_KEEPALIVE_CNT   , "ZMQ_TCP_KEEPALIVE_CNT"   , SOPT_INT       } ,
    {ZMQ_TCP_KEEPALIVE_INTVL , "ZMQ_TCP_KEEPALIVE_INTVL" , SOPT_INT       } ,
    {ZMQ_TCP_ACCEPT_FILTER   , "ZMQ_TCP_ACCEPT_FILTER"   , SOPT_STRING    } ,
    {ZMQ_MECHANISM           , "ZMQ_MECHANISM"           , SOPT_MECHANISM } ,
    {ZMQ_PLAIN_SERVER        , "ZMQ_PLAIN_SERVER"        , SOPT_INT       } ,
    {ZMQ_PLAIN_USERNAME      , "ZMQ_PLAIN_USERNAME"      , SOPT_STRING    } ,
    {ZMQ_PLAIN_PASSWORD      , "ZMQ_PLAIN_PASSWORD"      , SOPT_STRING    } ,
    {ZMQ_CURVE_SERVER        , "ZMQ_CURVE_SERVER"        , SOPT_INT       } ,
    {ZMQ_CURVE_PUBLICKEY     , "ZMQ_CURVE_PUBLICKEY"     , SOPT_KEY       } ,
    {ZMQ_CURVE_SECRETKEY     , "ZMQ_CURVE_SECRETKEY"     , SOPT_KEY       } ,
    {ZMQ_CURVE_SERVERKEY     , "ZMQ_CURVE_SERVERKEY"     , SOPT_KEY       } ,
    {ZMQ_ZAP_DOMAIN          , "ZMQ_ZAP_DOMAIN"          , SOPT_STRING    } ,
    {ZMQ_CONFLATE            , "ZMQ_CONFLATE"            , SOPT_INT       } ,
/* END of array (`name` member of string **MUST** be NULL)*/
    {-1                      , NULL                      , -1             }
};

/*
  Lookup table with metadata used to interpret/convert socket option types.

  The field `id` is a constant, identifying the type.
  The field `maxLen` is the maximum number of bytes that can be used for the
  option.
  The field `to_m` is a function pointer, that can be used to convert from C
  value to MATLAB.
 */
static const zmq_sockopt_type_t sockOptTypeLookup[] = {
    {SOPT_UINT64    , sizeof(uint64_t) , uint64_to_m    , uint64_from_m   } ,
    {SOPT_INT64     , sizeof(int64_t)  , int64_to_m     , int64_from_m    } ,
    {SOPT_UINT32    , sizeof(uint32_t) , uint32_to_m    , uint32_from_m   } ,
    {SOPT_INT       , sizeof(int)      , int_to_m       , int_from_m      } ,
    {SOPT_STRING    , 255*sizeof(char) , str_to_m       , str_from_m      } ,
    {SOPT_KEY       , 41*sizeof(char)  , str_to_m       , str_from_m      } , /* Z85 key (40) + terminator (1) */
    {SOPT_SOCKTYPE  , sizeof(int)      , socktype_to_m  , socktype_from_m } ,
    {SOPT_MECHANISM , sizeof(int)      , mechanism_to_m , mechanism_from_m} ,
#if defined _WIN32
    {SOPT_SOCKET    , sizeof(SOCKET)   , pointer_to_m   , pointer_from_m  } ,
#endif
    {SOPT_INVALID   , 0                , NULL           , NULL            }
} ;

/*
  Lookupt table with metadata fields, used to interpret security mechanism.

  The field `id` is the constant defined in `zmq.h`.
  The field `name` is the string representing this constant.
 */
static const zmq_sockopt_mechanism_t sockOptMechanismLookup[] = {
    {ZMQ_NULL , "ZMQ_NULL" } ,
    {ZMQ_PLAIN, "ZMQ_PLAIN"} ,
    {ZMQ_CURVE, "ZMQ_CURVE"} ,
    {-1       , NULL       }
};

/*
  Find the metadata related to the socket option.

  ## Arguments
    - option: pointer to string, containing the name for the option, e.g. "ZMQ_BACKLOG"

  ## Return
  Pointer to a struct with metada (`zmq_sockopt_desc_t`)
 */
const zmq_sockopt_desc_t* find_sockopt_by_name(char* option) {
    int i;
    const zmq_sockopt_desc_t* descriptor = NULL;

    /*
      TODO: as `sockOptLookup` table order can be chosen arbitrarily,
      we can use binary search to speed up this process.
     */
    for (i = 0; sockOptLookup[i].name != NULL; i++) {
        if (!strcmp(option, sockOptLookup[i].name)) {
            descriptor = &(sockOptLookup[i]);
            break;
        }
    }

    if (descriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:core:sockopt:invalidOptionName",
            "Error: socket_option %s is invalid.", option);
    }
    return descriptor;
}

/*
  Find the metadata related to the socket option type.

  ## Arguments
    - typeId: integer identifying the option type, e.g. SOPT_INT64, SOPT_INT, ...

  ## Return
  Pointer to a struct with metada (`zmq_sockopt_type_t`)
 */
const zmq_sockopt_type_t* find_sockopt_type_by_id(int typeId) {
    int i;
    const zmq_sockopt_type_t* descriptor = NULL;

    /*
      TODO: as `typeId` values can be chosen arbitrarily, as well as
      sockOptTypeLookup table order, we can use binary search to speed up this
      process. Although, the table is small...
     */
    for (i = 0; sockOptTypeLookup[i].id != SOPT_INVALID; i++) {
        if (typeId == sockOptTypeLookup[i].id) {
            descriptor = &(sockOptTypeLookup[i]);
            break;
        }
    }
    if (descriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:core:sockopt:invalidOptionTypeId",
            "Error: socket_option typeId %d is invalid.", typeId);
    }
    return descriptor;
}

/*
  Find the metadata related to the socket security mechanism.

  ## Arguments
    - mechanismId: integer identifying the option type, e.g. SOPT_INT64, SOPT_INT, ...

  ## Return
  Pointer to a struct with metada (`zmq_sockopt_type_t`)
 */
const zmq_sockopt_mechanism_t* find_sockopt_mechanism_by_id(int mechanismId) {
    int i;
    const zmq_sockopt_mechanism_t* descriptor = NULL;

    /*
      TODO: as `typeId` values can be chosen arbitrarily, as well as
      sockOptTypeLookup table order, we can use binary search to speed up this
      process. Although, the table is small...
     */
    for (i = 0; sockOptMechanismLookup[i].name != NULL; i++) {
        if (mechanismId == sockOptMechanismLookup[i].id) {
            descriptor = &(sockOptMechanismLookup[i]);
            break;
        }
    }
    if (descriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:core:sockopt:invalidOptionMechanismId",
            "Error: socket_option mechanism %d is invalid.", mechanismId);
    }
    return descriptor;
}

/*
  Find the metadata related to the socket security mechanism.

  ## Arguments
    - name: pointer to string, containing the name for the option, e.g. "ZMQ_NULL"

  ## Return
  Pointer to a struct with metada (`zmq_sockopt_type_t`)
 */
const zmq_sockopt_mechanism_t* find_sockopt_mechanism_by_name(char* name) {
    int i;
    const zmq_sockopt_mechanism_t* descriptor = NULL;

    for (i = 0; sockOptMechanismLookup[i].name != NULL; i++) {
        if (!strcmp(name, sockOptMechanismLookup[i].name)) {
            descriptor = &(sockOptMechanismLookup[i]);
            break;
        }
    }
    if (descriptor == NULL) {
        mexErrMsgIdAndTxt("zmq:core:sockopt:invalidOptionMechanism",
            "Error: socket_option mechanism %s is invalid.", name);
    }
    return descriptor;
}


/* Custom CONSTANT <=> STRING convertions */

mxArray* mechanism_to_m(void* handle) {
    mxArray* ret;
    const zmq_sockopt_mechanism_t* mechanism = NULL;

    mechanism = find_sockopt_mechanism_by_id(*((int*) handle));
    if (mechanism != NULL) ret = (mxArray*) str_to_m((void*) mechanism->name);

    return ret;
}

void* mechanism_from_m(const mxArray* param) {
    int* output = NULL;
    char* name = NULL;
    const zmq_sockopt_mechanism_t* mechanism = NULL;

    output = (int*) mxCalloc(1, sizeof(int));

    if (output == NULL) {
        mexErrMsgIdAndTxt("util:calloc", "Error: Unsuccessful memory allocation.");
    }

    name = (char*) str_from_m(param);
    if (name == NULL) return NULL;

    mechanism = find_sockopt_mechanism_by_name(name);
    if (mechanism != NULL) *output = mechanism->id;

    mxFree(name);

    return (void*) output;
}

