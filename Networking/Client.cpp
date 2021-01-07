#include "Networking.hpp"
#include "Client.hpp"
#include "Networking.hpp"

using boost::asio::ip::tcp;

/*
void werewolveClient::Client::setSocket(){
    socket = tcp::socket(my_service);
}
*/

void werewolveClient::Client::join(){
    boost::asio::async_write(socket,boost::asio::buffer(createJoinMessage(),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"Joined Server"<<std::endl;
        }
    });
    auto receiver = new char[200];
    socket->receive(receiver);
    clientData=Network::processPlayerInfo(receiver);
}

void werewolveClient::Client::action(char id, char ca){
    boost::asio::async_write(socket,boost::asio::buffer(createActionMessage(id,ca),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"Sended action informations"<<std::endl;
        }
    });
    auto receiver = new char[200];
    socket->receive(receiver);
    clientData=Network::processPlayerInfo(receiver);
}

void werewolveClient::Client::chatMessage(std::string msg){
    boost::asio::async_write(socket,boost::asio::buffer(createChatMessage(msg),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"Sended Chat Msg"<<std::endl;
        }
    });
    auto receiver = boost::asio::buffer("",200);
    socket->receive(receiver);
    //receiver -> Chat
}

void werewolveClient::Client::voting(char id){
    boost::asio::async_write(socket,boost::asio::buffer(createVotingMessage(id),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"Voted"<<std::endl;
        }
    });
    auto receiver = new char[200];
    socket->receive(receiver);
    clientData=Network::processPlayerInfo(receiver);
}

void werewolveClient::Client::werewolveVoting(char id){
    boost::asio::async_write(socket,boost::asio::buffer(createWerewolveVotingMessage(id),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"voted"<<std::endl;
        }
    });
    auto receiver = new char[200];
    socket->receive(receiver);
    clientData=Network::processPlayerInfo(receiver);
}

void werewolveClient::Client::requestData(){
    boost::asio::async_write(socket,boost::asio::buffer(createDataRequest(),200),err,[](boost::system::error_code err){
        if(!err){
            std::cout<<"requested Data"<<std::endl;
        }
    });
    auto receiver = new char[1];
    socket->receive(receiver);
    phase = receiver[0];
    //remove later
    std::cout<< (int)phase << std::endl;
}

void werewolveClient::Client::openConnection(){
    socket->connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"),8999));
}

void werewolveClient::Client::closeConnection(){
    socket->close();
}

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
/*void werewolveClient::Client::getPlayerData(){
    for(Player p : clientData.alivePlayers){
        if(p.id == id)
            player = &p;
    }
}

Game werewolveClient::Client::processPlayerInfoOnJoin(std::string input){
    const char* data = input.c_str();
    char* alivePlayers;
    char* villagers;
    char* werewolves;
    Game newData;
    newData.werewolveCount=data[1];
    newData.gameOver = data[2];
    int iterator = 3;
    char comp = 'a';
    int k=0;
    while(comp != '#'){
        alivePlayers[k++]=data[iterator++];
        comp=data[iterator];
    }
    comp='a';
    iterator++;
    k=0;
    while(comp != '#'){
        villagers[k++]=data[iterator++];
        comp=data[iterator];
    }
    comp='a';
    iterator++;
    k=0;
    while(comp != '#'){
    	werewolves[k++]=data[iterator++];
        comp=data[iterator];
    }
    newData.alivePlayers= Network::processAlivePlayers(alivePlayers);
    newData.villagers=Network::processVillagers(villagers);
    newData.werewolves=Network::processWerewolves(werewolves);
    id=data[iterator];
    return newData;
}
*/
