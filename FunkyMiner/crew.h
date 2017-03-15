#pragma once

#define DIFF 6
#define INSTANCE_SIZE 6

typedef struct {
	char *data;
	char *nonce;
	unsigned int nonce_size;
	unsigned long long counter;
	int success;
} Miner;

typedef struct {
	int powerdown;
	unsigned int count;
	Miner **miners;
} MinersCrew;

void create_crew(MinersCrew *crew, unsigned int count, const char *instance);
void destroy_crew(MinersCrew *crew);
