#include "Player.h"
#include <iostream>
#include <random>
#include <windows.h>

using namespace std;

std::random_device Random;
std::mt19937 generator(Random());
std::uniform_int_distribution<int> distribution(1, 3);

void Player::Print() {
        cout << "Player Level: " << Level << endl;
        cout << "Player Max HP: " << MaxHP << endl;
        cout << "Player Current HP: " << CurrentHP << endl;
        cout << "Player ATK: " << ATK << endl;
        cout << "Player DEF: " << DEF << endl;
        cout << "Player AG: " << AG << endl;
        cout << "Current EXP: " << CurrentEXP << endl;
        cout << "Required EXP: " << EXPToNextLevel << endl;
        cout << "Player Gold: " << Gold << endl;
}

int Player::GetLevel() const {
        return Level;
}

int Player::GetMaxHP() const {
        return MaxHP;
}

int Player::GetCurrentHP() const {
        return CurrentHP;
}

int Player::GetATK() const {
        return ATK;
}

int Player::GetDEF() const {
        return DEF;
}

int Player::GetAG() const {
        return AG;
}

int Player::GetCurrentEXP() const {
        return CurrentEXP;
}

int Player::GetEXPToNextLevel() const {
        return EXPToNextLevel;
}

int Player::GetGold() const {
        return Gold;
}

void Player::SetLevel(int EnteredLevel) {
        Level = EnteredLevel;
}

void Player::SetMaxHP(int EnteredHP) {
        MaxHP = EnteredHP;
}

void Player::SetCurrentHP(int EnteredHP) {
        CurrentHP = EnteredHP;
}

void Player::LowerCurrentHP(int EnteredHP) {
        CurrentHP -= EnteredHP;
}

void Player::SetATK(int EnteredATK) {
        ATK = EnteredATK;
}

void Player::SetDEF(int EnteredDEF) {
        DEF = EnteredDEF;
}

void Player::SetAG(int EnteredAG) {
        AG = EnteredAG;
}

void Player::SetCurrentEXP(int EnteredEXP) {
        CurrentEXP = EnteredEXP;
}

void Player::AddCurrentEXP(int EnteredEXP) {
        CurrentEXP += EnteredEXP;
        if (CurrentEXP >= EXPToNextLevel) {
                int StatRaise = 0;
                StatRaise = distribution(generator);
                cout << "Your HP went up by " << StatRaise << endl;
                RaiseMaxHP(StatRaise);
                StatRaise = distribution(generator);
                cout << "Your ATK went up by " << StatRaise << endl;
                RaiseATK(StatRaise);
                StatRaise = distribution(generator);
                cout << "Your DEF went up by " << StatRaise << endl;
                RaiseDEF(StatRaise);
                StatRaise = distribution(generator);
                cout << "Your AG went up by " << StatRaise << endl;
                RaiseAG(StatRaise);
                RaiseMaxHP(distribution(generator));
                CurrentEXP = 0;
                RaiseLevel(1);
                RaiseEXPToNextLevel();
                Sleep(800);
        }
}

void Player::SetEXPToNextLevel(int EnteredEXP) {
        EXPToNextLevel = EnteredEXP;
}

void Player::RaiseEXPToNextLevel() {
        EXPToNextLevel *= 1.4; // Raises required EXP every level.
}

void Player::SetGold(int EnteredGold) {
        Gold = EnteredGold;
}

void Player::RaiseGold(int EnteredGold) {
        Gold += EnteredGold;
}

void Player::LowerGold(int EnteredGold) {
        Gold -= EnteredGold;
}

void Player::RaiseLevel(int EnteredLevel) {
        Level += EnteredLevel;
}

void Player::RaiseMaxHP(int EnteredHP) {
        MaxHP += EnteredHP;
}

void Player::RaiseATK(int EnteredATK) {
        ATK += EnteredATK;
}

void Player::RaiseDEF(int EnteredDEF) {
        DEF += EnteredDEF;
}

void Player::RaiseAG(int EnteredAG) {
        AG += EnteredAG;
}
