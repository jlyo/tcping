#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "tcp.h"

#define incopy(a)       *((struct in_addr *)a)

int connect_to(char *host, char *portnr)
{
    int fd;
    struct addrinfo hints;
    struct addrinfo *resolved;
    struct addrinfo *addr;
    int errcode;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_protocol = 0;

    if ((errcode = getaddrinfo(host, portnr, &hints, &resolved)) != 0)
    {
        fprintf(stderr, "could not resolve %s:%s: %s\n", host, portnr, gai_strerror(errcode));
        exit(2);
    }

    /* try to connect for each of the entries: */
    addr = resolved;
    while (addr != NULL)
    {
        /* create socket */
        fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (fd == -1)
        {
            perror("problem creating socket ");
            exit(2);
        }

        /* connect to peer */
        if (connect(fd, addr->ai_addr, addr->ai_addrlen) == 0)
        {
            /* connection made, return */
            freeaddrinfo(resolved);
            return fd;
        }
        close(fd);
        addr = addr->ai_next;
    }

    freeaddrinfo(resolved);
    return -1;
}
