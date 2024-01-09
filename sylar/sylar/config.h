#ifndef _SYLAR_CONFIG_H_
#define _SYLAR_CONFIG_H_

#include<memory>
#include<sstream>
#include<string>
#include<boost/lexical_cast.hpp>
#include<iostream>
#include<map>
#include<list>
#include<set>
#include<unordered_map>
#include<unordered_set>
#include"util.h"
#include"log.h"
#include<yaml-cpp/yaml.h>
#include"thread.h"

namespace sylar{

class ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name, const std::string& description = "")
        :m_name(name),
        m_description(description){
            std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::towlower);
    }

    virtual ~ConfigVarBase() {}
    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }

    virtual std::string ToString() = 0;
    virtual bool FromString(const std::string& val) = 0;

    virtual std::string getTypeName() const = 0;
protected:
    std::string m_name;
    std::string m_description;
};

//把F类型转化为T类型，这是基础类型的实现
template<class F, class T>
class lexicalcast {
public:
    T operator()(const F& v){
        return boost::lexical_cast<T>(v);
    }
};

//string--->vecotr
template<class T>
class lexicalcast<std::string, std::vector<T> > {
public:
    std::vector<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream ss;
        for(auto i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.push_back(lexicalcast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

//vector--->string
template<class T>
class lexicalcast<std::vector<T>, std::string> {
public:
    std::string operator()(const std::vector<T>& v){
        YAML::Node node;
        for(auto& i : v){
            node.push_back(YAML::Load(lexicalcast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//string--->list
template<class T>
class lexicalcast<std::string, std::list<T> > {
public:
    std::list<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::list<T> vec;
        std::stringstream ss;
        for(auto i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.push_back(lexicalcast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

//list--->string
template<class T>
class lexicalcast<std::list<T>, std::string> {
public:
    std::string operator()(const std::list<T>& v){
        YAML::Node node;
        for(auto& i : v){
            node.push_back(YAML::Load(lexicalcast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//string--->set
template<class T>
class lexicalcast<std::string, std::set<T> > {
public:
    std::set<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::set<T> vec;
        std::stringstream ss;
        for(auto i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.insert(lexicalcast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

//set--->string
template<class T>
class lexicalcast<std::set<T>, std::string> {
public:
    std::string operator()(const std::set<T>& v){
        YAML::Node node;
        for(auto& i : v){
            node.push_back(YAML::Load(lexicalcast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//string--->unordered_set
template<class T>
class lexicalcast<std::string, std::unordered_set<T> > {
public:
    std::unordered_set<T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T> vec;
        std::stringstream ss;
        for(auto i = 0; i < node.size(); ++i){
            ss.str("");
            ss << node[i];
            vec.insert(lexicalcast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

//unordered_set--->string
template<class T>
class lexicalcast<std::unordered_set<T>, std::string> {
public:
    std::string operator()(const std::unordered_set<T>& v){
        YAML::Node node;
        for(auto& i : v){
            node.push_back(YAML::Load(lexicalcast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//string--->map
template<class T>
class lexicalcast<std::string, std::map<std::string, T> > {
public:
    std::map<std::string, T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T> vec;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); ++it){
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(), lexicalcast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

//map--->string
template<class T>
class lexicalcast<std::map<std::string, T>, std::string> {
public:
    std::string operator()(const std::map<std::string, T>& v){
        YAML::Node node;
        for(auto& i : v){
            node[i.first] = YAML::Load(lexicalcast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

//string--->unordered_map
template<class T>
class lexicalcast<std::string, std::unordered_map<std::string, T> > {
public:
    std::unordered_map<std::string, T> operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T> vec;
        std::stringstream ss;
        for(auto it = node.begin(); it != node.end(); ++it){
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(), lexicalcast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

//unordered_map--->string
template<class T>
class lexicalcast<std::unordered_map<std::string, T>, std::string> {
public:
    std::string operator()(const std::unordered_map<std::string, T>& v){
        YAML::Node node;
        for(auto& i : v){
            node[i.first] = YAML::Load(lexicalcast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T, class FromStr = lexicalcast<std::string, T>,
            class ToStr = lexicalcast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
    typedef RMMutex RWMutexType;
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void (const T &old_value, const T &new_value)> on_change_cb;
    ConfigVar(const std::string& name, const T& default_value, const std::string& description = "")
        :ConfigVarBase(name, description),
        m_val(default_value){

    }

    std::string ToString() override{
        try{
            //return boost::lexical_cast<std::string>(m_val);
            RWMutexType::ReadLock lock(m_mutex);
            return ToStr()(m_val);
        }catch(std::exception& e){
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::ToString exception"
                << e.what() << " convert: " << typeid(m_val).name() << " to string";
        }

        return "";
    }

    bool FromString(const std::string& val) override{
        try{
            //setValue(boost::lexical_cast<T>(val));
            setValue(FromStr()(val));
        }catch(std::exception& e){
             SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::ToString exception"
                << e.what() << " convert: string to " << typeid(m_val).name()
                << " - " << val;
        }

        return false;
    }

    const T getValue() { 
        RWMutexType::ReadLock lock(m_mutex);
        return m_val; 
    }
    void setValue(const T& v) {
        {
            RWMutexType::ReadLock lock(m_mutex);
            if(v == m_val){
                return;
            }
            for(auto& i : m_cbs){
                i.second(m_val, v);
            }
        }
        RWMutexType::WriteLock lock(m_mutex);
        m_val = v;
    }
    std::string getTypeName() const override { 
        return typeid(T).name(); 
    }

    uint64_t addListenner(on_change_cb cb){
        static uint64_t s_fun_id = 0;
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs[++s_fun_id] = cb;
        return s_fun_id;
    }

    void delListenner(uint64_t key){
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs.erase(key);
    }

    on_change_cb getListenner(uint64_t key){
        RWMutexType::ReadLock lock(m_mutex);
        auto it = m_cbs.find(key);
        return it == m_cbs.end() ? nullptr : it->second;
    }

    void clearListenner() {
        RWMutexType::WriteLock lock(m_mutex);
        m_cbs.clear();
    }
private:
    RWMutexType m_mutex;
    T m_val;
    //变更回调函数组，uint64_t为key，要求唯一，一般可以用hash
    std::map<uint64_t, on_change_cb> m_cbs;
};


class Config{
public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;
    typedef RMMutex RWMutexType;
    template<class T>
    static typename ConfigVar<T>::ptr LookUp(const std::string& name,//创建
        const T& default_value, const std::string& description = ""){
            RWMutexType::WriteLock lock(getMutex());
            auto it = getDatas().find(name);
            if(it != getDatas().end()){
                auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
                if(tmp){
                    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "LookUp name=" << name << "exists";
                    return tmp;
                }else{
                    SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name=" << name << "exists but type not "
                        << typeid(T).name() << " real type=" <<it->second->getTypeName() << " " << it->second->ToString();
                    return nullptr;
                }
            }

            if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678")
                != std::string::npos){
                    SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "LookUp name invalid" << name;
                    throw std::invalid_argument(name);
            }

            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
            getDatas()[name] = v;
            return v;
    }

    template<class T>
    static typename ConfigVar<T>::ptr LookUp(const std::string& name){//查找
        RWMutexType::ReadLock lock(getMutex());
        auto it = getDatas().find(name);
        if(it == getDatas().end()){
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
    }

    static void LoadFromYaml(const YAML::Node& root);
    static ConfigVarBase::ptr LookupBase(const std::string& name);

    static void Visit(std::function<void(ConfigVarBase::ptr)> cb);
private:
    static ConfigVarMap& getDatas(){
        static ConfigVarMap s_datas;
        return s_datas;
    }

    static RWMutexType& getMutex(){
        static RWMutexType s_mutex;
        return s_mutex;
    }
};


}



#endif