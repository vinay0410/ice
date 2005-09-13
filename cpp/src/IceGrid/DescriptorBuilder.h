// **********************************************************************
//
// Copyright (c) 2003-2005 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_GRID_DESCRIPTOR_BUILDER_H
#define ICE_GRID_DESCRIPTOR_BUILDER_H

#include <Ice/Logger.h>
#include <IceXML/Parser.h>
#include <IceGrid/Descriptor.h>

namespace IceGrid
{

class XmlAttributesHelper
{
public:

    XmlAttributesHelper(const IceXML::Attributes&, const Ice::LoggerPtr&, const std::string&, int);
    ~XmlAttributesHelper();

    bool contains(const std::string&) const;
    std::map<std::string, std::string> asMap() const;

    std::string operator()(const std::string&) const;
    std::string operator()(const std::string&, const std::string&) const;

private:
    
    const IceXML::Attributes& _attributes;
    const Ice::LoggerPtr _logger;
    const std::string _filename;
    const int _line;

    mutable std::set<std::string> _used;
};

class DescriptorBuilder
{
public:

    virtual ~DescriptorBuilder() {  }
    virtual void addVariable(const XmlAttributesHelper&);
};

class NodeDescriptorBuilder;
class TemplateDescriptorBuilder;

class ApplicationDescriptorBuilder : public DescriptorBuilder
{
public:

    ApplicationDescriptorBuilder(const XmlAttributesHelper&, const std::map<std::string, std::string>&);

    const ApplicationDescriptor& getDescriptor() const;

    void setVariableOverrides(const std::map<std::string, std::string>&);
    void setDescription(const std::string&);
    void addReplicatedAdapter(const XmlAttributesHelper&);
    void setLoadBalancing(const XmlAttributesHelper&);
    void addObject(const XmlAttributesHelper&);
    virtual void addVariable(const XmlAttributesHelper&);

    std::auto_ptr<NodeDescriptorBuilder> createNode(const XmlAttributesHelper&);
    std::auto_ptr<TemplateDescriptorBuilder> createServerTemplate(const XmlAttributesHelper&);
    std::auto_ptr<TemplateDescriptorBuilder> createServiceTemplate(const XmlAttributesHelper&);

    void addNode(const std::string&, const NodeDescriptor&);
    void addServerTemplate(const std::string&, const TemplateDescriptor&);
    void addServiceTemplate(const std::string&, const TemplateDescriptor&);
    
    void addPatch(const XmlAttributesHelper&);
    void addPatchDirectory(const std::string&);

    bool isOverride(const std::string&);

private:

    ApplicationDescriptor _descriptor;
    std::map<std::string, std::string> _overrides;
    std::string _lastPatch;
};

class ServerDescriptorBuilder;
class IceBoxDescriptorBuilder;

class NodeDescriptorBuilder : public DescriptorBuilder
{
public:

    NodeDescriptorBuilder(ApplicationDescriptorBuilder&, const XmlAttributesHelper&);

    std::auto_ptr<ServerDescriptorBuilder> createServer(const XmlAttributesHelper&);
    std::auto_ptr<IceBoxDescriptorBuilder> createIceBox(const XmlAttributesHelper&);

    void addVariable(const XmlAttributesHelper&);
    void addServerInstance(const XmlAttributesHelper&);
    void addServer(const ServerDescriptorPtr&);

    const std::string& getName() const { return _name; }
    const NodeDescriptor& getDescriptor() const { return _descriptor; }

private:
    
    ApplicationDescriptorBuilder& _application;
    std::string _name;
    NodeDescriptor _descriptor;
};

class ServiceDescriptorBuilder;

class TemplateDescriptorBuilder : public DescriptorBuilder
{
public:

    TemplateDescriptorBuilder(ApplicationDescriptorBuilder&, const XmlAttributesHelper&, bool);

    void addParameter(const XmlAttributesHelper&);
    void setDescriptor(const CommunicatorDescriptorPtr&);

    std::auto_ptr<ServerDescriptorBuilder> createServer(const XmlAttributesHelper&);
    std::auto_ptr<IceBoxDescriptorBuilder> createIceBox(const XmlAttributesHelper&);    
    std::auto_ptr<ServiceDescriptorBuilder> createService(const XmlAttributesHelper&);    

    const std::string& getId() const { return _id; }
    const TemplateDescriptor& getDescriptor() const { return _descriptor; }

protected:

    ApplicationDescriptorBuilder& _application;
    const bool _serviceTemplate;
    const std::string _id;
    TemplateDescriptor _descriptor;
};

class CommunicatorDescriptorBuilder : public DescriptorBuilder
{
public:

    void init(const CommunicatorDescriptorPtr&, const XmlAttributesHelper&);

    virtual void setDescription(const std::string&);
    virtual void addProperty(const XmlAttributesHelper&);
    virtual void addAdapter(const XmlAttributesHelper&);
    virtual void addObject(const XmlAttributesHelper&);
    virtual void addDbEnv(const XmlAttributesHelper&);
    virtual void addDbEnvProperty(const XmlAttributesHelper&);

private:

    CommunicatorDescriptorPtr _descriptor;
};

class ServerDescriptorBuilder : public CommunicatorDescriptorBuilder
{
public:
    
    ServerDescriptorBuilder(const XmlAttributesHelper&);
    ServerDescriptorBuilder();

    void init(const ServerDescriptorPtr&, const XmlAttributesHelper&);

    virtual std::auto_ptr<ServiceDescriptorBuilder> createService(const XmlAttributesHelper&);

    virtual void addOption(const std::string&);
    virtual void addEnv(const std::string&);
    virtual void addService(const ServiceDescriptorPtr&);
    virtual void addServiceInstance(const XmlAttributesHelper&);
    virtual void addPatch(const XmlAttributesHelper&);
    virtual void addPatchDirectory(const std::string&);
    virtual void addUsePatch(const XmlAttributesHelper&);

    const ServerDescriptorPtr& getDescriptor() const { return _descriptor; } 

private:

    ServerDescriptorPtr _descriptor;
};

class IceBoxDescriptorBuilder : public ServerDescriptorBuilder
{
public:

    IceBoxDescriptorBuilder(const XmlAttributesHelper&);

    void init(const IceBoxDescriptorPtr&, const XmlAttributesHelper&);

    virtual std::auto_ptr<ServiceDescriptorBuilder> createService(const XmlAttributesHelper&);

    virtual void addAdapter(const XmlAttributesHelper&);
    virtual void addDbEnv(const XmlAttributesHelper&);
    virtual void addServiceInstance(const XmlAttributesHelper&);
    virtual void addService(const ServiceDescriptorPtr&);

private:
    
    IceBoxDescriptorPtr _descriptor;
};

class ServiceDescriptorBuilder : public CommunicatorDescriptorBuilder
{
public:

    ServiceDescriptorBuilder(const XmlAttributesHelper&);
    void init(const ServiceDescriptorPtr&, const XmlAttributesHelper&);

    const ServiceDescriptorPtr& getDescriptor() const { return _descriptor; }
    
private:

    ServiceDescriptorPtr _descriptor;
};

};

#endif
