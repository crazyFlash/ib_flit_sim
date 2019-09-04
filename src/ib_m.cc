//
// Generated file, do not edit! Created by nedtool 5.4 from ib.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "ib_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("IB_MSGS");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("IB_MSGS"));
    e->insert(IB_DATA_MSG, "IB_DATA_MSG");
    e->insert(IB_FLOWCTRL_MSG, "IB_FLOWCTRL_MSG");
    e->insert(IB_SENT_MSG, "IB_SENT_MSG");
    e->insert(IB_TXCRED_MSG, "IB_TXCRED_MSG");
    e->insert(IB_RXCRED_MSG, "IB_RXCRED_MSG");
    e->insert(IB_MINTIME_MSG, "IB_MINTIME_MSG");
    e->insert(IB_POP_MSG, "IB_POP_MSG");
    e->insert(IB_HICCUP_MSG, "IB_HICCUP_MSG");
    e->insert(IB_FREE_MSG, "IB_FREE_MSG");
    e->insert(IB_DONE_MSG, "IB_DONE_MSG");
    e->insert(IB_APP_MSG, "IB_APP_MSG");
    e->insert(IB_PUSH_MSG, "IB_PUSH_MSG");
    e->insert(IB_TQ_LOAD_MSG, "IB_TQ_LOAD_MSG");
)

Register_Class(IBWireMsg)

IBWireMsg::IBWireMsg(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->VL = 0;
}

IBWireMsg::IBWireMsg(const IBWireMsg& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

IBWireMsg::~IBWireMsg()
{
}

IBWireMsg& IBWireMsg::operator=(const IBWireMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void IBWireMsg::copy(const IBWireMsg& other)
{
    this->VL = other.VL;
}

void IBWireMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->VL);
}

void IBWireMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->VL);
}

short IBWireMsg::getVL() const
{
    return this->VL;
}

void IBWireMsg::setVL(short VL)
{
    this->VL = VL;
}

class IBWireMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBWireMsgDescriptor();
    virtual ~IBWireMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBWireMsgDescriptor)

IBWireMsgDescriptor::IBWireMsgDescriptor() : omnetpp::cClassDescriptor("IBWireMsg", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

IBWireMsgDescriptor::~IBWireMsgDescriptor()
{
    delete[] propertynames;
}

bool IBWireMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBWireMsg *>(obj)!=nullptr;
}

const char **IBWireMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBWireMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBWireMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int IBWireMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *IBWireMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "VL",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int IBWireMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='V' && strcmp(fieldName, "VL")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBWireMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "short",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **IBWireMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBWireMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBWireMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBWireMsg *pp = (IBWireMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBWireMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBWireMsg *pp = (IBWireMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBWireMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBWireMsg *pp = (IBWireMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getVL());
        default: return "";
    }
}

bool IBWireMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBWireMsg *pp = (IBWireMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setVL(string2long(value)); return true;
        default: return false;
    }
}

const char *IBWireMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBWireMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBWireMsg *pp = (IBWireMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBDataMsg)

IBDataMsg::IBDataMsg(const char *name, short kind) : ::IBWireMsg(name,kind)
{
    this->appIdx = 0;
    this->msgIdx = 0;
    this->msgLen = 0;
    this->pktIdx = 0;
    this->packetId = 0;
    this->packetSn = 0;
    this->flitSn = 0;
    this->PacketLength = 0;
    this->PacketLengthBytes = 0;
    this->dstLid = 0;
    this->srcLid = 0;
    this->SL = 0;
    this->useStatic = 0;
    this->SwTimeStamp = 0;
    this->injectionTime = 0;
    this->beforeAnySwitch = false;
}

IBDataMsg::IBDataMsg(const IBDataMsg& other) : ::IBWireMsg(other)
{
    copy(other);
}

IBDataMsg::~IBDataMsg()
{
}

IBDataMsg& IBDataMsg::operator=(const IBDataMsg& other)
{
    if (this==&other) return *this;
    ::IBWireMsg::operator=(other);
    copy(other);
    return *this;
}

void IBDataMsg::copy(const IBDataMsg& other)
{
    this->appIdx = other.appIdx;
    this->msgIdx = other.msgIdx;
    this->msgLen = other.msgLen;
    this->pktIdx = other.pktIdx;
    this->packetId = other.packetId;
    this->packetSn = other.packetSn;
    this->flitSn = other.flitSn;
    this->PacketLength = other.PacketLength;
    this->PacketLengthBytes = other.PacketLengthBytes;
    this->dstLid = other.dstLid;
    this->srcLid = other.srcLid;
    this->SL = other.SL;
    this->useStatic = other.useStatic;
    this->SwTimeStamp = other.SwTimeStamp;
    this->injectionTime = other.injectionTime;
    this->beforeAnySwitch = other.beforeAnySwitch;
}

void IBDataMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::IBWireMsg::parsimPack(b);
    doParsimPacking(b,this->appIdx);
    doParsimPacking(b,this->msgIdx);
    doParsimPacking(b,this->msgLen);
    doParsimPacking(b,this->pktIdx);
    doParsimPacking(b,this->packetId);
    doParsimPacking(b,this->packetSn);
    doParsimPacking(b,this->flitSn);
    doParsimPacking(b,this->PacketLength);
    doParsimPacking(b,this->PacketLengthBytes);
    doParsimPacking(b,this->dstLid);
    doParsimPacking(b,this->srcLid);
    doParsimPacking(b,this->SL);
    doParsimPacking(b,this->useStatic);
    doParsimPacking(b,this->SwTimeStamp);
    doParsimPacking(b,this->injectionTime);
    doParsimPacking(b,this->beforeAnySwitch);
}

void IBDataMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::IBWireMsg::parsimUnpack(b);
    doParsimUnpacking(b,this->appIdx);
    doParsimUnpacking(b,this->msgIdx);
    doParsimUnpacking(b,this->msgLen);
    doParsimUnpacking(b,this->pktIdx);
    doParsimUnpacking(b,this->packetId);
    doParsimUnpacking(b,this->packetSn);
    doParsimUnpacking(b,this->flitSn);
    doParsimUnpacking(b,this->PacketLength);
    doParsimUnpacking(b,this->PacketLengthBytes);
    doParsimUnpacking(b,this->dstLid);
    doParsimUnpacking(b,this->srcLid);
    doParsimUnpacking(b,this->SL);
    doParsimUnpacking(b,this->useStatic);
    doParsimUnpacking(b,this->SwTimeStamp);
    doParsimUnpacking(b,this->injectionTime);
    doParsimUnpacking(b,this->beforeAnySwitch);
}

int IBDataMsg::getAppIdx() const
{
    return this->appIdx;
}

void IBDataMsg::setAppIdx(int appIdx)
{
    this->appIdx = appIdx;
}

int IBDataMsg::getMsgIdx() const
{
    return this->msgIdx;
}

void IBDataMsg::setMsgIdx(int msgIdx)
{
    this->msgIdx = msgIdx;
}

int IBDataMsg::getMsgLen() const
{
    return this->msgLen;
}

void IBDataMsg::setMsgLen(int msgLen)
{
    this->msgLen = msgLen;
}

int IBDataMsg::getPktIdx() const
{
    return this->pktIdx;
}

void IBDataMsg::setPktIdx(int pktIdx)
{
    this->pktIdx = pktIdx;
}

int IBDataMsg::getPacketId() const
{
    return this->packetId;
}

void IBDataMsg::setPacketId(int packetId)
{
    this->packetId = packetId;
}

int IBDataMsg::getPacketSn() const
{
    return this->packetSn;
}

void IBDataMsg::setPacketSn(int packetSn)
{
    this->packetSn = packetSn;
}

int IBDataMsg::getFlitSn() const
{
    return this->flitSn;
}

void IBDataMsg::setFlitSn(int flitSn)
{
    this->flitSn = flitSn;
}

int IBDataMsg::getPacketLength() const
{
    return this->PacketLength;
}

void IBDataMsg::setPacketLength(int PacketLength)
{
    this->PacketLength = PacketLength;
}

int IBDataMsg::getPacketLengthBytes() const
{
    return this->PacketLengthBytes;
}

void IBDataMsg::setPacketLengthBytes(int PacketLengthBytes)
{
    this->PacketLengthBytes = PacketLengthBytes;
}

int IBDataMsg::getDstLid() const
{
    return this->dstLid;
}

void IBDataMsg::setDstLid(int dstLid)
{
    this->dstLid = dstLid;
}

int IBDataMsg::getSrcLid() const
{
    return this->srcLid;
}

void IBDataMsg::setSrcLid(int srcLid)
{
    this->srcLid = srcLid;
}

int IBDataMsg::getSL() const
{
    return this->SL;
}

void IBDataMsg::setSL(int SL)
{
    this->SL = SL;
}

int IBDataMsg::getUseStatic() const
{
    return this->useStatic;
}

void IBDataMsg::setUseStatic(int useStatic)
{
    this->useStatic = useStatic;
}

::omnetpp::simtime_t IBDataMsg::getSwTimeStamp() const
{
    return this->SwTimeStamp;
}

void IBDataMsg::setSwTimeStamp(::omnetpp::simtime_t SwTimeStamp)
{
    this->SwTimeStamp = SwTimeStamp;
}

::omnetpp::simtime_t IBDataMsg::getInjectionTime() const
{
    return this->injectionTime;
}

void IBDataMsg::setInjectionTime(::omnetpp::simtime_t injectionTime)
{
    this->injectionTime = injectionTime;
}

bool IBDataMsg::getBeforeAnySwitch() const
{
    return this->beforeAnySwitch;
}

void IBDataMsg::setBeforeAnySwitch(bool beforeAnySwitch)
{
    this->beforeAnySwitch = beforeAnySwitch;
}

class IBDataMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBDataMsgDescriptor();
    virtual ~IBDataMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBDataMsgDescriptor)

IBDataMsgDescriptor::IBDataMsgDescriptor() : omnetpp::cClassDescriptor("IBDataMsg", "IBWireMsg")
{
    propertynames = nullptr;
}

IBDataMsgDescriptor::~IBDataMsgDescriptor()
{
    delete[] propertynames;
}

bool IBDataMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBDataMsg *>(obj)!=nullptr;
}

const char **IBDataMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBDataMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBDataMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 16+basedesc->getFieldCount() : 16;
}

unsigned int IBDataMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<16) ? fieldTypeFlags[field] : 0;
}

const char *IBDataMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "appIdx",
        "msgIdx",
        "msgLen",
        "pktIdx",
        "packetId",
        "packetSn",
        "flitSn",
        "PacketLength",
        "PacketLengthBytes",
        "dstLid",
        "srcLid",
        "SL",
        "useStatic",
        "SwTimeStamp",
        "injectionTime",
        "beforeAnySwitch",
    };
    return (field>=0 && field<16) ? fieldNames[field] : nullptr;
}

int IBDataMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "appIdx")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgIdx")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgLen")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktIdx")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetId")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetSn")==0) return base+5;
    if (fieldName[0]=='f' && strcmp(fieldName, "flitSn")==0) return base+6;
    if (fieldName[0]=='P' && strcmp(fieldName, "PacketLength")==0) return base+7;
    if (fieldName[0]=='P' && strcmp(fieldName, "PacketLengthBytes")==0) return base+8;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstLid")==0) return base+9;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcLid")==0) return base+10;
    if (fieldName[0]=='S' && strcmp(fieldName, "SL")==0) return base+11;
    if (fieldName[0]=='u' && strcmp(fieldName, "useStatic")==0) return base+12;
    if (fieldName[0]=='S' && strcmp(fieldName, "SwTimeStamp")==0) return base+13;
    if (fieldName[0]=='i' && strcmp(fieldName, "injectionTime")==0) return base+14;
    if (fieldName[0]=='b' && strcmp(fieldName, "beforeAnySwitch")==0) return base+15;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBDataMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "simtime_t",
        "simtime_t",
        "bool",
    };
    return (field>=0 && field<16) ? fieldTypeStrings[field] : nullptr;
}

const char **IBDataMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBDataMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBDataMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBDataMsg *pp = (IBDataMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBDataMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBDataMsg *pp = (IBDataMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBDataMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBDataMsg *pp = (IBDataMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAppIdx());
        case 1: return long2string(pp->getMsgIdx());
        case 2: return long2string(pp->getMsgLen());
        case 3: return long2string(pp->getPktIdx());
        case 4: return long2string(pp->getPacketId());
        case 5: return long2string(pp->getPacketSn());
        case 6: return long2string(pp->getFlitSn());
        case 7: return long2string(pp->getPacketLength());
        case 8: return long2string(pp->getPacketLengthBytes());
        case 9: return long2string(pp->getDstLid());
        case 10: return long2string(pp->getSrcLid());
        case 11: return long2string(pp->getSL());
        case 12: return long2string(pp->getUseStatic());
        case 13: return simtime2string(pp->getSwTimeStamp());
        case 14: return simtime2string(pp->getInjectionTime());
        case 15: return bool2string(pp->getBeforeAnySwitch());
        default: return "";
    }
}

bool IBDataMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBDataMsg *pp = (IBDataMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setAppIdx(string2long(value)); return true;
        case 1: pp->setMsgIdx(string2long(value)); return true;
        case 2: pp->setMsgLen(string2long(value)); return true;
        case 3: pp->setPktIdx(string2long(value)); return true;
        case 4: pp->setPacketId(string2long(value)); return true;
        case 5: pp->setPacketSn(string2long(value)); return true;
        case 6: pp->setFlitSn(string2long(value)); return true;
        case 7: pp->setPacketLength(string2long(value)); return true;
        case 8: pp->setPacketLengthBytes(string2long(value)); return true;
        case 9: pp->setDstLid(string2long(value)); return true;
        case 10: pp->setSrcLid(string2long(value)); return true;
        case 11: pp->setSL(string2long(value)); return true;
        case 12: pp->setUseStatic(string2long(value)); return true;
        case 13: pp->setSwTimeStamp(string2simtime(value)); return true;
        case 14: pp->setInjectionTime(string2simtime(value)); return true;
        case 15: pp->setBeforeAnySwitch(string2bool(value)); return true;
        default: return false;
    }
}

const char *IBDataMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBDataMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBDataMsg *pp = (IBDataMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBFlowControl)

IBFlowControl::IBFlowControl(const char *name, short kind) : ::IBWireMsg(name,kind)
{
    this->FCTBS = 0;
    this->FCCL = 0;
}

IBFlowControl::IBFlowControl(const IBFlowControl& other) : ::IBWireMsg(other)
{
    copy(other);
}

IBFlowControl::~IBFlowControl()
{
}

IBFlowControl& IBFlowControl::operator=(const IBFlowControl& other)
{
    if (this==&other) return *this;
    ::IBWireMsg::operator=(other);
    copy(other);
    return *this;
}

void IBFlowControl::copy(const IBFlowControl& other)
{
    this->FCTBS = other.FCTBS;
    this->FCCL = other.FCCL;
}

void IBFlowControl::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::IBWireMsg::parsimPack(b);
    doParsimPacking(b,this->FCTBS);
    doParsimPacking(b,this->FCCL);
}

void IBFlowControl::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::IBWireMsg::parsimUnpack(b);
    doParsimUnpacking(b,this->FCTBS);
    doParsimUnpacking(b,this->FCCL);
}

long IBFlowControl::getFCTBS() const
{
    return this->FCTBS;
}

void IBFlowControl::setFCTBS(long FCTBS)
{
    this->FCTBS = FCTBS;
}

long IBFlowControl::getFCCL() const
{
    return this->FCCL;
}

void IBFlowControl::setFCCL(long FCCL)
{
    this->FCCL = FCCL;
}

class IBFlowControlDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBFlowControlDescriptor();
    virtual ~IBFlowControlDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBFlowControlDescriptor)

IBFlowControlDescriptor::IBFlowControlDescriptor() : omnetpp::cClassDescriptor("IBFlowControl", "IBWireMsg")
{
    propertynames = nullptr;
}

IBFlowControlDescriptor::~IBFlowControlDescriptor()
{
    delete[] propertynames;
}

bool IBFlowControlDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBFlowControl *>(obj)!=nullptr;
}

const char **IBFlowControlDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBFlowControlDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBFlowControlDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int IBFlowControlDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *IBFlowControlDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "FCTBS",
        "FCCL",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int IBFlowControlDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='F' && strcmp(fieldName, "FCTBS")==0) return base+0;
    if (fieldName[0]=='F' && strcmp(fieldName, "FCCL")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBFlowControlDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "long",
        "long",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **IBFlowControlDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBFlowControlDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBFlowControlDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBFlowControl *pp = (IBFlowControl *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBFlowControlDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBFlowControl *pp = (IBFlowControl *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBFlowControlDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBFlowControl *pp = (IBFlowControl *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getFCTBS());
        case 1: return long2string(pp->getFCCL());
        default: return "";
    }
}

bool IBFlowControlDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBFlowControl *pp = (IBFlowControl *)object; (void)pp;
    switch (field) {
        case 0: pp->setFCTBS(string2long(value)); return true;
        case 1: pp->setFCCL(string2long(value)); return true;
        default: return false;
    }
}

const char *IBFlowControlDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBFlowControlDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBFlowControl *pp = (IBFlowControl *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBTQLoadUpdateMsg)

IBTQLoadUpdateMsg::IBTQLoadUpdateMsg(const char *name, short kind) : ::IBWireMsg(name,kind)
{
    this->load = 0;
    this->srcRank = 0;
    this->firstLid = 0;
    this->lastLid = 0;
}

IBTQLoadUpdateMsg::IBTQLoadUpdateMsg(const IBTQLoadUpdateMsg& other) : ::IBWireMsg(other)
{
    copy(other);
}

IBTQLoadUpdateMsg::~IBTQLoadUpdateMsg()
{
}

IBTQLoadUpdateMsg& IBTQLoadUpdateMsg::operator=(const IBTQLoadUpdateMsg& other)
{
    if (this==&other) return *this;
    ::IBWireMsg::operator=(other);
    copy(other);
    return *this;
}

void IBTQLoadUpdateMsg::copy(const IBTQLoadUpdateMsg& other)
{
    this->load = other.load;
    this->srcRank = other.srcRank;
    this->firstLid = other.firstLid;
    this->lastLid = other.lastLid;
}

void IBTQLoadUpdateMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::IBWireMsg::parsimPack(b);
    doParsimPacking(b,this->load);
    doParsimPacking(b,this->srcRank);
    doParsimPacking(b,this->firstLid);
    doParsimPacking(b,this->lastLid);
}

void IBTQLoadUpdateMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::IBWireMsg::parsimUnpack(b);
    doParsimUnpacking(b,this->load);
    doParsimUnpacking(b,this->srcRank);
    doParsimUnpacking(b,this->firstLid);
    doParsimUnpacking(b,this->lastLid);
}

int IBTQLoadUpdateMsg::getLoad() const
{
    return this->load;
}

void IBTQLoadUpdateMsg::setLoad(int load)
{
    this->load = load;
}

int IBTQLoadUpdateMsg::getSrcRank() const
{
    return this->srcRank;
}

void IBTQLoadUpdateMsg::setSrcRank(int srcRank)
{
    this->srcRank = srcRank;
}

int IBTQLoadUpdateMsg::getFirstLid() const
{
    return this->firstLid;
}

void IBTQLoadUpdateMsg::setFirstLid(int firstLid)
{
    this->firstLid = firstLid;
}

int IBTQLoadUpdateMsg::getLastLid() const
{
    return this->lastLid;
}

void IBTQLoadUpdateMsg::setLastLid(int lastLid)
{
    this->lastLid = lastLid;
}

class IBTQLoadUpdateMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBTQLoadUpdateMsgDescriptor();
    virtual ~IBTQLoadUpdateMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBTQLoadUpdateMsgDescriptor)

IBTQLoadUpdateMsgDescriptor::IBTQLoadUpdateMsgDescriptor() : omnetpp::cClassDescriptor("IBTQLoadUpdateMsg", "IBWireMsg")
{
    propertynames = nullptr;
}

IBTQLoadUpdateMsgDescriptor::~IBTQLoadUpdateMsgDescriptor()
{
    delete[] propertynames;
}

bool IBTQLoadUpdateMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBTQLoadUpdateMsg *>(obj)!=nullptr;
}

const char **IBTQLoadUpdateMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBTQLoadUpdateMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBTQLoadUpdateMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int IBTQLoadUpdateMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *IBTQLoadUpdateMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "load",
        "srcRank",
        "firstLid",
        "lastLid",
    };
    return (field>=0 && field<4) ? fieldNames[field] : nullptr;
}

int IBTQLoadUpdateMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='l' && strcmp(fieldName, "load")==0) return base+0;
    if (fieldName[0]=='s' && strcmp(fieldName, "srcRank")==0) return base+1;
    if (fieldName[0]=='f' && strcmp(fieldName, "firstLid")==0) return base+2;
    if (fieldName[0]=='l' && strcmp(fieldName, "lastLid")==0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBTQLoadUpdateMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : nullptr;
}

const char **IBTQLoadUpdateMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBTQLoadUpdateMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBTQLoadUpdateMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBTQLoadUpdateMsg *pp = (IBTQLoadUpdateMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBTQLoadUpdateMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBTQLoadUpdateMsg *pp = (IBTQLoadUpdateMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBTQLoadUpdateMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBTQLoadUpdateMsg *pp = (IBTQLoadUpdateMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getLoad());
        case 1: return long2string(pp->getSrcRank());
        case 2: return long2string(pp->getFirstLid());
        case 3: return long2string(pp->getLastLid());
        default: return "";
    }
}

bool IBTQLoadUpdateMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBTQLoadUpdateMsg *pp = (IBTQLoadUpdateMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setLoad(string2long(value)); return true;
        case 1: pp->setSrcRank(string2long(value)); return true;
        case 2: pp->setFirstLid(string2long(value)); return true;
        case 3: pp->setLastLid(string2long(value)); return true;
        default: return false;
    }
}

const char *IBTQLoadUpdateMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBTQLoadUpdateMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBTQLoadUpdateMsg *pp = (IBTQLoadUpdateMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBTxCredMsg)

IBTxCredMsg::IBTxCredMsg(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->VL = 0;
    this->FCCL = 0;
    this->FCTBS = 0;
}

IBTxCredMsg::IBTxCredMsg(const IBTxCredMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

IBTxCredMsg::~IBTxCredMsg()
{
}

IBTxCredMsg& IBTxCredMsg::operator=(const IBTxCredMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void IBTxCredMsg::copy(const IBTxCredMsg& other)
{
    this->VL = other.VL;
    this->FCCL = other.FCCL;
    this->FCTBS = other.FCTBS;
}

void IBTxCredMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->VL);
    doParsimPacking(b,this->FCCL);
    doParsimPacking(b,this->FCTBS);
}

void IBTxCredMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->VL);
    doParsimUnpacking(b,this->FCCL);
    doParsimUnpacking(b,this->FCTBS);
}

int IBTxCredMsg::getVL() const
{
    return this->VL;
}

void IBTxCredMsg::setVL(int VL)
{
    this->VL = VL;
}

long IBTxCredMsg::getFCCL() const
{
    return this->FCCL;
}

void IBTxCredMsg::setFCCL(long FCCL)
{
    this->FCCL = FCCL;
}

long IBTxCredMsg::getFCTBS() const
{
    return this->FCTBS;
}

void IBTxCredMsg::setFCTBS(long FCTBS)
{
    this->FCTBS = FCTBS;
}

class IBTxCredMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBTxCredMsgDescriptor();
    virtual ~IBTxCredMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBTxCredMsgDescriptor)

IBTxCredMsgDescriptor::IBTxCredMsgDescriptor() : omnetpp::cClassDescriptor("IBTxCredMsg", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

IBTxCredMsgDescriptor::~IBTxCredMsgDescriptor()
{
    delete[] propertynames;
}

bool IBTxCredMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBTxCredMsg *>(obj)!=nullptr;
}

const char **IBTxCredMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBTxCredMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBTxCredMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int IBTxCredMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *IBTxCredMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "VL",
        "FCCL",
        "FCTBS",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int IBTxCredMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='V' && strcmp(fieldName, "VL")==0) return base+0;
    if (fieldName[0]=='F' && strcmp(fieldName, "FCCL")==0) return base+1;
    if (fieldName[0]=='F' && strcmp(fieldName, "FCTBS")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBTxCredMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "long",
        "long",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **IBTxCredMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBTxCredMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBTxCredMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBTxCredMsg *pp = (IBTxCredMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBTxCredMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBTxCredMsg *pp = (IBTxCredMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBTxCredMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBTxCredMsg *pp = (IBTxCredMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getVL());
        case 1: return long2string(pp->getFCCL());
        case 2: return long2string(pp->getFCTBS());
        default: return "";
    }
}

bool IBTxCredMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBTxCredMsg *pp = (IBTxCredMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setVL(string2long(value)); return true;
        case 1: pp->setFCCL(string2long(value)); return true;
        case 2: pp->setFCTBS(string2long(value)); return true;
        default: return false;
    }
}

const char *IBTxCredMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBTxCredMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBTxCredMsg *pp = (IBTxCredMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBRxCredMsg)

IBRxCredMsg::IBRxCredMsg(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->VL = 0;
    this->FCCL = 0;
}

IBRxCredMsg::IBRxCredMsg(const IBRxCredMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

IBRxCredMsg::~IBRxCredMsg()
{
}

IBRxCredMsg& IBRxCredMsg::operator=(const IBRxCredMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void IBRxCredMsg::copy(const IBRxCredMsg& other)
{
    this->VL = other.VL;
    this->FCCL = other.FCCL;
}

void IBRxCredMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->VL);
    doParsimPacking(b,this->FCCL);
}

void IBRxCredMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->VL);
    doParsimUnpacking(b,this->FCCL);
}

int IBRxCredMsg::getVL() const
{
    return this->VL;
}

void IBRxCredMsg::setVL(int VL)
{
    this->VL = VL;
}

long IBRxCredMsg::getFCCL() const
{
    return this->FCCL;
}

void IBRxCredMsg::setFCCL(long FCCL)
{
    this->FCCL = FCCL;
}

class IBRxCredMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBRxCredMsgDescriptor();
    virtual ~IBRxCredMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBRxCredMsgDescriptor)

IBRxCredMsgDescriptor::IBRxCredMsgDescriptor() : omnetpp::cClassDescriptor("IBRxCredMsg", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

IBRxCredMsgDescriptor::~IBRxCredMsgDescriptor()
{
    delete[] propertynames;
}

bool IBRxCredMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBRxCredMsg *>(obj)!=nullptr;
}

const char **IBRxCredMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBRxCredMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBRxCredMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 2+basedesc->getFieldCount() : 2;
}

unsigned int IBRxCredMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<2) ? fieldTypeFlags[field] : 0;
}

const char *IBRxCredMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "VL",
        "FCCL",
    };
    return (field>=0 && field<2) ? fieldNames[field] : nullptr;
}

int IBRxCredMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='V' && strcmp(fieldName, "VL")==0) return base+0;
    if (fieldName[0]=='F' && strcmp(fieldName, "FCCL")==0) return base+1;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBRxCredMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "long",
    };
    return (field>=0 && field<2) ? fieldTypeStrings[field] : nullptr;
}

const char **IBRxCredMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBRxCredMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBRxCredMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBRxCredMsg *pp = (IBRxCredMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBRxCredMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBRxCredMsg *pp = (IBRxCredMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBRxCredMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBRxCredMsg *pp = (IBRxCredMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getVL());
        case 1: return long2string(pp->getFCCL());
        default: return "";
    }
}

bool IBRxCredMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBRxCredMsg *pp = (IBRxCredMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setVL(string2long(value)); return true;
        case 1: pp->setFCCL(string2long(value)); return true;
        default: return false;
    }
}

const char *IBRxCredMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBRxCredMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBRxCredMsg *pp = (IBRxCredMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBSentMsg)

IBSentMsg::IBSentMsg(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->VL = 0;
    this->usedStatic = 0;
    this->wasLast = 0;
}

IBSentMsg::IBSentMsg(const IBSentMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

IBSentMsg::~IBSentMsg()
{
}

IBSentMsg& IBSentMsg::operator=(const IBSentMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void IBSentMsg::copy(const IBSentMsg& other)
{
    this->VL = other.VL;
    this->usedStatic = other.usedStatic;
    this->wasLast = other.wasLast;
}

void IBSentMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->VL);
    doParsimPacking(b,this->usedStatic);
    doParsimPacking(b,this->wasLast);
}

void IBSentMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->VL);
    doParsimUnpacking(b,this->usedStatic);
    doParsimUnpacking(b,this->wasLast);
}

short IBSentMsg::getVL() const
{
    return this->VL;
}

void IBSentMsg::setVL(short VL)
{
    this->VL = VL;
}

short IBSentMsg::getUsedStatic() const
{
    return this->usedStatic;
}

void IBSentMsg::setUsedStatic(short usedStatic)
{
    this->usedStatic = usedStatic;
}

short IBSentMsg::getWasLast() const
{
    return this->wasLast;
}

void IBSentMsg::setWasLast(short wasLast)
{
    this->wasLast = wasLast;
}

class IBSentMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBSentMsgDescriptor();
    virtual ~IBSentMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBSentMsgDescriptor)

IBSentMsgDescriptor::IBSentMsgDescriptor() : omnetpp::cClassDescriptor("IBSentMsg", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

IBSentMsgDescriptor::~IBSentMsgDescriptor()
{
    delete[] propertynames;
}

bool IBSentMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBSentMsg *>(obj)!=nullptr;
}

const char **IBSentMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBSentMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBSentMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount() : 3;
}

unsigned int IBSentMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *IBSentMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "VL",
        "usedStatic",
        "wasLast",
    };
    return (field>=0 && field<3) ? fieldNames[field] : nullptr;
}

int IBSentMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='V' && strcmp(fieldName, "VL")==0) return base+0;
    if (fieldName[0]=='u' && strcmp(fieldName, "usedStatic")==0) return base+1;
    if (fieldName[0]=='w' && strcmp(fieldName, "wasLast")==0) return base+2;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBSentMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "short",
        "short",
        "short",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : nullptr;
}

const char **IBSentMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBSentMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBSentMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBSentMsg *pp = (IBSentMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBSentMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBSentMsg *pp = (IBSentMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBSentMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBSentMsg *pp = (IBSentMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getVL());
        case 1: return long2string(pp->getUsedStatic());
        case 2: return long2string(pp->getWasLast());
        default: return "";
    }
}

bool IBSentMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBSentMsg *pp = (IBSentMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setVL(string2long(value)); return true;
        case 1: pp->setUsedStatic(string2long(value)); return true;
        case 2: pp->setWasLast(string2long(value)); return true;
        default: return false;
    }
}

const char *IBSentMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBSentMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBSentMsg *pp = (IBSentMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBFreeMsg)

IBFreeMsg::IBFreeMsg(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->VL = 0;
}

IBFreeMsg::IBFreeMsg(const IBFreeMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

IBFreeMsg::~IBFreeMsg()
{
}

IBFreeMsg& IBFreeMsg::operator=(const IBFreeMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void IBFreeMsg::copy(const IBFreeMsg& other)
{
    this->VL = other.VL;
}

void IBFreeMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->VL);
}

void IBFreeMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->VL);
}

short IBFreeMsg::getVL() const
{
    return this->VL;
}

void IBFreeMsg::setVL(short VL)
{
    this->VL = VL;
}

class IBFreeMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBFreeMsgDescriptor();
    virtual ~IBFreeMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBFreeMsgDescriptor)

IBFreeMsgDescriptor::IBFreeMsgDescriptor() : omnetpp::cClassDescriptor("IBFreeMsg", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

IBFreeMsgDescriptor::~IBFreeMsgDescriptor()
{
    delete[] propertynames;
}

bool IBFreeMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBFreeMsg *>(obj)!=nullptr;
}

const char **IBFreeMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBFreeMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBFreeMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int IBFreeMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *IBFreeMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "VL",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int IBFreeMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='V' && strcmp(fieldName, "VL")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBFreeMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "short",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **IBFreeMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBFreeMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBFreeMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBFreeMsg *pp = (IBFreeMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBFreeMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBFreeMsg *pp = (IBFreeMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBFreeMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBFreeMsg *pp = (IBFreeMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getVL());
        default: return "";
    }
}

bool IBFreeMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBFreeMsg *pp = (IBFreeMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setVL(string2long(value)); return true;
        default: return false;
    }
}

const char *IBFreeMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBFreeMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBFreeMsg *pp = (IBFreeMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBDoneMsg)

IBDoneMsg::IBDoneMsg(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->VL = 0;
}

IBDoneMsg::IBDoneMsg(const IBDoneMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

IBDoneMsg::~IBDoneMsg()
{
}

IBDoneMsg& IBDoneMsg::operator=(const IBDoneMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void IBDoneMsg::copy(const IBDoneMsg& other)
{
    this->VL = other.VL;
}

void IBDoneMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->VL);
}

void IBDoneMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->VL);
}

short IBDoneMsg::getVL() const
{
    return this->VL;
}

void IBDoneMsg::setVL(short VL)
{
    this->VL = VL;
}

class IBDoneMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBDoneMsgDescriptor();
    virtual ~IBDoneMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBDoneMsgDescriptor)

IBDoneMsgDescriptor::IBDoneMsgDescriptor() : omnetpp::cClassDescriptor("IBDoneMsg", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

IBDoneMsgDescriptor::~IBDoneMsgDescriptor()
{
    delete[] propertynames;
}

bool IBDoneMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBDoneMsg *>(obj)!=nullptr;
}

const char **IBDoneMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBDoneMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBDoneMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int IBDoneMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *IBDoneMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "VL",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int IBDoneMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='V' && strcmp(fieldName, "VL")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBDoneMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "short",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **IBDoneMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBDoneMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBDoneMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBDoneMsg *pp = (IBDoneMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBDoneMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBDoneMsg *pp = (IBDoneMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBDoneMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBDoneMsg *pp = (IBDoneMsg *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getVL());
        default: return "";
    }
}

bool IBDoneMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBDoneMsg *pp = (IBDoneMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setVL(string2long(value)); return true;
        default: return false;
    }
}

const char *IBDoneMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBDoneMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBDoneMsg *pp = (IBDoneMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(IBAppMsg)

IBAppMsg::IBAppMsg(const char *name, short kind) : ::omnetpp::cMessage(name,kind)
{
    this->appIdx = 0;
    this->msgIdx = 0;
    this->SQ = 0;
    this->lenBytes = 0;
    this->lenPkts = 0;
    this->dstLid = 0;
    this->mtuBytes = 0;
    this->pktLenFlits = 0;
    this->pktLenBytes = 0;
    this->pktIdx = 0;
    this->flitIdx = 0;
    this->VL = 0;
}

IBAppMsg::IBAppMsg(const IBAppMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

IBAppMsg::~IBAppMsg()
{
}

IBAppMsg& IBAppMsg::operator=(const IBAppMsg& other)
{
    if (this==&other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void IBAppMsg::copy(const IBAppMsg& other)
{
    this->appIdx = other.appIdx;
    this->msgIdx = other.msgIdx;
    this->SQ = other.SQ;
    this->lenBytes = other.lenBytes;
    this->lenPkts = other.lenPkts;
    this->dstLid = other.dstLid;
    this->mtuBytes = other.mtuBytes;
    this->pktLenFlits = other.pktLenFlits;
    this->pktLenBytes = other.pktLenBytes;
    this->pktIdx = other.pktIdx;
    this->flitIdx = other.flitIdx;
    this->VL = other.VL;
}

void IBAppMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->appIdx);
    doParsimPacking(b,this->msgIdx);
    doParsimPacking(b,this->SQ);
    doParsimPacking(b,this->lenBytes);
    doParsimPacking(b,this->lenPkts);
    doParsimPacking(b,this->dstLid);
    doParsimPacking(b,this->mtuBytes);
    doParsimPacking(b,this->pktLenFlits);
    doParsimPacking(b,this->pktLenBytes);
    doParsimPacking(b,this->pktIdx);
    doParsimPacking(b,this->flitIdx);
    doParsimPacking(b,this->VL);
}

void IBAppMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->appIdx);
    doParsimUnpacking(b,this->msgIdx);
    doParsimUnpacking(b,this->SQ);
    doParsimUnpacking(b,this->lenBytes);
    doParsimUnpacking(b,this->lenPkts);
    doParsimUnpacking(b,this->dstLid);
    doParsimUnpacking(b,this->mtuBytes);
    doParsimUnpacking(b,this->pktLenFlits);
    doParsimUnpacking(b,this->pktLenBytes);
    doParsimUnpacking(b,this->pktIdx);
    doParsimUnpacking(b,this->flitIdx);
    doParsimUnpacking(b,this->VL);
}

unsigned int IBAppMsg::getAppIdx() const
{
    return this->appIdx;
}

void IBAppMsg::setAppIdx(unsigned int appIdx)
{
    this->appIdx = appIdx;
}

unsigned int IBAppMsg::getMsgIdx() const
{
    return this->msgIdx;
}

void IBAppMsg::setMsgIdx(unsigned int msgIdx)
{
    this->msgIdx = msgIdx;
}

unsigned int IBAppMsg::getSQ() const
{
    return this->SQ;
}

void IBAppMsg::setSQ(unsigned int SQ)
{
    this->SQ = SQ;
}

unsigned int IBAppMsg::getLenBytes() const
{
    return this->lenBytes;
}

void IBAppMsg::setLenBytes(unsigned int lenBytes)
{
    this->lenBytes = lenBytes;
}

unsigned int IBAppMsg::getLenPkts() const
{
    return this->lenPkts;
}

void IBAppMsg::setLenPkts(unsigned int lenPkts)
{
    this->lenPkts = lenPkts;
}

unsigned int IBAppMsg::getDstLid() const
{
    return this->dstLid;
}

void IBAppMsg::setDstLid(unsigned int dstLid)
{
    this->dstLid = dstLid;
}

unsigned int IBAppMsg::getMtuBytes() const
{
    return this->mtuBytes;
}

void IBAppMsg::setMtuBytes(unsigned int mtuBytes)
{
    this->mtuBytes = mtuBytes;
}

unsigned int IBAppMsg::getPktLenFlits() const
{
    return this->pktLenFlits;
}

void IBAppMsg::setPktLenFlits(unsigned int pktLenFlits)
{
    this->pktLenFlits = pktLenFlits;
}

unsigned int IBAppMsg::getPktLenBytes() const
{
    return this->pktLenBytes;
}

void IBAppMsg::setPktLenBytes(unsigned int pktLenBytes)
{
    this->pktLenBytes = pktLenBytes;
}

unsigned int IBAppMsg::getPktIdx() const
{
    return this->pktIdx;
}

void IBAppMsg::setPktIdx(unsigned int pktIdx)
{
    this->pktIdx = pktIdx;
}

unsigned int IBAppMsg::getFlitIdx() const
{
    return this->flitIdx;
}

void IBAppMsg::setFlitIdx(unsigned int flitIdx)
{
    this->flitIdx = flitIdx;
}

unsigned int IBAppMsg::getVL() const
{
    return this->VL;
}

void IBAppMsg::setVL(unsigned int VL)
{
    this->VL = VL;
}

class IBAppMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    IBAppMsgDescriptor();
    virtual ~IBAppMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IBAppMsgDescriptor)

IBAppMsgDescriptor::IBAppMsgDescriptor() : omnetpp::cClassDescriptor("IBAppMsg", "omnetpp::cMessage")
{
    propertynames = nullptr;
}

IBAppMsgDescriptor::~IBAppMsgDescriptor()
{
    delete[] propertynames;
}

bool IBAppMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<IBAppMsg *>(obj)!=nullptr;
}

const char **IBAppMsgDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *IBAppMsgDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IBAppMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 12+basedesc->getFieldCount() : 12;
}

unsigned int IBAppMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<12) ? fieldTypeFlags[field] : 0;
}

const char *IBAppMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "appIdx",
        "msgIdx",
        "SQ",
        "lenBytes",
        "lenPkts",
        "dstLid",
        "mtuBytes",
        "pktLenFlits",
        "pktLenBytes",
        "pktIdx",
        "flitIdx",
        "VL",
    };
    return (field>=0 && field<12) ? fieldNames[field] : nullptr;
}

int IBAppMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "appIdx")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgIdx")==0) return base+1;
    if (fieldName[0]=='S' && strcmp(fieldName, "SQ")==0) return base+2;
    if (fieldName[0]=='l' && strcmp(fieldName, "lenBytes")==0) return base+3;
    if (fieldName[0]=='l' && strcmp(fieldName, "lenPkts")==0) return base+4;
    if (fieldName[0]=='d' && strcmp(fieldName, "dstLid")==0) return base+5;
    if (fieldName[0]=='m' && strcmp(fieldName, "mtuBytes")==0) return base+6;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktLenFlits")==0) return base+7;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktLenBytes")==0) return base+8;
    if (fieldName[0]=='p' && strcmp(fieldName, "pktIdx")==0) return base+9;
    if (fieldName[0]=='f' && strcmp(fieldName, "flitIdx")==0) return base+10;
    if (fieldName[0]=='V' && strcmp(fieldName, "VL")==0) return base+11;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IBAppMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
        "unsigned int",
    };
    return (field>=0 && field<12) ? fieldTypeStrings[field] : nullptr;
}

const char **IBAppMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *IBAppMsgDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int IBAppMsgDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    IBAppMsg *pp = (IBAppMsg *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *IBAppMsgDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBAppMsg *pp = (IBAppMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string IBAppMsgDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    IBAppMsg *pp = (IBAppMsg *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getAppIdx());
        case 1: return ulong2string(pp->getMsgIdx());
        case 2: return ulong2string(pp->getSQ());
        case 3: return ulong2string(pp->getLenBytes());
        case 4: return ulong2string(pp->getLenPkts());
        case 5: return ulong2string(pp->getDstLid());
        case 6: return ulong2string(pp->getMtuBytes());
        case 7: return ulong2string(pp->getPktLenFlits());
        case 8: return ulong2string(pp->getPktLenBytes());
        case 9: return ulong2string(pp->getPktIdx());
        case 10: return ulong2string(pp->getFlitIdx());
        case 11: return ulong2string(pp->getVL());
        default: return "";
    }
}

bool IBAppMsgDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    IBAppMsg *pp = (IBAppMsg *)object; (void)pp;
    switch (field) {
        case 0: pp->setAppIdx(string2ulong(value)); return true;
        case 1: pp->setMsgIdx(string2ulong(value)); return true;
        case 2: pp->setSQ(string2ulong(value)); return true;
        case 3: pp->setLenBytes(string2ulong(value)); return true;
        case 4: pp->setLenPkts(string2ulong(value)); return true;
        case 5: pp->setDstLid(string2ulong(value)); return true;
        case 6: pp->setMtuBytes(string2ulong(value)); return true;
        case 7: pp->setPktLenFlits(string2ulong(value)); return true;
        case 8: pp->setPktLenBytes(string2ulong(value)); return true;
        case 9: pp->setPktIdx(string2ulong(value)); return true;
        case 10: pp->setFlitIdx(string2ulong(value)); return true;
        case 11: pp->setVL(string2ulong(value)); return true;
        default: return false;
    }
}

const char *IBAppMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *IBAppMsgDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    IBAppMsg *pp = (IBAppMsg *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


