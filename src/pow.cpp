// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <pow.h>

#include <arith_uint256.h>
#include <chain.h>
#include <primitives/block.h>
#include <uint256.h>

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    /*
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }
    */

    if (pindexLast->nHeight > params.nZawyLwmaAveragingWindow) {
        return LwmaCalculateNextWorkRequired(pindexLast, params);
    }

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);

    if (nHeightFirst < 0) {
        return bnPowLimit.GetCompact();
    }

    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

// LWMA for BTC clones
// Copyright (c) 2017-2018 The Bitcoin Gold developers
// Copyright (c) 2018 Zawy (M.I.T license continued)
// Algorithm by zawy, a modification of WT-144 by Tom Harding
// Code by h4x3rotab of BTC Gold, modified/updated by zawy
// https://github.com/zawy12/difficulty-algorithms/issues/3#issuecomment-388386175
//  FTL must be changed to 300 or N*T/20 whichever is higher.
//  FTL in BTC clones is MAX_FUTURE_BLOCK_TIME in chain.h.
//  FTL in Ignition, Numus, and others can be found in main.h as DRIFT.
//  FTL in Zcash & Dash clones need to change the 2*60*60 here:
//  if (block.GetBlockTime() > nAdjustedTime + 2 * 60 * 60)
//  which is around line 3450 in main.cpp in ZEC and validation.cpp in Dash

unsigned int LwmaCalculateNextWorkRequired(const CBlockIndex* pindexLast, const Consensus::Params& params)
{
    const int64_t T = params.nPowTargetSpacing;
    // N=45 for T=600.  N=60 for T=150.  N=90 for T=60.
    const int64_t N = params.nZawyLwmaAveragingWindow;
    const int64_t k = N*(N+1)*T/2; // BTG's code has a missing N here. They inserted it in the loop
    const int height = pindexLast->nHeight;
    assert(height > N);

    arith_uint256 sum_target;
    int64_t t = 0, j = 0, solvetime;

    // Loop through N most recent blocks.
    for (int i = height - N+1; i <= height; i++) {
        const CBlockIndex* block = pindexLast->GetAncestor(i);
        const CBlockIndex* block_Prev = block->GetAncestor(i - 1);
        solvetime = block->GetBlockTime() - block_Prev->GetBlockTime();
        solvetime = std::max(-6*T, std::min(solvetime, 6*T));
        j++;
        t += solvetime * j;  // Weighted solvetime sum.
        arith_uint256 target;
        target.SetCompact(block->nBits);
        sum_target += target / (k * N); // BTG added the missing N back here.
    }
    // Keep t reasonable to >= 1/10 of expected t.
    if (t < k/10 ) {   t = k/10;  }
    arith_uint256 next_target = t * sum_target;

    const arith_uint256 pow_limit = UintToArith256(params.powLimit);
    if (next_target > pow_limit) {
        next_target = pow_limit;
    }

    return next_target.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
