
#include <zmq.h>
#include <stdio.h>
//#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <iostream>

#define topicfilter 10001

int main(void)
{

    printf("Connecting to server\n");
    void * context = zmq_ctx_new();
    void * subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_connect(subscriber, "tcp://localhost:5552");
    int rc = zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, "", 0);
    assert (rc == 0);
    char buffer[20];

    for (int i = 0; i < 1000; ++i) {

        int a = zmq_recv(subscriber, buffer, 20, 0);
        assert(a != -1);
        std::cout << "Received" << buffer << std::endl;

    }
    
    zmq_close(subscriber);
    zmq_ctx_destroy(context);
    return 0;


}

