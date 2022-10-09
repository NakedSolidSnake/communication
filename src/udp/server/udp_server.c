#include "udp.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

static bool udp_server_is_args_valid (udp_args_t *args);
static bool udp_server_configure (udp_t *udp);
static bool udp_server_set_socket (udp_t *udp);
static bool udp_server_set_reuse_address (udp_t *udp);
static bool udp_server_set_bind (udp_t *udp);

bool udp_server_open (udp_t *udp, udp_args_t *args)
{
    bool status = false;

    if (udp_server_is_args_valid (args) == true)
    {
        udp->buffer = args->buffer;
        udp->size = args->size;
        udp->port = args->port;
        udp->on_receive = args->on_receive;
        udp->on_send = args->on_send;

        status = udp_server_configure (udp);
    }

    return status;
}

bool udp_server_run (udp_t *udp)
{
    bool status = false;
    struct sockaddr_in address_in;
    socklen_t length = sizeof (address_in);
    size_t size = 0;

    if (udp->socket >= 0)
    {
        size = recvfrom (udp->socket, udp->buffer, udp->size, MSG_WAITALL, (struct sockaddr *)&address_in, &length);
        udp->buffer [size] = 0;

        if (udp->on_receive)
            udp->on_receive (udp->buffer, &size, udp->data);

        if (udp->on_send)
        {
            udp->on_send (udp->buffer, &size, udp->data);
            size = size > udp->size ? udp->size : size;
            sendto (udp->socket, udp->buffer, size, 0, (struct sockaddr *)&address_in, sizeof (address_in));
        }

        memset (&address_in, 0, sizeof (struct sockaddr_in));
        memset (udp->buffer, 0, udp->size);

        status = true;
    }

    return status;
}

static bool udp_server_is_args_valid (udp_args_t *args)
{
    bool status = false;

    if (args->buffer != NULL && 
        args->port != NULL && 
        args->size > 0)
        status = true;

    return status;
}

static bool udp_server_configure (udp_t *udp)
{
    bool status = false;

    if (udp_server_set_socket (udp) == true && 
        udp_server_set_reuse_address (udp) == true && 
        udp_server_set_bind (udp) == true)
        status = true;

    return status;
}
static bool udp_server_set_socket (udp_t *udp)
{
    bool status = false;

    udp->socket = socket (AF_INET, SOCK_DGRAM, 0);
    if (udp->socket >= 0)
        status = true;

    return status;
}

static bool udp_server_set_reuse_address (udp_t *udp)
{
    bool status = false;
    int yes = 1;

    if (setsockopt (udp->socket, SOL_SOCKET, SO_REUSEADDR, (void *)&yes, sizeof (yes)) == 0)
        status = true;
    return status;
}

static bool udp_server_set_bind (udp_t *udp)
{
    bool status = false;
    struct sockaddr_in address;

    memset (&address, 0, sizeof (struct sockaddr_in));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons (atoi (udp->port));

    if (bind (udp->socket, (const struct sockaddr *)&address, sizeof (struct sockaddr_in)) == 0)
        status = true;

    return status;
}