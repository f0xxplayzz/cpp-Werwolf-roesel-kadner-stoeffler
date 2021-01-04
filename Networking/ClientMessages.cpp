#pragma once
#include "Client.hpp"
#include <string>

std::string werewolveClient::Client::createJoinMessage(){
std::string result ="";
/*
    Value(Index = 0) = 1
    index = 1 -> ID
    index = 2 -> Role
    index = 3-12 -> Name 
*/
result += (char)1;
//result += id;
result += getRole();
result += getName();
return result;
}

std::string werewolveClient::Client::createActionMessage(char idp, char ca){
std::string result ="";
/*
    Value(Index = 0) = 2
    index = 1 -> Role 
    index = 2 -> Case
    index = 3 -> ID of Target
*/
result += (char)2;
result += id;
result += idp;
result += getRole();
result += ca; //important for witch
return result;
}

std::string werewolveClient::Client::createChatMessage(std::string msg){
std::string result ="";
/*
    Value(Index = 0) = 4
    Index = 1 -> alive/dead
    Index = 2-... -> Message 
*/
result += (char)4;
result += id;
result += getLivingStatus();
result += msg;
return result;
}

std::string werewolveClient::Client::createVotingMessage(char idp){
std::string result ="";
/*
    Value(Index=0) = 5
    Index = 1 -> ID of target
*/
result += (char)5;
result += id;
result += idp;
return result;
}

std::string werewolveClient::Client::createWerewolveVotingMessage(char idp){
std::string result ="";
/*
    Value(Index =0) = 6
    Index=1 -> target ID
*/
result += (char)6;
result += id;
result += idp;
return result;
}

std::string werewolveClient::Client::createDataRequest(){
    std::string result = "";
    result += (char)3;
    return result;
}