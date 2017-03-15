#pragma once

#define DIFF 6
#define INSTANCE_SIZE 6

typedef struct {
	char *data;
	char *nonce;
	void *hash;
	unsigned short data_length;
	unsigned int nonce_size;
	unsigned long long counter;
	int success;
} Miner;

typedef struct {
	char *result;
	unsigned int count;
	Miner **miners;
	void *zeroes;
} MinersCrew;

void create_crew(MinersCrew *crew, unsigned int count, const char *instance);
void destroy_crew(MinersCrew *crew);
