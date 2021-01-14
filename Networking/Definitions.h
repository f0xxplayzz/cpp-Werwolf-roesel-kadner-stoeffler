#pragma once
//phases of the game
#define JOIN 1
#define WEREWOLVEVOTING 2
#define WITCH 3
#define SEER 4
#define WEREWOLVEKILL 5
#define VOTING 6
#define EXECUTION 7
#define GAMEOVER 9

/*
Code for the witch (not implemented yet)
#define KILLED 1
#define HEALED 2
#define DO_NOTHING 3
*/

//The following three definitions, describes if a phase is either not completed, completed or skipped
#define NOTDONE 1
#define DONE 2
#define SKIPPED 3

//defines the time (in millis) until the client asks for new Data
#define SLEEPINGTIME 500

//Definitions for the Roles, that a Player can have
#define VILLAGER_ROLE 1
#define WEREWOLVE_ROLE 2
#define SEER_ROLE 3
#define WITCH_ROLE 4

//defines the max Length of a message (Server and Client)
#define BUFFERLENGTH 500

#define SERVER_IP "127.0.0.1"
#define PORT "8999" 