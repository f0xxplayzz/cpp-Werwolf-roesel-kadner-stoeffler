#include "./../Definitions.hpp"
#include "./../Makros.hpp"

/*
    Structure of a Client Message:
    0:id
    1:role
    2:phase
    3:DONE/NOTDONE/SKIPPED
    4:Case
    5:ID of target
    6-15:string name
*/

namespace ClientMessages{
void createStdHeader(char* c,char id, char role)
    {
        c[0] = id;
        c[1] = role;
    }
char* createWerewolveVoting(char voted , char id, char role)
    {
        /*
            Creates a message, which informs the server who was voted by the Werewolve
            C-String, because then there is no conversion needed
        */
        STD_HEADER
        res[2] = WEREWOLVEVOTING;
        res[3] = DONE;
        res[4] = 1;
        res[5] = voted;
        return res;
    }
char* createSkipMessage(char id, char role, char phase)
    {
        /*
            Creates a message to inform the Server, that a phase was skipped by the client
        */
        STD_HEADER
        res[2] = phase;
        res[3] = SKIPPED;
        res[4] = '#';
        res[5] = '#';
        return res;
    }
char* createSeerCompleted(char id, char role)
    {
        /*
            creates a message which informs the server, that the client completed his turn
            as a seer
        */
        STD_HEADER
        res[2]=SEER;
        res[3]=DONE;
        res[4] = '#';
        res[5] = '#';
        return res;
    }
char* createWerewolveKillCompleted(char id, char role)
    {
        /*
            creates a message which informs the server, that the Client received who died
        */
       STD_HEADER
       res[2] = WEREWOLVEKILL;
       res[3] = DONE;
       res[4] = '#';
       res[5] = '#';
       return res;
    }
char* createVoting( char voted, char id, char role)
    {
        /*
            Creates a message, which informs the server who was voted by the Client
            C-String, because then there is no conversion needed
        */
        STD_HEADER
        res[2] = VOTING;
        res[3] = DONE;
        res[4] = 1;
        res[5] = voted;
        std::cout<<"Sended voting ballot" << std::endl;
        return res;
    }
char* createExecutionCompleted(char id, char role)
    {
        /*
            informs the Server that the client received the Execution-command
        */
        STD_HEADER
        res[2] = EXECUTION;
        res[3] = DONE;
        res[4] = '#';
        res[5] = '#';
        return res;
    }
char* createDataRequest(char id, char role, char phase)
    {
        /*
            If this message is received by the Server it will send Data back
        */
        STD_HEADER
        res[2] = phase;
        res[3] = NOTDONE;
        res[4] = '#';
        res[5] = '#';
        return res;
    }
char* createJoinMessage(std::string name, char id, char role)
    {
        STD_HEADER
        res[2] = JOIN;
        res[3] = '#';
        res[4] = '#';
        res[5] = '#';
        strcat(res,name.c_str());
        return res;
    }
}