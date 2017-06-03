#include "args.h"
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
namespace args
{
GlobalRegistry* GlobalRegistry::s_instance_ = NULL;
void GlobalRegistry::Regist(std::string name, ArgValue value)
{
    RegistryMapItor itor = registry_map_.find(name);
    if (itor != registry_map_.end())
    {
        return;
    }
    registry_map_.insert(std::make_pair(name, value));
}

ArgValue* GlobalRegistry::GetArgValue(std::string name)
{
    RegistryMapItor itor = registry_map_.find(name);
    if (itor == registry_map_.end())
    {
        return NULL;
    }
    return (&(itor->second));    
}

void GlobalRegistry::HelpInfo()
{
    std::cout<<"help:"<<std::endl;
    RegistryMapItor itor = registry_map_.begin();
    while (itor != registry_map_.end())
    {
        std::cout<<"      --"<<itor->second.arg_name<<"=xx    "<<itor->second.desc<<std::endl;
        ++itor;
    }
}

static std::string g_progname;
const std::string& ProgName()
{
    return g_progname;
}

class ArgParser
{
public:
    ArgParser(char* input_arg);
    void Parse(const std::string& key, const char* value);
    void SplitArgument(std::string& key, char** value_buf);
private:
    char* input_arg_;
};

ArgParser::ArgParser(char* input_arg)
                : input_arg_(input_arg)
{}

void ArgParser::SplitArgument(std::string& key, char** value)
{
    char* v = strchr(input_arg_, '=');
    if (v == NULL)
    {
        key.assign(input_arg_);
        *value = NULL;
    }
    else
    {
        key.assign(input_arg_, v - input_arg_);
        *value = ++v;
    }
}

template <typename T>  
T StringToNum(std::string& str)  
{  
    std::istringstream iss(str);  
    T num;  
    iss >> num;  
    return num;  
}

void ArgParser::Parse(const std::string& key, const char* value)
{
    ArgValue* arg_value = GlobalRegistry::Instance()->GetArgValue(key);
    if (!value || !arg_value)
    {
        return;
    }
    switch (arg_value->type)
    {
    case ARG_TYPE_STRING:
    {
        *reinterpret_cast<std::string*>(arg_value->value_ptr) = std::string(value);
        break;
    }
    case ARG_TYPE_UINT32:
    {
        std::string strvalue = std::string(value);
        uint32_t v = StringToNum<uint32_t>(strvalue);
        *reinterpret_cast<uint32_t*>(arg_value->value_ptr) = v;
        break;
    }
    case ARG_TYPE_INT32:
    {
        std::string strvalue = std::string(value);
        int32_t v = StringToNum<int32_t>(strvalue);
        *reinterpret_cast<int32_t*>(arg_value->value_ptr) = v;
        break;
    }
    case ARG_TYPE_UINT64:
    {
        std::string strvalue = std::string(value);
        uint64_t v = StringToNum<uint64_t>(strvalue);
        *reinterpret_cast<uint64_t*>(arg_value->value_ptr) = v;
        break;
    }
    case ARG_TYPE_INT64:
    {
        std::string strvalue = std::string(value);
        int64_t v = StringToNum<int64_t>(strvalue);
        *reinterpret_cast<int64_t*>(arg_value->value_ptr) = v;
        break;
    }
    case ARG_TYPE_DOUBLE:
    {
        std::string strvalue = std::string(value);
        double v = StringToNum<double>(strvalue);
        *reinterpret_cast<double*>(arg_value->value_ptr) = v;
        break;
    }
    default:
        break;
    }
}

void ParseCommandLine(int argc, char* argv[])
{
    if (argv[0] != NULL)
    {
        g_progname = std::string(argv[0]);
    }
    for (int i = 1; i < argc; ++i)
    {
        char* arg = argv[i];
        // need start with -- or -
        if (arg[0] != '-' || (arg[0] == '-' && arg[1] == '\0'))
        {
            continue;
        }
        if (arg[0] == '-')
        {
            ++arg;
        }
        if (arg[0] == '-')
        {
            ++arg;
        }
        ArgParser parser(arg);
        std::string key;
        char* value;
        parser.SplitArgument(key, &value);
        if (key == "help")
        {
            GlobalRegistry::Instance()->HelpInfo();
            exit(0);
        }
        parser.Parse(key, value);
    }
}
}

