#pragma once

// Simple wrapper for Win32/Linux portability
void make_thread(void(*routine)(void*), void *arg);
void go_sleep(unsigned int millis);
