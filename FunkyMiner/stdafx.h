#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <openssl/sha.h>

#ifdef _WIN32

#include <Windows.h>

#else

#include <ctype.h>
#include <unistd.h>
#include <pthread.h>

#endif
