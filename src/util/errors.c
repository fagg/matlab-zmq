#include "errors.h"
#include <zmq.h>
#include <mex.h>
#include <stdio.h>
#include <errno.h>

static const error_desc_t errorLookup[] = {
    {EADDRINUSE      , "EADDRINUSE"     },
    {EADDRNOTAVAIL   , "EADDRNOTAVAIL"  },
    {EAGAIN          , "EAGAIN"         },
    {EFAULT          , "EFAULT"         },
    {EFSM            , "EFSM"           },
    {EHOSTUNREACH    , "EHOSTUNREACH"   },
    {EINTR           , "EINTR"          },
    {EINVAL          , "EINVAL"         },
    {EMFILE          , "EMFILE"         },
    {EMTHREAD        , "EMTHREAD"       },
    {ENOCOMPATPROTO  , "ENOCOMPATPROTO" },
    {ENODEV          , "ENODEV"         },
    {ENOMEM          , "ENOMEM"         },
    {ENOTSOCK        , "ENOTSOCK"       },
    {ENOTSUP         , "ENOTSUP"        },
    {EPROTONOSUPPORT , "EPROTONOSUPPORT"},
    {ETERM           , "ETERM"          },
    {0               , "unknownOops"    }
};

const error_desc_t* find_error_by_id(int id) {
    int i;

    for (i = 0; errorLookup[i].id != 0; i++) {
        if (errorLookup[i].id == id) break;
    }

    return &(errorLookup[i]);
}

void handle_error() {
    int err;
    char identifier[255]; /* I think it's large enough */
    const char* message;
    const char* caller;
    const error_desc_t* errDesc;

    err = errno;
    /* Windows users can have problems with errno, see http://api.zeromq.org/master:zmq-errno */
    if (err == 0)
        err = zmq_errno();

    if (err) {
        message = (const char*) zmq_strerror(err);
        caller  = mexFunctionName();
        errDesc = find_error_by_id(err);

        if (message == NULL)
            message = "Error: Something has gone very, very wrong. Unknown error.";

        if (caller == NULL) {
            sprintf(identifier, "zmq:%s", errDesc->name);
        } else {
            sprintf(identifier, "zmq:core:%s:%s", caller, errDesc->name);
        }

        mexErrMsgIdAndTxt(identifier, message);
    }
}