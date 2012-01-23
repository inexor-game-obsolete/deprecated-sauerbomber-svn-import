#ifndef FPSGAME_SERVER_H
#define FPSGAME_SERVER_H

#include "sbpy.h"
#include "geom.h"
#include "fpsstate.h"
#include "tstring.h"
#include "priviledges.h"
#include <enet/enet.h>

extern ENetPeer *getclientpeer(int i);

namespace server {

    extern int gamemode;
    extern int gamemillis, nextexceeded;

    /**
     * server side version of entities and movables
     */
    struct server_entity {
        int type;
        int spawntime;
        char spawned;
        int maxrespawns;
        uchar state;
        int health;
    };

    static const int DEATHMILLIS = 300;

    struct clientinfo;

    struct hitinfo
    {
        int target;
        int lifesequence;
        int rays;
        float dist;
        vec dir;
    };

    struct gameevent
    {
        virtual ~gameevent() {}

        virtual bool flush(clientinfo *ci, int fmillis);
        virtual void process(clientinfo *ci) {}

        virtual bool keepable() const { return false; }
    };

    struct timedevent : gameevent
    {
        int millis;

        bool flush(clientinfo *ci, int fmillis);
    };

    struct shotevent : timedevent
    {
        int id, gun;
        vec from, to;
        vector<hitinfo> hits;

        void process(clientinfo *ci);
    };

    struct explodeevent : timedevent
    {
        int id, gun;
        vector<hitinfo> hits;

        bool keepable() const { return true; }

        void process(clientinfo *ci);
    };

    struct suicideevent : gameevent
    {
        void process(clientinfo *ci);
    };

    struct pickupevent : gameevent
    {
        int ent;

        void process(clientinfo *ci);
    };



    template <int N>
    struct projectilestate
    {
        int projs[N];
        int numprojs;

        projectilestate() : numprojs(0) {}

        void reset() { numprojs = 0; }

        void add(int val)
        {
            if(numprojs>=N) numprojs = 0;
            projs[numprojs++] = val;
        }

        bool remove(int val)
        {
            loopi(numprojs) if(projs[i]==val)
            {
                projs[i] = projs[--numprojs];
                return true;
            }
            return false;
        }
    };

    struct gamestate : fpsstate
    {
        vec o;
        int state, editstate;
        int lastdeath, lastspawn, lifesequence;
        int lastshot;
        projectilestate<8> rockets, grenades, bombs;
        int frags, flags, deaths, teamkills, shotdamage, damage, damage_rec, tokens;
        int lasttimeplayed, timeplayed;
        int shots, hits;
        float effectiveness;

        gamestate() : state(CS_DEAD), editstate(CS_DEAD) {}

        bool isalive(int gamemillis)
        {
            return state==CS_ALIVE || (state==CS_DEAD && gamemillis - lastdeath <= DEATHMILLIS);
        }

        bool waitexpired(int gamemillis)
        {
            return gamemillis - lastshot >= gunwait;
        }

        void reset()
        {
            if(state!=CS_SPECTATOR) state = editstate = CS_DEAD;
            maxhealth = 100;
            rockets.reset();
            grenades.reset();
            bombs.reset();

            timeplayed = 0;
            effectiveness = 0;
            frags = flags = deaths = teamkills = shotdamage = damage = tokens = 0;
            shots = hits = 0;

            respawn(gamemode);
        }

        void respawn(int gamemode = NULL)
        {
            fpsstate::respawn(gamemode);
            o = vec(-1e10f, -1e10f, -1e10f);
            lastdeath = 0;
            lastspawn = -1;
            lastshot = 0;
            tokens = 0;
        }

        void reassign()
        {
            respawn(gamemode);
            rockets.reset();
            grenades.reset();
            bombs.reset();
        }

        void setbackupweapon(int backupweapon)
        {
          fpsstate::backupweapon = GUN_BOMB;
        }

    };

    struct savedscore
    {
        uint ip;
        string name;
        int maxhealth, frags, flags, deaths, teamkills, shotdamage, damage;
        int timeplayed;
        float effectiveness;
        int bombradius;
        int bombdelay;
        int racetime;
        int racelaps;
        int racecheckpoint;
        int racerank;
        int racestate;

        void save(gamestate &gs)
        {
            maxhealth = gs.maxhealth;
            frags = gs.frags;
            flags = gs.flags;
            deaths = gs.deaths;
            teamkills = gs.teamkills;
            shotdamage = gs.shotdamage;
            damage = gs.damage;
            timeplayed = gs.timeplayed;
            effectiveness = gs.effectiveness;
            bombradius = gs.bombradius;
            bombdelay = gs.bombdelay;
            racetime = gs.racetime;
            racelaps = gs.racelaps;
            racecheckpoint = gs.racecheckpoint;
            racerank = gs.racerank;
            racestate = gs.racestate;
        }

        void restore(gamestate &gs)
        {
            if(gs.health==gs.maxhealth) gs.health = maxhealth;
            gs.maxhealth = maxhealth;
            gs.frags = frags;
            gs.flags = flags;
            gs.deaths = deaths;
            gs.teamkills = teamkills;
            gs.shotdamage = shotdamage;
            gs.damage = damage;
            gs.timeplayed = timeplayed;
            gs.effectiveness = effectiveness;
            gs.bombradius = bombradius;
            gs.bombdelay = bombdelay;
            gs.racetime = racetime;
            gs.racelaps = racelaps;
            gs.racecheckpoint = racecheckpoint;
            gs.racerank = racerank;
            gs.racestate = racestate;
        }
    };

    struct clientinfo
    {
        int clientnum, ownernum, connectmillis, sessionid, overflow;
        string name, team, mapvote;
        int playermodel;
        int modevote;
        int privilege;
        bool connected, local, timesync;
        int gameoffset, lastevent, pushed, exceeded;
        gamestate state;
        vector<gameevent *> events;
        vector<uchar> position, messages;
        int posoff, poslen, msgoff, msglen;
        vector<clientinfo *> bots;
        uint authreq;
        string authname;
        int ping, aireinit;
        string clientmap;
        int mapcrc;
        bool warned, gameclip, active;
        ENetPacket *getdemo, *getmap, *clipboard;
        int lastclipboard, needclipboard;

        clientinfo() : getdemo(NULL), getmap(NULL), clipboard(NULL) { reset(); }
        ~clientinfo() { events.deletecontents(); cleanclipboard(); }

        void addevent(gameevent *e)
        {
            if(state.state==CS_SPECTATOR || events.length()>100) delete e;
            else events.add(e);
        }

        enum
        {
            PUSHMILLIS = 2500
        };

        int calcpushrange()
        {
            ENetPeer *peer = getclientpeer(ownernum);
            return PUSHMILLIS + (peer ? peer->roundTripTime + peer->roundTripTimeVariance : ENET_PEER_DEFAULT_ROUND_TRIP_TIME);
        }

        bool checkpushed(int millis, int range)
        {
            return millis >= pushed - range && millis <= pushed + range;
        }

        void scheduleexceeded()
        {
            if(state.state!=CS_ALIVE || !exceeded) return;
            int range = calcpushrange();
            if(!nextexceeded || exceeded + range < nextexceeded) nextexceeded = exceeded + range;
        }

        void setexceeded()
        {
            if(state.state==CS_ALIVE && !exceeded && !checkpushed(gamemillis, calcpushrange())) exceeded = gamemillis;
            scheduleexceeded();
        }

        void setpushed()
        {
            pushed = max(pushed, gamemillis);
            if(exceeded && checkpushed(exceeded, calcpushrange())) exceeded = 0;
        }

        bool checkexceeded()
        {
            return state.state==CS_ALIVE && exceeded && gamemillis > exceeded + calcpushrange() && !m_race; // TODO: check if there are physics manipulation entities
        }

        void mapchange()
        {
            mapvote[0] = 0;
            state.reset();
            events.deletecontents();
            overflow = 0;
            timesync = false;
            lastevent = 0;
            exceeded = 0;
            pushed = 0;
            clientmap[0] = '\0';
            mapcrc = 0;
            warned = false;
            gameclip = false;
        }

        void reassign()
        {
            state.reassign();
            events.deletecontents();
            timesync = false;
            lastevent = 0;
        }

        void cleanclipboard(bool fullclean = true)
        {
            if(clipboard) { if(--clipboard->referenceCount <= 0) enet_packet_destroy(clipboard); clipboard = NULL; }
            if(fullclean) lastclipboard = 0;
        }

        void reset()
        {
            name[0] = team[0] = 0;
            playermodel = -1;
            privilege = PRIV_NONE;
            connected = local = false;
            authreq = 0;
            position.setsize(0);
            messages.setsize(0);
            ping = 0;
            aireinit = 0;
            needclipboard = 0;
            active = false;
            cleanclipboard();
            mapchange();
        }

        int geteventmillis(int servmillis, int clientmillis)
        {
            if(!timesync || (events.empty() && state.waitexpired(servmillis)))
            {
                timesync = true;
                gameoffset = servmillis - clientmillis;
                return servmillis;
            }
            else return gameoffset + clientmillis;
        }
    };

    struct demofile
    {
        string info;
        uchar *data;
        int len;
    };

    struct worldstate
    {
        int uses;
        vector<uchar> positions, messages;
    };

    struct ban
    {
        int time;
        uint ip;
    };

    namespace aiman
    {
        extern void removeai(clientinfo *ci);
        extern void clearai();
        extern void checkai();
        extern void reqadd(clientinfo *ci, int skill);
        extern void reqdel(clientinfo *ci);
        extern void setbotlimit(clientinfo *ci, int limit);
        extern void setbotbalance(clientinfo *ci, bool balance);
        extern void changemap();
        extern void addclient(clientinfo *ci);
        extern void changeteam(clientinfo *ci);
    }

#define MM_MODE 0xF
#define MM_AUTOAPPROVE 0x1000
#define MM_PRIVSERV (MM_MODE | MM_AUTOAPPROVE)
#define MM_PUBSERV ((1<<MM_OPEN) | (1<<MM_VETO))
#define MM_COOPSERV (MM_AUTOAPPROVE | MM_PUBSERV | (1<<MM_LOCKED))

    extern int gamelimit;
    extern bool gamepaused;
    extern int mastermode;
    extern int mastermask;
    extern char smapname[260];
    extern char *adminpass;
    extern char *serverpass;
    extern vector<clientinfo *> connects, clients, bots;
    extern vector<demofile> demos;
    extern bool demonextmatch;

    extern char *pyconfigpath;

    // const char *modename(int n, const char *unknown = "unknown");
    // const char *mastermodename(int n, const char *unknown = "unknown");

    // int numclients(int exclude = -1, bool nospec = true, bool noai = true, bool priv = false);
    void sendservmsg(const char *s);
    clientinfo *getinfo(int n);
    void hashpassword(int cn, int sessionid, const char *pwd, char *result, int maxlen);
    void setcimaster(clientinfo *ci);
    void setciadmin(clientinfo *ci);
    void pausegame(bool);
    void setmap(const char *, int);
    void setmastermode(int mm);
    void challengeauth(clientinfo *, uint, const char *);
    bool setteam(clientinfo *, char *);
    bool pregame_setteam(clientinfo *, char *);
    bool spectate(clientinfo *, bool, int);
    void setremainingmillis(int millis);
    void endgame();
    void resetpriv(clientinfo *ci);
    void sendmapreload();
    void senddemo(clientinfo *ci, int num);
    void suicide(clientinfo *ci);

}

#endif
