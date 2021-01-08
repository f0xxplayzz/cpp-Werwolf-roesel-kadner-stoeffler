#include <string>

char* to_cString(std::string str)
{
    char* res = new char[str.length()+1];
    std::strcpy(res,str.c_str());
    return res;
}