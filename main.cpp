// Link command: g++ main.cpp Monster.cpp Player.cpp -o TurnBasedBattleSystem.exe
// Run in the Command Prompt in the folder containing the "Monster.h", "Monster.cpp", and "TurnBasedBattleSystem.cpp" files.

/* Still to be added:
    Monster images
    Balancing difficulty
*/

#include "Monster.h"
#include "Player.h"
#include <iostream>
#include <random>
#include <windows.h>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <conio.h>

// Used for the ClearScreenWithoutInput() function. Sets what command is used.

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

// Random number generators used for damage and hit chance:

std::random_device Rand;
std::mt19937 Generator(Rand());
std::uniform_int_distribution<int> HitChanceDistribution(1, 20);
std::uniform_int_distribution<int> DamageDistribution(0, 3);

//------------------------------------------------------------------------ Variable definitions:

int const AMOUNT_OF_MONSTERS = 37;                                                                       // The amount of monsters in the game.
string QUIT = "99";                                                                                      // Used to quit the game when asked.
string UserInput = "";                                                                                   // Holds what action the player entered.
string Line = "--------------------------------------------------------------------------------";        // Used to separate output.
Monster ArrayOfMonsters[AMOUNT_OF_MONSTERS];                                                             // An array of Monster objects.
string ArrayOfMonsterSprites[AMOUNT_OF_MONSTERS][8];                                                     // An array holding the battle sprite of each monster.
Player ThePlayer;                                                                                        // The player object. Holds and manipulates player stats.
bool Validated = false;                                                                                  // Used to hold the return value from Validate().
string Text = "";                                                                                        // Used to hold text to be printed one character at a time.

int RedPotionsInBag = 0;                                                                                 // The amount of potions in the player's bag.
int BluePotionsInBag = 0;                                                                                // -
int PurplePotionsInBag = 0;                                                                              // -

int RedPotionCost = 10;                                                                                  // Cost of potions. Increases after every purchase.
int BluePotionCost = 20;                                                                                 // -
int PurplePotionCost = 30;                                                                               // -

int RedPotionHeal = 16;                                                                                  // The amount a given potion heals.
int BluePotionHeal = 32;                                                                                 // -
int PurplePotionHeal = 48;                                                                               // -

int CurrentArena = 0;                                                                                    // The arena the player is currently in.
int DeathCounter = 0;                                                                                    // The number of times the player has died.
bool HasRunAway = false;                                                                                 // Holds whether the player has given up on an arena.

bool Arena1Beat = false;                                                                                 // Whether the player has beat a given arena.
bool Arena2Beat = false;                                                                                 // -
bool Arena3Beat = false;                                                                                 // -
bool Arena4Beat = false;                                                                                 // -
bool Arena5Beat = false;                                                                                 // -
bool Arena6Beat = false;                                                                                 // -

// ---------------------------------------------------------------------- Function Prototypes:

void Arena();                                                           // The main battle scene.
bool MonsterTurn(int);                                                  // Computes the monster's damage to the player.
bool PlayerTurn(int);                                                   // Computes the players damage to the monster.
bool FinalBattle();                                                     // The final boss battle.
void DisplayFinalBattle(int);                                           // Displays the final boss fight graphic.
void DisplayFatherAnimation(int);                                       // Displays the in-battle father animation.
void ShowFatherAndSon();                                                // Displays the father and son image.
void DisplaySunrise();                                                  // Displays the end screen.

void DisplayStats(int);                                                 // Displays Player and Monster stats.
std::string toLowerCase(const std::string &str);                        // Changes user input to lower case.
void SaveToFile(const std::string& fileName);                           // Saves the player stats
void LoadFromFile(const std::string& fileName);                         // Loads the player's stats from the save.txt file.
void OpeningMenuScreen();                                               // Displays the Save Screen
void OpeningTextScroll();                                               // Displays story text at the beginning.
void StatsScreen();                                                     // Displays the Stats Screen

void FillArrayOfMonsterObjects();                                       // Creates an array of Monster objects.
void FillArrayOfMonsterSprites();                                       // Fills the ArrayOfMonsterSprites with data from the MonsterSprites.txt file.
void ClearScreenWithoutInput();                                         // Clears text from the screen. Activates when called.
void ChangeColor(int);                                                  // Changes the text color.

void Scroll(string);                                                    // Prints text one character at a time.
void SwordAnimation1();                                                 // Title animation.
void SwordAnimation2();                                                 // Title animation.
void Introduction();                                                    // Text introducing the game.
void DisplayOpeningTextGraphic();                                       // Shows the opening text-scroll image.
void MainMenu();                                                        // The main menu image and text.

void CH(string, int);                                                   // Prints the entered chunk of text in the entered color.
void PotionScreen();                                                    // Displays the potion shop.
bool BattlePotionMenu(string, int, int);                                // Displays the in-battle potion menu.
void ArenaMenuPage1();                                                  // Displays Arena options menu 1.
void ArenaMenuPage2();                                                  // Displays Arena options menu 2.

bool Validate(string, string, string, string, string, string, string);  // Validate input against six options.
bool Validate(string, string, string, string, string, string);          // Validate input against five options.
bool Validate(string, string, string, string, string);                  // Validate input against four options.
bool Validate(string, string, string, string);                          // Validate input against three options.
bool Validate(string, string, string);                                  // Validate input against two options.

// ---------------------------------------------------------------------- Main Program:

int main() {
    FillArrayOfMonsterObjects();
    FillArrayOfMonsterSprites();
    HasRunAway = false;
    Introduction();
    std::cout << "Press Enter to begin..." << endl;
    std::cin.get();
    SaveScreen:
    OpeningMenuScreen();
    cin >> UserInput;
    UserInput = toLowerCase(UserInput);
    Validated = Validate(UserInput, "c", "s", "e");
    if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto SaveScreen;}
    if (UserInput == "c"){
        LoadFromFile("save.txt");
        Sleep(1200);
    } else if (UserInput == "s"){
        OpeningTextScroll();
    } else { // If player selects exit, then close game.
        return 0;
    }
    MainMenu: 
    ThePlayer.SetCurrentHP(ThePlayer.GetMaxHP());
    ChangeColor(11);
    MainMenu();
    MainMenuInput:                                                                                                  // The main menu option box.
    cin >> UserInput;
    UserInput =  toLowerCase(UserInput);
    Validated = Validate(UserInput, "m", "p", "s", "d");
    // Repeats the question if input is incorrect.
    if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto MainMenuInput;} 
    if (UserInput == "p") {                                                                                         //Chose to buy potions.
        PotionMenu:           
        PotionScreen();
        PotionMenuInput:
        cin >> UserInput;
        UserInput=  toLowerCase(UserInput);
        Validated = Validate(UserInput, "r", "b", "p", "n");
        if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto PotionMenuInput;}
        if (UserInput == "r") {                                                                                     // Chose to buy Red Potion.
            if (ThePlayer.GetGold() < RedPotionCost) {std::cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(RedPotionCost);
            RedPotionCost *= 1.1;
            RedPotionsInBag += 1;
            goto PotionMenu;
        } else if (UserInput == "b") {                                                                              // Chose to buy Blue Potion.
            if (ThePlayer.GetGold() < BluePotionCost) {std::cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(BluePotionCost);
            BluePotionCost *= 1.1;
            BluePotionsInBag += 1;
            goto PotionMenu;
        } else if (UserInput == "p") {                                                                              // Chose to buy Purple Potion.
            if (ThePlayer.GetGold() < PurplePotionCost) {std::cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(PurplePotionCost);
            PurplePotionCost *= 1.1;
            PurplePotionsInBag += 1;
            goto PotionMenu;
        } else {                                                                                                    // Chose to leave potion shop.
            goto MainMenu;
        }
    } else if (UserInput == "m"){                                                                                   // Chose to enter an arena.
        ArenaMenu:                                                                                              
        ArenaMenuPage1();
        ArenaInputPg1:                                                                                              // Arena menu page 1.
        cin >> UserInput;
        UserInput=  toLowerCase(UserInput);
        Validated = Validate(UserInput, "a", "b", "c", "p", "n");
        if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto ArenaInputPg1;} 
        if (UserInput == "a") {                                                                                     // Chose Wooden Arena.
            CurrentArena = 1;
            Arena();
        } else if (UserInput == "b") {                                                                              // Chose Tin Arena.
            if (Arena1Beat == true) {
                CurrentArena = 2;
                Arena();
            } else {
                std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg1;
            }
        } else if (UserInput == "c") {                                                                              // Chose Bronze Arena.
            if (Arena2Beat == true) {
                CurrentArena = 3;
                Arena();
            } else {
                std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg1;
            }
        }
        if (UserInput == "p") {
            ArenaMenuPage2();
            ArenaInputPg2:                                                                                          // Arena menu page 2.
            cin >> UserInput;
            UserInput =  toLowerCase(UserInput);
            Validated = Validate(UserInput, "d", "e", "f", "g", "p", "n");
            if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto ArenaInputPg2;}
            if (UserInput == "d") {                                                                                 // Chose Silver Arena.
                if (Arena3Beat == true) {
                    CurrentArena = 4;
                    Arena();
                    goto MainMenu;
                } else {
                    std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg2;
                }
            } else if (UserInput == "e") {                                                                          // Chose Gold Arena.
                if (Arena4Beat == true) {
                    CurrentArena = 5;
                    Arena();
                    goto MainMenu;
                } else {
                    std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg2;
                }
            } else if (UserInput == "f") {                                                                          // Chose Platinum Arena.
                if (Arena5Beat == true) {
                    CurrentArena = 6;
                    Arena();
                    goto MainMenu;
                } else {
                    std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg2;
                }
            } else if (UserInput == "g") {                                                                          // Chose Final Boss.
                if (Arena6Beat == true) {
                    CurrentArena = 7;
                    // If player beats the final boss, then show the credits and ending screen.
                    if (FinalBattle()) {
                        ClearScreenWithoutInput();
                        Sleep(300);
                        ShowFatherAndSon();
                        Sleep(5000);
                        DisplaySunrise();
                        return 0;
                    } else { 
                        goto MainMenu;
                    }
                } else {
                    std::cout << "Invalid entry! Please try again." << endl; goto ArenaInputPg2;
                }
            } else if (UserInput == "p") {goto ArenaMenu;
            } else {goto MainMenu;}
        } else {goto MainMenu;}
    } else if(UserInput == "s"){
        SaveToFile("save.txt");
        Sleep(800);

        goto MainMenu;
    }else if(UserInput == "d"){
        StatsScreen();
        goto MainMenu;
    }
    
    return 0;
}

// ---------------------------------------------------------------------- Function Definitions:


// Function to convert a string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(c);
    }
    return result;
}

// Saves relevant variables to a text file.
void SaveToFile(const std::string& fileName) {
        std::ofstream outFile(fileName, std::ios::out);

        if (outFile.is_open()) {
            outFile << ThePlayer.GetLevel() << ' '
                    << ThePlayer.GetMaxHP() << ' '
                    << ThePlayer.GetATK() << ' '
                    << ThePlayer.GetDEF() << ' '
                    << ThePlayer.GetAG() << ' '
                    << ThePlayer.GetCurrentEXP() << ' '
                    << ThePlayer.GetEXPToNextLevel() << ' '
                    << ThePlayer.GetGold() << ' '
                    << RedPotionsInBag << ' '
                    << BluePotionsInBag << ' '
                    << PurplePotionsInBag << ' '
                    << RedPotionCost << ' '
                    << BluePotionCost << ' '
                    << PurplePotionCost << ' '
                    << DeathCounter << ' '
                    << Arena1Beat << ' '
                    << Arena2Beat << ' '
                    << Arena3Beat << ' '
                    << Arena4Beat << ' '
                    << Arena5Beat << ' '
                    << Arena6Beat;

            outFile.close();
            std::cout << "Player data saved successfully.\n";
        }
}

// Loads data from a save file and inputs it into relevant variables.
void LoadFromFile(const std::string& fileName) {
        std::ifstream inFile(fileName, std::ios::in);
        int level;
        int maxHP;
        int ATK;
        int DEF;
        int AG;
        int currentEXP;
        int expToNextLevel;
        int gold;

        if (inFile.is_open()) {
            inFile >> level >> maxHP >> ATK >> DEF >> AG >> currentEXP >> expToNextLevel
                   >> gold >> RedPotionsInBag >> BluePotionsInBag >> PurplePotionsInBag
                   >> RedPotionCost >> BluePotionCost >> PurplePotionCost >> DeathCounter >> Arena1Beat
                   >> Arena2Beat >> Arena3Beat >> Arena4Beat >> Arena5Beat >> Arena6Beat;

            inFile.close();
            std::cout << "Player data loaded successfully.\n";

            // Set player stats using setter functions
            ThePlayer.SetLevel(level);
            ThePlayer.SetMaxHP(maxHP);
            ThePlayer.SetATK(ATK);
            ThePlayer.SetDEF(DEF);
            ThePlayer.SetAG(AG);
            ThePlayer.SetCurrentEXP(currentEXP);
            ThePlayer.SetEXPToNextLevel(expToNextLevel);
            ThePlayer.SetGold(gold);

        } else {
            std::cout << "You don't have a save file. Starting a new game!\n";
            Sleep(1000);
            OpeningTextScroll();
        }
    }

// The main arena system.
void Arena() {
    bool PlayerIsAlive = true;                                                                  // Holds whether to leave the arena because of player defeat.                                                              // 
    bool MonsterIsAlive = true;                                                                 // Holds whether to move to the next monster because of victory.
    int CurrentMonster = (CurrentArena - 1) * 6;                                                // What monster is being fought.
    int CurrentRound = 1;                                                                       // Used to print the roound number at the beginning of each fight.
    for (int i = CurrentMonster; i < CurrentMonster + 6; i++) {  // Six monsters for each arena.
        ClearScreenWithoutInput();
        ChangeColor(10);
        cout << "Round " << CurrentRound << ": " << ArrayOfMonsters[i].GetName() << endl;
        Sleep(3000);
        ArrayOfMonsters[i].SetCurrentHP(ArrayOfMonsters[i].GetMaxHP());

        while (PlayerIsAlive && MonsterIsAlive) {
            ClearScreenWithoutInput();
            if (ThePlayer.GetCurrentHP() <= 5) {
                ChangeColor(12);
            } else {
                ChangeColor(9);
            }
            DisplayStats(i);

            if (ArrayOfMonsters[i].GetAG() < ThePlayer.GetAG()) {
                MonsterIsAlive = PlayerTurn(i);
                if (HasRunAway) {
                    break;
                }
                if (MonsterIsAlive) {
                    PlayerIsAlive = MonsterTurn(i);
                } 
            } else {
                PlayerIsAlive = MonsterTurn(i);
                if (PlayerIsAlive) {
                    MonsterIsAlive = PlayerTurn(i);
                    if (HasRunAway) {
                        break;
                    }
                } 
            }
            Sleep(600);
            while(kbhit()) getch(); // Clear any keyboard inputs entered while the battle was looping.
        }
        MonsterIsAlive = true;
        CurrentRound += 1;
        if (PlayerIsAlive == false || HasRunAway == true) {
            break;
        }
    }
    if (PlayerIsAlive && HasRunAway == false) {
        cout << Line << endl;
        cout << "You beat the Gauntlet!" << endl;
        if (CurrentArena == 6) {
            cout << "- The LegacyBeast awaits..." << endl;
        }
        switch (CurrentArena) {
            case 1: Arena1Beat = true; break;
            case 2: Arena2Beat = true; break;
            case 3: Arena3Beat = true; break;
            case 4: Arena4Beat = true; break;
            case 5: Arena5Beat = true; break;
            case 6: Arena6Beat = true; break;
        }
        cout << Line << endl;
        Sleep(3000);
    }
}

// The monster's attack. Computes damage with MonsterATK - PlayerDEF. Also checks for game over.
bool MonsterTurn(int MonsterNumber) {
    int MissChance = 0;                     // Chance for player to miss. Filled with a random number each attack.
    MissChance = HitChanceDistribution(Generator);
    if (MissChance != 20) {
        int DamageDealt;
        DamageDealt = (ArrayOfMonsters[MonsterNumber].GetATK() - ThePlayer.GetDEF()) + DamageDistribution(Generator);
        if (DamageDealt <= 0) {
            DamageDealt = 1;
        }
        ThePlayer.LowerCurrentHP(DamageDealt);
        // If monster is faster than player, save damage text, refresh screen, then reprint damage text.
        if (ArrayOfMonsters[MonsterNumber].GetAG() > ThePlayer.GetAG()) {
            DisplayStats(MonsterNumber);
        }
        ChangeColor(12);
        cout << ArrayOfMonsters[MonsterNumber].GetName() << " "
            << ArrayOfMonsters[MonsterNumber].GetMethodOfAttack()
            << " you for " << DamageDealt << " damage!" << endl;
        Sleep(200);
        //DisplayStats(MonsterNumber);
        if (ThePlayer.GetCurrentHP() <= 0) {
            ChangeColor(12);
            cout << "Too bad! You've lost the Gauntlet!" << endl;
            DeathCounter++;
            Sleep(2000);
            ThePlayer.SetCurrentHP(ThePlayer.GetMaxHP());
            return false;
        } else {
            return true;
        }
    } else {
        ChangeColor(13);
        cout << ArrayOfMonsters[MonsterNumber].GetName() << " misses!" << endl;
        return true;
    }
}

// The player's turn in which they can choose to attack, use a potion, or give up. Also checks for victory.
bool PlayerTurn(int MonsterNumber) {
    HasRunAway = false;
    PlayerBattleChoice:
    ChangeColor(11);
    cout << "Enter an action: ('a' = ATTACK, 'p' = POTION, 'g' = GIVE UP)" << endl;
    cin >> UserInput;
    UserInput=  toLowerCase(UserInput);
    Validated = Validate(UserInput, "a", "p", "g");
    if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto PlayerBattleChoice;}
    if (UserInput == "a") {                                                                                                 // Chose to attack.
        int MissChance = 0;
        MissChance = HitChanceDistribution(Generator);
        if (MissChance != 20) {

            int DamageDealt;
            DamageDealt = (ThePlayer.GetATK() - ArrayOfMonsters[MonsterNumber].GetDEF()) + DamageDistribution(Generator);
            if (DamageDealt <= 0) {
                DamageDealt = 1;
            }
            MissChance = HitChanceDistribution(Generator);
            if (MissChance == 20) {
                DamageDealt *= 2;
                ChangeColor(13);
                cout << "Critical hit! You hit the enemy for " << DamageDealt << " damage!" << endl;
            } else {
                ChangeColor(9);
                cout << "You hit the enemy for " << DamageDealt << " damage!" << endl;
            }
            
            ArrayOfMonsters[MonsterNumber].LowerCurrentHP(DamageDealt);
            Sleep(200);
            if (ArrayOfMonsters[MonsterNumber].GetCurrentHP() <= 0) {
                ChangeColor(14);
                cout << "You've won!" << endl;
                cout << "You've gained " << ArrayOfMonsters[MonsterNumber].GetEXP() << " EXP and " 
                    << ArrayOfMonsters[MonsterNumber].GetGold() << " gold!" << endl;
                ThePlayer.AddCurrentEXP(ArrayOfMonsters[MonsterNumber].GetEXP());
                ThePlayer.RaiseGold(ArrayOfMonsters[MonsterNumber].GetGold());
                Sleep(1000);
                ArrayOfMonsters[MonsterNumber].SetCurrentHP(ArrayOfMonsters[MonsterNumber].GetMaxHP());
                return false;
            } else {
                return true;
            }
        } else {
            ChangeColor(13);
            cout << "You missed!" << endl;
            Sleep(200);
            return true;
        }
    } else if (UserInput == "p") {                                                                          // Chose to use a potion.
        //Displaying current number of potions
        PotionInventory:
        ClearScreenWithoutInput();
        ChangeColor(11);
        std::cout << Line << endl;
        std::cout << "Potion Inventory" << endl;
        std::cout << Line << endl;
        ChangeColor(4);
        std::cout << "You have " << RedPotionsInBag << " Red Potion(s)." << endl;
        ChangeColor(9);
        std::cout << "You have " << BluePotionsInBag << " Blue Potion(s)." << endl;
        ChangeColor(5);
        std::cout << "You have " << PurplePotionsInBag << " Purple Potion(s)." << endl;
        ChangeColor(11);
        std::cout << Line << endl;
        std::cout << "Enter the potion you want to use ('r', 'b', 'p') or 'n' to go back: " << endl;
        PotionInventoryInput:
        cin >> UserInput;
        UserInput=  toLowerCase(UserInput);
        Validated = Validate(UserInput, "r", "b", "p", "n");
        if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto PotionInventoryInput;}
        
        //Creating logic of the Red Potion
        if (UserInput == "r"){
            if(!BattlePotionMenu("Red", RedPotionHeal, MonsterNumber)) {
                goto PotionInventoryInput;
            }
        //Creating the logic of the Blue Potion
        } else if (UserInput == "b"){
            if(!BattlePotionMenu("Blue", BluePotionHeal, MonsterNumber)) {
                goto PotionInventoryInput;
            }
        //Creating the logic of the Purple Potion
        } else if (UserInput == "p"){
            if(!BattlePotionMenu("Purple", PurplePotionHeal, MonsterNumber)) {
                goto PotionInventoryInput;
            }
        } else {
            DisplayStats(MonsterNumber);
            goto PlayerBattleChoice;
        }
        return true;
    } else {                                                                                                // Chose to run away.
        cout << "You have given up the fight!";
        Sleep(2000);
        HasRunAway = true;
        return true;
    }
}

// Opens the "MonsterData.txt" file and reads it into ArrayOfMonsters objects.
void FillArrayOfMonsterObjects() {
    ifstream File("MonsterData.txt");
    string Text = "";
    for (int i = 0; i < AMOUNT_OF_MONSTERS; i++) {
        File >> Text;
        ArrayOfMonsters[i].SetName(Text);
        File >> Text;
        ArrayOfMonsters[i].SetMaxHP(Text);
        File >> Text;
        ArrayOfMonsters[i].SetATK(Text);
        File >> Text;
        ArrayOfMonsters[i].SetDEF(Text);
        File >> Text;
        ArrayOfMonsters[i].SetAG(Text);
        File >> Text;
        ArrayOfMonsters[i].SetEXP(Text);
        File >> Text;
        ArrayOfMonsters[i].SetGold(Text);
        File >> Text;
        ArrayOfMonsters[i].SetMethodOfAttack(Text);
    }
    File.close();
}

// Fills an array with the text displaying a monster sprite.
void FillArrayOfMonsterSprites() {
    ifstream File("MonsterSprites.txt");
    string InText = "";
    for (int i = 0; i < AMOUNT_OF_MONSTERS; i++) {
        for (int j = 0; j < 6; j++) {
            getline(File, InText);
            ArrayOfMonsterSprites[i][j] = InText;

        }
    }
    File.close();
}

// Clears the screen using different commands depending on the system the program is run on. No input is needed.
void ClearScreenWithoutInput() {
    if (OS == "Windows") {
        system("cls");  // Clear the screen
    } else if (OS == "Mac OS" || OS == "Linux") {
        system("clear");
    }
}

// Changes the color of text on the screen.
void ChangeColor(int Color) {
    if (OS == "Windows") {
        HANDLE console_color; 
        console_color = GetStdHandle(STD_OUTPUT_HANDLE); 
        SetConsoleTextAttribute(console_color, Color); 
    }
}

// Prints text one character at a time.
void Scroll(string Text) {
    for (int i = 0; i < Text.length(); i++) {
        if (Text[i] == '|') {
            Sleep(300);
        } else {
            cout << Text[i];
            Sleep(20);
        }
        
    }
}

// The first animated frame of the title screen.
void SwordAnimation1() {
    ClearScreenWithoutInput();
    ChangeColor(13);
    cout << "   *                       *       .   _                            .    *      " << endl;
    cout << "                                      /-\\     .        *                        " << endl;
    cout << "                     .                \\_/                     .                 " << endl;
    cout << "    .       *                         |-|                                       " << endl;
    cout << "                                .     |-|                                       " << endl;
    ChangeColor(9);
    cout << "                                   __T:::T__                                    " << endl;
    cout << "          .          *            /__\\___/__\\            .        *        .    " << endl;
    cout << "                                 //         \\\\                                  " << endl;
    cout << "       *      .                  U   | / |   U         .                        " << endl;
    ChangeColor(11);
    cout << "    _               _                |   |                _               _     " << endl;
    cout << " __/ \\__         __/ \\__             |   |      *      __/ \\__         __/ \\__  " << endl;
    ChangeColor(15);
    cout << " \\  _  /    *    \\  _  /     .       | / |             \\  _  /    *    \\  _  /  " << endl;
    cout << "  |/ \\|           |/ \\|              |   |              |/ \\|           |/ \\|  " << endl;
    ChangeColor(9);
    cout << Line << endl;
    cout << "Welcome the the Monster Arena! In this game, you'll battle through different" << endl;
    cout << "Gauntlets fighting strange and terrifying monsters. As you battle, you'll" << endl;
    cout << "gain gold that can be used to purchase potions, and experience points that" << endl;
    cout << "can help you level up and gain better stats, such as Health, Attack, and" << endl;
    cout << "Defence. You'll have to beat all six gauntlets to win the game." << endl;
    cout << Line << endl;
}

// The second animated frame of the title screen.
void SwordAnimation2() {
    ClearScreenWithoutInput();
    ChangeColor(13);
    cout << "   .          .            .       *   _                            *    .      " << endl;
    cout << "                                      /-\\     *        .                        " << endl;
    cout << "                     *                \\_/                                       " << endl;
    cout << "    *       .                         |-|          .            .               " << endl;
    cout << "                                *     |-|                                       " << endl;
    ChangeColor(9);
    cout << "                                   __T:::T__                                    " << endl;
    cout << "        .            .            /__\\___/__\\            *        .        *    " << endl;
    cout << "                         .       //         \\\\                                  " << endl;
    cout << "       .      *  .               U   | / |   U                                  " << endl;
    ChangeColor(11);
    cout << "    _               _                |   |                _               _     " << endl;
    cout << " __/ \\__         __/ \\__             |   |      .      __/ \\__         __/ \\__  " << endl;
    ChangeColor(15);
    cout << " \\  _  /    .    \\  _  /     *       | / |             \\  _  /    .    \\  _  /  " << endl;
    cout << "  |/ \\|           |/ \\|              |   |              |/ \\|           |/ \\|  " << endl;
    ChangeColor(9);
    cout << Line << endl;
    cout << "Welcome the the Monster Arena! In this game, you'll battle through different" << endl;
    cout << "Gauntlets fighting strange and terrifying monsters. As you battle, you'll" << endl;
    cout << "gain gold that can be used to purchase potions, and experience points that" << endl;
    cout << "can help you level up and gain better stats, such as Health, Attack, and" << endl;
    cout << "Defence. You'll have to beat all six gauntlets to win the game." << endl;
    cout << Line << endl;
    ChangeColor(11);
}

// Prints text introducing the game.
void Introduction() {
    for (int i = 0; i < 4; i++) {
        SwordAnimation1();
        Sleep(450);
        SwordAnimation2();
        Sleep(450);
    }
}

// Displays the starting menu screen.
void OpeningMenuScreen (){
    ClearScreenWithoutInput();
    ChangeColor(9);
    std::cout << "\t\t\t*************************************\n";
    std::cout << "\t\t\t*             GAME MENU             *\n";
    std::cout << "\t\t\t*************************************\n";
    std::cout << "\t\t\t*                                   *\n";
    std::cout << "\t\t\t*  S. Start New Game                *\n";
    std::cout << "\t\t\t*  C. Continue                      *\n";
    std::cout << "\t\t\t*  E. Exit                          *\n";
    std::cout << "\t\t\t*                                   *\n";
    std::cout << "\t\t\t*************************************\n";
    std::cout << Line << endl;
    ChangeColor(11);
    std::cout << "Please enter what you want to do: " << endl;
}

// Displays the opening text-scroll image.
void DisplayOpeningTextGraphic() {
    ClearScreenWithoutInput();
    CH("            .              .                    .                *             .", 9);cout << endl;
    CH("    +                           .                                     .         ", 11);cout << endl;
    CH(" .             ___/|                ___/|                ___/|                  ", 15);cout << endl;
    CH("          .   /___||               /___||               /___||               .  ", 15);cout << endl;
    CH("              ____||____           ____||____           ____||____              ", 15);cout << endl;
    CH("             |__       _|         |_ _       |         |___       |         +   ", 11);cout << endl;
    CH("     .       |_     _ __|         |        __|         |      _ __|     .       ", 11);cout << endl;
    CH(" *           |___      _|-_______-|_        _|-_______-|_      ___|            .", 9);cout << endl;
    CH("             | _      '            '          '            '    _ |             ", 9);cout << endl;
    CH("          ... \\ _                                      _  _    _ /      ....    ", 9);cout << endl;
    CH("  ''-_--_.     \\ _   _____ __  __________________   __ ___ _  _ /  _--_.    ..  ", 1);cout << endl;
    CH("   _/    \\  _-- | _ ____|__   |_  ____________  _|    |_ _  |  | _/_   \\__    . ", 1);cout << endl;
    CH("  /       \\/    |    |___ _     ||/TTTTTTTTTT\\||    __  _| _ __|    \\  /  \\     ", 1);cout << endl;
    CH("          /     |___    |   _ __||            || _ ___|_ _  | _|     .     ..   ", 13);cout << endl;
    CH("   _'..''--___  |               ||            ||               |  ___--''.._    ", 13);cout << endl;
    ChangeColor(9);
    cout << Line << endl;
}

// Displays intro text one character at a time. Any '|'s in the text denote a pause.
void OpeningTextScroll() {
    DisplayOpeningTextGraphic();
    Sleep(1400);
    Scroll("Once upon a time, |a warrior left his wife and child in hopes of overcoming"); cout << endl;
    Scroll("the Monster Arena,| a challenging sequence of battles run by a twisted King.");
    Sleep(1400);
    DisplayOpeningTextGraphic();
    Scroll("He fought marvelously, |swiftly beating every monster until he reached"); cout << endl;
    Scroll("the final fight against the king's prized fighter. ||After many hours, |he slew"); cout << endl;
    Scroll("the creature with a mighty blow.");
    Sleep(1400);
    DisplayOpeningTextGraphic();
    Scroll("The king, |seized by a fit of anger, |cast a magic blast at the warrior that "); cout << endl;
    Scroll("turned him into a mindless beast. ||The king set this beast as his new champion.");
    Sleep(1400);
    DisplayOpeningTextGraphic();
    Scroll("When the King died, |the monsters remained,| but the Monster Arena fell into ruin."); cout << endl;
    Scroll("||No-one ever went back into that arena to free the warrior.");
    Sleep(1400);
    DisplayOpeningTextGraphic();
    Sleep(600);
    Scroll("No-one, |that is, |until his son arrived.");
    Sleep(2800);
    ClearScreenWithoutInput();
    Sleep(800);
    while(kbhit()) getch(); // Clear any keyboard inputs entered while the text was scrolling.
}

// Prints the main menu screen. Each CH() call is changing the color of the text inside it to the entered number (CH defined below).
void MainMenu() {
    ClearScreenWithoutInput();
    ChangeColor(9);
    cout << endl;
    cout << "        .                                         .                .            " << endl;
    CH("                    .          .  ",11);CH("  /  ",12);CH("                         .               ",11);cout << endl;
    CH("           '    ,         .     ", 15);CH("   /", 12);CH("        '   .      +   .         .          ", 15);cout << endl;
    CH("          _   _   _            ", 15);CH("  _/  ", 12);CH("    .         .              ____ T         ", 15);cout << endl;
    CH("         | |_| |_| |         . ", 15);CH("  /\\ _/\\_   ", 12);CH("               .      / +  \\|         ", 15);cout << endl;
    CH("         \\         /   '  +    ", 15);CH("    \\{* }   ", 12);CH("       .            ./______\\         ", 15);cout << endl;
    CH("          |  ___  |           '", 15);CH("     /YY\\\\__", 12);CH(".            .   ,    |__ X|          ", 15);cout << endl;
    CH("        -_| _| |_ |__     .    ", 11);CH("     |==|   |", 12);CH("                   __|||-_|_         ", 11);cout << endl;
    CH("         _-__        \\------- -", 9);CH("     /  \\\\_/ ", 12);CH(".	      +     _-/   _  -           ", 9);cout << endl;
    CH("             \\__    ____\\__-_____-", 9);CH("  |/\\|  ", 12);CH("___  -   --------/ __ /                ", 9);cout << endl;
    CH("                _-_/           ", 1);CH("    <=  =>   ", 12);CH(" \\__|___-_=_                         ", 1);cout << endl;
    cout << "        ---====/                                        \\=======--------        " << endl;
    ChangeColor(9);
    std::cout << Line << endl;
    std::cout << "'m' = Enter the Monster Arena, 'p' = Buy potions, 'd' = Data screen, 's' = Save." << endl;
    std::cout << Line << endl;
    ChangeColor(11);
    std::cout << "Please enter what you want to do: " << endl;

}

// Displays the player's stats screen. Each CH() call is changing the color of the text inside it to the entered number (CH defined below).
void StatsScreen(){
    ClearScreenWithoutInput();
    CH("|                A                |",12);CH("*********************************************",9);cout << endl;
    CH("|           ____/ \\____           |",12);CH("                Player Stats            ",9);cout << endl;
    CH("|  [\\       \\__ ___ __/           |",12);CH("*********************************************",9);cout << endl;
    CH("|  \\ \\        ( *   )             |",12);CH("            Level:            ",6);cout << ThePlayer.GetLevel() << endl;
    CH("|   \\ \\      _.'=-='._            |",12);CH("            Max Health:       ",6);cout << ThePlayer.GetMaxHP() << endl;
    CH("|    \\ \\    /* /YYY\\  \\           |",12);CH("            Attack:           ",6);cout << ThePlayer.GetATK() << endl;
    CH("|     \\ \\__'_/\\-___-/\\ \\__        |",12);CH("            Defense:          ",6);cout << ThePlayer.GetDEF() << endl;
    CH("|      \\/ /// ]=====[ [* |  ]     |",12);CH("            Agility:          ",6);cout << ThePlayer.GetAG() << endl;
    CH("|      |/()   / ___ \\ \\ -+- /     |",12);CH("*********************************************\n",9);
    CH("|            /_/ _ \\_\\ \\_|_/      |",12);CH("            Experience:       ",6);cout << ThePlayer.GetCurrentEXP() << endl;
    CH("|             |__|__|             |",12);CH("            Gold:             ",6);cout << ThePlayer.GetGold() << endl;
    CH("|            _|_ | _|_            |",12);CH("            Defeats:          ",6);cout << DeathCounter << endl;
    CH("|_      - --|*  \\|/   |-- -      _|",12);CH("*********************************************",9);cout << endl;
    cout << Line << endl;
    cout << "Enter 'n' to go back to main menu. " << endl;
    cout << Line << endl;
    ChangeColor(11);
    cout << "Please enter what you want to do: " << endl;
    Stats:
    cin >> UserInput;
    UserInput = toLowerCase(UserInput);
    if (UserInput == "n"){
        MainMenu();
    }else {
        cout << "Invalid entry! Please try again." << endl;
        goto Stats;
    }
}

// Accepts a string to print and a color to print it in and displays the entered string in that color. CH is short for Chunk.
void CH (string Chunk, int Color) {
    ChangeColor(Color);
    cout << Chunk;
}

// Prints text showing the potion shop menu.
void PotionScreen() {
    ClearScreenWithoutInput();
    ChangeColor(14);
    cout << "                                                                                " << endl;
    cout << "     ___                               ___                              ___     " << endl;
    cout << "    |___|                             |___|                            |___|    " << endl;
    ChangeColor(6);
    cout << "     / \\                               / \\                              / \\     " << endl;
    cout << " .  /*  \\    .   .   .   .   .   .    /*  \\    .   .   .   .   .   .   /*  \\  . " << endl;
    ChangeColor(12);
    cout << ".  /     \\  . . . . . . . . . . . .  /     \\  . . . . . . . . . . . . /     \\  ." << endl;
    cout << ".. \\_===_/ ......................... \\_===_/ ........................ \\_===_/ ..." << endl;
    ChangeColor(9);
    cout << Line << endl;
    cout << "You have " << ThePlayer.GetGold() << " Gold in your bag."<< endl;
    cout << "'r' = Red Potion"<< "("<<RedPotionHeal<<" Health)"<<"    ($" << RedPotionCost << ")" << " (" << RedPotionsInBag << " in bag)" << endl;
    cout << "'b' = Blue Potion"<< "("<<BluePotionHeal<<" Health)"<<"   ($" << BluePotionCost << ")" << " (" << BluePotionsInBag << " in bag)" << endl;
    cout << "'p' = Purple Potion"<< "("<<PurplePotionHeal<<" Health)"<<" ($" << PurplePotionCost << ")" << " (" << PurplePotionsInBag << " in bag)" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    ChangeColor(11);
    cout << "Enter the type of potion you want:" << endl;
}

// The in-battle potion inventory.
bool BattlePotionMenu(string PotionColor, int PotionHealAmount, int MonsterNumber) {
    int PotionsInBag;
    if (PotionColor == "Red") {PotionsInBag = RedPotionsInBag;} 
    else if (PotionColor == "Blue") {PotionsInBag = BluePotionsInBag;}
    else{PotionsInBag = PurplePotionsInBag;};
    if (PotionsInBag >= 1){
        if (PotionColor == "Red") {ChangeColor(4);}
        if (PotionColor == "Blue") {ChangeColor(9);}
        if (PotionColor == "Purple") {ChangeColor(13);}
        std::cout << "You used a " << PotionColor << " Potion!" << endl;
        ThePlayer.SetCurrentHP(ThePlayer.GetCurrentHP() + PotionHealAmount);
        //Checking if player current health won't go over max health. If it will, set current health to max health.
        if (ThePlayer.GetCurrentHP() > ThePlayer.GetMaxHP()) {
            ThePlayer.SetCurrentHP(ThePlayer.GetMaxHP());
        }
        std::cout << "You've healed! Current Health: " << ThePlayer.GetCurrentHP() << endl;
            //Decrementing the amount of red potions
        if (PotionColor == "Red") {RedPotionsInBag--;}
        else if (PotionColor == "Blue") {BluePotionsInBag--;}
        else{PurplePotionsInBag--;}
                
        Sleep(1000);
        DisplayStats(MonsterNumber);
        return true;
    } else {
        std::cout << "You don't have any " << PotionColor << " Potions to use! " <<endl;
        return false;
    }
}

// Prints text showing the first arena menu.
void ArenaMenuPage1() {
    ClearScreenWithoutInput();
    ChangeColor(8);
    cout << "        _______________________________________________________________     " << endl;
    cout << "        _______________________________________________________________     " << endl;
    cout << "        ___|___/     \\_____|____|___/     \\_____|____|___/     \\_____|_      " << endl;
    cout << "        _|___/         \\_|____|___/         \\_|____|___/         \\_|___      " << endl;
    cout << "        ___|_|         |___|____|_|         |___|____|_|         |___|_      " << endl;
    cout << "        _|___|         |_|____|___|         |_|____|___|         |_|___     " << endl;
    cout << "        ___|_|  -   -  |___|____|_|  -   -  |___|____|_|  -   -  |___|_     " << endl;
    cout << "        _|___| - - - - |_|____|___| - - - - |_|____|___| - - - - |_|___     " << endl;
    cout << "        ___|_| ------- |___|____|_| ------- |___|____|_| ------- |___|_     " << endl;
    cout << "        -----           ----------           ----------           -----     " << endl;
    cout << "        _______________________________________________________________     " << endl;
    ChangeColor(9);
    cout << Line << endl;
    cout << "'a' = Wooden Gauntlet" << endl;
    if (Arena1Beat == true) {cout << "'b' = Tin Gauntlet" << endl;} else {cout << "'b' = [Locked]" << endl;}
    if (Arena2Beat == true) {cout << "'c' = Bronze Gauntlet" << endl;} else {cout << "'c' = [Locked]" << endl;}
    cout << "'p' = Page ->" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    ChangeColor(11);
    cout << "Please enter the Gauntlet that would you like to challenge: " << endl;
}

// Prints text showing the second arena menu.
void ArenaMenuPage2() {
    ClearScreenWithoutInput();
    ChangeColor(8);
    cout << "        _______________________________________________________________     " << endl;
    cout << "        _______________________________________________________________     " << endl;
    cout << "        ___|___/     \\_____|____|___/     \\_____|____|___/     \\_____|_      " << endl;
    cout << "        _|___/         \\_|____|___/         \\_|____|___/         \\_|___      " << endl;
    cout << "        ___|_|         |___|____|_|         |___|____|_|         |___|_      " << endl;
    cout << "        _|___|         |_|____|___|         |_|____|___|         |_|___     " << endl;
    cout << "        ___|_|  -   -  |___|____|_|  -   -  |___|____|_|  -   -  |___|_     " << endl;
    cout << "        _|___| - - - - |_|____|___| - - - - |_|____|___| - - - - |_|___     " << endl;
    cout << "        ___|_| ------- |___|____|_| ------- |___|____|_| ------- |___|_     " << endl;
    cout << "        -----           ----------           ----------           -----     " << endl;
    cout << "        _______________________________________________________________     " << endl;
    ChangeColor(9);
    cout << Line << endl;
    if (Arena3Beat == true) {cout << "'d' = Silver Gauntlet" << endl;} else {cout << "'d' = [Locked]" << endl;}
    if (Arena4Beat == true) {cout << "'e' = Gold Gauntlet" << endl;} else {cout << "'e' = [Locked]" << endl;}
    if (Arena5Beat == true) {cout << "'f' = Platinum Gauntlet" << endl;} else {cout << "'f' = [Locked]" << endl;}
    if (Arena6Beat == true) {cout << "'g' = LegacyBeast" << endl;}
    cout << "'p' = Page <-" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    ChangeColor(11);
    cout << "Please enter the Gauntlet that would you like to challenge: " << endl;
}

//Function to display Player and Monster Stats: Each CH() call is changing the color of the text inside it to the entered number (CH defined below).
void DisplayStats(int MonNum){
    ClearScreenWithoutInput();
    ChangeColor(11);
    cout << "    o *                                                                   * o " << endl; 
    CH("    .               ", 11);CH(ArrayOfMonsterSprites[MonNum][0], 12);CH("                                  /\\            . ", 15);cout << endl;
    CH("                    ", 11);CH(ArrayOfMonsterSprites[MonNum][1], 12);CH("                                 /* \\             ", 15);cout << endl;
    CH("                    ", 9);CH(ArrayOfMonsterSprites[MonNum][2], 12);CH("                              __/    \\__          ", 15);cout << endl;
    CH("                    ", 9);CH(ArrayOfMonsterSprites[MonNum][3], 12);CH("                             /__________\\         ", 15);cout << endl;
    CH("                    ", 9);CH(ArrayOfMonsterSprites[MonNum][4], 12);CH("                      /\\      (|\\_____/|)         ", 15);cout << endl;
    CH("                    ", 1);CH(ArrayOfMonsterSprites[MonNum][5], 12);CH("                      \\ \\      \\______/           ", 15);cout << endl;
    ChangeColor(11);
    cout << "                 --==========--                    \\ \\     / *YYY* \\          " << endl; 
    ChangeColor(9);
    cout << "                                                    \\ \\   / *       \\         " << endl; 
    ChangeColor(1);
    cout << "          ++..'.          ...         ...  .'.oOo.   \\ \\ / |=======| |        " << endl; 
    ChangeColor(13);
    cout << "   -  - --__--________________---___________________________________--=-_-- - " << endl; 
    if (ThePlayer.GetCurrentHP() <= 5){
        ChangeColor(12);

    }else {
        ChangeColor(9);
    }
    cout << Line << endl;
    cout << "Player Stats: (HP: " << ThePlayer.GetCurrentHP() << "/" <<ThePlayer.GetMaxHP() 
         << ") (EXP: " << ThePlayer.GetCurrentEXP() << ") (Needed EXP: " << ThePlayer.GetEXPToNextLevel() 
         << ") (Gold: " << ThePlayer.GetGold() << ")" << endl;
    cout << Line << endl;
    // Displaying monster health information
    cout << ArrayOfMonsters[MonNum].GetName() <<" Stats: (Current HP: "
         << ArrayOfMonsters[MonNum].GetCurrentHP() << ")"<< endl;
    cout << Line << endl;
}

// The final battle loop. Exits upon victory, defeat, or giving up.
bool FinalBattle() {
    bool PlayerIsAlive = true; // Holds whether to leave the fight loop because of defeat.
    bool MonsterIsAlive = true; // Holds whether to leave the fight loop because of victory.

    // Clears the screen and displays the current round. Sets up the current monster's HP.
    ClearScreenWithoutInput();
    ChangeColor(10);
    std::cout << "Final round: " << ArrayOfMonsters[36].GetName() << endl;
    Sleep(3000);
    ArrayOfMonsters[36].SetCurrentHP(ArrayOfMonsters[36].GetMaxHP());

        // The battle loops while the player is alive. This section displays the player and monster data and the battle graphic.
        while (PlayerIsAlive && MonsterIsAlive) {
            ClearScreenWithoutInput();
            if (ThePlayer.GetCurrentHP() <= 5) {
                ChangeColor(12);
            } else {
                ChangeColor(9);
            }
            DisplayFinalBattle(12);
            Sleep(100);

            // Alternates between calling the PlayerTurn and MonsterTurn functions until one returns false (denoting either victory or defeat).
            if (ArrayOfMonsters[36].GetAG() < ThePlayer.GetAG()) {
                MonsterIsAlive = PlayerTurn(36);
                if (HasRunAway) {
                    break;
                }
                if (MonsterIsAlive) {
                    PlayerIsAlive = MonsterTurn(36);
                } 
            } else {
                PlayerIsAlive = MonsterTurn(36);
                if (PlayerIsAlive) {
                    MonsterIsAlive = PlayerTurn(36);
                    if (HasRunAway) {
                        break;
                    }
                } 
            }
            Sleep(600);
            while(kbhit()) getch(); // Clear any keyboard inputs entered while the battle was looping.
        }

        // Checks if the player lost or gave up and returns false if so.
        MonsterIsAlive = true;
        if (PlayerIsAlive == false || HasRunAway == true) {
            return false;
        }

    // This section displays the final boss's defeat animation in which it flashes different colors at an increasing rate.
    if (PlayerIsAlive && HasRunAway == false) {
        float TimeBetweenColorFlashes = 1.0; // Decreases the time between the final boss's defeated color flashes.
        for (int j = 0; j < 8; j++) {
            for (int i = 1; i < 16; i++) {
                DisplayFinalBattle(i);
                Sleep(600 * (TimeBetweenColorFlashes));
                TimeBetweenColorFlashes -= 0.04;
                if (TimeBetweenColorFlashes <= 0) {
                    TimeBetweenColorFlashes = 0.04;
                }
            } 
        }
        // Display greyed out boss.
        DisplayFinalBattle(8);
        Sleep(200);
        // Displays the battle screen with the father's animation and prints the victory text.
        DisplayFatherAnimation(0);
        Sleep(2000);
        DisplayFatherAnimation(1);
        Sleep(500);
        DisplayFatherAnimation(2);
        Sleep(3000);
        cout << Line << endl;
        ChangeColor(6);
        cout << "You saved your father!" << endl;
        ChangeColor(9);
        cout << Line << endl;
        Sleep(3000);
        return true;
    }
    return false;
}

// Displays the final boss fight and stats. 
void DisplayFinalBattle(int Color) {
    ClearScreenWithoutInput();
    ChangeColor(11);
    cout << " O *                                                                          " << endl;
    CH("                    /\\                   ", Color);CH("                               * o   \n", 11);
    CH("              _-/\\=/* \\=/\\-_             ", Color);CH("                                 .   \n", 11);
    CH("            _/      /\\      \\_           ", Color);CH("                                     \n", 15);
    CH("           /  *   _ /\\ _      \\          ", Color);CH("                     /\\              \n", 15);
    CH("          |*     //    \\\\      |         ", Color);CH("                    /  \\             \n", 15);
    CH("          |  \\_ | \\____/ | _/  |         ", Color);CH("                 __/    \\__          \n", 15);
    CH("         /  |   \\ /*   \\ /   |  \\        ", Color);CH("                /__________\\         \n", 15);
    CH("    _ _='  _\\    \\______/    /  _'=_ _   ", Color);CH("         /\\      (|\\_____/|)         \n", 15);
    CH("   / / / }_/-'=- \\ X  X / -='-\\_{ \\ \\ \\  ", Color);CH("         \\ \\      \\______/           \n", 15);
    CH("   \\|\\|\\|         \\ '' /         |/|/|/  ", Color);CH("          \\ \\     / \\/\\/\\/\\          \n", 11);
    CH("                   |\\/|                  ", Color);CH("           \\ \\   /         \\         \n", 9);
    ChangeColor(1);
    cout << "++..'.                      ...  .'.oOo.   A         \\ \\ / |=======| |      \n";
    ChangeColor(13);
    cout << "   -  - --__--A_______________---_________/ \\_A____A________________--=-_-- - \n";
    if (ThePlayer.GetCurrentHP() <= 5){
        ChangeColor(12);
    }else {
        ChangeColor(9);
    }
    cout << Line << endl;
    cout << "Player Stats: (HP: " << ThePlayer.GetCurrentHP() << "/" <<ThePlayer.GetMaxHP()
         << ") (EXP: " << ThePlayer.GetCurrentEXP() << ") (Needed EXP: " << ThePlayer.GetEXPToNextLevel()
         << ") (Gold: " << ThePlayer.GetGold() << ")" << endl;
    std::cout << Line << endl;
    // Displaying monster health information
    std::cout << ArrayOfMonsters[36].GetName() <<" Stats: (Current HP: "
         << ArrayOfMonsters[36].GetCurrentHP() << ")"<< endl;
    std::cout << Line << endl;
}

// Displays the father's recovery cutscene. 
void DisplayFatherAnimation(int Frame) {
    string ArrayOfFatherLines[7]; // An array holding each father sprite.
    if (Frame == 0) {
        ArrayOfFatherLines[0] = "        ";
        ArrayOfFatherLines[1] = "        ";
        ArrayOfFatherLines[2] = "        ";
        ArrayOfFatherLines[3] = "        ";
        ArrayOfFatherLines[4] = "   __   ";
        ArrayOfFatherLines[5] = " /'  '\\ ";
        ArrayOfFatherLines[6] = "n </\\> n";
    } else if (Frame == 1) {
        ArrayOfFatherLines[0] = "        ";
        ArrayOfFatherLines[1] = "        ";
        ArrayOfFatherLines[2] = "  _/\\_  ";
        ArrayOfFatherLines[3] = " /'v '\\ ";
        ArrayOfFatherLines[4] = "/ \\  / |";
        ArrayOfFatherLines[5] = "| [_[] n";
        ArrayOfFatherLines[6] = "n |/ [_ ";
    } else if (Frame == 2) {
        ArrayOfFatherLines[0] = "  _/\\_  ";
        ArrayOfFatherLines[1] = "  _v _  ";
        ArrayOfFatherLines[2] = " / __ \\ ";
        ArrayOfFatherLines[3] = "| \\  / |";
        ArrayOfFatherLines[4] = "n [__] n";
        ArrayOfFatherLines[5] = "  |/\\|  ";
        ArrayOfFatherLines[6] = " _]  [_ "; 
    }
    ClearScreenWithoutInput();
    ChangeColor(11);
    cout << " O *                                                                          " << endl;
    CH("                                          ", 13);CH("                              * o   ", 11);cout << endl;
    CH("                                          ", 13);CH("                                .   ", 11);cout << endl; ChangeColor(15);
    cout << "                   " << ArrayOfFatherLines[0];CH("                                     ", 15);cout << endl;
    cout << "                   " << ArrayOfFatherLines[1];CH("                                   /\\              ", 15);cout << endl;
    cout << "                   " << ArrayOfFatherLines[2];CH("                                  /  \\             ", 15);cout << endl;
    cout << "                   " << ArrayOfFatherLines[3];CH("                               __/    \\__          ", 15);cout << endl;
    cout << "                   " << ArrayOfFatherLines[4];CH("                              /__________\\         ", 15);cout << endl;
    cout << "                   " << ArrayOfFatherLines[5];CH("                       /\\      (|\\_____/|)         ", 15);cout << endl;
    cout << "                   " << ArrayOfFatherLines[6];CH("                       \\ \\      \\______/           ", 15);cout << endl;
    CH("                                          ", 13);CH("         \\ \\     / \\/\\/\\/\\          ", 11);cout << endl;
    CH("                                          ", 13);CH("          \\ \\   /         \\         ", 9);cout << endl;
    ChangeColor(1);
    cout << "++..'.                      ...  .'.oOo.   A         \\ \\ / |=======| |      " << endl;
    ChangeColor(13);
    cout << "   -  - --__--A_______________---_________/ \\_A____A________________--=-_-- - " << endl;
    if (ThePlayer.GetCurrentHP() <= 5){
        ChangeColor(12);
    }else {
        ChangeColor(9);
    }
    cout << Line << endl;
    cout << "Player Stats: (HP: " << ThePlayer.GetCurrentHP() << "/" <<ThePlayer.GetMaxHP()
         << ") (EXP: " << ThePlayer.GetCurrentEXP() << ") (Needed EXP: " << ThePlayer.GetEXPToNextLevel()
         << ") (Gold: " << ThePlayer.GetGold() << ")" << endl;
    std::cout << Line << endl;
    // Displaying monster health information
    std::cout << ArrayOfMonsters[36].GetName() <<" Stats: (Current HP: "
         << ArrayOfMonsters[36].GetCurrentHP() << ")"<< endl;
    std::cout << Line << endl;
}

void ShowFatherAndSon() {
    ClearScreenWithoutInput();
    ChangeColor(15);
    cout << "\n\n\n\n\n\n";
    cout << "\t\t\t\t    u   _/\\_             \n";
    cout << "\t\t\t\t     \\___v _      _/\\_  o\n";
    cout << "\t\t\t\t       | __ \\     _()__/ \n";ChangeColor(11);
    cout << "\t\t\t\t        \\  / |   /\\__/   \n";ChangeColor(9);
    cout << "\t\t\t\t        [__] n  o /  \\   \n";ChangeColor(1);
    cout << "\t\t\t\t        |/\\|      |/\\|   \n";ChangeColor(13);
    cout << "\t\t\t\t       _]  [_    _=  =_  \n\n";ChangeColor(9);
    
    cout << "--_-_--''-'-==--'-':-----'---'--^-----==----'-||-'--'----^----'-_--===----'---=^-'-_-'---'--:-\n" << endl;

    cout << "\t\t\t";Scroll("Program Managers:              Felipe Krainer\n");
    cout << "\t\t\t";Scroll("                               Samuel Johnson\n\n");
    Sleep(200);
    cout << "\t\t\t";Scroll("Programming:                   Felipe Krainer\n");
    cout << "\t\t\t";Scroll("                               Samuel Johnson\n\n");
    Sleep(200);
    cout << "\t\t\t";Scroll("Art Director:                  Samuel Johnson\n\n\n");
    Sleep(200);
    cout << "\t\t\t     ";Scroll("Thanks for playing!");
    Sleep(200);
    Scroll(" You're the best!\n\n");
}

void DisplaySunrise() {
    IncorrectInput:
    ClearScreenWithoutInput();
    while(kbhit()) getch(); // Clear any keyboard inputs entered while the Sleep() function was active.
    ChangeColor(15); 
    cout << "\n                         _________  _    _    ______                            " << endl;
    cout << "                         \\__   __/ | |  | |  |  ____|                           " << endl;
    cout << "                            | |    | |__| |  | |__                              " << endl;
    cout << "                            | |    | |  | |  | |____                            " << endl;
    ChangeColor(14); 
    cout << "                            |_|    |_|  |_|  |______|                           " << endl;
    cout << "                          ______    __    _    _____                            " << endl;
    cout << "                         |  ____|  |  \\  | |  |  _  \\                           " << endl;
    cout << "                         | |__     |   \\_| |  | | \\  \\                          " << endl;
    CH("  ''''===---___          ", 9);CH("|  __|    |  _    |  | | [  |", 14);CH("         ___---===''''    ", 9); cout << endl;
    CH("           .__ \\         ", 9);CH("| |____   | | \\   |  | |_/  /", 6);CH("        / __. ___.        ", 9); cout << endl;
    CH("              \\ \\_____   ", 1);CH("|______|  |_|  \\__|  |_____/ ", 6);CH("  _____/ /                ", 1); cout << endl;
    CH("                  .__ \\__ ", 13);CH("                          ", 6);CH(" __/ __.                    ", 13); cout << endl;
    CH("                     \\   \\", 13);CH(" - --=-============-=-- - ", 15);CH("/___/                       ", 13); cout << endl;
    CH("                 _ _ _\\   ", 13);CH("  - ------------------- - ", 11);CH("   /_ _ _                   ", 13); cout << endl;
    CH("            ' ' '         ", 13);CH("       - --------- -      ", 9);CH("          ' ' '             ", 13); cout << endl;
    ChangeColor(1);
    cout << "                                     - - -                                      " << endl;
    ChangeColor(13);
    cout << "                                       _                                        " << endl;
    ChangeColor(9);
    cout << "                 _____     _..__     _____     __.__     __.._                  " << endl;
    ChangeColor(1);
    cout << "      .  . . .-='     '==='     '==='     '==='     '==='     '=-. . .  .       " << endl;
    ChangeColor(13);
    cout << "             ..   .:.                                  .:..      .. .           \n" << endl;
    ChangeColor(9);
    cout << Line << endl;
    ChangeColor(11);
    cout << "Enter 'c' to close the game. " << endl;
    cin >> UserInput;
    UserInput = toLowerCase(UserInput);
    if (UserInput != "c"){
        goto IncorrectInput;
    }
}

// Validates input compared to different characters. Overloaded.
bool Validate(string Input, string Option1, string Option2, string Option3, string Option4, string Option5, string Option6) {
    return (Input == Option1 || Input == Option2 || Input == Option3 || Input == Option4 || Input == Option5 || Input == Option6);
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