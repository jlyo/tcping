#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

ssize_t myread(int fd, char *whereto, size_t len)
{
	ssize_t cnt=0;

	while(len>0)
	{
		ssize_t rc;

		rc = read(fd, whereto, len);

		if (rc == -1)
		{
			if (errno != EINTR)
				return -1;
		}
		else if (rc == 0)
		{
			break;
		}
		else
		{
			whereto += rc;
			len -= rc;
			cnt += rc;
		}
	}

	return cnt;
}

ssize_t mywrite(int fd, char *wherefrom, size_t len)
{
	ssize_t cnt=0;

	while(len>0)
	{
		ssize_t rc;

		rc = write(fd, wherefrom, len);

		if (rc == -1)
		{
			if (errno != EINTR)
				return -1;
		}
		else if (rc == 0)
		{
			break;
		}
		else
		{
			wherefrom += rc;
			len -= rc;
			cnt += rc;
		}
	}

	return cnt;
}
