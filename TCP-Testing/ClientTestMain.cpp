#include "./../Networking/Networking.hpp"

int main(){
    werewolveClient::Client* cl = new werewolveClient::Client();
    cl->openConnection();
    cl->requestData();
    cl->closeConnection(); 
}