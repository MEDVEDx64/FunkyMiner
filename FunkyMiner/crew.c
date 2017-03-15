#include "crew.h"
#include "stdafx.h"

#define GET_NONCE_SIZE() (rand() % 8 + 18)

void create_crew(MinersCrew *crew, unsigned int count, const char *instance) {
	memset(crew, 0, sizeof(MinersCrew));
	crew->miners = malloc(sizeof(void*) * count);
	crew->count = count;
	unsigned int i; for (i = 0; i < count; i++) {
		Miner *miner = malloc(sizeof(Miner));
		memset(miner, 0, sizeof(Miner));
		miner->nonce_size = GET_NONCE_SIZE();

		miner->data = malloc(43); // Max possible FunkyStore's MagicFSC1 voucher code length + zero byte
		miner->nonce = (char*)((int)miner->data + 10 + INSTANCE_SIZE);
		sprintf(miner->data, "%s", "sw-101499-"); // Code header, telling us that it is single-usable mined money voucher

		crew->miners[i] = miner;
	}
}

void destroy_crew(MinersCrew *crew) {
}
