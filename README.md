# SFC_MC


# 🚀 SFC_MC

> Super Fast Compact Multifunctional Cipher

---

## 📖 Description

**SFC_MC** is a high-performance, compact simple cryptor.  

> **Key features:**
> - Super high-performance (faster than chacha20)
> - Super simple
> - Easily optimized for multithreading and new hardware architectures
> - Passed all tests from NIST SP 800-22 Statistical Test Suite.

---

## 🧪 Benchmarks
Encryption speed: ~5%-20% faster than ChaCha20 on Intel Xeon E5-2670 (single core)

Entropy: =8.00000 bits/byte stable

Autocorellation: ->0  [0,0.0004)

## 📊 Benchmark Results
Intel Xeon E5-2670 (single core)
builded in one application for testing without optimizations. compiler LLVM 142
| Cipher    | Speed (MB/s) | Entropy | Autocorrelation |
|-----------|--------------|---------|-----------------|
| ChaCha20  | 205.28       | 8.0000  | -0.0004          |
| SFCMS     | 216.19       | 8.0000  | -0.0000          |
| ChaCha20  | 208.09       | 8.0000  | -0.0004          |
| SFCMS     | 217.36       | 8.0000  | -0.0002          |
| ChaCha20  | 208.00       | 8.0000  | -0.0003          |
| SFCMS     | 219.59       | 8.0000  | -0.0001          |

Random test: 100MB random block, random key & nonce.


NIST STS Results:
[ChaCha20 each time a different random buffer and random key](Results/chachaRND.txt)



## 🛡 License
This project is licensed under the MIT License.
Free for personal, academic, and commercial use with attribution.

© 2025 PsyCrypt Project. All rights reserved.



