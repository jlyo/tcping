#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "tcp.h"

int connect_to(char *host, char *portnr, struct timeval *rtt)
{
    int fd;
    struct addrinfo hints;
    struct addrinfo *resolved;
    struct addrinfo *addr;
    int errcode;
    struct timeval start;
    int connect_result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_protocol = 0;

    resolved = NULL;
    if ((errcode = getaddrinfo(host, portnr, &hints, &resolved)) != 0)
    {
        fprintf(stderr, "could not resolve %s:%s: %s\n", host, portnr, gai_strerror(errcode));
        goto connect_to_error;
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
            goto connect_to_error;
        }

        if (gettimeofday(&start, NULL) == -1)
        {
            perror("gettimeofday");
            goto connect_to_error;
        }

        /* connect to peer */
        connect_result = connect(fd, addr->ai_addr, addr->ai_addrlen);
        if ((connect_result == 0) || (errno == ECONNREFUSED))
        {
            if (gettimeofday(rtt, NULL) == -1)
            {
                perror("gettimeofday");
                goto connect_to_error;
            }
            close(fd);
            freeaddrinfo(resolved);
            resolved = NULL;
            rtt->tv_sec = rtt->tv_sec - start.tv_sec;
            rtt->tv_usec = rtt->tv_usec - start.tv_usec;
            return 0;
        }

        close(fd);
        addr = addr->ai_next;
    }

connect_to_error:
    if (resolved != NULL)
    {
        freeaddrinfo(resolved);
        resolved = NULL;
    }
    return -1;
}
