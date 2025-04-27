/* Copyright (c) 2025 Lim Troev (pseudonim) [PsiCrypt Project]
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 */

#define MULTITHREAD

#define DATA_SIZE_MB     16
#define NUM_THREADS      10
#define BLOCK_SIZE        (2LL * 1024 * 1024)

#define DATA_SIZE        (DATA_SIZE_MB * 1024LL * 1024LL)

#define MAGIC_FRAME_SIZE 1177
#define MASTER_KEY_LEN   64     // 512 bit Master key
#define NONCE_LEN        32     // 256 bit nonce
#define KEY_HASH_SIZE    64     // 512-bit hash master key + nonce

#include "sfcms.h"
//-------------------------------------------------------------------------
//	      SFCMC (c) 2025 Lim Troev (pseudonim) [PsyCrypt Project]
//-------------------------------------------------------------------------
//Core SFCMC ->
uint32_t __forceinline gen_keystream_byte(size_t idx,const PSFCMC psfmc)
{
	uint64_t h2 = psfmc->hash_key[fast_idx(wyrand(&idx),KEY_HASH_SIZE)];
	uint64_t u = fast_idx(idx,64) ^ h2;
	uint64_t chaos = wyrand(&u);
	uint32_t ripv = psfmc->stream[fast_idx(wyrand(&idx),MAGIC_FRAME_SIZE)];
	chaos ^= arx_round((uint32_t)chaos, ripv);
	return (uint32_t)chaos;
}

void sfcmc_crypt(uint8_t* data,PSFCMC psfmc)
{
	for(size_t i = 0; i < DATA_SIZE; ++i)
	{
		uint32_t ks = gen_keystream_byte(i, psfmc);
		data[i] = data[i] ^ ks;
	}
}
//-------------------------------------------------------------------------
// <--End Core SFCMC
//-------------------------------------------------------------------------
//For testing ->

// Encryption worker with quantum reverse
DWORD WINAPI EncryptWorker(LPVOID lp)
{
	ThreadParams* p = (ThreadParams*)lp;
	size_t cnt = p->end - p->start;
	for(size_t i = 0; i < cnt; ++i)
	{
		uint8_t b = p->data[i] ^ ((uint8_t*)&p->feedback)[i % 8];
		uint8_t m = gen_keystream_byte(p->start + i, p->pSFCMC);
		b ^= m;
		p->data[i] = (b << 1) | (b >> 7);
	}
	return 0;
}

// Decryption worker (inverse quantum)
DWORD WINAPI DecryptWorker(LPVOID lp)
{
	ThreadParams* p = (ThreadParams*)lp;
	int64_t cnt = p->end - p->start;
	for(int64_t i = 0; i < cnt; ++i)
	{
		uint8_t b = p->data[i];
		b = (b >> 1) | (b << 7);
		uint8_t m = gen_keystream_byte(p->start + i, p->pSFCMC);
		b ^= m;
		p->data[i] = b ^ ((uint8_t*)&p->feedback)[i % 8];
	}
	return 0;
}

// Thread dispatcher
void ProcessData(ThreadParams* tp, DWORD(WINAPI* worker)(LPVOID))
{
	HANDLE th[NUM_THREADS]; ThreadParams params[NUM_THREADS];
	int64_t step = (tp->end - tp->start) / NUM_THREADS;
	for(int i = 0; i < NUM_THREADS; ++i)
	{
		params[i] = *tp;
		params[i].start = tp->start + i * step;
		params[i].end = (i == NUM_THREADS - 1 ? tp->end : params[i].start + step);
		params[i].data = tp->data + (params[i].start - tp->start);
		th[i] = CreateThread(NULL, 0, worker, &params[i], 0, NULL);
	}
	WaitForMultipleObjects(NUM_THREADS, th, TRUE, INFINITE);
	for(int i = 0; i < NUM_THREADS; ++i) CloseHandle(th[i]);
}


int main()
{
	SFCMC sfcmc = { 0 };
	Timer test_timer, total_timer;
	double test_time, test_speed;
	uint32_t histogram[256] = { 0 };
	double entropy = 0;
	double autocorr = 0;
	uint32_t hash_len = KEY_HASH_SIZE;
	uint8_t* data = (uint8_t*)VirtualAlloc(NULL, DATA_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	uint8_t master_key[MASTER_KEY_LEN]={0};
	uint8_t nonce[NONCE_LEN]={0};
	uint8_t hash_key[KEY_HASH_SIZE]={0};

	printf("[Size]    MB:%lld\nTest data prepare...",DATA_SIZE_MB);

	for(int i = 0; i < DATA_SIZE; i++)
	{
		data[i] = TestRandom();
	}
	for(int i = 0; i < 64; ++i)
	{
		master_key[i] = TestRandom();
	}
	for(int i = 0; i < 32; i++)
	{
		nonce[i] = TestRandom();
	}
	//here is a working example of a method for obtaining high-entropy data,
	//but in real conditions you need to obtain the master key hash and nonce 
	//separately and use them separately
	total_timer.begin();
	sha3_512_hash(master_key, MASTER_KEY_LEN, nonce, NONCE_LEN, sfcmc.hash_key,&hash_len);
	sfcmc.stream = (uint8_t*)_aligned_malloc(MAGIC_FRAME_SIZE,64);
	derive_key_shake256(hash_key,hash_len,sfcmc.stream,MAGIC_FRAME_SIZE);

	
	printf("\r[Start]");

	//Encrypt
	test_timer.begin();
	#ifdef MULTITHREAD
		//Block feedback
		size_t init_fb = *(uint64_t*)sfcmc.nonce;
		size_t part1 = *(uint64_t*)(sfcmc.nonce + 12);
		size_t part2 = *(uint64_t*)(sfcmc.nonce + 24);
		init_fb ^= (part1 + part2);
		size_t fb = init_fb; //Backup feedback
		for(size_t off = 0; off < DATA_SIZE; off += BLOCK_SIZE)
		{
			size_t bs = (DATA_SIZE - off < BLOCK_SIZE ? DATA_SIZE - off : BLOCK_SIZE);
			ThreadParams tp = { data + off,off,off + bs,fb,&sfcmc }; ProcessData(&tp, EncryptWorker);
			memcpy(&fb, data + off + bs - sizeof(fb), sizeof(fb));
		}
	#else
	    sfcmc_crypt(data,&sfcmc);
	#endif 

	test_time = test_timer.elapsed();
	test_speed = (double)DATA_SIZE / test_time / (1024.0 * 1024.0);
	printf("\r[Encrypt] Time: %.3fs, Speed: %.2f MB/s\n", test_time, test_speed);

	calculate_histogram(data, DATA_SIZE, histogram);
	entropy = calculate_entropy(histogram, DATA_SIZE);
	autocorr = calculate_autocorrelation(data, DATA_SIZE);
	
	//Decrypt
	test_timer.begin();
	#ifdef MULTITHREAD
		for(size_t off = 0; off < DATA_SIZE; off += BLOCK_SIZE)
		{
			size_t bs = (DATA_SIZE - off < BLOCK_SIZE ? DATA_SIZE - off : BLOCK_SIZE);
			ThreadParams tp = { data + off,off,off + bs,fb,&sfcmc }; ProcessData(&tp, EncryptWorker);
			memcpy(&fb, data + off + bs - sizeof(fb), sizeof(fb));
		}
	#else
	    sfcmc_crypt(data, &sfcmc);
	#endif 

	test_time = test_timer.elapsed();
	test_speed = (double)DATA_SIZE / test_time / (1024.0 * 1024.0);
	printf("[Decrypt] Time: %.3fs, Speed: %.2f MB/s\n", test_time, test_speed);

	test_time = total_timer.elapsed();
	printf("[Total]   Time: %.3fs\n\n", test_time);

	printf("Entropy:         %.4f\n", entropy);
	printf("Autocorrelation: %.4f\n", autocorr);

	_aligned_free(sfcmc.stream);
	VirtualFree(data,DATA_SIZE,MEM_FREE);
	_getch();
}