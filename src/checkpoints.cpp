// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "txdb.h"
#include "main.h"
#include "uint256.h"


static const int nCheckpointSpan = 5000;

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
		(0, uint256("0x000032b50c33d26dc61a29409d83ca9a4d39668dd1808f4dc9fe275b4251abcf"))
		(2, uint256("0x186014d5882e9d438574c7d46d9ca20c8e30a07066e4b429dbf74ca5c6475331"))
		(27, uint256("0x5a761bf604100e3706fdde1417f0e6c3056ae39e648c641a0dc95987fb683ba1"))
		(358, uint256("0xa963e5f8de630dfc08ff8b3f70f89443410b081f0e287479dc85148edc224f4e"))
		(1223, uint256("0x0d8f8398579beb9de30b2ceb2e79328df32cfd975f50a9beaa2711a0a2ee281f"))
		(1911, uint256("0x2ab982554aa869a02c0f4cfa4a30e942295bddfc23c133b452b651a599323f23"))
		(3971, uint256("0xf49ccb9ba494e9eca456dac8aa98d642ce9c8f629eab9971899500ed99b6dfc4"))
		(7524, uint256("0x32c1bfb36c4fc703a8bd02528a9b4190932e1579a298468189eff975e7f96301"))
		(11483, uint256("0x283ad6d9e50027c4ee650816e91003fa4b2bf5fba353c6e8de29f5a79ca0e536"))
		(16815, uint256("0xcd27c289a117885def8c75a82734df0277431f19b5fba50e2e17825169a027c1"))
		(22981, uint256("0xdcec9ec2891c04cc4ebcaabc0b5683386d52b81aea5114dcfb05af6d705063b5"))
		(28126, uint256("0x73c1518aa8b65f7a75d75397ad8fb87aa40e1907d13e4a80abab6378cee629cc"))
		
    ;

    // TestNet has no checkpoints
    static MapCheckpoints mapCheckpointsTestnet;

    bool CheckHardened(int nHeight, const uint256& hash)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        if (checkpoints.empty())
            return 0;
        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        MapCheckpoints& checkpoints = (TestNet() ? mapCheckpointsTestnet : mapCheckpoints);

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }

    // Automatically select a suitable sync-checkpoint 
    const CBlockIndex* AutoSelectSyncCheckpoint()
    {
        const CBlockIndex *pindex = pindexBest;
        // Search backward for a block within max span and maturity window
        while (pindex->pprev && pindex->nHeight + nCheckpointSpan > pindexBest->nHeight)
            pindex = pindex->pprev;
        return pindex;
    }

    // Check against synchronized checkpoint
    bool CheckSync(int nHeight)
    {
        const CBlockIndex* pindexSync = AutoSelectSyncCheckpoint();
        if (nHeight <= pindexSync->nHeight){
            return false;
        }
        return true;
    }
}
