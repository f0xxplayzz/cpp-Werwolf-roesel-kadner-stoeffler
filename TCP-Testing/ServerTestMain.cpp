#include "./../Networking/Networking.hpp"

int main(){
    WerewolveServer::Server* s = new WerewolveServer::Server();
    s->host(s->hostData);
}