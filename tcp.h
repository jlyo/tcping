#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>

int lookup(char *host, char *portnr, struct addrinfo **res);
int connect_to(struct addrinfo *addr, struct timeval *rtt);
