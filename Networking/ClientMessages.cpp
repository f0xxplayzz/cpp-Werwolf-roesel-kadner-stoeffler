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
result += id;
result += getRole();
result += getName();
return result;
}

std::string werewolveClient::Client::createActionMessage(char id){
std::string result ="";
/*
    Value(Index = 0) = 2
    index = 1 -> Role 
    index = 2 -> Case
    index = 3 -> ID of Target
*/
result += (char)2;
result += getRole();
//result +=case;
result += id;
return result;
}

std::string werewolveClient::Client::createChatMessage(std::string msg){
std::string result ="";
/*
    Value(Index = 0) = 3
    Index = 1 -> alive/dead
    Index = 2-... -> Message 
*/
result += (char)3;
result += getLivingStatus();
result += msg;
return result;
}

std::string werewolveClient::Client::createVotingMessage(char id){
std::string result ="";
/*
    Value(Index=0) = 4
    Index = 1 -> ID of target
*/
result += (char)4;
result += id;
return result;
}

std::string werewolveClient::Client::createWerewolveVotingMessage(char id){
std::string result ="";
/*
    Value(Index =0) = 5
    Index=1 -> target ID
*/
result += (char)5;
result += id;
return result;
}