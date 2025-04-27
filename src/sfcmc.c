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

#include <intrin.h>
#include <widemath.h>

#define MAGIC_FRAME_SIZE 1177
#define KEY_HASH_SIZE    64     // 512-bit hash master key + nonce

// XOR-based random number generator
static inline uint64_t wyrand(uint64_t* state)
{
    *state += 0xA0761D6478BD642FULL;
    uint64_t m = *state ^ 0xE7037ED1A0B428DBULL;
    m *= (*state | 1);
    return m ^ (m >> 32);
}

// ARX round transformation function
static inline uint32_t arx_round(uint32_t v, uint32_t c)
{
    v += c;                         // ADD mod 2^32
    v = (v << 7) | (v >> (32 - 7)); // ROTL 7
    v ^= c;                         // XOR
    return v;
}

// Fast index calculation using the high part of 64-bit multiplication result
static inline size_t fast_idx(uint64_t rnd, size_t N)
{
    uint64_t high;
    (void)_umul128(rnd, (uint64_t)N, &high); // low = rnd * N mod 2^64, high = floor(rnd*N/2^64)
    return (size_t)high;
}

//-------------------------------------------------------------------------
// SFCMC (c) 2025 Lim Troev (pseudonim) [PsiCrypt Project]
//-------------------------------------------------------------------------

// Core SFCMC generation function
uint32_t inline gen_keystream_byte(size_t idx, const uint8_t* hash_key, const uint8_t* stream)
{
    uint64_t h2 = hash_key[fast_idx(wyrand(&idx), KEY_HASH_SIZE)];
    uint64_t u = fast_idx(idx, 64) ^ h2;
    uint64_t chaos = wyrand(&u);
    uint32_t ripv = stream[fast_idx(wyrand(&idx), MAGIC_FRAME_SIZE)];
    chaos ^= arx_round((uint32_t)chaos, ripv);
    return (uint32_t)chaos;
}

// Function to encode or decode data using the SFCMC algorithm
// data      - pointer to data for encode/decode
// size_data - size of data in bytes
// hash_key  - hash derived from Master key or nonce, sized KEY_HASH_SIZE
// stream    - pointer to high-entropy data stream, size MAGIC_FRAME_SIZE
void sfcmc_encode_decode(uint8_t* data, const size_t size_data, const uint8_t* hash_key, const uint8_t* stream)
{
    for(size_t i = 0; i < size_data; ++i)
    {
        uint32_t ks = gen_keystream_byte(i, hash_key, stream);
        data[i] = data[i] ^ ks;
    }
}

//-------------------------------------------------------------------------
// End Core SFCMC
//-------------------------------------------------------------------------
