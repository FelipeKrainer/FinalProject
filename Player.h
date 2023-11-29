class Player {
    public:
        void Print();
        // These functions return the Player's current stats.
        int GetLevel() const;
        int GetMaxHP() const;
        int GetCurrentHP() const;
        int GetATK() const;
        int GetDEF() const;
        int GetAG() const;
        int GetCurrentEXP() const;
        int GetEXPToNextLevel() const;
        int GetGold() const;
        // These functions set the Player's stats.
        void SetLevel(int);
        void SetMaxHP(int);
        void SetCurrentHP(int);
        void SetATK(int);
        void SetDEF(int);
        void SetAG(int);
        void SetCurrentEXP(int);
        void SetEXPToNextLevel(int);
        void SetGold(int);
        // These functions raise (or lower) the Player's stats.
        void RaiseLevel(int);
        void RaiseMaxHP(int);
        void LowerCurrentHP(int);
        void RaiseATK(int);
        void RaiseDEF(int);
        void RaiseAG(int);
        void AddCurrentEXP(int);
        void RaiseEXPToNextLevel();
        void RaiseGold(int);
        void LowerGold(int);

    private:
        int Level = 1;
        int CurrentHP = 20;
        int MaxHP = 20;
        int ATK = 5;
        int DEF = 5;
        int AG = 5;
        int CurrentEXP = 0;
        int EXPToNextLevel = 5;
        int Gold = 15;
};
