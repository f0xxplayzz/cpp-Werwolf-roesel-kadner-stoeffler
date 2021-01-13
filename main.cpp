#include "roleDeployment.cpp"

int main()
{
	//(re-)starts the game for Hot-chair
	Narrator nrt;
    std::string mode;
    std::string choice = "0";  

    std::cout << "Welcome to Werewolf, press the associated Number for the mode you want to play!" << std::endl;
    std::cout << "1. Hotseat-Mode" << std::endl;
    std::cout << "2. Online-Mode" << std::endl;
    std::cout << "3. Exit" << std::endl;
    
    while (mode != "1" && mode != "2" && mode != "3") {
    std::cout << "Enter a number: ";
    std::cin >> mode;
    };

    if (mode == "1") {
	    std::string answer = "y";
        do {
            std::shared_ptr<Game> mainGame = roleDeployment(10);
            nrt.gameStartVillage();
            while (mainGame->gameOver == false) {
                turnNight(mainGame);
                if (mainGame->gameOver == false) {
                    turnDay(mainGame);
                }
            }
            std::cout << "Do you want to play again? (Enter 'y' for yes otherwise enter anything else) "; 
            std::cin >> answer;
        } while (answer == "y");
    } else if (mode == "2") {
        std::cout << "Do you want to Host or do you want to join a Host?" << std::endl;
        std::cout << "1. Host" << std::endl;
        std::cout << "2. Join" << std::endl;
        while (choice != "1" && choice != "2") 
        {
            std::cout << "Enter a number: ";
            std::cin >> choice;
        };
        
    } else if (mode == "3") {
        std::cout << "Good bye!";
    }
    
    return 0;
}