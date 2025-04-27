# 🚀 SFCMC

> Super Fast Compact Multifunctional Cipher

---

<p align="center">
  <a href="https://github.com/PsiCrypt/SFC_MC/actions">
    <img src="https://img.shields.io/badge/build-passing-brightgreen.svg?style=for-the-badge" alt="Build Status">
  </a>
  <a href="https://github.com/PsiCrypt/SFC_MC/issues">
    <img src="https://img.shields.io/github/issues-raw/PsiCrypt/SFC_MC?style=for-the-badge" alt="Open Issues">
  </a>
  <a href="https://github.com/PsiCrypt/SFC_MC/blob/main/LICENSE">
    <img src="https://img.shields.io/badge/license-MIT-blue.svg?style=for-the-badge" alt="License">
  </a>
  <a href="https://github.com/PsiCrypt/SFC_MC">
    <img src="https://img.shields.io/github/last-commit/PsiCrypt/SFC_MC?style=for-the-badge" alt="Last Commit">
  </a>
</p>

## 📖 Description

**SFCMC** is a high-performance, compact simple cryptor.  

> **Key features:**
> - Super high-performance (faster than chacha20)
> - Super simple and easy
> - Easy change of parameters for experiments
> - Easily optimized for multithreading and new hardware architectures
> - Passed tests from NIST SP 800-22 Statistical Test Suite.

---

## 🧪 Benchmarks
Encryption speed: ~5%-20% faster than ChaCha20

Entropy: =8.00000 bits/byte stable

Autocorellation: ~0.0000  (0.0004 max)

## 📊 Benchmark Results
Intel Xeon E5-2670 v2 (single core)
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

[SFCMS each time a different random buffer and random key](Results/SFCMC_RND.txt)

[SFCMS each time filled with zeros buffer and a different random key](Results/SFCMC_0.txt)


## 📚 Visualizations 

- [Bite card SFCMC](https://github.com/PsiCrypt/SFC_MC/blob/main/Img/CFCMC.png)

- [Bite card ChaCha20](https://github.com/PsiCrypt/SFC_MC/blob/main/Img/ChaCha20.png)

 - [Bite Map SFCMC](https://github.com/PsiCrypt/SFC_MC/blob/main/Img/SFCMC_bitmap.png)


## 🛡 License
This project is licensed under the MIT License.
Free for personal, academic, and commercial use with attribution.

© 2025 PsiCrypt Project. All rights reserved.



