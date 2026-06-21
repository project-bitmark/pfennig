// Copyright (c) 2026 The Pfennig / Bitmark developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/license/mit/.

#include <primitives/pureheader.h>

#include <hash.h>
#include <primitives/block.h>
#include <streams.h>

#include <array>

namespace {

/** Default backend: plain double-SHA256 of the 80-byte header (== GetHash()). */
uint256 PowSha256d(Span<const std::byte> header)
{
    HashWriter h{};
    h.write(header);
    return h.GetHash();
}

/** Per-algorithm backend table; SHA256D registered by default so stock
 *  validation is unchanged until Bitmark injects the others. */
std::array<PowHasher, NUM_POW_ALGOS>& Backends()
{
    static std::array<PowHasher, NUM_POW_ALGOS> backends = [] {
        std::array<PowHasher, NUM_POW_ALGOS> a{};
        a[static_cast<size_t>(PowAlgo::SHA256D)] = PowSha256d;
        return a;
    }();
    return backends;
}

/** Algorithm selector; default policy: every block is SHA256D. Bitmark replaces
 *  this with a fork/height-aware policy that reads the algo from nVersion. */
PowAlgoSelector& Selector()
{
    static PowAlgoSelector selector = [](int32_t) { return PowAlgo::SHA256D; };
    return selector;
}

} // namespace

void SetPowBackend(PowAlgo algo, PowHasher hasher)
{
    Backends()[static_cast<size_t>(algo)] = std::move(hasher);
}

void SetPowAlgoSelector(PowAlgoSelector selector)
{
    Selector() = std::move(selector);
}

bool HasPowBackend(PowAlgo algo)
{
    return static_cast<bool>(Backends()[static_cast<size_t>(algo)]);
}

uint256 GetBlockPoWHash(const CBlockHeader& block)
{
    const PowAlgo algo = Selector()(block.nVersion);
    const PowHasher& backend = Backends()[static_cast<size_t>(algo)];

    // No backend injected for this algo yet (e.g. selector picked SCRYPT before
    // Phase 2 registered it): fall back to the sha256d identity hash so we never
    // crash. Such a block simply won't satisfy the target until a real backend
    // is registered. With the default policy this branch is never taken.
    if (!backend) return block.GetHash();

    DataStream ss{};
    ss << block;
    return backend(Span<const std::byte>{ss.data(), ss.size()});
}
