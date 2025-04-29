#pragma once
#include <windows.h>
#include <intrin.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include "openssl/evp.h"

//For Core
typedef __declspec(align(64)) struct _SFCMC
{
	uint8_t master_key[MASTER_KEY_LEN];
	uint8_t hash_key[KEY_HASH_SIZE];
	uint8_t nonce[NONCE_LEN];
	uint8_t* stream;
	size_t stream_size;
} SFCMC, * PSFCMC;

static uint64_t __forceinline wyrand(uint64_t* state)
{
	*state += 0xA0761D6478BD642FULL;
	uint64_t m = *state ^ 0xE7037ED1A0B428DBULL;
	m *= (*state | 1);
	return m ^ (m >> 32);
}


void sha3_512_hash(const uint8_t* input, size_t in_len,
				   const uint8_t* key, size_t key_len,
				   uint8_t* out, uint32_t* out_len)
{
	EVP_MD_CTX* mdctx = EVP_MD_CTX_create();
	const EVP_MD* algo = EVP_sha3_512();
	EVP_DigestInit_ex(mdctx, algo, NULL);
	if(key_len) EVP_DigestUpdate(mdctx, key, key_len);
	EVP_DigestUpdate(mdctx, input, in_len);
	EVP_DigestFinal_ex(mdctx, out, out_len);
	EVP_MD_CTX_destroy(mdctx);
}

void inline derive_key_shake256(const uint8_t* in, size_t in_len, uint8_t* out, size_t out_len)
{
	EVP_MD_CTX* ctx = EVP_MD_CTX_new();
	EVP_DigestInit_ex(ctx, EVP_shake256(), NULL);
	EVP_DigestUpdate(ctx, in, in_len);
	EVP_DigestFinalXOF(ctx, out, out_len);
	EVP_MD_CTX_free(ctx);
}



static __forceinline uint32_t arx_round(uint32_t v, uint32_t c)
{
	v += c;                         // ADD mod 2^32
	v = (v << 7) | (v >> (32 - 7)); // ROTL 7
	v ^= c;                         // XOR
	return v;
}

static __forceinline size_t fast_idx(uint64_t rnd, size_t N)
{
	uint64_t high;
	// low = rnd * N mod 2^64, high = floor(rnd*N/2^64)
	(void)_umul128(rnd, (uint64_t)N, &high); 
	return (size_t)high;
}



//-------------------------------------------------------------
//For test

// Thread parameters
typedef __declspec(align(64))  struct _ThreadParams
{
	uint8_t* data;
	size_t  start;
	size_t  end;
	size_t feedback;
	PSFCMC pSFCMC;

}ThreadParams,*PThreadParams;


typedef struct _Timer
{
	LARGE_INTEGER start, freq;
	void begin()
	{
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&start);
	}
	double elapsed()
	{
		LARGE_INTEGER end;
		QueryPerformanceCounter(&end);
		return double(end.QuadPart - start.QuadPart) / freq.QuadPart;
	}
} Timer;

void calculate_histogram(uint8_t* data, size_t size, uint32_t* histogram)
{
	for(size_t i = 0; i < size; ++i)
	{
		histogram[data[i]]++;
	}
}

double calculate_entropy(uint32_t* histogram, size_t size)
{
	double entropy = 0.0;
	for(int i = 0; i < 256; ++i)
	{
		double p = (double)histogram[i] / (double)size;
		if(p > 0.0)
			entropy -= p * log2(p);
	}
	return entropy;
}

double calculate_autocorrelation(uint8_t* data, size_t size)
{
	if(size < 2) return 0.0;

	double mean = 0.0;
	for(size_t i = 0; i < size; ++i)
		mean += data[i];
	mean /= size;

	double var = 0.0, autocov = 0.0;
	for(size_t i = 0; i < size - 1; ++i)
	{
		double a = data[i] - mean;
		double b = data[i + 1] - mean;
		autocov += a * b;
		var += a * a;
	}
	var += (data[size - 1] - mean) * (data[size - 1] - mean);

	if(var == 0.0) return 0.0;

	return autocov / var;
}
