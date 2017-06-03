#include "args.h"
#include <iostream>
#include <string>
using namespace args;
DEFINE_string(mystr, "name", "this is name");
DEFINE_uint32(myint, 3, "this is count");
DEFINE_int32(myuint, -3, "this is count");
DEFINE_double(mydouble, 3.45, "this is count");
DEFINE_uint64(myull, 47392, "this is count");
DEFINE_int64(myll, 4643, "this is count");

int main(int argc, char* argv[])
{
    args::ParseCommandLine(argc, argv);
    std::cout<<ARG_mystr<<std::endl;
    std::cout<<ARG_myint<<std::endl;
    std::cout<<ARG_myuint<<std::endl;
    std::cout<<ARG_mydouble<<std::endl;
    std::cout<<ARG_myull<<std::endl;
    std::cout<<ARG_myll<<std::endl;
    return 0;
}

