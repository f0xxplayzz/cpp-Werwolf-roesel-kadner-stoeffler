WEREWOLVE by Jan Stöffler, Lars Rösel & Linus Kadner

Compile only the main.cpp file:
-on windows use "g++ main.cpp -o main.exe -IC:\installation\path\include\boost-1_75  -D__USE_W32_SOCKETS -D_WIN32_WINNT=0x0501 -lws2_32 -lwsock32 -lpthread" to Compile
-be sure Boost is installed
-be sure ur winsockets are installed

Known Bugs:
-Less than 5 players -> game crashes.
-Game crashes sometimes when Host dies first
-WinCondition is spammed in the console

Bugs are not fixed because of time limitations
