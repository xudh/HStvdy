#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/time.h>
#include <sys/select.h>
#include <pthread.h>

static pthread_mutex_t t_mutex = PTHREAD_MUTEX_INITIALIZER; 
static pthread_cond_t t_cond = PTHREAD_COND_INITIALIZER; 
 
void ParseEvent(const struct input_event *Evt)
{
	switch (Evt->type) {
		case EV_KEY:
			printf("鼠标按键\t\t");
			switch (Evt->code) {
				case BTN_LEFT:
					printf("左键\t\t");
					break;
				case BTN_RIGHT:
					printf("右键\t\t");
					break;
				case BTN_MIDDLE:
					printf("中轮\t\t");
					break;
				default:
					break;
			}
			printf("%s\n", (Evt->value == 1) ? "按下" : "松开");
			break;
		case EV_REL:
			printf("相对坐标\t\t");
			switch (Evt->code) {
				case REL_X:
					printf("横向\t\t");
					break;
				case REL_Y:
					printf("纵向\t\t");
					break;
				case REL_WHEEL:
					printf("中轮\t\t");
					break;
				default:
					break;
			}
			printf("%d\n", Evt->value);
			break;
		case EV_ABS:
			printf("绝对坐标\t\t");
			switch (Evt->code) {
				case ABS_X:
					printf("横向\t\t");
					break;
				case ABS_Y:
					printf("纵向\t\t");
					break;
				case ABS_WHEEL:
					printf("中轮\t\t");
					break;
				default:
					break;
			}
			printf("%d\n", Evt->value);
			break;
		default:
			break;
	}
}

void ParseMice(const char mic[])
{
	int iType = mic[0] & 0x07;
	switch (iType) {
		case 0:
			printf("移动\t\t");
			break;
		case 1:
			printf("左键\t\t");
			break;
		case 2:
			printf("右键\t\t");
			break;
		case 4:
			printf("中轮\t\t");
			break;
		default:
			break;
	}

	printf("相对坐标\t\t横向: %d, 纵向: %d\n", mic[1], mic[2]);
}

void *MonitorEvent(void *arg)
{
	int iEventFd = open("/dev/input/event2", O_RDONLY);	// cat /proc/bus/input/devices能看到鼠标对应的设备文件和事件文件，
	if (iEventFd <= 0) {
		printf("Can't open /dev/input/event2.\n");
		return NULL;
	}

	fd_set rfds;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	struct input_event inputEvt;
	size_t iSize = sizeof(inputEvt);

	while (true) {
		FD_ZERO(&rfds);	
		FD_SET(iEventFd, &rfds);
		if (select(iEventFd + 1, &rfds, NULL, NULL, &tv) > 0) {
			if (read(iEventFd, &inputEvt, iSize) == iSize) {
				pthread_mutex_lock(&t_mutex);
				ParseEvent(&inputEvt);
				pthread_mutex_unlock(&t_mutex);
			}
		}
	}

	close(iEventFd);
	pthread_cond_signal(&t_cond);
}

void *MonitorMice(void *arg)
{
	int iMiceFd = open("/dev/input/mice", O_RDONLY);	// 所有类鼠标设备的坐标值都在/dev/input/mice里
	if (iMiceFd <= 0) {
		printf("Can't open /dev/input/mice.\n");
		return NULL;
	}

	fd_set rfds;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	char cBuf[3] = "";
	size_t iSize = sizeof(cBuf);

	while (true) {
		FD_ZERO(&rfds);	
		FD_SET(iMiceFd, &rfds);
		if (select(iMiceFd + 1, &rfds, NULL, NULL, &tv) > 0) {
			if (read(iMiceFd, cBuf, iSize) == iSize) {
				pthread_mutex_lock(&t_mutex);
				ParseMice(cBuf);
				pthread_mutex_unlock(&t_mutex);
			}
		}
	}

	close(iMiceFd);
	pthread_cond_signal(&t_cond);
}

int main(void)
{
	pthread_t tid = 0;

	// 两种方式都无法获取到绝对坐标
	pthread_create(&tid, NULL, MonitorEvent, NULL);
	pthread_create(&tid, NULL, MonitorMice, NULL);

	pthread_cond_wait(&t_cond, &t_mutex);

	return 0;
}

