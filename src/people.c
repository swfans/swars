/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file people.c
 *     Routines implementing people.
 * @par Purpose:
 *     Implement general functions for handling person things.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     27 May 2022 - 12 Jun 2023
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "people.h"

#include "pepgroup.h"
#include "player.h"
#include "game.h"
#include "sound.h"
#include "thing.h"
#include "weapon.h"
#include "swlog.h"
/******************************************************************************/
struct PeepStat peep_type_stats[] = {
    {   0,    0,    0,    0,   0, 0,   0, 0, 0, 0},
    {1000, 1024, 2048, 1024,  20, 5, 512, 0, 0, 0},
    {1000, 1024, 2048, 1024,  20, 1, 512, 0, 0, 0},
    { 600,  524, 2048,  601,  10, 2, 312, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 172, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 160, 0, 0, 0},
    { 600,  600,  900,  601,  15, 3, 312, 0, 0, 0},
    {4000, 1000,  800, 2001,9999, 2, 362, 0, 0, 0},
    { 600,  400,  800,  601,   6, 2, 362, 0, 0, 0},
    { 600,  524, 2024,  601,  10, 2, 312, 0, 0, 0},
    { 200,  100,  300,  200,   0, 1, 212, 0, 0, 0},
    { 600,  524, 2024,  601,   0, 1, 312, 0, 0, 0},
    {2000, 1024, 2048, 1024,  20, 5, 512, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 150, 0, 0, 0},
    { 110,  100,  400,  300,   0, 1, 212, 0, 0, 0},
    { 100,  100,  400,  300,   0, 1, 212, 0, 0, 0},
    {   0,    0,    0,    0,   0, 0,   0, 0, 0, 0},
};


ubyte person_mod_chest_level(struct Thing *p_person)
{
    return cybmod_chest_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_chest_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_chest_level(&p_person->U.UPerson.UMod, nmod);
}

ubyte person_mod_legs_level(struct Thing *p_person)
{
    return cybmod_legs_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_legs_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_legs_level(&p_person->U.UPerson.UMod, nmod);
}

ubyte person_mod_arms_level(struct Thing *p_person)
{
    return cybmod_arms_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_arms_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_arms_level(&p_person->U.UPerson.UMod, nmod);
}

ubyte person_mod_brain_level(struct Thing *p_person)
{
    return cybmod_brain_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_brain_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_brain_level(&p_person->U.UPerson.UMod, nmod);
}

ubyte person_mod_skin_level(struct Thing *p_person)
{
    return cybmod_skin_level(&p_person->U.UPerson.UMod);
}

void set_person_mod_skin_level(struct Thing *p_person, ubyte nmod)
{
    set_cybmod_skin_level(&p_person->U.UPerson.UMod, nmod);
}

TbBool person_carries_weapon(struct Thing *p_person, ubyte weapon)
{
    return weapons_has_weapon(p_person->U.UPerson.WeaponsCarried, weapon);
}

void person_give_best_mods(struct Thing *p_person)
{
    set_person_mod_legs_level(p_person, 3);
    set_person_mod_arms_level(p_person, 3);
    set_person_mod_brain_level(p_person, 3);
    set_person_mod_chest_level(p_person, 3);
}

TbBool person_is_persuaded(short thing)
{
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_person = &things[thing];
    return ((p_person->Flag & TngF_Persuaded) != 0);
}

TbBool person_is_persuaded_by_person(short thing, short owntng)
{
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_person = &things[thing];
    if ((p_person->Flag & TngF_Persuaded) == 0)
        return false;

    return (p_person->Owner == owntng);
}

TbBool person_is_persuaded_by_player(short thing, ushort plyr)
{
    struct Thing *p_thing;
    short plyagent, plygroup;
    struct Thing *p_person;

    if (thing <= 0)
        return false;

    p_thing = &things[thing];
    if ((p_thing->Flag & TngF_Persuaded) == 0)
        return false;

    plyagent = players[plyr].DirectControl[0];
    plygroup = things[plyagent].U.UPerson.Group;

    p_person = &things[p_thing->Owner];
    return (p_person->U.UPerson.Group == plygroup);
}

short calc_person_speed(struct Thing *p_person)
{
    struct PeepStat *pstat;
    short mood;
    short speed;

    pstat = &peep_type_stats[p_person->SubType];

    mood = p_person->U.UPerson.Mood;
    if (mood < 0)
        mood = -mood;

    speed = pstat->Speed + 2 * mood + 100 * person_mod_legs_level(p_person);

    // Flamer slows you down? Why? This makes it even more useless..
    if (p_person->U.UPerson.CurrentWeapon == WEP_FLAMER)
        speed >>= 1;

    if (p_person->Flag & TngF_Persuaded)
        speed += 250;

    if (p_person->Flag2 & 0x80000)
        speed += 512;

    if (speed > PERSON_MAX_SPEED)
        speed = PERSON_MAX_SPEED;

    return speed;
}

void set_person_health_energy_shield_stamina_type(struct Thing *p_person, ushort stype)
{
    struct PeepStat *pstat;

    pstat = &peep_type_stats[stype];

    p_person->U.UPerson.Energy = pstat->MaxEnergy +
        (person_mod_chest_level(p_person) * pstat->MaxEnergy * 50 / 100);

    p_person->U.UPerson.ShieldEnergy = pstat->MaxShield +
        (person_mod_brain_level(p_person) * pstat->MaxShield * 50 / 100);

    p_person->Health = pstat->MaxHealth +
       ((person_mod_chest_level(p_person) * pstat->MaxHealth * 50 / 100) +
        (person_mod_brain_level(p_person) * pstat->MaxHealth * 50 / 100) +
        (person_mod_legs_level(p_person) * pstat->MaxHealth * 25 / 100) +
        (person_mod_arms_level(p_person) * pstat->MaxHealth * 25 / 100)) / 4;

    p_person->U.UPerson.Stamina = pstat->MaximumStamina;

    p_person->U.UPerson.PersuadePower = 0;

    p_person->U.UPerson.MaxHealth = p_person->Health;
    p_person->U.UPerson.MaxEnergy = p_person->U.UPerson.Energy;
    p_person->U.UPerson.MaxShieldEnergy = p_person->U.UPerson.ShieldEnergy;
    p_person->U.UPerson.MaxStamina = p_person->U.UPerson.Stamina;

    if (person_mod_chest_level(p_person) == 4)
    {
        p_person->Health = 2 * PERSON_MAX_HEALTH_LIMIT;
        p_person->U.UPerson.MaxHealth = PERSON_MAX_HEALTH_LIMIT;
    }
}

ushort calc_person_radius_type(struct Thing *p_person, ushort stype)
{
    ushort r;
    switch (stype)
    {
    case SubTT_PERS_AGENT:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_POLICE:
    case SubTT_PERS_PUNK_M:
    default:
        r = 80;
        break;
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_HIGH_PRIEST:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
        r = 100;
        break;
    case SubTT_PERS_MECH_SPIDER:
        r = 384;
        break;
    }
    return r;
}

void set_person_stats_type(struct Thing *p_person, ushort stype)
{
#if 0
    asm volatile ("call ASM_set_person_stats_type\n"
        : : "a" (p_person), "d" (type));
#endif
    set_person_health_energy_shield_stamina_type(p_person, stype);
    p_person->Speed = calc_person_speed(p_person);
}

void init_person_thing(struct Thing *p_person)
{
#if 0
    asm volatile ("call ASM_init_person_thing\n"
        : : "a" (p_person));
#endif
    set_person_health_energy_shield_stamina_type(p_person, p_person->SubType);

    p_person->Radius = calc_person_radius_type(p_person, p_person->SubType);

    if (current_level != 0)
        p_person->U.UPerson.CurrentWeapon = 0;

    if ((p_person->Flag & TngF_Unkn0002) != 0)
    {
        p_person->U.UPerson.AnimMode = 20;
        p_person->State = PerSt_DEAD;
        p_person->U.UPerson.FrameId.Version[3] = 1;
        p_person->U.UPerson.FrameId.Version[4] = 0;
    }
    else
    {
        ushort person_anim;
        person_anim = people_frames[p_person->SubType][p_person->U.UPerson.AnimMode];
        p_person->Frame -= nstart_ani[person_anim + p_person->U.UPerson.Angle];
        p_person->U.UPerson.AnimMode = 0;
        p_person->Frame += nstart_ani[person_anim + p_person->U.UPerson.Angle];
        p_person->StartFrame = person_anim - 1;
        p_person->Speed = calc_person_speed(p_person);
    }
}

/** Frees a linked list of paths, and adds the paths to free list.
 */
void path_free_linked_list(ushort first_path)
{
    ushort free_head, path;
    ushort count;

    free_head = head_my_path;
    count = path_count;

    path = first_path;
    while (path != 0)
    {
        ushort tmpath;

        tmpath = my_paths[path].Next;
        my_paths[path].Flag = 0;
        my_paths[path].Next = free_head;
        free_head = path;
        path = tmpath;
        count--;
    }
    path_count = count;
    head_my_path = free_head;
}

void remove_path(struct Thing *p_thing)
{
#if 0
    asm volatile ("call ASM_remove_path\n"
        : : "a" (p_thing));
#else
    ushort path;

    p_thing->Flag2 &= ~0x0040;
    if (p_thing->U.UPerson.PathIndex != 0)
    {
        if (p_thing->Type == TT_PERSON)
            p_thing->U.UPerson.LastDist = 32000;
        path = p_thing->U.UPerson.PathIndex;
        p_thing->U.UPerson.PathIndex = 0;
        path_free_linked_list(path);
        p_thing->PathOffset = 0;
        p_thing->Flag &= ~0x00020000;
    }
#endif
}

void set_person_animmode_walk(struct Thing *p_person)
{
    asm volatile ("call ASM_set_person_animmode_walk\n"
        : : "a" (p_person));
}

void persuaded_person_add_to_stats(struct Thing *p_person, ushort brief)
{
    switch (p_person->SubType)
    {
    case SubTT_PERS_AGENT:
          ++mission_status[brief].AgentsGained;
          // fall through
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_PUNK_M:
    case SubTT_PERS_HIGH_PRIEST:
          ++mission_status[brief].EnemiesPersuaded;
          break;
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
    case SubTT_PERS_FAST_BLOND_F:
          ++mission_status[brief].CivsPersuaded;
          break;
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_MECH_SPIDER:
    case SubTT_PERS_POLICE:
          ++mission_status[brief].SecurityPersuaded;
          break;
    default:
          break;
    }
}

void persuaded_person_remove_from_stats(struct Thing *p_person, ushort brief)
{
    switch (p_person->SubType)
    {
    case SubTT_PERS_AGENT:
        --mission_status[brief].AgentsGained;
        // fall through
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_PUNK_M:
    case SubTT_PERS_HIGH_PRIEST:
        --mission_status[brief].EnemiesPersuaded;
        break;
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
    case SubTT_PERS_FAST_BLOND_F:
        --mission_status[brief].CivsPersuaded;
        break;
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_MECH_SPIDER:
    case SubTT_PERS_POLICE:
        --mission_status[brief].SecurityPersuaded;
        break;
    default:
        break;
    }
}

void set_person_persuaded(struct Thing *p_person, struct Thing *p_attacker, ushort energy)
{
#if 0
    asm volatile ("call ASM_set_person_persuaded\n"
        : : "a" (p_person), "d" (p_attacker), "b" (energy));
#endif
    if ((p_person->Flag & 0x2000) != 0)
        return;

    if (p_person->State == PerSt_DEAD)
        return;

    play_dist_sample(p_person, 20, 127, 64, 100, 0, 3);
    remove_path(p_person);
    set_person_animmode_walk(p_person);
    p_attacker->U.UPerson.Energy -= energy;
    if (p_attacker->U.UPerson.Energy < 0)
        p_attacker->U.UPerson.Energy = 0;
    p_person->PTarget = NULL;
    p_person->U.UPerson.Target2 = 0;
    p_person->State = 0;

    p_person->Flag |= TngF_Unkn40000000 | TngF_Unkn0004;
    p_person->Owner = p_attacker->ThingOffset;
    p_person->Flag &= ~(TngF_Unkn00800000|TngF_Unkn00040000|TngF_Unkn00020000|TngF_Unkn0800|TngF_Unkn0080);
    set_person_animmode_walk(p_person);
    p_person->U.UPerson.ComTimer = -1;
    p_person->U.UPerson.ComRange = 3;
    p_person->U.UPerson.Timer2 = 5;
    p_person->U.UPerson.StartTimer2 = 5;
    p_person->SubState = 0;

    p_person->U.UPerson.EffectiveGroup = p_attacker->U.UPerson.EffectiveGroup;
    p_person->U.UPerson.Within = 0;
    {
        struct PeepStat *pstat;

        pstat = &peep_type_stats[p_person->SubType];
        p_attacker->U.UPerson.PersuadePower += pstat->PersuadeWorth;
    }
    if ((p_person->Flag2 & 0x0010) == 0)
    {
          p_person->StartFrame = 1059;
          p_person->Frame = nstart_ani[p_person->StartFrame + 1];
    }
    p_person->Timer1 = 48;
    p_person->StartTimer1 = 48;
    {
        word_17FA58[word_1531DA] = p_person->ThingOffset;
        word_1531DA++;
    }
    p_person->State = PerSt_BEING_PERSUADED;
    {
        ushort group;
        group = p_person->U.UPerson.Group & 0x1F;
        group_actions[group].Persuaded++;
    }
    if (!in_network_game && (p_attacker->Flag & 0x2000) && (p_attacker->U.UPerson.EffectiveGroup == ingame.MyGroup))
    {
        short plagent;

        persuaded_person_add_to_stats(p_person, open_brief);

        for (plagent = 0; plagent < playable_agents; plagent++)
        {
            struct Thing *p_agent;

            p_agent = players[local_player_no].MyAgent[plagent];
            if (p_person == p_agent->PTarget)
                p_agent->PTarget = NULL;
            if (p_agent->U.UPerson.Target2 == p_person->ThingOffset)
                p_agent->U.UPerson.Target2 = 0;
        }
    }
}

void unpersuade_my_peeps(struct Thing *p_owntng)
{
#if 0
    asm volatile ("call ASM_unpersuade_my_peeps\n"
        : : "a" (p_owntng));
#else
    short person;
    struct Thing *p_person;
    ushort count;
    int i, k;

    count = word_1531DA;
    for (i = 1; i < count; i++)
    {
        person = word_17FA58[i];
        p_person = &things[person];
        if (p_person->Owner != p_owntng->ThingOffset)
            continue;
        p_person->Flag &= ~TngF_Persuaded;
        p_person->U.UPerson.EffectiveGroup = p_owntng->U.UPerson.Group;
        p_person->State = 0;
        p_person->Flag |= TngF_Unkn0040 | TngF_Unkn0004;
        --group_actions[p_person->U.UPerson.Group].Persuaded;
        for (k = i; k < count - 1; k++) {
            word_17FA58[k] = word_17FA58[k+1];
        }
        count--;
        if (!in_network_game && (p_owntng->U.UPerson.EffectiveGroup == ingame.MyGroup))
        {
            persuaded_person_remove_from_stats(p_person, open_brief);
        }
    }
    p_owntng->U.UPerson.PersuadePower = 0;
    word_1531DA = count;
#endif
}
/******************************************************************************/
