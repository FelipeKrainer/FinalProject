#include "Monster.h"
#include <iostream>

using namespace std;

void Monster::PrintInfo() const {
    cout <<  "Name: " << Name << endl;
    cout <<  "Max health: " << HP << endl;
    cout <<  "Attack: " << ATK << endl;
    cout <<  "Defence: " << DEF << endl;
    cout <<  "Agility: " << AG << endl;
    cout <<  "Experience: " << EXP << endl;
    cout <<  "Method of attack: " << MethodOfAttack << endl;
}

string Monster::GetName() const {
    return Name;
}

int Monster::GetHP() const {
    return HP;
}

int Monster::GetATK() const {
    return ATK;
}

int Monster::GetDEF() const {
    return DEF;
}

int Monster::GetAG() const {
    return AG;
}

int Monster::GetEXP() const {
    return EXP;
}

string Monster::GetMethodOfAttack() const {
    return MethodOfAttack;
}

void Monster::SetName(string EnteredName) {
    Name = EnteredName;
}

void Monster::SetHP(string EnteredHP) {
    HP = stoi(EnteredHP);
}

void Monster::SetATK(string EnteredATK) {
    ATK = stoi(EnteredATK);
}

void Monster::SetDEF(string EnteredDEF) {
    DEF = stoi(EnteredDEF);
}

void Monster::SetAG(string EnteredAG) {
    AG = stoi(EnteredAG);
}

void Monster::SetEXP(string EnteredEXP) {
    EXP = stoi(EnteredEXP);
}

void Monster::SetMethodOfAttack(string EnteredMethod) {
    MethodOfAttack = EnteredMethod;
}
