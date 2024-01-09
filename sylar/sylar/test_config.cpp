#include"config.h"
#include<yaml-cpp/yaml.h>
#include<iostream>

sylar::ConfigVar<int>::ptr g_int_value_config = sylar::Config::LookUp("system.port", (int)8080, "system port");
sylar::ConfigVar<float>::ptr g_int_xvalue_config = sylar::Config::LookUp("system.port", (float)8080, "system port");
sylar::ConfigVar<float>::ptr g_float_value_config = sylar::Config::LookUp("system.value", (float)10.2f, "system value");
sylar::ConfigVar<std::vector<int> >::ptr g_int_vec_value_config
    = sylar::Config::LookUp("system.int_vec", std::vector<int>{1,2}, "system int vec");
sylar::ConfigVar<std::list<int> >::ptr g_int_list_value_config
    = sylar::Config::LookUp("system.int_list", std::list<int>{1,2}, "system int list");
sylar::ConfigVar<std::set<int> >::ptr g_int_set_value_config
    = sylar::Config::LookUp("system.int_set", std::set<int>{1,2}, "system int set");
sylar::ConfigVar<std::unordered_set<int> >::ptr g_int_unordered_set_value_config
    = sylar::Config::LookUp("system.int_unordered_set", std::unordered_set<int>{1,2}, "system int unordered set");
sylar::ConfigVar<std::map<std::string, int> >::ptr g_str_int_map_set_value_config
    = sylar::Config::LookUp("system.str_int_map", std::map<std::string, int>{{"key",1}}, "system str int map");
sylar::ConfigVar<std::unordered_map<std::string, int> >::ptr g_str_int_umap_set_value_config
    = sylar::Config::LookUp("system.str_int_umap", std::unordered_map<std::string, int>{{"key",1}}, "system str int umap");



void print_yaml(const YAML::Node& node, int level){
    if(node.IsScalar()){
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4, ' ')
        << node.Scalar() << " - " << node.Type() << " - " << level;
    }else if(node.IsNull()){
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4, ' ')
        << "NULL - " << node.Type() << " - " << level;
    }else if(node.IsMap()){
        for(auto it = node.begin();
            it != node.end(); ++it){
               SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4, ' ')
               << it->first << " - " << it->second.Type() << " - " << level;
               print_yaml(it->second, level + 1); 
        }
    }else if(node.IsSequence()){
        for(size_t i = 0; i < node.size(); ++i){
            SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << std::string(level * 4, ' ')
            << i << " - " << node[i].Type() << " - " << level;
            print_yaml(node[i], level + 1); 
        }
    }
}

void test_yaml(){
    YAML::Node root = YAML::LoadFile("/home/wangxian/program/workspace/sylar/bin/config/test.yml");

    print_yaml(root, 0);

    //SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << root;
}

void test_config(){
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "before: " << g_float_value_config->ToString();

#define XX(g_var, name, prefix)\
{\
    auto& v = g_var->getValue();\
    for(auto& i : v){\
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << #prefix " " #name ": " << i;\
    }\
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->ToString();\
}

#define XXM(g_var, name, prefix)\
{\
    auto& v = g_var->getValue();\
    for(auto& i : v){\
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << #prefix " " #name ": {"\
            << i.first << " - " << i.second << "}";\
    }\
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->ToString();\
}

    //XX(g_int_vec_value_config, int_vec, before);
    //XX(g_int_list_value_config, int_list, before);
    //XX(g_int_set_value_config, int_set, before);
    //XX(g_int_unordered_set_value_config, int_unordered_set, before);
    //XXM(g_str_int_map_set_value_config, str_int_map, before);
    XXM(g_str_int_umap_set_value_config, str_int_umap, before);

    YAML::Node root = YAML::LoadFile("/home/wangxian/program/workspace/sylar/bin/config/test.yml");
    sylar::Config::LoadFromYaml(root);

    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "after: " << g_float_value_config->ToString();

    //XX(g_int_vec_value_config, int_vec, after);
    //XX(g_int_list_value_config, int_list, after); 
    //XX(g_int_set_value_config, int_set, after);   
    //XX(g_int_unordered_set_value_config, int_unordered_set, after);
    //XXM(g_str_int_map_set_value_config, str_int_map, after);
    XXM(g_str_int_umap_set_value_config, str_int_umap, after);

}

class Person {
public:
    std::string m_name;
    int m_age = 0;
    bool m_sex = 0;

    std::string toString() const {
        std::stringstream ss;
        ss << "{Person name=" << m_name
           << " age=" << m_age
           << " sex=" << m_sex << "}";
        return ss.str();
    }

    bool operator==(const Person& h) const {
        return h.m_age == m_age
                && h.m_name == m_name 
                && h.m_sex == m_sex;
    }
};

namespace sylar{

template<>
class lexicalcast<std::string, Person> {
public:
    Person operator()(const std::string& v){
        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_age = node["age"].as<int>();
        p.m_name = node["name"].as<std::string>();
        p.m_sex = node["sex"].as<bool>();
        return p;
    }
};

template<>
class lexicalcast<Person, std::string> {
public:
    std::string operator()(const Person& p){
        YAML::Node node;
        node["name"] = p.m_name;
        node["age"] = p.m_age;
        node["sex"] = p.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

}

sylar::ConfigVar<Person>::ptr g_person = sylar::Config::LookUp("class.person", Person(), "class person");
sylar::ConfigVar<std::map<std::string,Person> >::ptr g_person_map = sylar::Config::LookUp("class.map", std::map<std::string, Person>(), "class person");
sylar::ConfigVar<std::map<std::string,std::vector<Person> > >::ptr g_person_vec_map = sylar::Config::LookUp("class.vec_map",
    std::map<std::string, std::vector<Person> >(), "class person");

void test_class(){
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "before: " << g_person->getValue().toString() << " - " << g_person->ToString();

#define XX_PM(g_var, prefix)\
{    auto m = g_var->getValue();\
    for(auto& it : m){\
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << prefix << ":" << it.first << " - " <<it.second.toString();\
    }\
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << prefix << ": size=" << m.size();\
}

    g_person->addListenner([](const Person& old_value, const Person& new_value){
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "old_value=" << old_value.toString()
                    << " new_value=" << new_value.toString();
    });

XX_PM(g_person_map, "class map before");
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "before: " << g_person_vec_map->ToString();

    YAML::Node root = YAML::LoadFile("/home/wangxian/program/workspace/sylar/bin/config/test.yml");
    sylar::Config::LoadFromYaml(root);

    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "after: " << g_person->getValue().toString() << " - " << g_person->ToString();
XX_PM(g_person_map, "class map after");
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "after: " << g_person_vec_map->ToString();
}

void test_log(){
    sylar::Logger::ptr system_log = SYLAR_LOG_NAME("system");
    SYLAR_LOG_INFO(system_log) << "hello " <<std::endl;
    std::cout << sylar::LoggerMgr::getInstance()->toYamlString() << std::endl;

    YAML::Node root = YAML::LoadFile("/home/wangxian/program/workspace/sylar/bin/config/log.yml");
    sylar::Config::LoadFromYaml(root);

    std::cout << "==========================" << std::endl;
    std::cout << sylar::LoggerMgr::getInstance()->toYamlString() << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << root << std::endl;

    SYLAR_LOG_INFO(system_log) << "hello " <<std::endl;

    system_log->setFormatter("%d - %m%n");
    SYLAR_LOG_INFO(system_log) << "hello " <<std::endl;
}

int main(int argc, char** argv){


    //test_class();
    //test_yaml();
    //test_config();
    test_log();
    
    sylar::Config::Visit([](sylar::ConfigVarBase::ptr var){
        SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "name=" << var->getName()
                << " description=" << var->getDescription()
                << " typename=" << var->getTypeName()
                << " value=" << var->ToString();
    });
    return 0;
}