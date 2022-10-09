#include "udp_client.h"
#include <sys/socket.h>
#include <arpa/inet.h>

static bool udp_client_set_socket (udp_t *udp);

bool udp_client_open (udp_t *udp, udp_args_t *args)
{
    bool status = false;

    if (args->hostname != NULL && args->port != NULL)
    {
        udp->hostname = args->hostname;
        udp->port = args->port;

        status = udp_client_set_socket (udp);
    }

    return status;
}

static bool udp_client_set_socket (udp_t *udp)
{
    bool status = false;

    udp->socket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udp->socket >= 0)
        status = true;

    return status;
}