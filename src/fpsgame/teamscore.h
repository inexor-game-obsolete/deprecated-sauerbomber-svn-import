#ifndef FPSGAME_TEAMSCORE_H
#define FPSGAME_TEAMSCORE_H


struct teamscore {
    const char *team;
    int score;
    teamscore() {}
    teamscore(const char *s, int n) : team(s), score(n) {}

    static bool compare(const teamscore &x, const teamscore &y)
    {
        if(x.score > y.score) return true;
        if(x.score < y.score) return false;
        return strcmp(x.team, y.team) < 0;
    }
};

namespace game {
    struct scoregroup : teamscore {
        vector<fpsent *> players;
    };
}

#endif
