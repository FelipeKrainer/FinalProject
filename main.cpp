// Link command: g++ main.cpp Monster.cpp Player.cpp -o TurnBasedBattleSystem.exe
// Run in the Command Prompt in the folder containing the "Monster.h", "Monster.cpp", and "TurnBasedBattleSystem.cpp" files.

/* Still to be added:
    In-battle potion use ✓
    In-battle running ✓
    Miss chance for player and monsters ✓
    Variable damage ✓
    Menu graphic ✓
    Battle graphic
    Eighteen monster data entries ✓
    Balancing difficulty
    Save Player values and relevant in-game variables to a text file. ✓
    Load Player values and relevant in-game variables from a text file.
    
*/

#include "Monster.h"
#include "Player.h"
#include <iostream>
#include <random>
#include <windows.h>
#include <fstream>
#include <cctype>

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

std::random_device Rand;
std::mt19937 Generator(Rand());
std::uniform_int_distribution<int> Distribution(1, 20);
std::uniform_int_distribution<int> DamageDistribution(0, 3);

int const AMOUNT_OF_MONSTERS = 36;                                                                       // The amount of monsters in the game.
string QUIT = "99";                                                                                      // Used to quit the game when asked.
string UserInput = "";                                                                                   // Used to decide what action to take in the menus.
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
bool HasRunAway = false;                                                                                 // Holds whether the player has given up on an arena.

bool Arena1Beat = false;                                                                                 // Whether the player has beat a given arena.
bool Arena2Beat = false;                                                                                 // -
bool Arena3Beat = false;                                                                                 // -
bool Arena4Beat = false;                                                                                 // -
bool Arena5Beat = false;                                                                                 // -

// ---------------------------------------------------------------------- Function Prototypes:

void Arena();                                                   // The main battle scene.
bool MonsterTurn(int);                                          // Computes the monster's damage to the player.
bool PlayerTurn(int);                                           // Computes the players damage to the monster.

void DisplayStats(int);                                         // Displays Player and Monster stats.
std::string toLowerCase(const std::string &str);                // Changes user input to lower case.
void SaveToFile(const std::string& fileName);                   // Saves the player stats
void LoadFromFile(const std::string& fileName);
void SaveScreen();                                              // Save Screen
void FillArrayOfMonsterObjects();                               // Creates an array of Monster objects.
void FillArrayOfMonsterSprites();                               // Fills the ArrayOfMonsterSprites with data from the MonsterSprites.txt file.
void ClearScreen();                                             // Clears text from the screen. Waits for input to do so.
void ClearScreenWithoutInput();                                 // Clears text from the screen. Activates when called.
void ChangeColor(int);                                          // Changes the text color.
void Scroll(string);                                            // Prints text one character at a time.
void SwordAnimation1();                                         // Title animation.
void SwordAnimation2();                                         // Title animation.
void Introduction();                                            // Text introducing the game.
void MainMenu();                                                // The main menu image and text.
void CH(string, int);                                           // Prints the entered chunk of text in the entered color.
void PotionScreen();                                            // Displays the potion shop.
void BattlePotionMenu(string, int, int);                        // Displays the in-battle potion menu.
void ArenaMenuPage1();                                          // Displays Arena options menu 1.
void ArenaMenuPage2();                                          // Displays Arena options menu 2.

bool Validate(string, string, string, string, string, string);  // Validate input against five options.
bool Validate(string, string, string, string, string);          // Validate input against four options.
bool Validate(string, string, string, string);                  // Validate input against three options.
bool Validate(string, string, string);                          // Validate input against two options.

// ---------------------------------------------------------------------- Main Program:

int main() {
    FillArrayOfMonsterObjects();
    FillArrayOfMonsterSprites();
    HasRunAway = false;
    Introduction();
    std::cout << "Press Enter to begin..." << endl;
    std::cin.get();
    SaveScreen:
    SaveScreen();
    cin >> UserInput;
    UserInput = toLowerCase(UserInput);
    Validated = Validate(UserInput, "c", "s");
    if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto SaveScreen;}
    if (UserInput == "c"){
        LoadFromFile("player_data.txt");
        Sleep(1200);
        ClearScreenWithoutInput();
    }else {
        ClearScreenWithoutInput();
    }
    MainMenu: 
    ClearScreen();
    ThePlayer.SetCurrentHP(ThePlayer.GetMaxHP());


    ChangeColor(11);
    MainMenu();
    MainMenuInput:                                                                                                  // The main menu option box.
    cin >> UserInput;
    UserInput=  toLowerCase(UserInput);
    Validated = Validate(UserInput, "g", "p", "s");
    // Repeats the question if input is incorrect.
    if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto MainMenuInput;} 
    if (UserInput == "p") {                                                                                         //Chose to buy potions.
        PotionMenu:           
        ClearScreen();
        PotionScreen();
        PotionMenuInput:
        cin >> UserInput;
        UserInput=  toLowerCase(UserInput);
        Validated = Validate(UserInput, "r", "b", "p", "n");
        if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto PotionMenuInput;}
        if (UserInput == "r") {                                                                                     // Chose to buy Red Potion.
            if (ThePlayer.GetGold() < RedPotionCost) {std::cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(RedPotionCost);
            RedPotionCost *= 1.2;
            RedPotionsInBag += 1;
            goto PotionMenu;
        } else if (UserInput == "b") {                                                                              // Chose to buy Blue Potion.
            if (ThePlayer.GetGold() < BluePotionCost) {std::cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(BluePotionCost);
            BluePotionCost *= 1.2;
            BluePotionsInBag += 1;
            goto PotionMenu;
        } else if (UserInput == "p") {                                                                              // Chose to buy Purple Potion.
            if (ThePlayer.GetGold() < PurplePotionCost) {std::cout << "You do not have enough gold to buy that!" << endl; goto PotionMenuInput;}
            ThePlayer.LowerGold(PurplePotionCost);
            PurplePotionCost *= 1.2;
            PurplePotionsInBag += 1;
            goto PotionMenu;
        } else {                                                                                                    // Chose to leave potion shop.
            goto MainMenu;
        }
    } else if (UserInput == "g"){                                                                                                        // Chose to enter an arena.
        ArenaMenu:                                                                                              
        ArenaMenuPage1();
        ArenaInputPg1:                                                                                              // Arena menu page 1.
        cin >> UserInput;
        UserInput=  toLowerCase(UserInput);
        Validated = Validate(UserInput, "a", "b", "c", "p", "n");
        if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto ArenaInputPg1;} 
        if (UserInput == "a") {                                                                                     // Chose Arena 1.
            CurrentArena = 1;
            Arena();
        } else if (UserInput == "b") {                                                                              // Chose Arena 2.
            if (Arena1Beat == true) {
                CurrentArena = 2;
                Arena();
            } else {
                std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg1;
            }
        } else if (UserInput == "c") {                                                                              // Chose Arena 3.
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
            UserInput=  toLowerCase(UserInput);
            Validated = Validate(UserInput, "d", "e", "f", "p", "n");
            if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto ArenaInputPg2;}
            if (UserInput == "d") {                                                                                 // Chose Arena 4.
                if (Arena3Beat == true) {
                    CurrentArena = 4;
                    Arena();
                } else {
                    std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg2;
                }
            } else if (UserInput == "e") {                                                                          // Chose Arena 5.
                if (Arena4Beat == true) {
                    CurrentArena = 5;
                    Arena();
                } else {
                    std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg2;
                }
            } else if (UserInput == "f") {                                                                          // Chose Arena 6.
                if (Arena5Beat == true) {
                    CurrentArena = 6;
                    Arena();
                } else {
                    std::cout << "That Gauntlet is locked! Beat the previous Gauntlet to unlock it." << endl; goto ArenaInputPg2;
                }
            }
            else if (UserInput == "p") {goto ArenaMenu;}
            else {goto MainMenu;}
        } else {goto MainMenu;}
    } else {
        SaveToFile("player_data.txt");
        Sleep(800);

        goto MainMenu;
    }
    
    std::cout << "Press Enter to exit...";
    std::cin.get();  // Wait for the user to press Enter
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
                    << Arena1Beat << ' '
                    << Arena2Beat << ' '
                    << Arena3Beat << ' '
                    << Arena4Beat << ' '
                    << Arena5Beat;

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
                   >> RedPotionCost >> BluePotionCost >> PurplePotionCost >> Arena1Beat
                   >> Arena2Beat >> Arena3Beat >> Arena4Beat >> Arena5Beat;

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
        }
    }

// The main arena system.
void Arena() {
    bool PlayerIsAlive = true; 
    bool PlayerIsDead = false; 
    bool MonsterIsAlive = true;
    int CurrentMonster = (CurrentArena - 1) * 6;
    int CurrentRound = 1;
    for (int i = CurrentMonster; i < CurrentMonster + 6; i++) {
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
            Sleep(100);
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
        switch (CurrentArena) {
            case 1: Arena1Beat = true; break;
            case 2: Arena2Beat = true; break;
            case 3: Arena3Beat = true; break;
        }
        cout << Line << endl;
        Sleep(3000);
    }
}

// The monster's attack. Computes damage with MonsterATK - PlayerDEF. Also checks for game over.
bool MonsterTurn(int MonsterNumber) {
    int MissChance = 0;
    MissChance = Distribution(Generator);
    if (MissChance != 20) {
        int DamageDealt;
        DamageDealt = (ArrayOfMonsters[MonsterNumber].GetATK() - ThePlayer.GetDEF()) + DamageDistribution(Generator);
        if (DamageDealt <= 0) {
            DamageDealt = 1;
        }
        ChangeColor(12);
        cout << ArrayOfMonsters[MonsterNumber].GetName() << " "
            << ArrayOfMonsters[MonsterNumber].GetMethodOfAttack()
            << " you for " << DamageDealt << " damage!" << endl;
        ThePlayer.LowerCurrentHP(DamageDealt);
        Sleep(200);
        if (ThePlayer.GetCurrentHP() <= 0) {
            ChangeColor(12);
            cout << "Too bad! You've lost the Gauntlet!" << endl;
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
        MissChance = Distribution(Generator);
        if (MissChance != 20) {

            int DamageDealt;
            DamageDealt = (ThePlayer.GetATK() - ArrayOfMonsters[MonsterNumber].GetDEF()) + DamageDistribution(Generator);
            if (DamageDealt <= 0) {
                DamageDealt = 1;
            }
            ChangeColor(9);
            cout << "You hit the enemy for " << DamageDealt << " damage!" << endl;
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
        PotionInventoryInput:
        std::cout << "Enter the potion you want to use ('r', 'b', 'p') or 'n' to go back: " << endl;
        cin >> UserInput;
        UserInput=  toLowerCase(UserInput);
        Validated = Validate(UserInput, "r", "b", "p", "n");
        if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto PotionInventoryInput;}
        
        //Creating logic of the Red Potion
        if (UserInput == "r"){
            BattlePotionMenu("Red", RedPotionHeal, MonsterNumber);
        //Creating the logic of the Blue Potion
        } else if (UserInput == "b"){
            BattlePotionMenu("Blue", BluePotionHeal, MonsterNumber);
        //Creating the logic of the Purple Potion
        } else if (UserInput == "p"){
            BattlePotionMenu("Purple", PurplePotionHeal, MonsterNumber);
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
    string Text = "";
    for (int i = 0; i < AMOUNT_OF_MONSTERS; i++) {
        for (int j = 0; j < 6; j++) {
            getline(File, Text);
            ArrayOfMonsterSprites[i][j] = Text;

        }
    }
    File.close();
}

// Clears the screen using different commands depending on the system the program is run on. Requires player to press enter.
void ClearScreen() {
    cin.get();
    if (OS == "Windows") {
        system("cls");  // Clear the screen
    } else if (OS == "Mac OS" || OS == "Linux") {
        system("clear");
    }
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
        cout << Text[i];
        Sleep(1);
    }
    cout << endl;
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
    cout << "Defence. You'll have to beat all [NUMBER] gauntlets to become the Champion." << endl;
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
    cout << "Defence. You'll have to beat all [NUMBER] gauntlets to become the Champion." << endl;
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

// Prints the main menu screen. Each CH() call is changing the color of the text inside it (CH defined below).
void MainMenu() {
    // ...Yes, it's gross looking...
    // ...Yes, it's probably not done with coding 'best practices'...
    // ...Yes, I might lose points for this function... 
    // But I still like how it outputs to the screen.
    ChangeColor(9);
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
    std::cout << "Enter whether you want to start a Gauntlet ('g'), buy potions ('p'), or save ('s'): " << endl;
    std::cout << Line << endl;

}

// Displays the starting menu screen.
void SaveScreen (){
    ClearScreenWithoutInput();
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
}

// Accepts a string to print and a color to print it in.
void CH (string Chunk, int Color) {
    ChangeColor(Color);
    cout << Chunk;
}

// Prints text showing the potion shop menu.
void PotionScreen() {
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
    cout << "'r' = Red Potion    ($" << RedPotionCost << ")" << " (" << RedPotionsInBag << " in bag)" << endl;
    cout << "'b' = Blue Potion   ($" << BluePotionCost << ")" << " (" << BluePotionsInBag << " in bag)" << endl;
    cout << "'p' = Purple Potion ($" << PurplePotionCost << ")" << " (" << PurplePotionsInBag << " in bag)" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    ChangeColor(11);
    cout << "Enter the type of potion you want:" << endl;
}

void BattlePotionMenu(string PotionColor, int PotionHealAmount, int MonsterNumber) {
    int PotionsInBag;
    if (PotionColor == "Red") {PotionsInBag = RedPotionsInBag;} 
    else if (PotionColor == "Blue") {PotionsInBag = BluePotionsInBag;}
    else{PotionsInBag = PurplePotionsInBag;};
    if (PotionsInBag >= 1){
        ChangeColor(4);
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
    } else {
        std::cout << "You don't have any " << PotionColor << " Potions to use! " <<endl;
        Sleep(1000);
        DisplayStats(MonsterNumber);
        PlayerTurn(MonsterNumber);
    }
}

// Prints text showing the first arena menu.
void ArenaMenuPage1() {
    ClearScreen();
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
    cout << "'a' = Wooden Arena" << endl;
    if (Arena1Beat == true) {cout << "'b' = Tin Arena" << endl;} else {cout << "'b' = [Locked]" << endl;}
    if (Arena2Beat == true) {cout << "'c' = Bronze Arena" << endl;} else {cout << "'c' = [Locked]" << endl;}
    cout << "'p' = Page ->" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    ChangeColor(11);
    cout << "Please enter the Gauntlet that would you like to challenge: " << endl;
}

// Prints text showing the second arena menu.
void ArenaMenuPage2() {
    ClearScreen();
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
    if (Arena3Beat == true) {cout << "'d' = Silver Arena" << endl;} else {cout << "'d' = [Locked]" << endl;}
    if (Arena4Beat == true) {cout << "'e' = Gold Arena" << endl;} else {cout << "'e' = [Locked]" << endl;}
    if (Arena5Beat == true) {cout << "'f' = Platinum Arena" << endl;} else {cout << "'f' = [Locked]" << endl;}
    cout << "'p' = Page <-" << endl;
    cout << "'n' = Nevermind..." << endl;
    cout << Line << endl;
    ChangeColor(11);
    cout << "Please enter the Gauntlet that would you like to challenge: " << endl;
}

//Function to display Player and Monster Stats:
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

// Validates input compared to different characters. Overloaded.
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
