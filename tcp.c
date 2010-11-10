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

int connect_to(char *host, int portnr)
{
        int     fd;
        int     loop;
        struct sockaddr_in      addr;
        struct hostent  *hostdnsentries;

        /* create socket */
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd == -1)
        {
                perror("problem creating socket ");
                exit(2);
        }

        hostdnsentries = gethostbyname(host);
        if (hostdnsentries == NULL)
        {
                fprintf(stderr, "could not resolve %s:%d: %s\n", host, portnr, strerror(errno));
                close(fd);
                exit(2);
        }

        /* initialize address structure */
        memset((void *)&addr, 0, sizeof(addr));
        addr.sin_port   = htons(portnr);
        addr.sin_family = hostdnsentries -> h_addrtype;

        /* try to connect for each of the entries: */
        for(loop=0; ; loop++)
        {
                if ((hostdnsentries -> h_addr_list[loop]) == NULL)
                        break;

                addr.sin_addr = incopy(hostdnsentries -> h_addr_list[loop]);

                /* connect to peer */
                if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
                {
                        /* connection made, return */
                        return fd;
                }
        }

        close(fd);

        return -1;
}
