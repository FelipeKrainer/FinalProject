// Link command: g++ main.cpp Monster.cpp Player.cpp -o TurnBasedBattleSystem.exe
// Run in the Command Prompt in the folder containing the "Monster.cpp", "Player.cpp", and "main.cpp" files.

#include "Monster.h"
#include "Player.h"
#include <iostream>
#include <fstream>

using namespace std;

int const AMOUNT_OF_MONSTERS = 18;                              // The amount of monsters in the game.
string QUIT = "99";                                             // Used to quit the game when asked.
string UserInput = "";                                          // Used to decide what action to take in the menus.
string Line = "---------------------------------------";        // Used to separate output.
Monster ArrayOfMonsters[AMOUNT_OF_MONSTERS];                    // An array of Monster objects.
Player ThePlayer;                                               // The player object. Holds and manipulates player stats.

// ---------------------------------------------------------------------- Function Prototypes:

void FillArrayOfMonsterObjects(); 

// ---------------------------------------------------------------------- Main Program:

int main() {
    FillArrayOfMonsterObjects();
    while (1 == 1) {
        std::cout << "Please enter a monster's ID number (or '" << QUIT << "' to quit): ";
        std::cin >> UserInput;
        if (UserInput == QUIT) {
            break;
        } else if (UserInput == "20") {
            ThePlayer.Print();
            ThePlayer.AddCurrentEXP(1);
        } else {
            std::cout << Line << endl;
            ArrayOfMonsters[stoi(UserInput) - 1].PrintInfo();
            std::cout << Line << endl;
        }
    }
    std::cout << "Press Enter to exit...";
    std::cin.get();  // Wait for the user to press Enter
    return 0;
}

// ---------------------------------------------------------------------- Function Definitions:

// Opens the "MonsterData.txt" file and reads it into ArrayOfMonsters objects.
void FillArrayOfMonsterObjects() {
    ifstream File("MonsterData.txt");
    string Text = "";
    for (int i = 0; i < AMOUNT_OF_MONSTERS; i++) {
        File >> Text;
        ArrayOfMonsters[i].SetName(Text);
        File >> Text;
        ArrayOfMonsters[i].SetHP(Text);
        File >> Text;
        ArrayOfMonsters[i].SetATK(Text);
        File >> Text;
        ArrayOfMonsters[i].SetDEF(Text);
        File >> Text;
        ArrayOfMonsters[i].SetAG(Text);
        File >> Text;
        ArrayOfMonsters[i].SetEXP(Text);
        File >> Text;
        ArrayOfMonsters[i].SetMethodOfAttack(Text);
    }
    File.close();
}
