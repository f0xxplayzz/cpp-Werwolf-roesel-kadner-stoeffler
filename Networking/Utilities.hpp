#pragma once

#include <string>
#include <string.h>
#include "Definitions.hpp"

char* to_cString(std::string str)
{
    char* res = new char[str.length()+1];
    strcpy(res,str.c_str());
    return res;
}
std::string roleAsString(char c)
{
    /*
        converts the role-char to a std::string
        need for output
    */
    std::string role_str = "";
    switch(c)
    {
        case VILLAGER_ROLE:
        role_str="Villager";
        break;
        case WEREWOLVE_ROLE:
        role_str="Werewolve";
        break;
        case SEER_ROLE:
        role_str="Seer";
        break;
        case WITCH_ROLE:
        role_str="Witch";
        break;
    }
    return role_str;
}