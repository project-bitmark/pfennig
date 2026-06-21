// Copyright (c) 2026 The Pfennig / Bitmark developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/license/mit/.

#ifndef BITCOIN_PRIMITIVES_ALGO_H
#define BITCOIN_PRIMITIVES_ALGO_H

#include <cstdint>

/**
 * Multi-algorithm Proof-of-Work (Bitmark mPoW, BIP-101, activated at block 450,947).
 *
 * The 32-bit block nVersion field packs the PoW algorithm and protocol flags:
 *
 *   bits 0-7   base version number (>= 4 after the mPoW fork)
 *   bit  8     AUXPOW         merged-mining flag
 *   bits 9-11  ALGO           algorithm selector (0-7)
 *   bit  12    UPDATE_SSF     subsidy scaling-factor recalculation (CEM, BIP-102)
 *   bit  13    VARIANT1       reserved fork-activation lever (legacy onFork2)
 *   bit  14    VARIANT2       reserved fork-activation lever (legacy onFork3)
 *   bits 16+   CHAIN ID       AuxPow parent-chain identifier
 *
 * NOTE: this collides with Bitcoin's use of nVersion for BIP9 version-bit
 * signalling. On Pfennig these bits are NOT softfork signals; the multi-algo
 * dispatch must therefore be gated on the Bitmark chain/fork being active
 * (see primitives/pureheader / the PoW backend registry). Stock validation
 * must keep treating PoW as plain sha256d until that policy is injected.
 */
enum class PowAlgo : uint8_t {
    SCRYPT      = 0,  // pre-fork algorithm; Litecoin params N=1024,r=1,p=1
    SHA256D     = 1,  // Bitcoin-compatible double-SHA256
    YESCRYPT    = 2,
    ARGON2D     = 3,
    X17         = 4,
    LYRA2REv2   = 5,
    EQUIHASH    = 6,  // N=200, K=9; identity hash uses GetHashE (distinct from sha256d)
    CRYPTONIGHT = 7,
    // GROESTL (=8) appears in some legacy Bitmark source but is NON-CANONICAL:
    // a 3-bit field cannot encode 8, and it never appears on mainnet. Excluded.
};
static constexpr int NUM_POW_ALGOS = 8;

// nVersion bit layout (BIP-101).
static constexpr int32_t BLOCK_VERSION_BASE_MASK  = 0x000000ff; // bits 0-7
static constexpr int32_t BLOCK_VERSION_AUXPOW     = 1 << 8;     // bit 8  (0x100)
static constexpr int32_t BLOCK_VERSION_ALGO_MASK  = 7 << 9;     // bits 9-11 (0xE00)
static constexpr int     BLOCK_VERSION_ALGO_SHIFT = 9;
static constexpr int32_t BLOCK_VERSION_UPDATE_SSF = 1 << 12;    // bit 12 (0x1000)
static constexpr int32_t BLOCK_VERSION_VARIANT1   = 1 << 13;    // bit 13 (0x2000)
static constexpr int32_t BLOCK_VERSION_VARIANT2   = 1 << 14;    // bit 14 (0x4000)

/** Algorithm encoded in a block's nVersion (bits 9-11). Only meaningful for
 *  Bitmark post-fork blocks; callers must gate on the multi-algo policy. */
inline PowAlgo GetPowAlgo(int32_t nVersion)
{
    return static_cast<PowAlgo>((nVersion & BLOCK_VERSION_ALGO_MASK) >> BLOCK_VERSION_ALGO_SHIFT);
}
inline bool IsAuxpow(int32_t nVersion)       { return (nVersion & BLOCK_VERSION_AUXPOW) != 0; }
inline bool UpdatesSSF(int32_t nVersion)     { return (nVersion & BLOCK_VERSION_UPDATE_SSF) != 0; }
inline int  GetBaseVersion(int32_t nVersion) { return nVersion & BLOCK_VERSION_BASE_MASK; }

inline const char* GetAlgoName(PowAlgo algo)
{
    switch (algo) {
        case PowAlgo::SCRYPT:      return "scrypt";
        case PowAlgo::SHA256D:     return "sha256d";
        case PowAlgo::YESCRYPT:    return "yescrypt";
        case PowAlgo::ARGON2D:     return "argon2d";
        case PowAlgo::X17:         return "x17";
        case PowAlgo::LYRA2REv2:   return "lyra2rev2";
        case PowAlgo::EQUIHASH:    return "equihash";
        case PowAlgo::CRYPTONIGHT: return "cryptonight";
    }
    return "unknown";
}

#endif // BITCOIN_PRIMITIVES_ALGO_H
