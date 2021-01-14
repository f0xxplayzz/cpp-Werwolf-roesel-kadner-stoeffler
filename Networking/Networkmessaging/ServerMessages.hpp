#include "./../Makros.hpp"
#include "./../Definitions.hpp"
#include "./../../turn.cpp"
#include <string>
#include <vector>

/*
    Structure of Server Messages:
    (normal)
    0:phase -> if 9: gameOver
    1:length_of_vector
    2...2+lengthOfVector:IDs of possibilities
    ...:String of possibilities
*/
namespace ServerMessages
{
    std::string createJoin(char phase,std::string name)
    {
        std::string result = "";
        result += phase;
        result += "Joined the game as ";
        result += name;
        return result;
    }
    std::string createWerewolveVoting(Game* g)
    {
        std::string result = "";
        std::string result2 = "";
        std::vector<char> ids;
        int iterator = 1;
        for (int i = 0; i < g->alivePlayers->size();i++) 
        {
            if(g->alivePlayers->at(i)->role != WEREWOLVE_ROLE)
            {
                result2 += std::to_string(iterator++); 
                result2 += ".) ";
                result2 += g->alivePlayers->at(i)->name;
                result2 += '\n';
                ids.push_back(g->alivePlayers->at(i)->id);
            }
        }
        result += WEREWOLVEVOTING;
        result += (char) ids.size();
        for(int i=0;i<ids.size();i++)
        {
            result += ids.at(i);
        }
        result += result2;
        return result;
    }
    std::string createSeer(Game* g)
    {
        std::string result = "";
        std::string result2 = "";
        std::vector<char> roles;
        int iterator = 1;
        for (int i = 0; i < g->alivePlayers->size(); i++) 
        {
            if (g->alivePlayers->at(i)->role != 3 ) 
            {
                result2 += std::to_string(iterator++);
                result2 += "). ";
                result2 += g->alivePlayers->at(i)->name;
                result2 += '\n';
                roles.push_back(g->alivePlayers->at(i)->role);
            }
        }
        result += SEER;
        result += (char) roles.size();
        for(int i=0;i<roles.size();i++)
        {
            result += roles.at(i);
        }
        result += result2;
        return result;
    }
    std::string createWerewolveKill(Game* g)
    {
        std::string result = "";
        result += WEREWOLVEKILL;
        result += 1;
        result += (char) g->getMostVoted();
        result += g->getMostVoted_name();
        return result;
    }
    std::string createVoting(Game* g, char id)
    {
        std::cout << " " <<std::endl;
        std::string result = "";
        std::string result2 = "";
        std::vector<char> ids;
        int iterator = 1;
        for (int i = 0; i < g->alivePlayers->size();i++) 
        {
            if(g->alivePlayers->at(i)->id != id)
            {
                result2 += std::to_string(iterator++); 
                result2 += ".) ";
                result2 += g->alivePlayers->at(i)->name;
                result2 += '\n';
                ids.push_back((char)g->alivePlayers->at(i)->id);
            }
        }
        result += VOTING;
        result += (char) ids.size();
        for(int i=0;i<ids.size();i++)
        {
            result += ids.at(i);
        }
        result += result2;
        return result;
    }
    std::string createExecution(Game* g)
    {
        std::string result = "";
        result += EXECUTION;
        result += 1;
        result += (char) g->getMostVoted();
        result += g->getMostVoted_name();
        return result;
    }
    std::string createGameover (Game* g)
    {
        std::string result = "";
        result += GAMEOVER;
        result += g->winCondition;
        return result;
    }
}