// **********************************************************************
//
// Copyright (c) 2003-2013 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.5.1
//
// <auto-generated>
//
// Generated from file `FootPreassureSensor.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#ifndef ____FootPreassureSensor_h__
#define ____FootPreassureSensor_h__

#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/StreamHelpers.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <IceUtil/ScopedArray.h>
#include <IceUtil/Optional.h>
#include <Ice/StreamF.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 305
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

namespace IceProxy
{

namespace RoboCompFootPreassureSensor
{

class FootPreassureSensor;
void __read(::IceInternal::BasicStream*, ::IceInternal::ProxyHandle< ::IceProxy::RoboCompFootPreassureSensor::FootPreassureSensor>&);
::IceProxy::Ice::Object* upCast(::IceProxy::RoboCompFootPreassureSensor::FootPreassureSensor*);

}

}

namespace RoboCompFootPreassureSensor
{

class FootPreassureSensor;
bool operator==(const FootPreassureSensor&, const FootPreassureSensor&);
bool operator<(const FootPreassureSensor&, const FootPreassureSensor&);
::Ice::Object* upCast(::RoboCompFootPreassureSensor::FootPreassureSensor*);
typedef ::IceInternal::Handle< ::RoboCompFootPreassureSensor::FootPreassureSensor> FootPreassureSensorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::RoboCompFootPreassureSensor::FootPreassureSensor> FootPreassureSensorPrx;
void __patch(FootPreassureSensorPtr&, const ::Ice::ObjectPtr&);

}

namespace RoboCompFootPreassureSensor
{

typedef ::std::map< ::std::string, ::Ice::Int> Buffer;

}

namespace RoboCompFootPreassureSensor
{

class Callback_FootPreassureSensor_readSensors_Base : virtual public ::IceInternal::CallbackBase { };
typedef ::IceUtil::Handle< Callback_FootPreassureSensor_readSensors_Base> Callback_FootPreassureSensor_readSensorsPtr;

class Callback_FootPreassureSensor_readSensor_Base : virtual public ::IceInternal::CallbackBase { };
typedef ::IceUtil::Handle< Callback_FootPreassureSensor_readSensor_Base> Callback_FootPreassureSensor_readSensorPtr;

}

namespace IceProxy
{

namespace RoboCompFootPreassureSensor
{

class FootPreassureSensor : virtual public ::IceProxy::Ice::Object
{
public:

    ::RoboCompFootPreassureSensor::Buffer readSensors()
    {
        return readSensors(0);
    }
    ::RoboCompFootPreassureSensor::Buffer readSensors(const ::Ice::Context& __ctx)
    {
        return readSensors(&__ctx);
    }
#ifdef ICE_CPP11
    ::Ice::AsyncResultPtr
    begin_readSensors(const ::IceInternal::Function<void (const ::RoboCompFootPreassureSensor::Buffer&)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& __sent = ::IceInternal::Function<void (bool)>())
    {
        return __begin_readSensors(0, __response, __exception, __sent);
    }
    ::Ice::AsyncResultPtr
    begin_readSensors(const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_readSensors(0, ::Ice::newCallback(__completed, __sent), 0);
    }
    ::Ice::AsyncResultPtr
    begin_readSensors(const ::Ice::Context& __ctx, const ::IceInternal::Function<void (const ::RoboCompFootPreassureSensor::Buffer&)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& __sent = ::IceInternal::Function<void (bool)>())
    {
        return __begin_readSensors(&__ctx, __response, __exception, __sent);
    }
    ::Ice::AsyncResultPtr
    begin_readSensors(const ::Ice::Context& __ctx, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_readSensors(&__ctx, ::Ice::newCallback(__completed, __sent));
    }
    
private:

    ::Ice::AsyncResultPtr __begin_readSensors(const ::Ice::Context* __ctx, const ::IceInternal::Function<void (const ::RoboCompFootPreassureSensor::Buffer&)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception, const ::IceInternal::Function<void (bool)>& __sent)
    {
        class Cpp11CB : public ::IceInternal::Cpp11FnCallbackNC
        {
        public:

            Cpp11CB(const ::std::function<void (const ::RoboCompFootPreassureSensor::Buffer&)>& responseFunc, const ::std::function<void (const ::Ice::Exception&)>& exceptionFunc, const ::std::function<void (bool)>& sentFunc) :
                ::IceInternal::Cpp11FnCallbackNC(exceptionFunc, sentFunc),
                _response(responseFunc)
            {
                CallbackBase::checkCallback(true, responseFunc || exceptionFunc != nullptr);
            }

            virtual void __completed(const ::Ice::AsyncResultPtr& __result) const
            {
                ::RoboCompFootPreassureSensor::FootPreassureSensorPrx __proxy = ::RoboCompFootPreassureSensor::FootPreassureSensorPrx::uncheckedCast(__result->getProxy());
                ::RoboCompFootPreassureSensor::Buffer __ret;
                try
                {
                    __ret = __proxy->end_readSensors(__result);
                }
                catch(::Ice::Exception& ex)
                {
                    Cpp11FnCallbackNC::__exception(__result, ex);
                    return;
                }
                if(_response != nullptr)
                {
                    _response(__ret);
                }
            }
        
        private:
            
            ::std::function<void (const ::RoboCompFootPreassureSensor::Buffer&)> _response;
        };
        return begin_readSensors(__ctx, new Cpp11CB(__response, __exception, __sent));
    }
    
public:
#endif

    ::Ice::AsyncResultPtr begin_readSensors()
    {
        return begin_readSensors(0, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_readSensors(const ::Ice::Context& __ctx)
    {
        return begin_readSensors(&__ctx, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_readSensors(const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_readSensors(0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_readSensors(const ::Ice::Context& __ctx, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_readSensors(&__ctx, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_readSensors(const ::RoboCompFootPreassureSensor::Callback_FootPreassureSensor_readSensorsPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_readSensors(0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_readSensors(const ::Ice::Context& __ctx, const ::RoboCompFootPreassureSensor::Callback_FootPreassureSensor_readSensorsPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_readSensors(&__ctx, __del, __cookie);
    }

    ::RoboCompFootPreassureSensor::Buffer end_readSensors(const ::Ice::AsyncResultPtr&);
    
private:

    ::RoboCompFootPreassureSensor::Buffer readSensors(const ::Ice::Context*);
    ::Ice::AsyncResultPtr begin_readSensors(const ::Ice::Context*, const ::IceInternal::CallbackBasePtr&, const ::Ice::LocalObjectPtr& __cookie = 0);
    
public:

    ::Ice::Int readSensor(const ::std::string& name)
    {
        return readSensor(name, 0);
    }
    ::Ice::Int readSensor(const ::std::string& name, const ::Ice::Context& __ctx)
    {
        return readSensor(name, &__ctx);
    }
#ifdef ICE_CPP11
    ::Ice::AsyncResultPtr
    begin_readSensor(const ::std::string& name, const ::IceInternal::Function<void (::Ice::Int)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& __sent = ::IceInternal::Function<void (bool)>())
    {
        return __begin_readSensor(name, 0, __response, __exception, __sent);
    }
    ::Ice::AsyncResultPtr
    begin_readSensor(const ::std::string& name, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_readSensor(name, 0, ::Ice::newCallback(__completed, __sent), 0);
    }
    ::Ice::AsyncResultPtr
    begin_readSensor(const ::std::string& name, const ::Ice::Context& __ctx, const ::IceInternal::Function<void (::Ice::Int)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception = ::IceInternal::Function<void (const ::Ice::Exception&)>(), const ::IceInternal::Function<void (bool)>& __sent = ::IceInternal::Function<void (bool)>())
    {
        return __begin_readSensor(name, &__ctx, __response, __exception, __sent);
    }
    ::Ice::AsyncResultPtr
    begin_readSensor(const ::std::string& name, const ::Ice::Context& __ctx, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __completed, const ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>& __sent = ::IceInternal::Function<void (const ::Ice::AsyncResultPtr&)>())
    {
        return begin_readSensor(name, &__ctx, ::Ice::newCallback(__completed, __sent));
    }
    
private:

    ::Ice::AsyncResultPtr __begin_readSensor(const ::std::string& name, const ::Ice::Context* __ctx, const ::IceInternal::Function<void (::Ice::Int)>& __response, const ::IceInternal::Function<void (const ::Ice::Exception&)>& __exception, const ::IceInternal::Function<void (bool)>& __sent)
    {
        class Cpp11CB : public ::IceInternal::Cpp11FnCallbackNC
        {
        public:

            Cpp11CB(const ::std::function<void (::Ice::Int)>& responseFunc, const ::std::function<void (const ::Ice::Exception&)>& exceptionFunc, const ::std::function<void (bool)>& sentFunc) :
                ::IceInternal::Cpp11FnCallbackNC(exceptionFunc, sentFunc),
                _response(responseFunc)
            {
                CallbackBase::checkCallback(true, responseFunc || exceptionFunc != nullptr);
            }

            virtual void __completed(const ::Ice::AsyncResultPtr& __result) const
            {
                ::RoboCompFootPreassureSensor::FootPreassureSensorPrx __proxy = ::RoboCompFootPreassureSensor::FootPreassureSensorPrx::uncheckedCast(__result->getProxy());
                ::Ice::Int __ret;
                try
                {
                    __ret = __proxy->end_readSensor(__result);
                }
                catch(::Ice::Exception& ex)
                {
                    Cpp11FnCallbackNC::__exception(__result, ex);
                    return;
                }
                if(_response != nullptr)
                {
                    _response(__ret);
                }
            }
        
        private:
            
            ::std::function<void (::Ice::Int)> _response;
        };
        return begin_readSensor(name, __ctx, new Cpp11CB(__response, __exception, __sent));
    }
    
public:
#endif

    ::Ice::AsyncResultPtr begin_readSensor(const ::std::string& name)
    {
        return begin_readSensor(name, 0, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_readSensor(const ::std::string& name, const ::Ice::Context& __ctx)
    {
        return begin_readSensor(name, &__ctx, ::IceInternal::__dummyCallback, 0);
    }

    ::Ice::AsyncResultPtr begin_readSensor(const ::std::string& name, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_readSensor(name, 0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_readSensor(const ::std::string& name, const ::Ice::Context& __ctx, const ::Ice::CallbackPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_readSensor(name, &__ctx, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_readSensor(const ::std::string& name, const ::RoboCompFootPreassureSensor::Callback_FootPreassureSensor_readSensorPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_readSensor(name, 0, __del, __cookie);
    }

    ::Ice::AsyncResultPtr begin_readSensor(const ::std::string& name, const ::Ice::Context& __ctx, const ::RoboCompFootPreassureSensor::Callback_FootPreassureSensor_readSensorPtr& __del, const ::Ice::LocalObjectPtr& __cookie = 0)
    {
        return begin_readSensor(name, &__ctx, __del, __cookie);
    }

    ::Ice::Int end_readSensor(const ::Ice::AsyncResultPtr&);
    
private:

    ::Ice::Int readSensor(const ::std::string&, const ::Ice::Context*);
    ::Ice::AsyncResultPtr begin_readSensor(const ::std::string&, const ::Ice::Context*, const ::IceInternal::CallbackBasePtr&, const ::Ice::LocalObjectPtr& __cookie = 0);
    
public:
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_context(const ::Ice::Context& __context) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_context(__context).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_adapterId(const ::std::string& __id) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_adapterId(__id).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_endpoints(const ::Ice::EndpointSeq& __endpoints) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_endpoints(__endpoints).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_locatorCacheTimeout(int __timeout) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_locatorCacheTimeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_connectionCached(bool __cached) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_connectionCached(__cached).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_endpointSelection(::Ice::EndpointSelectionType __est) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_endpointSelection(__est).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_secure(bool __secure) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_secure(__secure).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_preferSecure(bool __preferSecure) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_preferSecure(__preferSecure).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_router(const ::Ice::RouterPrx& __router) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_router(__router).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_locator(const ::Ice::LocatorPrx& __locator) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_locator(__locator).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_collocationOptimized(bool __co) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_collocationOptimized(__co).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_twoway() const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_twoway().get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_oneway() const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_oneway().get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_batchOneway() const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_batchOneway().get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_datagram() const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_datagram().get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_batchDatagram() const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_batchDatagram().get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_compress(bool __compress) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_compress(__compress).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_timeout(int __timeout) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_timeout(__timeout).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_connectionId(const ::std::string& __id) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_connectionId(__id).get());
    }
    
    ::IceInternal::ProxyHandle<FootPreassureSensor> ice_encodingVersion(const ::Ice::EncodingVersion& __v) const
    {
        return dynamic_cast<FootPreassureSensor*>(::IceProxy::Ice::Object::ice_encodingVersion(__v).get());
    }
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
    virtual ::IceProxy::Ice::Object* __newInstance() const;
};

}

}

namespace IceDelegate
{

namespace RoboCompFootPreassureSensor
{

class FootPreassureSensor : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::RoboCompFootPreassureSensor::Buffer readSensors(const ::Ice::Context*, ::IceInternal::InvocationObserver&) = 0;

    virtual ::Ice::Int readSensor(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&) = 0;
};

}

}

namespace IceDelegateM
{

namespace RoboCompFootPreassureSensor
{

class FootPreassureSensor : virtual public ::IceDelegate::RoboCompFootPreassureSensor::FootPreassureSensor,
                            virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::RoboCompFootPreassureSensor::Buffer readSensors(const ::Ice::Context*, ::IceInternal::InvocationObserver&);

    virtual ::Ice::Int readSensor(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&);
};

}

}

namespace IceDelegateD
{

namespace RoboCompFootPreassureSensor
{

class FootPreassureSensor : virtual public ::IceDelegate::RoboCompFootPreassureSensor::FootPreassureSensor,
                            virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::RoboCompFootPreassureSensor::Buffer readSensors(const ::Ice::Context*, ::IceInternal::InvocationObserver&);

    virtual ::Ice::Int readSensor(const ::std::string&, const ::Ice::Context*, ::IceInternal::InvocationObserver&);
};

}

}

namespace RoboCompFootPreassureSensor
{

class FootPreassureSensor : virtual public ::Ice::Object
{
public:

    typedef FootPreassureSensorPrx ProxyType;
    typedef FootPreassureSensorPtr PointerType;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::RoboCompFootPreassureSensor::Buffer readSensors(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___readSensors(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int readSensor(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::Ice::DispatchStatus ___readSensor(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

protected:
    virtual void __writeImpl(::IceInternal::BasicStream*) const;
    virtual void __readImpl(::IceInternal::BasicStream*);
    #ifdef __SUNPRO_CC
    using ::Ice::Object::__writeImpl;
    using ::Ice::Object::__readImpl;
    #endif
};

inline bool operator==(const FootPreassureSensor& l, const FootPreassureSensor& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

inline bool operator<(const FootPreassureSensor& l, const FootPreassureSensor& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

}

namespace RoboCompFootPreassureSensor
{

template<class T>
class CallbackNC_FootPreassureSensor_readSensors : public Callback_FootPreassureSensor_readSensors_Base, public ::IceInternal::TwowayCallbackNC<T>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception&);
    typedef void (T::*Sent)(bool);
    typedef void (T::*Response)(const ::RoboCompFootPreassureSensor::Buffer&);

    CallbackNC_FootPreassureSensor_readSensors(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallbackNC<T>(obj, cb != 0, excb, sentcb), response(cb)
    {
    }

    virtual void __completed(const ::Ice::AsyncResultPtr& __result) const
    {
        ::RoboCompFootPreassureSensor::FootPreassureSensorPrx __proxy = ::RoboCompFootPreassureSensor::FootPreassureSensorPrx::uncheckedCast(__result->getProxy());
        ::RoboCompFootPreassureSensor::Buffer __ret;
        try
        {
            __ret = __proxy->end_readSensors(__result);
        }
        catch(::Ice::Exception& ex)
        {
            ::IceInternal::CallbackNC<T>::__exception(__result, ex);
            return;
        }
        if(response)
        {
            (::IceInternal::CallbackNC<T>::callback.get()->*response)(__ret);
        }
    }

    Response response;
};

template<class T> Callback_FootPreassureSensor_readSensorsPtr
newCallback_FootPreassureSensor_readSensors(const IceUtil::Handle<T>& instance, void (T::*cb)(const ::RoboCompFootPreassureSensor::Buffer&), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_FootPreassureSensor_readSensors<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_FootPreassureSensor_readSensorsPtr
newCallback_FootPreassureSensor_readSensors(T* instance, void (T::*cb)(const ::RoboCompFootPreassureSensor::Buffer&), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_FootPreassureSensor_readSensors<T>(instance, cb, excb, sentcb);
}

template<class T, typename CT>
class Callback_FootPreassureSensor_readSensors : public Callback_FootPreassureSensor_readSensors_Base, public ::IceInternal::TwowayCallback<T, CT>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception& , const CT&);
    typedef void (T::*Sent)(bool , const CT&);
    typedef void (T::*Response)(const ::RoboCompFootPreassureSensor::Buffer&, const CT&);

    Callback_FootPreassureSensor_readSensors(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallback<T, CT>(obj, cb != 0, excb, sentcb), response(cb)
    {
    }

    virtual void __completed(const ::Ice::AsyncResultPtr& __result) const
    {
        ::RoboCompFootPreassureSensor::FootPreassureSensorPrx __proxy = ::RoboCompFootPreassureSensor::FootPreassureSensorPrx::uncheckedCast(__result->getProxy());
        ::RoboCompFootPreassureSensor::Buffer __ret;
        try
        {
            __ret = __proxy->end_readSensors(__result);
        }
        catch(::Ice::Exception& ex)
        {
            ::IceInternal::Callback<T, CT>::__exception(__result, ex);
            return;
        }
        if(response)
        {
            (::IceInternal::Callback<T, CT>::callback.get()->*response)(__ret, CT::dynamicCast(__result->getCookie()));
        }
    }

    Response response;
};

template<class T, typename CT> Callback_FootPreassureSensor_readSensorsPtr
newCallback_FootPreassureSensor_readSensors(const IceUtil::Handle<T>& instance, void (T::*cb)(const ::RoboCompFootPreassureSensor::Buffer&, const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_FootPreassureSensor_readSensors<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_FootPreassureSensor_readSensorsPtr
newCallback_FootPreassureSensor_readSensors(T* instance, void (T::*cb)(const ::RoboCompFootPreassureSensor::Buffer&, const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_FootPreassureSensor_readSensors<T, CT>(instance, cb, excb, sentcb);
}

template<class T>
class CallbackNC_FootPreassureSensor_readSensor : public Callback_FootPreassureSensor_readSensor_Base, public ::IceInternal::TwowayCallbackNC<T>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception&);
    typedef void (T::*Sent)(bool);
    typedef void (T::*Response)(::Ice::Int);

    CallbackNC_FootPreassureSensor_readSensor(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallbackNC<T>(obj, cb != 0, excb, sentcb), response(cb)
    {
    }

    virtual void __completed(const ::Ice::AsyncResultPtr& __result) const
    {
        ::RoboCompFootPreassureSensor::FootPreassureSensorPrx __proxy = ::RoboCompFootPreassureSensor::FootPreassureSensorPrx::uncheckedCast(__result->getProxy());
        ::Ice::Int __ret;
        try
        {
            __ret = __proxy->end_readSensor(__result);
        }
        catch(::Ice::Exception& ex)
        {
            ::IceInternal::CallbackNC<T>::__exception(__result, ex);
            return;
        }
        if(response)
        {
            (::IceInternal::CallbackNC<T>::callback.get()->*response)(__ret);
        }
    }

    Response response;
};

template<class T> Callback_FootPreassureSensor_readSensorPtr
newCallback_FootPreassureSensor_readSensor(const IceUtil::Handle<T>& instance, void (T::*cb)(::Ice::Int), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_FootPreassureSensor_readSensor<T>(instance, cb, excb, sentcb);
}

template<class T> Callback_FootPreassureSensor_readSensorPtr
newCallback_FootPreassureSensor_readSensor(T* instance, void (T::*cb)(::Ice::Int), void (T::*excb)(const ::Ice::Exception&), void (T::*sentcb)(bool) = 0)
{
    return new CallbackNC_FootPreassureSensor_readSensor<T>(instance, cb, excb, sentcb);
}

template<class T, typename CT>
class Callback_FootPreassureSensor_readSensor : public Callback_FootPreassureSensor_readSensor_Base, public ::IceInternal::TwowayCallback<T, CT>
{
public:

    typedef IceUtil::Handle<T> TPtr;

    typedef void (T::*Exception)(const ::Ice::Exception& , const CT&);
    typedef void (T::*Sent)(bool , const CT&);
    typedef void (T::*Response)(::Ice::Int, const CT&);

    Callback_FootPreassureSensor_readSensor(const TPtr& obj, Response cb, Exception excb, Sent sentcb)
        : ::IceInternal::TwowayCallback<T, CT>(obj, cb != 0, excb, sentcb), response(cb)
    {
    }

    virtual void __completed(const ::Ice::AsyncResultPtr& __result) const
    {
        ::RoboCompFootPreassureSensor::FootPreassureSensorPrx __proxy = ::RoboCompFootPreassureSensor::FootPreassureSensorPrx::uncheckedCast(__result->getProxy());
        ::Ice::Int __ret;
        try
        {
            __ret = __proxy->end_readSensor(__result);
        }
        catch(::Ice::Exception& ex)
        {
            ::IceInternal::Callback<T, CT>::__exception(__result, ex);
            return;
        }
        if(response)
        {
            (::IceInternal::Callback<T, CT>::callback.get()->*response)(__ret, CT::dynamicCast(__result->getCookie()));
        }
    }

    Response response;
};

template<class T, typename CT> Callback_FootPreassureSensor_readSensorPtr
newCallback_FootPreassureSensor_readSensor(const IceUtil::Handle<T>& instance, void (T::*cb)(::Ice::Int, const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_FootPreassureSensor_readSensor<T, CT>(instance, cb, excb, sentcb);
}

template<class T, typename CT> Callback_FootPreassureSensor_readSensorPtr
newCallback_FootPreassureSensor_readSensor(T* instance, void (T::*cb)(::Ice::Int, const CT&), void (T::*excb)(const ::Ice::Exception&, const CT&), void (T::*sentcb)(bool, const CT&) = 0)
{
    return new Callback_FootPreassureSensor_readSensor<T, CT>(instance, cb, excb, sentcb);
}

}

#endif