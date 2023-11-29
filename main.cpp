// Link command: g++ main.cpp Monster.cpp Player.cpp -o TurnBasedBattleSystem.exe
// Run in the Command Prompt in the folder containing the "Monster.h", "Monster.cpp", and "TurnBasedBattleSystem.cpp" files.

/* Still to be added:
    In-battle potion use
    In-battle running
    Miss chance for player and monsters
    Variable damage
    Battle graphic
    Eighteen monster data entries
    Balancing difficulty
    Save Player values and relevant in-game variables to a text file.
    Load Player values and relevant in-game variables from a text file.
    
*/

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
string Text = "";                                                                                        // Used to hold text to be printed one character at a time.

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

void Arena();                                                   // The main battle scene.
bool MonsterTurn(int);                                          // Computes the monster's damage to the player.
bool PlayerTurn(int);                                           // Computes the players damage to the monster.

void DisplayStats(); //testing

void FillArrayOfMonsterObjects();                               // Creates an array of Monster objects.
void ClearScreen();                                             // Clears text from the screen. Waits for input to do so.
void ClearScreenWithoutInput();                                 // Clears text from the screen. Activates when called.
void ChangeColor(int);                                          // Changes the text color.
void Scroll(string);                                            // Prints text one character at a time.
void SwordAnimation1();                                         // Title animation.
void SwordAnimation2();                                         // Title animation.
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
    std::cout << "Press Enter to begin..." << endl;
    MainMenu: 
    ClearScreen();
    ChangeColor(11);
    std::cout << Line << endl;
    std::cout << "Enter whether you want to start a Gauntlet ('g'), or buy potions ('p'): " << endl;
    std::cout << Line << endl;
    MainMenuInput:                                                                                                  // The main menu option box.
    cin >> UserInput;
    Validated = Validate(UserInput, "g", "p");
    // Repeats the question if input is incorrect.
    if (!Validated) {std::cout << "Invalid entry! Please try again!" << endl; goto MainMenuInput;} 
    if (UserInput == "p") {                                                                                         //Chose to buy potions.
        PotionMenu:           
        ClearScreen();
        PotionScreen();
        PotionMenuInput:
        cin >> UserInput;
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
    } else {                                                                                                        // Chose to enter an arena.
        ArenaMenu:                                                                                              
        ArenaMenuPage1();
        ArenaInputPg1:                                                                                              // Arena menu page 1.
        cin >> UserInput;
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
    }
    
    std::cout << "Press Enter to exit...";
    std::cin.get();  // Wait for the user to press Enter
    return 0;
}

// ---------------------------------------------------------------------- Function Definitions:



void DisplayStats(){
    ClearScreenWithoutInput();
    if (ThePlayer.GetCurrentHP() <= 5){
        ChangeColor(12);

    }else {
        ChangeColor(9);
    }
    cout << "Player Stats: " << endl;
            cout << "(HP: " << ThePlayer.GetCurrentHP() << "/MaxHP: " <<ThePlayer.GetMaxHP() << ") (EXP: " << ThePlayer.GetCurrentEXP() 
                 << ") (Needed EXP: " << ThePlayer.GetEXPToNextLevel() << ") (Gold: " 
                 << ThePlayer.GetGold() << ")" << endl;
            cout << Line << endl;
}

void DisplayMonsterStats(int MonsterNumber) {
    
    // Displaying monster health information
    cout << ArrayOfMonsters[MonsterNumber].GetName() <<" Stats: " << endl;
            cout << "(Current HP: " << ArrayOfMonsters[MonsterNumber].GetCurrentHP() << ")"<< endl;
            cout << Line << endl;
}





// The main arena system.
void Arena() {
    bool PlayerIsAlive = true; 
    bool PlayerIsDead = false; 
    bool MonsterIsAlive = true;
    int CurrentMonster = (CurrentArena - 1) * 6;
    for (int i = CurrentMonster; i < CurrentMonster + 6; i++) {
        ClearScreenWithoutInput();
        ChangeColor(10);
        cout << "Round " << i + 1 << ": " << ArrayOfMonsters[i].GetName() << endl;
        Sleep(3000);
        ArrayOfMonsters[i].SetCurrentHP(ArrayOfMonsters[i].GetMaxHP());

        while (PlayerIsAlive && MonsterIsAlive) {
            ClearScreenWithoutInput();
            if (ThePlayer.GetCurrentHP() <= 5) {
                ChangeColor(12);
            } else {
                ChangeColor(9);
            }
            cout << "Player Stats: " << endl;
            cout << "(HP: " << ThePlayer.GetCurrentHP() << "/MaxHP: " <<ThePlayer.GetMaxHP() << ") (EXP: " << ThePlayer.GetCurrentEXP() 
                 << ") (Needed EXP: " << ThePlayer.GetEXPToNextLevel() << ") (Gold: " 
                 << ThePlayer.GetGold() << ")" << endl;
            cout << Line << endl;
            //Displaying monster health information
            cout << ArrayOfMonsters[i].GetName() <<" Stats: " << endl;
            cout << "(Current HP: " << ArrayOfMonsters[i].GetCurrentHP() << ")"<< endl;
            cout << Line << endl;

            Sleep(100);
            if (ArrayOfMonsters[i].GetAG() < ThePlayer.GetAG()) {
                MonsterIsAlive = PlayerTurn(i);
                if (MonsterIsAlive) {
                    PlayerIsAlive = MonsterTurn(i);
                } 
            } else {
                PlayerIsAlive = MonsterTurn(i);
                if (PlayerIsAlive) {
                    MonsterIsAlive = PlayerTurn(i);
                } 
            }
            
        }
        MonsterIsAlive = true;
        if (PlayerIsAlive == false) {
            break;
        }
    }
    if (PlayerIsAlive) {
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
    int DamageDealt;
    DamageDealt = ArrayOfMonsters[MonsterNumber].GetATK() - ThePlayer.GetDEF();
    if (DamageDealt <= 0) {
        DamageDealt = 1;
    }
    ChangeColor(12);
    cout << ArrayOfMonsters[MonsterNumber].GetName() << " "
         << ArrayOfMonsters[MonsterNumber].GetMethodOfAttack()
         << " you for " << DamageDealt << " damage!" << endl;
    ThePlayer.LowerCurrentHP(DamageDealt);
    Sleep(2000);
    if (ThePlayer.GetCurrentHP() <= 0) {
        ChangeColor(12);
        cout << "Too bad! You've lost the Gauntlet!" << endl;
        Sleep(800);
        ThePlayer.SetCurrentHP(ThePlayer.GetMaxHP());
        return false;
    } else {
        return true;
    }
}

// The player's turn in which they can choose to attack, use a potion, or give up. Also checks for victory.
bool PlayerTurn(int MonsterNumber) {
    PlayerBattleChoice:
    ChangeColor(11);
    cout << "Enter an action: ('a' = ATTACK, 'p' = POTION, 'g' = GIVE UP)" << endl;
    cin >> UserInput;
    Validated = Validate(UserInput, "a", "p", "g");
    if (!Validated) {cout << "Invalid entry! Please try again!" << endl; goto PlayerBattleChoice;}
    if (UserInput == "a") {
        int DamageDealt;
        DamageDealt = ThePlayer.GetATK() - ArrayOfMonsters[MonsterNumber].GetDEF();
        if (DamageDealt <= 0) {
            DamageDealt = 1;
        }
        ChangeColor(9);
        cout << "You hit the enemy for " << DamageDealt << " damage!" << endl;
        ArrayOfMonsters[MonsterNumber].LowerCurrentHP(DamageDealt);
        Sleep(600);
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
    } else if (UserInput == "p") {
        //Creating Variables of the potion values
        int RedPotionHeal = 10;
        int BluePotionHeal = 20;
        int PurplePotionHeal = 30;

        //Setting up new health value
        int newHealth;

        //Creating User Input variable
        string ChosenPotion = "";
        //Displaying current number of potions
        PotionInventory:
        ClearScreenWithoutInput();
        ChangeColor(11);
        cout << Line << endl;
        cout << "Potion Inventory" << endl;
        cout << Line << endl;
        ChangeColor(4);
        cout << "You have " << RedPotionsInBag << " Red Potions." << endl;
        ChangeColor(9);
        cout << "You have " << BluePotionsInBag << " Blue Potions." << endl;
        ChangeColor(5);
        cout << "You have " << PurplePotionsInBag << " Purple Potions." << endl;
        ChangeColor(11);
        cout << Line << endl;
        cout << "Enter the potion you want to use ('r', 'b', 'p') or 'n' to go back: " << endl;
        cin >> ChosenPotion;
        
        //Creating logic of the Red Potion
        if (ChosenPotion == "r"){
            //Checking if player has potions to use
            if (RedPotionsInBag >= 1){
                ChangeColor(4);
                cout << "You used a red potion" << endl;
                newHealth = ThePlayer.GetCurrentHP() + RedPotionHeal;
                //Checking if player current health won't go over max health. If it will, set current health to max health.
                if (newHealth > ThePlayer.GetMaxHP()){
                    ThePlayer.SetCurrentHP(ThePlayer.GetMaxHP());
                    cout << "You've healed! Current Health: " << ThePlayer.GetCurrentHP() << endl;
                    //Decrementing the amount of red potions
                    RedPotionsInBag--;
                    Sleep(1000);
                    DisplayStats();
                    DisplayMonsterStats(MonsterNumber);
                    
                    

                }
                else {
                    //Setting new value for current health
                    ThePlayer.SetCurrentHP(ThePlayer.GetCurrentHP() + RedPotionHeal);
                    cout << "You've healed! Current Health: " << ThePlayer.GetCurrentHP() << endl;
                    //Decrementing the amount of red potions
                    RedPotionsInBag--;
                    Sleep(1000);
                    DisplayStats();
                    DisplayMonsterStats(MonsterNumber);
                    
                    
                    
                }

            }else {
                cout << "You don't have red potions to use: " <<endl;
                Sleep(1000);
                DisplayStats();
                DisplayMonsterStats(MonsterNumber);
                
            }
            

        }
        //Creating the logic of the Blue Potion
        else if (ChosenPotion == "b"){
            //Checking if player has potions to use
            if (BluePotionsInBag >= 1){
                ChangeColor(9);
                cout << "You used a blue potion" << endl;
                newHealth = ThePlayer.GetCurrentHP() + BluePotionHeal;
                //Checking if player current health won't go over max health. If it will, set current health to max health.
                if (newHealth > ThePlayer.GetMaxHP()){
                    ThePlayer.SetCurrentHP(ThePlayer.GetMaxHP());
                    cout << "You've healed! Current Health: " << ThePlayer.GetCurrentHP() << endl;
                    //Decrementing the amount of Blue potions
                    BluePotionsInBag--;
                    Sleep(1000);
                    DisplayStats();
                    DisplayMonsterStats(MonsterNumber);
                    
                    

                }
                else {
                    //Setting new value for current health
                    ThePlayer.SetCurrentHP(ThePlayer.GetCurrentHP() + BluePotionHeal);
                    cout << "You've healed! Current Health: " << ThePlayer.GetCurrentHP() << endl;
                    //Decrementing the amount of Blue potions
                    BluePotionsInBag--;
                    Sleep(1000);
                    DisplayStats();
                    DisplayMonsterStats(MonsterNumber);
                    
                    
                    
                }

            }else {
                cout << "You don't have blue potions to use: " <<endl;
                Sleep(1000);
                DisplayStats();
                DisplayMonsterStats(MonsterNumber);
                
            }
            
        }
        //Creating the logic of the Purple Potion
        else if (ChosenPotion == "p"){
            //Checking if player has potions to use
            if (PurplePotionsInBag >= 1){
                ChangeColor(5);
                cout << "You used a purple potion" << endl;
                newHealth = ThePlayer.GetCurrentHP() + PurplePotionHeal;
                //Checking if player current health won't go over max health. If it will, set current health to max health.
                if (newHealth > ThePlayer.GetMaxHP()){
                    ThePlayer.SetCurrentHP(ThePlayer.GetMaxHP());
                    cout << "You've healed! Current Health: " << ThePlayer.GetCurrentHP() << endl;
                    //Decrementing the amount of Purple potions
                    PurplePotionsInBag--;
                    Sleep(1000);
                    DisplayStats();
                    DisplayMonsterStats(MonsterNumber);
                    
                    

                }
                else {
                    //Setting new value for current health
                    ThePlayer.SetCurrentHP(ThePlayer.GetCurrentHP() + PurplePotionHeal);
                    cout << "You've healed! Current Health: " << ThePlayer.GetCurrentHP() << endl;
                    //Decrementing the amount of Purple potions
                    PurplePotionsInBag--;
                    Sleep(1000);
                    DisplayStats();
                    DisplayMonsterStats(MonsterNumber);
                    
                    
                    
                }

            }else {
                cout << "You don't have purple potions to use: " <<endl;
                Sleep(1000);
                DisplayStats();
                DisplayMonsterStats(MonsterNumber);
                
            }
            
            
        }
        else if (ChosenPotion == "n"){
            Sleep(500);
            DisplayStats();
            DisplayMonsterStats(MonsterNumber);
            goto PlayerBattleChoice;
            
            

        }


        return true;
    } 
    else {
        main();
        return false;
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

// The first frame of the title screen.
void SwordAnimation1() {
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
}

// The second frame of the title screen.
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
