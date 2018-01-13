#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

int main(void)
{
	FILE *pF = fopen("1.txt", "w+");
	if (pF == NULL)
	{
		printf("%s:%d errno = %d, means: %s\n", __func__, __LINE__, errno, strerror(errno));
		return -1;
	}

	uint8_t buf[BUFSIZ] = {0};
	printf("%s:%d file stream buffer size = %d\n", __func__, __LINE__, BUFSIZ);
	size_t bufSize = sizeof(buf);
	for (size_t i = 0; i < bufSize; ++i)
	{
		buf[i] = i % 256;
	}

	// 用strace ./a.out跟踪，fwrite会收集到BUFSIZ/2时，再调用write
	for (size_t i = 0; i < 8; ++i)
	{
		if (fwrite(buf + i * bufSize / 8, bufSize / 8, 1, pF) != 1)
		{
			printf("%s:%d errno = %d, means: %s\n", __func__, __LINE__, errno, strerror(errno));
			break;
		}
	}

	if (fseek(pF, -(long)bufSize / 2, SEEK_CUR) == 0)
		printf("%s:%d current position %ld\n", __func__, __LINE__, ftell(pF));
	else
		printf("%s:%d errno = %d, means: %s\n", __func__, __LINE__, errno, strerror(errno));

	fclose(pF);
	remove("1.txt");
	return 0;
}

