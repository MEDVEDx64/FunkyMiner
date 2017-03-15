#include "threading.h"
#include "stdafx.h"

struct ThrData {
	void(*routine)(void*);
	void *arg;
};

#ifdef _WIN32

DWORD WINAPI thrproc(LPVOID param) {
	struct ThrData *data = (struct ThrData*)param;
	data->routine(data->arg);
	free(param);
	return 0;
}

void make_thread(void(*routine)(void*), void *arg) {
	struct ThrData *data = malloc(sizeof(struct ThrData));
	data->routine = routine;
	data->arg = arg;
	CreateThread(NULL, 0, thrproc, data, 0, NULL);
}

void go_sleep(unsigned int millis) {
	Sleep(millis);
}

#else

void *thrproc(void *param) {
	struct ThrData *data = (struct ThrData*)param;
	data->routine(data->arg);
	free(param);
	return NULL;
}

void make_thread(void(*routine)(void*), void *arg) {
	pthread_t thread;
	struct ThrData *data = malloc(sizeof(struct ThrData));
	data->routine = routine;
	data->arg = arg;
	pthread_create(&thread, NULL, thrproc, arg);
}

void go_sleep(unsigned int millis) {
	sleep(millis * 1000);
}

#endif
