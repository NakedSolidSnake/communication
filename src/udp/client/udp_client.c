#include "udp_client.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

static bool udp_client_set_socket (udp_t *udp);
static bool udp_client_get_ip_by_hostname (udp_t *udp, udp_args_t *args);

bool udp_client_open (udp_t *udp, udp_args_t *args)
{
    bool status = false;

    if (args->hostname != NULL && args->port != NULL)
    {
        strncpy (udp->hostname, args->hostname, UDP_HOSTNAME_SIZE);
        udp->port = args->port;

        if (udp_client_get_ip_by_hostname (udp, args) == true &&
            udp_client_set_socket (udp) == true)
            status = true;
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

static bool udp_client_get_ip_by_hostname (udp_t *udp, udp_args_t *args)
{
    struct hostent *he = NULL;
    bool status = false;

    he = gethostbyname (args->hostname);
    
    if (he != NULL)
    {
        strncpy (udp->hostname, inet_ntoa(*(struct in_addr*)he->h_addr), UDP_HOSTNAME_SIZE);
        status = true;
    }

    return status;
}