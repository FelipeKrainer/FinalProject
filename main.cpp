// Link command: g++ main.cpp Monster.cpp Player.cpp -o TurnBasedBattleSystem.exe
// Run in the Command Prompt in the folder containing the "Monster.h", "Monster.cpp", and "TurnBasedBattleSystem.cpp" files.

#include "Monster.h"
#include "Player.h"
#include <iostream>
#include <windows.h>
#include <fstream>

// Used for the ClearScreen() function.
#ifdef _WIN32
const std::string OS = "Windows";
#elif __APPLE__
const std::string OS = "Mac OS";
#elif __linux__
const std::string OS = "Linux";
#else
const std::string OS = "Unknown";
#endif

using namespace std;

int const AMOUNT_OF_MONSTERS = 18;                                                                       // The amount of monsters in the game.
string QUIT = "99";                                                                                      // Used to quit the game when asked.
string UserInput = "";                                                                                   // Used to decide what action to take in the menus.
string Line = "--------------------------------------------------------------------------------";        // Used to separate output.
Monster ArrayOfMonsters[AMOUNT_OF_MONSTERS];                                                             // An array of Monster objects.
Player ThePlayer;                                                                                        // The player object. Holds and manipulates player stats.
bool Validated = false;                                                                                  // Used to hold the return value from Validate().

int RedPotionsInBag = 0;                                                                                 // The amount of potions in the player's bag.
int BluePotionsInBag = 0;                                                                                // -
int PurplePotionsInBag = 0;                                                                              // -

int RedPotionCost = 10;                                                                                  // Cost of potions. Increases after every purchase.
int BluePotionCost = 20;                                                                                 // -
int PurplePotionCost = 30;                                                                               // -

int CurrentArena = 0;                                                                                    // The arena the player is currently in.

bool Arena1Beat = false;                                                                                 // Whether the player has beat a given arena.
bool Arena2Beat = false;                                                                                 // -
bool Arena3Beat = false;                                                                                 // -
bool Arena4Beat = false;                                                                                 // -
bool Arena5Beat = false;                                                                                 // -

// ---------------------------------------------------------------------- Function Prototypes:

void FillArrayOfMonsterObjects();                               // Creates an array of Monster objects.
void ClearScreen();                                             // Clears text from the screen.
void Introduction();                                            // Text introducing the game.
void PotionScreen();                                            // Displays the potion shop.
void ArenaMenuPage1();                                          // Displays Arena options menu 1.
void ArenaMenuPage2();                                          // Displays Arena options menu 2.

bool Validate(string, string, string, string, string, string);  // Validate input against five options.
bool Validate(string, string, string, string, string);          // Validate input against four options.
bool Validate(string, string, string, string);                  // Validate input against three options.
bool Validate(string, string, string);                          // Validate input against two options.

// ---------------------------------------------------------------------- Main Program:

int main() {
    FillArrayOfMonsterObjects();
    Introduction();
    MainMenu: 
    ClearScreen();
    cout << Line << endl;
    cout << "Enter whether you want to start a Gauntlet ('g'), or buy potions ('p'): " << endl;
    cout << Line << endl;
    MainMenuInput:                                                                                                  // The main menu option box.
    cin >> UserInput;
    Validated = Validate(UserInput, "g", "p");
    // Repeats the question if input is incorrect.
    if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto MainMenuInput;} 
    if (UserInput == "p") {                                                                                         //Chose to buy potions.
        PotionMenu:           
        ClearScreen();
        PotionScreen();
        PotionMenuInput:
        cin >> UserInput;
        Validated = Validate(UserInput, "r", "b", "p", "n");
        if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto PotionMenuInput;}
        if (UserInput == "r") {                                                                                     // Chose to buy Red Potion.
            if (ThePlayer.GetGold() < RedPotionCost) {cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(RedPotionCost);
            RedPotionCost *= 1.2;
            RedPotionsInBag += 1;
            goto PotionMenu;
        } else if (UserInput == "b") {                                                                              // Chose to buy Blue Potion.
            if (ThePlayer.GetGold() < BluePotionCost) {cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(BluePotionCost);
            BluePotionCost *= 1.2;
            BluePotionsInBag += 1;
            goto PotionMenu;
        } else if (UserInput == "p") {                                                                              // Chose to buy Purple Potion.
            if (ThePlayer.GetGold() < PurplePotionCost) {cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(PurplePotionCost);
            PurplePotionCost *= 1.2;
            PurplePotionsInBag += 1;
            goto PotionMenu;
        } else {                                                                                                    // Chose to leave potion shop.
            goto MainMenu;
        }
    } else {                                                                                                        // Chose to enter an arena.
        ArenaMenu:                                                                                              
        ArenaMenuPage1();
        ArenaInputPg1:                                                                                              // Arena menu page 1.
        cin >> UserInput;
        Validated = Validate(UserInput, "a", "b", "c", "p", "n");
        if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto ArenaInputPg1;} 
        if (UserInput == "a") {cout << "Chose Wooden Arena!" << endl;}
        else if (UserInput == "b") {cout << "Chose Tin Arena!" << endl;}
        else if (UserInput == "c") {cout << "Chose Bronze Arena!" << endl;}
        if (UserInput == "p") {
            ArenaMenuPage2();
            ArenaInputPg2:                                                                                          // Arena menu page 2.
            cin >> UserInput;
            Validated = Validate(UserInput, "d", "e", "f", "p", "n");
            if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto ArenaInputPg2;}
            if (UserInput == "d") {cout << "Chose Silver Arena!" << endl;}
            else if (UserInput == "e") {cout << "Chose Gold Arena!" << endl;}
            else if (UserInput == "f") {cout << "Chose Platinum Arena!" << endl;}
            else if (UserInput == "p") {goto ArenaMenu;}
            else {goto MainMenu;}
        } else {goto MainMenu;}
    }
    
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

// Clears the screen using different commands depending on the system the program is run on.
void ClearScreen() {
    cin.get();
    if (OS == "Windows") {
        system("cls");  // Clear the screen
        Sleep(80);
    } else if (OS == "Mac OS" || OS == "Linux") {
        system("clear");
    }
}

// Prints text introducing the game.
void Introduction() {
    cout << Line << endl;
    cout << "Welcome the the Monster Arena! In this game, you'll battle through different" << endl;
    cout << "gauntlets fighting strange and terrifying monsters. As you battle, you'll" << endl;
    cout << "gain Gold that can be used to purchase potions, and Experience Points that" << endl;
    cout << "can help you level up and gain better stats, such as Health, Attack, and" << endl;
    cout << "Defence. You'll have to beat all [NUMBER] gauntlets to become the Champion." << endl;
    cout << Line << endl;
    cout << "Press Enter to begin...";
}

// Prints text showing the potion shop menu.
void PotionScreen() {
    cout << Line << endl;
    cout << "You have " << ThePlayer.GetGold() << " Gold in your bag."<< endl;
    cout << "'r' = Red Potion    ($" << RedPotionCost << ")" << " (" << RedPotionsInBag << " in bag)" << endl;
    cout << "'b' = Blue Potion   ($" << BluePotionCost << ")" << " (" << BluePotionsInBag << " in bag)" << endl;
    cout << "'p' = Purple Potion ($" << PurplePotionCost << ")" << " (" << PurplePotionsInBag << " in bag)" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    cout << "Enter the type of potion you want:" << endl;
}

// Prints text showing the first arena menu.
void ArenaMenuPage1() {
    ClearScreen();
    cout << Line << endl;
    cout << "'a' = Wooden Arena" << endl;
    if (Arena1Beat == true) {cout << "'b' = Tin Arena" << endl;} else {cout << "'b' = [Locked]" << endl;}
    if (Arena2Beat == true) {cout << "'c' = Bronze Arena" << endl;} else {cout << "'c' = [Locked]" << endl;}
    cout << "'p' = Page ->" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    cout << "Please enter the Gauntlet that would you like to challenge: " << endl;
}

// Prints text showing the second arena menu.
void ArenaMenuPage2() {
    ClearScreen();
    cout << Line << endl;
    if (Arena3Beat == true) {cout << "'d' = Silver Arena" << endl;} else {cout << "'d' = [Locked]" << endl;}
    if (Arena4Beat == true) {cout << "'e' = Gold Arena" << endl;} else {cout << "'e' = [Locked]" << endl;}
    if (Arena5Beat == true) {cout << "'f' = Platinum Arena" << endl;} else {cout << "'f' = [Locked]" << endl;}
    cout << "'p' = Page <-" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    cout << "Please enter the Gauntlet that would you like to challenge: " << endl;
}

bool Validate(string Input, string Option1, string Option2, string Option3, string Option4, string Option5) {
    return (Input == Option1 || Input == Option2 || Input == Option3 || Input == Option4 || Input == Option5);
}

bool Validate(string Input, string Option1, string Option2, string Option3, string Option4) {
    return (Input == Option1 || Input == Option2 || Input == Option3 || Input == Option4);
}

bool Validate(string Input, string Option1, string Option2, string Option3) {
    return (Input == Option1 || Input == Option2 || Input == Option3);
}

bool Validate(string Input, string Option1, string Option2) {
    return (Input == Option1 || Input == Option2);
}
