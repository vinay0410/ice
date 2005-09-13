// **********************************************************************
//
// Copyright (c) 2003-2005 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_GRID_ADAPTERCACHE_H
#define ICE_GRID_ADAPTERCACHE_H

#include <IceUtil/Mutex.h>
#include <IceUtil/Shared.h>
#include <IceGrid/Cache.h>
#include <IceGrid/Internal.h>

namespace IceGrid
{

class AdapterCache;

class ServerEntry;
typedef IceUtil::Handle<ServerEntry> ServerEntryPtr;
typedef std::vector<ServerEntryPtr> ServerEntrySeq;

class AdapterEntry;
typedef IceUtil::Handle<AdapterEntry> AdapterEntryPtr;

enum LoadSample
{
    LoadSample1,
    LoadSample5,
    LoadSample15
};

class AdapterEntry : public IceUtil::Shared, public IceUtil::Mutex
{
public:
    
    AdapterEntry(Cache<std::string, AdapterEntry>&, const std::string&);

    AdapterPrx getProxy(const std::string&) const;
    std::vector<std::pair<std::string, AdapterPrx> > getProxies(int&);
    
    void enableReplication(const LoadBalancingPolicyPtr&);
    void disableReplication();

    void addServer(const ServerEntryPtr&);
    void removeServer(const ServerEntryPtr&);

    bool canRemove();
    
private:
    
    Cache<std::string, AdapterEntry>& _cache;
    const std::string _id;
    bool _replicated;
    LoadBalancingPolicyPtr _loadBalancing;
    int _loadBalancingNReplicas;
    LoadSample _loadSample;
    ServerEntrySeq _servers;
    int _lastServer;
};
typedef IceUtil::Handle<AdapterEntry> AdapterEntryPtr;

class AdapterCache : public CacheByString<AdapterEntry>
{
public:

    AdapterCache(const TraceLevelsPtr&);

    AdapterEntryPtr get(const std::string&, bool = false) const;
};

};

#endif
