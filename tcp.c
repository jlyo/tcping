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

int lookup(char *host, char *portnr, struct addrinfo **res)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_protocol = 0;

    return getaddrinfo(host, portnr, &hints, res);
}

int connect_to(struct addrinfo *addr, struct timeval *rtt)
{
    int fd;
    struct timeval start;
    int connect_result;
    int errno_save;

    /* try to connect for each of the entries: */
    while (addr != NULL)
    {
        /* create socket */
        fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (fd == -1)
        {
            return -errno;
        }

        if (gettimeofday(&start, NULL) == -1)
        {
            errno_save = errno;
            close(fd);
            errno = errno_save;
            return -errno;
        }

        /* connect to peer */
        connect_result = connect(fd, addr->ai_addr, addr->ai_addrlen);
        if ((connect_result == 0) || (errno == ECONNREFUSED))
        {
            if (gettimeofday(rtt, NULL) == -1)
            {
                errno_save = errno;
                close(fd);
                return -errno_save;
            }
	    errno_save = errno;
	    close(fd);
	    errno = errno_save;
            rtt->tv_sec = rtt->tv_sec - start.tv_sec;
            rtt->tv_usec = rtt->tv_usec - start.tv_usec;
            return 0;
        }

	errno_save = errno;
        close(fd);
	errno = errno_save;
        addr = addr->ai_next;
    }

    return errno ? -errno : -1;
}
