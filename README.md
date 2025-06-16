# 🚀 SFCMC

> [Super Fast Compact Multifunctional Cipher](https://github.com/PsiCrypt/SFC_MC)

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
CFCMC and [ChaCha20](https://github.com/shiffthq/chacha20/tree/master/src) builded in one application for testing without optimizations. compiler LLVM 142

<div style="overflow-x:auto;">
  <table>
    <thead>
      <tr>
        <th>Cipher</th>
        <th>Speed (MB/s)</th>
        <th>Entropy</th>
        <th>Autocorrelation</th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td>ChaCha20</td>
        <td>205.28</td>
        <td>8.0000</td>
        <td>-0.0004</td>
      </tr>
      <tr>
        <td>SFCMS</td>
        <td>216.19</td>
        <td>8.0000</td>
        <td>-0.0000</td>
      </tr>
      <tr>
        <td>ChaCha20</td>
        <td>208.09</td>
        <td>8.0000</td>
        <td>-0.0004</td>
      </tr>
      <tr>
        <td>SFCMS</td>
        <td>217.36</td>
        <td>8.0000</td>
        <td>-0.0002</td>
      </tr>
      <tr>
        <td>ChaCha20</td>
        <td>208.00</td>
        <td>8.0000</td>
        <td>-0.0003</td>
      </tr>
      <tr>
        <td>SFCMS</td>
        <td>219.59</td>
        <td>8.0000</td>
        <td>-0.0001</td>
      </tr>
    </tbody>
  </table>
</div>


Random test: 100MB random block, random key & nonce.


NIST STS Results:

[ChaCha20 each time a different random buffer and random key](Results/chachaRND.txt)

[SFCMS each time a different random buffer and random key](Results/SFCMC_RND.txt)

[SFCMS each time filled with zeros buffer and a different random key](Results/SFCMC_0.txt)


## 📚 Visualizations 

- [Bite card SFCMC](https://github.com/PsiCrypt/SFC_MC/blob/main/Img/CFCMC.png)

- [Bite card ChaCha20](https://github.com/PsiCrypt/SFC_MC/blob/main/Img/ChaCha20.png)

- [Bite Map SFCMC](https://github.com/PsiCrypt/SFC_MC/blob/main/Img/SFCMC_bitmap.png)

## 📚 Source code
- [C source code - single thread](https://github.com/PsiCrypt/SFC_MC/blob/main/src/sfcmc.c)
- [C++ source code with test multi/single threading for MSVC](https://github.com/PsiCrypt/SFC_MC/tree/main/src/MSVC)

The source code is intended to demonstrate the principle of the algorithm without optimizations. You can improve it and unsubscribe here.

## 🛡 License
This project is licensed under the MIT License.
Free for personal and academic use with attribution.

E-mail: psicrypt@keemail.me

© 2025 PsiCrypt Project. All rights reserved.



