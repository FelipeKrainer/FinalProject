class Player {
    public:
        void Print();
        // These functions return the Player's current stats.
        int GetLevel();
        int GetHP();
        int GetATK();
        int GetDEF();
        int GetAG();
        int GetCurrentEXP();
        int GetEXPToNextLevel();
        int GetGold();
        // These functions set the Player's stats.
        void SetLevel(int);
        void SetHP(int);
        void SetATK(int);
        void SetDEF(int);
        void SetAG(int);
        void SetCurrentEXP(int);
        void SetEXPToNextLevel(int);
        void SetGold(int);
        // These functions raise (or lower) the Player's stats.
        void RaiseLevel(int);
        void RaiseHP(int);
        void RaiseATK(int);
        void RaiseDEF(int);
        void RaiseAG(int);
        void AddCurrentEXP(int);
        void RaiseEXPToNextLevel();
        void RaiseGold(int);
        void LowerGold(int);

    private:
        // Starting player stats.
        int Level = 1;
        int HP = 20;
        int ATK = 5;
        int DEF = 5;
        int AG = 5;
        int CurrentEXP = 0;
        int EXPToNextLevel = 12;
        int Gold = 15;
};
