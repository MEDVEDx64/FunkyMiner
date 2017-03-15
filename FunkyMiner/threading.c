#include "threading.h"
#include "stdafx.h"

#ifdef _WIN32

struct W32ThrData {
	void(*routine)(void*);
	void *arg;
};

DWORD WINAPI thrproc(LPVOID param) {
	struct W32ThrData *data = (struct W32ThrData*)param;
	data->routine(data->arg);
	free(param);
	return 0;
}

void make_thread(void(*routine)(void*), void *arg) {
	struct W32ThrData *data = malloc(sizeof(struct W32ThrData));
	data->routine = routine;
	data->arg = arg;
	CreateThread(NULL, 0, thrproc, data, 0, NULL);
}

void sleep(unsigned int millis) {
	Sleep(millis);
}

#else

#endif
