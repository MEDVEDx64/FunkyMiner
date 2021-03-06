#pragma once

#define INSTANCE_SIZE 6

typedef struct {
	char *data;
	char *nonce;
	void *hash;
	unsigned short data_length;
	unsigned int nonce_size;
	unsigned long long counter;
} Miner;

typedef struct {
	char *result;
	unsigned int count;
	Miner **miners;
} MinersCrew;

void create_crew(MinersCrew *crew, unsigned int count, const char *instance);
void destroy_crew(MinersCrew *crew);
