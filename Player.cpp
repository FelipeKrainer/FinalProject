#include "Player.h"
#include <iostream>

using namespace std;

void Player::Print() {
        cout << "Player Level: " << Level << endl;
        cout << "Player HP: " << HP << endl;
        cout << "Player ATK: " << ATK << endl;
        cout << "Player DEF: " << DEF << endl;
        cout << "Player AG: " << AG << endl;
        cout << "Current EXP: " << CurrentEXP << endl;
        cout << "Required EXP: " << EXPToNextLevel << endl;
        cout << "Player Gold: " << Gold << endl;
}

int Player::GetLevel() {
        return Level;
}

int Player::GetHP() {
        return HP;
}

int Player::GetATK() {
        return ATK;
}

int Player::GetDEF() {
        return DEF;
}

int Player::GetAG() {
        return AG;
}

int Player::GetCurrentEXP() {
        return CurrentEXP;
}

int Player::GetEXPToNextLevel() {
        return EXPToNextLevel;
}

int Player::GetGold() {
        return Gold;
}

void Player::SetLevel(int EnteredLevel) {
        Level = EnteredLevel;
}

void Player::SetHP(int EnteredHP) {
        HP = EnteredHP;
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
                CurrentEXP = 0;
                RaiseLevel(1);
                RaiseEXPToNextLevel();
        }
}

void Player::SetEXPToNextLevel(int EnteredEXP) {
        EXPToNextLevel = EnteredEXP;
}

void Player::RaiseEXPToNextLevel() {
        EXPToNextLevel *= 1.2; // Raises required EXP every level.
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

void Player::RaiseHP(int EnteredHP) {
        HP += EnteredHP;
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
