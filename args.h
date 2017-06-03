#ifndef __ARGS_H__
#define __ARGS_H__
#include <string>
#include <map>
#include <stdint.h>
namespace args
{
enum ArgType
{
    ARG_TYPE_UINT32,
    ARG_TYPE_INT32,
    ARG_TYPE_UINT64,
    ARG_TYPE_INT64,
    ARG_TYPE_DOUBLE,
    ARG_TYPE_STRING
};

struct ArgValue
{
    std::string arg_name;
    std::string desc;
    void* value_ptr;
    ArgType type;
};

class GlobalRegistry
{
public:
    static GlobalRegistry* Instance()
    {
        if (s_instance_ == NULL)
        {
            s_instance_ = new GlobalRegistry;
        }
        return s_instance_;
    }
    void Regist(std::string name, ArgValue value);
    ArgValue* GetArgValue(std::string name);

    void HelpInfo();
private:
    typedef std::map<std::string, ArgValue> RegistryMap;
    typedef RegistryMap::iterator RegistryMapItor;
    RegistryMap registry_map_;
    static GlobalRegistry* s_instance_;
};

class ArgRegister
{
public:
    template <typename T>
    ArgRegister(T* variable, const char* name, ArgType arg_type, 
                const char* desc);
};

template <typename T>
ArgRegister::ArgRegister(T* variable, const char* name, ArgType arg_type, 
                         const char* desc)
{
    ArgValue arg_value;
    arg_value.desc = desc;
    arg_value.type = arg_type;
    arg_value.value_ptr = variable;
    arg_value.arg_name = std::string(name);
    GlobalRegistry::Instance()->Regist(name, arg_value);
}

void ParseCommandLine(int argc, char* argv[]);
const std::string& ProgName();
}

#define DEFINE_VARIABLE(name, type, arg_type, default_value, desc) \
    namespace ARGNS##name \
    { \
        static type ARG_##name = default_value; \
        static args::ArgRegister arg_register(&ARG_##name, #name, \
                                              arg_type, desc); \
    } \
    using ARGNS##name::ARG_##name

#define DEFINE_string(name, default_value, desc) \
    DEFINE_VARIABLE(name, std::string, ARG_TYPE_STRING, default_value, desc)

#define DEFINE_uint32(name, default_value, desc) \
    DEFINE_VARIABLE(name, uint32_t, ARG_TYPE_UINT32, default_value, desc)

#define DEFINE_int32(name, default_value, desc) \
    DEFINE_VARIABLE(name, int32_t, ARG_TYPE_INT32, default_value, desc)

#define DEFINE_double(name, default_value, desc) \
    DEFINE_VARIABLE(name, double, ARG_TYPE_DOUBLE, default_value, desc)

#define DEFINE_uint64(name, default_value, desc) \
    DEFINE_VARIABLE(name, uint64_t, ARG_TYPE_UINT64, default_value, desc)

#define DEFINE_int64(name, default_value, desc) \
    DEFINE_VARIABLE(name, int64_t, ARG_TYPE_INT64, default_value, desc)

#endif

