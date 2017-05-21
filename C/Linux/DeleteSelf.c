// 执行时会删除自己
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (chmod(argv[0], S_IWUSR|S_IWGRP|S_IWOTH) == 0)
	{
		if (unlink(argv[0]) == 0)
			return 0;
		else
		{
			printf("%s:%d errno = %d, means: %s\n", __func__, __LINE__, errno, strerror(errno));
			return -1;
		}
	}
	else
	{
		printf("%s:%d errno = %d, means: %s\n", __func__, __LINE__, errno, strerror(errno));
		return -1;
	}
}

