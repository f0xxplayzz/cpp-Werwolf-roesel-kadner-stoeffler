#include "./../Networking/Client.cpp"
#include "./../Networking/Definitions.hpp"

class ClientInterface
{
    /*
        Not used yet, because no GUI implemented yet

        -> not finished, just a prototype
    */
    Client* source;
    void bindToClient(Client* client)
    {
        source=client;
    }
    std::string getPhase()
    {
        char charPhase = source->phase;
        switch(charPhase)
        {
            case WEREWOLVEVOTING:
            return "The Werewolves are awake";
            break;
            case SEER:
            return "The Seeer is awake";
            break;
            case WEREWOLVEKILL:
            return "A person died";
            break;
            case VOTING:
            return "The Village has a trial";
            break;
            case EXECUTION:
            return "The trial has ended";
            break;
            case GAMEOVER:
            return "Game Over";
        }
        return "ERROR"
    }
    std::string getRole()
    {
        //not implemented yet
    }
    std::string getLastDead()
    {
        //not implemented yet
    }
    bool isAwake()
    {
        //not implemented yet
    }
    bool isAlive()
    {
        //not implemented yet
    }
};