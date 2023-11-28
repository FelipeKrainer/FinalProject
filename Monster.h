#include <string>

class Monster {
    public:
        void PrintInfo() const;
            // Prints all of a monster's private variables. 
            // Used for debugging. 
        std::string GetName() const;
            // Returns a monster's name.
        int GetHP() const;
            // Returns a monster's maximum health points.
        int GetATK() const;
            // Returns the value stored in a monster's attack stat.
        int GetDEF() const;
            // Returns the value stored in a monster's defence stat.
        int GetAG() const;
            // Returns the value stored in a monster's agility stat.
        int GetEXP() const;
            // Returns the amount of experience a monster drops.
        std::string GetMethodOfAttack() const;
            // Returns the word that describes how a monster hits the player.
        void SetName(std::string);
            // Accepts a string and uses it to set a monster's name.
        void SetHP(std::string);
            // Accepts a string and uses it to set a monster's maximum health points. 
            // Converts the string to an integer.
        void SetATK(std::string);
            // Accepts a string and uses it to set a monster's attack. 
            // Converts the string to an integer.
        void SetDEF(std::string);
            // Accepts a string and uses it to set a monster's defence. 
            // Converts the string to an integer.
        void SetAG(std::string);
            //Accepts a string and uses it to set a monster's agility. 
            // Converts the string to an integer.
        void SetEXP(std::string);
            // Accepts a string and uses it to set a monster's experience. 
            // Converts the string to an integer.
        void SetMethodOfAttack(std::string);
            // Accepts a string and uses it to set the word used to describe a monster's attack.
    private:
        std::string Name;
        int HP;
        int ATK;
        int DEF;
        int AG;
        int EXP;
        std::string MethodOfAttack;
};
