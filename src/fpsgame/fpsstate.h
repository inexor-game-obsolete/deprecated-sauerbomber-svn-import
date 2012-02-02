#ifndef _FPSGAME_FPSSTATE_H
#define _FPSGAME_FPSSTATE_H

#include "guns.h"
#include "armor.h"
#include "modes.h"
#include "ai.h"
#include "items.h"
#include "staticents.h"

// inherited by fpsent and server clients
struct fpsstate
{
    int health, maxhealth;
    int armour, armourtype;
    int quadmillis;
    int gunselect, gunwait;
    int ammo[NUMGUNS];
    int aitype, skill;
    int backupweapon;
    //bomberman
    int bombradius;
    int bombdelay;
    //race
    int racetime;
    int racelaps;
    int racecheckpoint;
    int racerank;
    int racestate;

    fpsstate() : maxhealth(100), aitype(AI_NONE), skill(0), backupweapon(GUN_FIST) {}

    void baseammo(int gun, int k = 2, int scale = 1) {
        switch(gun) {
            case GUN_BOMB:
                ammo[gun] = (itemstats[P_AMMO_BO].add*k)/scale;
                break;
            case GUN_FGL:
                ammo[gun] = (itemstats[P_AMMO_FG].add*k)/scale;
                break;
            case GUN_PULSED_THRUSTER:
                ammo[gun] = (itemstats[P_AMMO_PT].add*k)/scale;
                break;
            case GUN_CONTINOUS_THRUSTER:
                ammo[gun] = (itemstats[P_AMMO_CT].add*k)/scale;
                break;
            default:
                ammo[gun] = (itemstats[gun-GUN_SG].add*k)/scale;
                break;
        }
    }

    void addammo(int gun, int k = 1, int scale = 1) {
        switch(gun) {
            case GUN_BOMB:
                ammo[gun] = min(ammo[gun] + (itemstats[P_AMMO_BO].add*k)/scale, itemstats[P_AMMO_BO].max);
                break;
            case GUN_FGL:
                ammo[gun] = min(ammo[gun] + (itemstats[P_AMMO_FG].add*k)/scale, itemstats[P_AMMO_FG].max);
                break;
            case GUN_PULSED_THRUSTER:
                ammo[gun] = min(ammo[gun] + (itemstats[P_AMMO_PT].add*k)/scale, itemstats[P_AMMO_PT].max);
                break;
            case GUN_CONTINOUS_THRUSTER:
                ammo[gun] = min(ammo[gun] + (itemstats[P_AMMO_CT].add*k)/scale, itemstats[P_AMMO_CT].max);
                break;
            default:
                ammo[gun] = min(ammo[gun] + (itemstats[gun-GUN_SG].add*k)/scale, itemstats[gun-GUN_SG].max);
                break;
        }
    }

    bool hasmaxammo(int type) {
        switch(type) {
            case I_BOMBS:
                return ammo[GUN_BOMB] >= itemstats[P_AMMO_BO].max;
            case I_BOMBRADIUS:
                return bombradius >= itemstats[P_UP_BR].max;
            case I_BOMBDELAY:
                return bombdelay >= itemstats[P_UP_BD].max;
            case I_FOGGRANADES:
                return ammo[GUN_FGL] >= itemstats[P_AMMO_FG].max;
            case I_PULSED_THRUSTER:
                return ammo[GUN_PULSED_THRUSTER] >= itemstats[P_AMMO_PT].max;
            case I_CONTINOUS_THRUSTER:
                return ammo[GUN_CONTINOUS_THRUSTER] >= itemstats[P_AMMO_CT].max;
            default:
                return ammo[type-I_SHELLS+GUN_SG] >= itemstats[type-I_SHELLS].max;
        }
    }

    bool canpickup(int type) {
        switch(type) {
            case I_SHELLS:
                return ammo[GUN_SG] < itemstats[P_AMMO_SG].max;
            case I_BULLETS:
                return ammo[GUN_CG] < itemstats[P_AMMO_CG].max;
            case I_ROCKETS:
                return ammo[GUN_RL] < itemstats[P_AMMO_RL].max;
            case I_ROUNDS:
                return ammo[GUN_RIFLE] < itemstats[P_AMMO_RI].max;
            case I_GRENADES:
                return ammo[GUN_GL] < itemstats[P_AMMO_GL].max;
            case I_CARTRIDGES:
                return ammo[GUN_PISTOL] < itemstats[P_AMMO_PI].max;
            case I_BOMBS:
                return ammo[GUN_BOMB] < itemstats[P_AMMO_BO].max;
            case I_BOMBRADIUS:
                return bombradius < itemstats[P_UP_BR].max;
            case I_BOMBDELAY:
                return bombdelay < itemstats[P_UP_BD].max;
            case I_FOGGRANADES:
                return ammo[GUN_FGL] < itemstats[P_AMMO_FG].max;
            case I_PULSED_THRUSTER:
                return ammo[GUN_PULSED_THRUSTER] < itemstats[P_AMMO_PT].max;
            case I_CONTINOUS_THRUSTER:
                return ammo[GUN_CONTINOUS_THRUSTER] < itemstats[P_AMMO_CT].max;
            case I_HEALTH:
                return health < maxhealth;
            case I_BOOST:
                return maxhealth < itemstats[P_HEALTH_MH].max;
            case I_GREENARMOUR:
                if(armourtype == A_YELLOW && armour >= 100) {
                    return false;
                } else {
                    return !armourtype || armour < itemstats[P_ARMOUR_GA].max;
                }
            case I_YELLOWARMOUR:
                return !armourtype || armour < itemstats[P_ARMOUR_YA].max;
            case I_QUAD:
                return quadmillis < itemstats[P_UP_Q].max;
            case RACE_START:
            case RACE_FINISH:
            case RACE_CHECKPOINT:
                return true;
            default:
                return false;
        }
    }

    void pickup(int type) {
        switch(type) {
            case I_SHELLS:
                ammo[GUN_SG] = min(ammo[GUN_SG] + itemstats[P_AMMO_SG].add, itemstats[P_AMMO_SG].max);
                break;
            case I_BULLETS:
                ammo[GUN_CG] = min(ammo[GUN_CG] + itemstats[P_AMMO_CG].add, itemstats[P_AMMO_CG].max);
                break;
            case I_ROCKETS:
                ammo[GUN_RL] = min(ammo[GUN_RL] + itemstats[P_AMMO_RL].add, itemstats[P_AMMO_RL].max);
                break;
            case I_ROUNDS:
                ammo[GUN_RIFLE] = min(ammo[GUN_RIFLE] + itemstats[P_AMMO_RI].add, itemstats[P_AMMO_RI].max);
                break;
            case I_GRENADES:
                ammo[GUN_GL] = min(ammo[GUN_GL] + itemstats[P_AMMO_GL].add, itemstats[P_AMMO_GL].max);
                break;
            case I_CARTRIDGES:
                ammo[GUN_PISTOL] = min(ammo[GUN_PISTOL] + itemstats[P_AMMO_PI].add, itemstats[P_AMMO_PI].max);
                break;
            case I_BOMBS:
                ammo[GUN_BOMB] = min(ammo[GUN_BOMB] + itemstats[P_AMMO_BO].add, itemstats[P_AMMO_BO].max);
                break;
            case I_BOMBRADIUS:
                bombradius = min(bombradius + itemstats[P_UP_BR].add, itemstats[P_UP_BR].max);
                break;
            case I_BOMBDELAY:
                bombdelay = min(bombdelay + itemstats[P_UP_BD].add, itemstats[P_UP_BD].max);
                break;
            case I_FOGGRANADES:
                ammo[GUN_FGL] = min(ammo[GUN_FGL] + itemstats[P_AMMO_FG].add, itemstats[P_AMMO_FG].max);
                break;
            case I_PULSED_THRUSTER:
                ammo[GUN_PULSED_THRUSTER] = min(ammo[GUN_PULSED_THRUSTER] + itemstats[P_AMMO_PT].add, itemstats[P_AMMO_PT].max);
                break;
            case I_CONTINOUS_THRUSTER:
                ammo[GUN_CONTINOUS_THRUSTER] = min(ammo[GUN_CONTINOUS_THRUSTER] + itemstats[P_AMMO_CT].add, itemstats[P_AMMO_CT].max);
                break;
            case I_HEALTH:
                health = min(health + itemstats[P_HEALTH_H].add, maxhealth);
                break;
            case I_BOOST:
                maxhealth = min(maxhealth + itemstats[P_HEALTH_MH].add, itemstats[P_HEALTH_MH].max);
                health = min(health + itemstats[P_HEALTH_MH].add, maxhealth);
                break;
            case I_GREENARMOUR:
                armour = min(armour + itemstats[P_ARMOUR_GA].add, itemstats[P_ARMOUR_GA].max);
                armourtype = itemstats[P_ARMOUR_GA].info;
                break;
            case I_YELLOWARMOUR:
                armour = min(armour + itemstats[P_ARMOUR_YA].add, itemstats[P_ARMOUR_YA].max);
                armourtype = itemstats[P_ARMOUR_YA].info;
                break;
            case I_QUAD:
                quadmillis = min(quadmillis + itemstats[P_UP_Q].add, itemstats[P_UP_Q].max);
                break;
            default:
                break;
        }
    }

    void respawn(int gamemode = NULL) {
        health = maxhealth;
        armour = 0;
        armourtype = A_BLUE;
        quadmillis = 0;
        gunselect = GUN_PISTOL;
        gunwait = 0;
        bombradius = 1;
        bombdelay = 1;
        loopi(NUMGUNS) ammo[i] = 0;
        if (m_bomb) {
            backupweapon = GUN_BOMB;
        } else if (m_hideandseek) {
            backupweapon = GUN_CONTINOUS_THRUSTER;
        } else {
            backupweapon = GUN_FIST;
        }
        ammo[backupweapon] = 1;
    }

    void spawnstate(int gamemode) {
        if(m_demo) {
            gunselect = GUN_FIST;
        } else if(m_insta) {
            armour = 0;
            health = 1;
            gunselect = GUN_RIFLE;
            ammo[GUN_RIFLE] = 100;
        } else if(m_regencapture) {
            armourtype = A_GREEN;
            armour = 0;
            gunselect = GUN_PISTOL;
            ammo[GUN_PISTOL] = 40;
            ammo[GUN_GL] = 1;
        } else if(m_tactics) {
            armourtype = A_GREEN;
            armour = 100;
            ammo[GUN_PISTOL] = 40;
            int spawngun1 = rnd(5)+1, spawngun2;
            gunselect = spawngun1;
            baseammo(spawngun1, m_noitems ? 2 : 1);
            do spawngun2 = rnd(5)+1; while(spawngun1==spawngun2);
            baseammo(spawngun2, m_noitems ? 2 : 1);
            if(m_noitems) ammo[GUN_GL] += 1;
        } else if(m_efficiency) {
            armourtype = A_GREEN;
            armour = 100;
            loopi(5) baseammo(i+1);
            gunselect = GUN_CG;
            ammo[GUN_CG] /= 2;
        } else if(m_ctf || m_collect) {
            armourtype = A_BLUE;
            armour = 50;
            ammo[GUN_PISTOL] = 40;
            ammo[GUN_GL] = 1;
        } else if(m_bomb) {
            health = 1;
            armourtype = A_GREEN;
            armour = 0;
            gunselect = GUN_BOMB;
            backupweapon = GUN_BOMB;
        } else if(m_race) {
            // racetime = 0;
            // racelaps = 0;
            // racecheckpoint = 0;
            health = 100;
            armourtype = A_GREEN;
            armour = 50;
            gunselect = GUN_GL;
            ammo[GUN_PISTOL] = 0;
            ammo[GUN_GL] = 8;
        } else if(m_hideandseek) {
            health = 100;
            armour = 0;
            gunselect = GUN_PULSED_THRUSTER;
            ammo[GUN_GL] = 10;
            ammo[GUN_FGL] = 10;
            ammo[GUN_PULSED_THRUSTER] = 5;
            ammo[GUN_CONTINOUS_THRUSTER] = 999;
            ammo[GUN_PISTOL] = 0;
            ammo[GUN_GL] = 0;
        } else {
            ammo[GUN_PISTOL] = m_sp ? 80 : 40;
            ammo[GUN_GL] = 1;
        }
    }

    // just subtract damage here, can set death, etc. later in code calling this
    int dodamage(int damage) {
        int ad = damage*(armourtype+1)*25/100; // let armour absorb when possible
        if(ad>armour) ad = armour;
        armour -= ad;
        damage -= ad;
        health -= damage;
        return damage;
    }

    int hasammo(int gun, int exclude = -1) {
        return gun >= 0 && gun <= NUMGUNS && gun != exclude && ammo[gun] > 0;
    }
};

#endif
