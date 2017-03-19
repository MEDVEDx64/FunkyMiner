#include "crew.h"
#include "threading.h"
#include "stdafx.h"

#define GET_NONCE_SIZE() (rand() % 8 + 18)

struct MinerData {
	Miner *miner;
	MinersCrew *crew;
};

char get_random_char() {
	unsigned char r = 0;
	RAND_bytes(&r, 1);
	char c = r % 36 + 87;
	return (c < 'a' ? (c - 39) : c);
}

void feed_initial(Miner *miner) {
	unsigned register int i;
	for (i = 0; i < miner->nonce_size; ++i) {
		miner->nonce[i] = get_random_char();
	}
}

void feed_next(Miner *miner) {
	unsigned register int i;
	for (i = 0; i < miner->nonce_size; ++i) {
		miner->nonce[i]++;
		if (miner->nonce[i] == ':') {
			miner->nonce[i] = 'a';
			break;
		}

		if (miner->nonce[i] == '{') {
			miner->nonce[i] = '0';
			continue;
		}

		break;
	}
}

void miner_thread(struct MinerData *data) {
	Miner *miner = data->miner;
	int *hash = (int*)miner->hash;
	while(data->crew->result == NULL) {
		feed_next(miner);

		SHA256_CTX ctx;
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, miner->data, miner->data_length);
		SHA256_Final(miner->hash, &ctx);

		++miner->counter;
		if(!(*hash)) {
			data->crew->result = miner->data;
		}
	}

	free(data);
}

void create_crew(MinersCrew *crew, unsigned int count, const char *instance) {
	memset(crew, 0, sizeof(MinersCrew));
	crew->miners = malloc(sizeof(void*) * count);
	crew->count = count;

	unsigned int i; for (i = 0; i < count; i++) {
		Miner *miner = malloc(sizeof(Miner));
		memset(miner, 0, sizeof(Miner));
		miner->nonce_size = GET_NONCE_SIZE();

		miner->data = malloc(43); // Max possible FunkyStore's MagicFSC1 voucher code length + zero byte
		memset(miner->data, 0, 43);
		miner->nonce = (char*)((long)miner->data + 10 + INSTANCE_SIZE);
		miner->data_length = 10 + INSTANCE_SIZE + miner->nonce_size;
		miner->hash = malloc(SHA256_DIGEST_LENGTH);
		sprintf(miner->data, "%s%s", "sw-101499-", instance); // Code header, telling us that it is single-usable mined money voucher
		feed_initial(miner);

		struct MinerData *data = malloc(sizeof(struct MinerData));
		data->miner = miner;
		data->crew = crew;
		make_thread((void (*)(void*))miner_thread, data);
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
}
