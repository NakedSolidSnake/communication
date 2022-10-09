#ifndef UDP_H_
#define UDP_H_

#include <stdbool.h>
#include <stdlib.h>

typedef void (*event_t) (char *buffer, size_t *size, void *data);

typedef enum 
{
    udp_type_server,
    udp_type_client
} udp_type_t;

typedef struct 
{
    int socket;
    udp_type_t type;
    const char *hostname;
    const char *port;
    char *buffer;
    size_t size;
    event_t on_receive;
    event_t on_send;
    void *data;
} udp_t;

typedef struct 
{
    udp_type_t type;
    const char *hostname;
    const char *port;
    char *buffer;
    size_t size;
    event_t on_receive;
    event_t on_send;
    void *data;
} udp_args_t;

bool udp_init (udp_t *udp);
bool udp_open (udp_t *udp, udp_args_t *args);
bool udp_run (udp_t *udp);
bool udp_send (udp_t *udp, const char *data, size_t size);
bool udp_receive (udp_t *udp, char *data, size_t *size);
bool udp_close (udp_t *udp);

#endif /* UDP_H_ */
