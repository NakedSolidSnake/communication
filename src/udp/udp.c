#include <udp.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "client/udp_client.h"
#include "server/udp_server.h"

typedef bool (*udp_open_t) (udp_t *udp, udp_args_t *args);

static bool udp_type_open (udp_t *udp, udp_args_t *args);
static bool udp_type_error_open (udp_t *udp, udp_args_t *args);

bool udp_init (udp_t *udp)
{
    bool status = false;

    if (udp != NULL)
    {
        memset (udp, 0, sizeof (udp_t));
        status = true;
    }

    return status;
}

bool udp_open (udp_t *udp, udp_args_t *args)
{
    bool status = false;

    if (udp != NULL && args != NULL)
    {
        status = udp_type_open (udp, args);
    }

    return status;
}

bool udp_run (udp_t *udp)
{
    bool status = false;

    while (udp != NULL && udp->type == udp_type_server)
    {
        udp_server_run (udp);
    }

    return status;
}

bool udp_send (udp_t *udp, const char *data, size_t size)
{
    bool status = false;
    struct sockaddr_in address_in;
    size_t _size;

    if (udp != NULL && data != NULL && size > 0)
    {
        memset (&address_in, 0, sizeof (struct sockaddr_in));
        address_in.sin_family = AF_INET;
        address_in.sin_addr.s_addr = inet_addr (udp->hostname);
        address_in.sin_port = htons (atoi(udp->port));

        _size = sendto (udp->socket, data, size, 0, (struct sockaddr *)&address_in, sizeof (address_in));
        status = _size == size;
    }

    return status;
}

bool udp_receive (udp_t *udp, char *data, size_t *size)
{
    bool status = false;
    struct sockaddr_in address_in;
    socklen_t length = sizeof (address_in);
    size_t _size = 0;

    if (udp != NULL && data != NULL && size > 0)
    {
        memset (data, 0, *size);
        _size = recvfrom (udp->socket, data, *size, MSG_WAITALL, (struct sockaddr *)&address_in, &length);
        data[_size] = 0;
        status = true;
    }

    return status;
}

bool udp_close (udp_t *udp)
{
    bool status = false;

    if (udp != NULL)
    {
        close (udp->socket);
        memset (udp, 0, sizeof (udp_t));
        status = true;
    }

    return status;
}

static bool udp_type_open (udp_t *udp, udp_args_t *args)
{
    udp_open_t open;

    udp->type = args->type;

    if (udp->type == udp_type_server)
        open = udp_server_open;

    else if (udp->type == udp_type_client)
        open = udp_client_open;

    else 
        open = udp_type_error_open;

    return open (udp, args);
}

static bool udp_type_error_open (udp_t *udp, udp_args_t *args)
{
    (void)udp;
    (void)args;
    return false;
}