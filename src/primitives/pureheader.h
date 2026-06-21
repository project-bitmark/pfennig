// Copyright (c) 2026 The Pfennig / Bitmark developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/license/mit/.

#ifndef BITCOIN_PRIMITIVES_PUREHEADER_H
#define BITCOIN_PRIMITIVES_PUREHEADER_H

#include <primitives/algo.h>
#include <span.h>
#include <uint256.h>

#include <cstddef>
#include <functional>

class CBlockHeader;

/**
 * Multi-algorithm PoW dispatch seam (Bitmark mPoW, BIP-101).
 *
 * Two pluggable pieces, mirroring the bitcoin-kernel injection pattern
 * (setSha256Backend / setVerifyBackend):
 *
 *   1. A per-algorithm PoW hash backend:  80-byte header -> 32-byte PoW hash.
 *   2. An algorithm *selector*: given a header's nVersion, which algo to use.
 *
 * DEFAULT POLICY (stock, pre-Bitmark): the selector always returns SHA256D and
 * only the SHA256D backend is registered (== CBlockHeader::GetHash()). So
 * GetBlockPoWHash(block) == block.GetHash() and validation is byte-for-byte
 * identical to upstream Bitcoin Core until Bitmark injects real backends +
 * a fork/height-aware selector (Phases 2-3).
 *
 * IMPORTANT: the PoW hash is DISTINCT from CBlockHeader::GetHash(). GetHash()
 * stays the sha256d identity/linkage hash (getblockhash, prev-block links);
 * GetBlockPoWHash() is the algo-specific value compared against the target.
 */

/** A PoW backend: hashes the serialized 80-byte header to a 32-byte PoW hash. */
using PowHasher = std::function<uint256(Span<const std::byte> header80)>;

/** Selects which algorithm a header is mined with (default: always SHA256D). */
using PowAlgoSelector = std::function<PowAlgo(int32_t nVersion)>;

/** Inject/override the PoW backend for an algorithm. Bitmark registers
 *  scrypt/x17/lyra2/... here. SHA256D is registered by default. */
void SetPowBackend(PowAlgo algo, PowHasher hasher);

/** Inject the algorithm-selection policy (default returns SHA256D for all). */
void SetPowAlgoSelector(PowAlgoSelector selector);

/** True if a backend is registered for this algorithm. */
bool HasPowBackend(PowAlgo algo);

/** Algorithm-specific PoW hash of a header (the value checked against target). */
uint256 GetBlockPoWHash(const CBlockHeader& block);

#endif // BITCOIN_PRIMITIVES_PUREHEADER_H
