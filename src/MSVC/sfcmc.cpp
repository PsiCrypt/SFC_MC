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

#define DATA_SIZE_MB     16
#define DATA_SIZE        (DATA_SIZE_MB * 1024LL * 1024LL)

#define MAGIC_FRAME_SIZE 1177
#define MASTER_KEY_LEN   64     // 512 bit Master key
#define NONCE_LEN        32     // 256 bit nonce
#define KEY_HASH_SIZE    64     // 512-bit hash master key + nonce

#include "sfcms.h"
//-------------------------------------------------------------------------
//	      SFCMC (c) 2025 Lim Troev (pseudonim) [PsiCrypt Project]
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

	sfcmc_crypt(data,&sfcmc);

	test_time = test_timer.elapsed();
	test_speed = (double)DATA_SIZE / test_time / (1024.0 * 1024.0);
	printf("\r[Encrypt] Time: %.3fs, Speed: %.2f MB/s\n", test_time, test_speed);

	calculate_histogram(data, DATA_SIZE, histogram);
	entropy = calculate_entropy(histogram, DATA_SIZE);
	autocorr = calculate_autocorrelation(data, DATA_SIZE);
	
	//Decrypt
	test_timer.begin();

	sfcmc_crypt(data, &sfcmc);

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
