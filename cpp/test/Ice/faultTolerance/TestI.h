// **********************************************************************
//
// Copyright (c) 2003-2004 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef TEST_I_H
#define TEST_I_H

#include <Test.h>

class TestI : public Test
{
public:

    TestI(const Ice::ObjectAdapterPtr&, const CleanerPtr& cleaner);

    virtual void shutdown(const Ice::Current&);
    virtual void abort(const Ice::Current&);
    virtual void idempotentAbort(const Ice::Current&);
    virtual void nonmutatingAbort(const Ice::Current&) const;
    virtual Ice::Int pid(const Ice::Current&);

private:

    Ice::ObjectAdapterPtr _adapter;
    CleanerPtr _cleaner;
};

class CleanerI : public Cleaner
{
public:

    CleanerI(const Ice::CommunicatorPtr&);

    virtual void cleanup(const Ice::Current&);

private:

    Ice::CommunicatorPtr _communicator;
};

#endif
