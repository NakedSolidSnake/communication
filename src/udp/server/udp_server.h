#ifndef UDP_SERVER_H_
#define UDP_SERVER_H_

#include <udp.h>

bool udp_server_open (udp_t *udp, udp_args_t *args);
bool udp_server_run (udp_t *udp);

#endif /* UDP_SERVER_H_ */
