#include "crew.h"
#include "stdafx.h"

#define GET_NONCE_SIZE() (rand() % 8 + 18)

struct MinerData {
	Miner *miner;
	MinersCrew *crew;
};

char get_random_char() {
	char c = rand() % 37 + 87;
	return (c < 'a' ? (c - 39) : c);
}

void feed_initial(Miner *miner) {
	unsigned register int i;
	for (i = 0; i < miner->nonce_size; ++i) {
		miner->nonce[i] = get_random_char();
	}
}

void feed_next(Miner *miner) {
	register int i;
	for (i = 0; i < 4; ++i) {
		miner->nonce[rand() % miner->nonce_size] = get_random_char();
	}
}

void miner_thread(struct MinerData *data) {
	Miner *miner = data->miner;
	while(data->crew->result == NULL) {
		feed_next(miner);

		SHA256_CTX ctx;
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, miner->data, miner->data_length);
		SHA256_Final(miner->hash, &ctx);

		++miner->counter;
		if(!memcmp(miner->hash, data->crew->zeroes, DIFF)) {
			data->crew->result = miner->data;
		}
	}

	free(data);
}

void create_crew(MinersCrew *crew, unsigned int count, const char *instance) {
	memset(crew, 0, sizeof(MinersCrew));
	crew->miners = malloc(sizeof(void*) * count);
	crew->zeroes = malloc(SHA256_DIGEST_LENGTH);
	memset(crew->zeroes, 0, SHA256_DIGEST_LENGTH);
	crew->count = count;

	unsigned int i; for (i = 0; i < count; i++) {
		Miner *miner = malloc(sizeof(Miner));
		memset(miner, 0, sizeof(Miner));
		miner->nonce_size = GET_NONCE_SIZE();

		miner->data = malloc(43); // Max possible FunkyStore's MagicFSC1 voucher code length + zero byte
		miner->nonce = (char*)((long)miner->data + 10 + INSTANCE_SIZE);
		miner->data_length = 10 + INSTANCE_SIZE + miner->nonce_size;
		miner->hash = malloc(SHA256_DIGEST_LENGTH);
		sprintf(miner->data, "%s%s", "sw-101499-", instance); // Code header, telling us that it is single-usable mined money voucher
		feed_initial(miner);

		struct MinerData *data = malloc(sizeof(struct MinerData));
		data->miner = miner;
		data->crew = crew;
		make_thread(miner_thread, data);
		crew->miners[i] = miner;
	}
}

void destroy_crew(MinersCrew *crew) {
	unsigned int i;
	for (i = 0; i < crew->count; i++) {
		free(crew->miners[i]->data);
		free(crew->miners[i]->hash);
		free(crew->miners[i]);
	}

	free(crew->miners);
	free(crew->zeroes);
	free(crew);
}
