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

    void baseammo(int gun, int k = 2, int scale = 1)
    {
        if(gun==GUN_BOMB) ammo[gun] = (itemstats[P_AMMO_BO].add*k)/scale;
        else ammo[gun] = (itemstats[gun-GUN_SG].add*k)/scale;
    }

    void addammo(int gun, int k = 1, int scale = 1)
    {
        if(gun==GUN_BOMB) {
            itemstat &is = itemstats[P_AMMO_BO];
            ammo[gun] = min(ammo[gun] + (is.add*k)/scale, is.max);
        } else {
            itemstat &is = itemstats[gun-GUN_SG];
            ammo[gun] = min(ammo[gun] + (is.add*k)/scale, is.max);
        }
    }

    bool hasmaxammo(int type)
    {
        if(type>=I_BOMBS && type<=I_BOMBDELAY) {
            itemstat &is = itemstats[P_AMMO_BO+type-I_BOMBS];
            return ammo[GUN_BOMB]>=is.max;
        } else {
            itemstat &is = itemstats[type-I_SHELLS];
            return ammo[type-I_SHELLS+GUN_SG]>=is.max;
        }
    }

    bool canpickup(int type)
    {
        if(type>=I_BOMBS && type<=I_BOMBDELAY) {
            itemstat &is = itemstats[P_AMMO_BO+type-I_BOMBS];
            switch(type) {
                case I_BOMBRADIUS:
                    return bombradius<is.max;
                    break;
                case I_BOMBDELAY:
                    return bombdelay<is.max;
                    break;
                default:
                    return ammo[is.info]<is.max;
                    break;
            }
      } else if(type>=I_SHELLS && type<=I_QUAD) {
            itemstat &is = itemstats[type-I_SHELLS];
            switch(type) {
                case I_BOOST:
                    return maxhealth<is.max;
                case I_HEALTH:
                    return health<maxhealth;
                case I_GREENARMOUR:
                    // (100h/100g only absorbs 200 damage)
                    if(armourtype==A_YELLOW && armour>=100) return false;
                case I_YELLOWARMOUR:
                    return !armourtype || armour<is.max;
                case I_QUAD:
                    return quadmillis<is.max;
                default:
                    return ammo[is.info]<is.max;
            }
      } else if(type==RACE_START || type==RACE_FINISH || type==RACE_CHECKPOINT) {
          return true;
      } else {
          return false;
      }
    }

    void pickup(int type)
    {
        if(type>=I_BOMBS && type<=I_BOMBDELAY) {
            itemstat &is = itemstats[P_AMMO_BO+type-I_BOMBS];
            switch(type)
            {
                case I_BOMBRADIUS:
                    bombradius = min(bombradius+is.add, is.max);
                    break;
                case I_BOMBDELAY:
                    bombdelay = min(bombdelay+is.add, is.max);
                    break;
                default:
                    ammo[is.info] = min(ammo[is.info]+is.add, is.max);
                    break;
            }
        } else if(type>=I_SHELLS && type<=I_QUAD) {
            itemstat &is = itemstats[type-I_SHELLS];
            switch(type)
            {
                case I_BOOST:
                    maxhealth = min(maxhealth+is.add, is.max);
                case I_HEALTH: // boost also adds to health
                    health = min(health+is.add, maxhealth);
                    break;
                case I_GREENARMOUR:
                case I_YELLOWARMOUR:
                    armour = min(armour+is.add, is.max);
                    armourtype = is.info;
                    break;
                case I_QUAD:
                    quadmillis = min(quadmillis+is.add, is.max);
                    break;
                default:
                    ammo[is.info] = min(ammo[is.info]+is.add, is.max);
                    break;
            }
        }
    }

    void respawn(int gamemode = NULL)
    {
        health = maxhealth;
        armour = 0;
        armourtype = A_BLUE;
        quadmillis = 0;
        gunselect = GUN_PISTOL;
        gunwait = 0;
        bombradius = 1;
        bombdelay = 1;
        loopi(NUMGUNS) ammo[i] = 0;
        if (m_bomb) backupweapon = GUN_BOMB;
        else backupweapon = GUN_FIST;
        ammo[backupweapon] = 1;
    }

    void spawnstate(int gamemode)
    {
        if(m_demo)
        {
            gunselect = GUN_FIST;
        }
        else if(m_insta)
        {
            armour = 0;
            health = 1;
            gunselect = GUN_RIFLE;
            ammo[GUN_RIFLE] = 100;
        }
        else if(m_regencapture)
        {
            armourtype = A_GREEN;
            armour = 0;
            gunselect = GUN_PISTOL;
            ammo[GUN_PISTOL] = 40;
            ammo[GUN_GL] = 1;
        }
        else if(m_tactics)
        {
            armourtype = A_GREEN;
            armour = 100;
            ammo[GUN_PISTOL] = 40;
            int spawngun1 = rnd(5)+1, spawngun2;
            gunselect = spawngun1;
            baseammo(spawngun1, m_noitems ? 2 : 1);
            do spawngun2 = rnd(5)+1; while(spawngun1==spawngun2);
            baseammo(spawngun2, m_noitems ? 2 : 1);
            if(m_noitems) ammo[GUN_GL] += 1;
        }
        else if(m_efficiency)
        {
            armourtype = A_GREEN;
            armour = 100;
            loopi(5) baseammo(i+1);
            gunselect = GUN_CG;
            ammo[GUN_CG] /= 2;
        }
        else if(m_ctf || m_collect)
        {
            armourtype = A_BLUE;
            armour = 50;
            ammo[GUN_PISTOL] = 40;
            ammo[GUN_GL] = 1;
        }
        else if(m_bomb)
        {
            health = 1;
            armourtype = A_GREEN;
            armour = 0;
            gunselect = GUN_BOMB;
            backupweapon = GUN_BOMB;
        }
        else if(m_race)
        {
            // racetime = 0;
            // racelaps = 0;
            // racecheckpoint = 0;
            health = 100;
            armourtype = A_GREEN;
            armour = 50;
            gunselect = GUN_GL;
            ammo[GUN_PISTOL] = 0;
            ammo[GUN_GL] = 8;
        }
        else if(m_hideandseek)
        {
            health = 100;
            armour = 0;
            gunselect = GUN_RL;
            ammo[GUN_RL] = 10;
            ammo[GUN_PISTOL] = 0;
            ammo[GUN_GL] = 0;
        }
        else
        {
            ammo[GUN_PISTOL] = m_sp ? 80 : 40;
            ammo[GUN_GL] = 1;
        }
    }

    // just subtract damage here, can set death, etc. later in code calling this
    int dodamage(int damage)
    {
        int ad = damage*(armourtype+1)*25/100; // let armour absorb when possible
        if(ad>armour) ad = armour;
        armour -= ad;
        damage -= ad;
        health -= damage;
        return damage;
    }

    int hasammo(int gun, int exclude = -1)
    {
        return gun >= 0 && gun <= NUMGUNS && gun != exclude && ammo[gun] > 0;
    }
};

#endif
