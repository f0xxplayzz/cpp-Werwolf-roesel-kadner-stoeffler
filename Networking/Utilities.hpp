#include <string>
#include <string.h>

char* to_cString(std::string str)
{
    char* res = new char[str.length()+1];
    strcpy(res,str.c_str());
    return res;
}