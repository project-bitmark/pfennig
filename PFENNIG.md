# Pfennig — Bitmark on Bitcoin Core v29 (reboot)

This branch (`v29-reboot`) is a **clean reboot** of Pfennig/Bitmark onto **Bitcoin Core v29.0**,
replacing the legacy 0.9.4 line. It tracks upstream `bitcoin/bitcoin` (remote `bitcoin`) so future
v30/v31 bumps stay cheap.

## Status: Phase 0 — branded base (NOT yet consensus-correct)

Done in this commit:
- **Branding:** `CLIENT_NAME` "Pfennig Core"; subver `/Pfennig:29.0.0/` (`UA_NAME`); binaries
  `pfennigd` / `pfennig-cli`; package description / homepage / bug-report URLs.
- **Chainparams scaffold (mainnet):** P2P port `9265`; base58 prefixes (PUBKEY 85, SCRIPT 5, SECRET 213);
  `bech32_hrp` placeholder; Bitmark DNS seed `da.bitmark.guru`; **Taproot disabled** (`NEVER_ACTIVE`).
  Network magic `f9beb4d9` is shared with Bitcoin (unchanged).

**This node still validates _Bitcoin's_ chain, not Bitmark's** — intentional. Genesis, the soft-fork
heights (incl. making Segwit effectively never-active), and all Bitmark consensus are later phases.

## Roadmap (see `~/ideas/bitcoin-schema/pfennig-v29-reboot-plan.md`)
1. **Phase 0** — brand + chainparams scaffold ← *you are here*
2. **Phase 1** — isolate an algo-aware PoW backend seam (kernel-style injection)
3. **Phase 2** — scrypt (keystone: Bitmark genesis is scrypt-PoW'd) → validates genesis→450,947
4. **Phase 3** — mPoW (bip-101: 8 algos + DGWv3 + AuxPow) + CEM (bip-102: emission + SSF) @ block 450,947

Authority for consensus = **the BIPs (`project-bitmark/BIP` 100/101/102) + the live chain**, cross-checked
against `master` (0.9.7.4, the BIP-cited reference impl) and the JS `bitmark-kernel`. Build not run yet.
