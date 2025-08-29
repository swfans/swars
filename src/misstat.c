/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file misstat.c
 *     Mission stats gathering and maintaining.
 * @par Purpose:
 *     Functions for handling mission statistics during the game.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     22 Oct 2024 - 23 Dec 2024
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "misstat.h"

#include "bfmemut.h"

#include "febrief.h"
#include "game.h"
#include "people.h"
#include "player.h"
#include "swlog.h"
#include "thing.h"
/******************************************************************************/

void clear_mission_status_all(void)
{
    LbMemorySet(mission_status, 0, MISSION_STATUS_LIMIT * sizeof(struct MissionStatus));
}

void clear_mission_status(ushort brief)
{
    struct MissionStatus *p_mistat;

    p_mistat = &mission_status[brief];

    p_mistat->SP.CivsKilled = 0;
    p_mistat->SP.EnemiesKilled = 0;
    p_mistat->SP.CivsPersuaded = 0;
    p_mistat->SP.SecurityPersuaded = 0;
    p_mistat->SP.EnemiesPersuaded = 0;
    p_mistat->SP.SecurityKilled = 0;
    p_mistat->AgentsGained = 0;
    p_mistat->AgentsLost = 0;
    p_mistat->CityDays = 0;
    p_mistat->CityHours = 0;
}

void clear_open_mission_status(void)
{
    ulong id;

    if (in_network_game)
    {
        // In network game, mission status is per-player rather than per-mission
        for (id = 0; id < PLAYERS_LIMIT; id++)
        {
            clear_mission_status(id);
        }
    }
    else if (!in_network_game)
    {
        // Each mission has its status (unless in network game)
        clear_mission_status(open_brief);
    }
}

void persuaded_person_add_to_stats(struct Thing *p_person, ushort brief)
{
    struct MissionStatus *p_mistat;

    p_mistat = &mission_status[brief];

    switch (p_person->SubType)
    {
    case SubTT_PERS_AGENT:
          p_mistat->AgentsGained++;
          // fall through
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_HIGH_PRIEST:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_PUNK_M:
          p_mistat->SP.EnemiesPersuaded++;
          break;
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
    case SubTT_PERS_FAST_BLOND_F:
          p_mistat->SP.CivsPersuaded++;
          break;
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_MECH_SPIDER:
    case SubTT_PERS_POLICE:
          p_mistat->SP.SecurityPersuaded++;
          break;
    default:
          break;
    }
}

void persuaded_person_remove_from_stats(struct Thing *p_person, ushort brief)
{
    struct MissionStatus *p_mistat;

    p_mistat = &mission_status[brief];

    switch (p_person->SubType)
    {
    case SubTT_PERS_AGENT:
        p_mistat->AgentsGained--;
        // fall through
    case SubTT_PERS_ZEALOT:
    case SubTT_PERS_PUNK_F:
    case SubTT_PERS_PUNK_M:
    case SubTT_PERS_HIGH_PRIEST:
        p_mistat->SP.EnemiesPersuaded--;
        break;
    case SubTT_PERS_BRIEFCASE_M:
    case SubTT_PERS_WHITE_BRUN_F:
    case SubTT_PERS_SCIENTIST:
    case SubTT_PERS_SHADY_M:
    case SubTT_PERS_WHIT_BLOND_F:
    case SubTT_PERS_LETH_JACKT_M:
    case SubTT_PERS_FAST_BLOND_F:
        p_mistat->SP.CivsPersuaded--;
        break;
    case SubTT_PERS_MERCENARY:
    case SubTT_PERS_MECH_SPIDER:
    case SubTT_PERS_POLICE:
        p_mistat->SP.SecurityPersuaded--;
        break;
    default:
        break;
    }
}

int stats_mp_count_players_agents_killed(PlayerIdx plyr)
{
    struct MissionStatus *p_mistat;
    int n;
    PlayerIdx k;

    n = 0;
    p_mistat = &mission_status[plyr];
    for (k = 0; k < PLAYERS_LIMIT; k++)
    {
        if (unkn2_names[k][0] == '\0')
            continue;

        n += p_mistat->MP.AgentsKilled[k];
    }
    return n;
}

int stats_mp_count_net_players_agents_kills(PlayerIdx plyr)
{
    struct MissionStatus *p_mistat;
    int n;
    PlayerIdx k;

    n = 0;
    p_mistat = &mission_status[plyr];
    for (k = 0; k < PLAYERS_LIMIT; k++)
    {
        if (unkn2_names[k][0] == '\0')
            continue;

        if (k == plyr)
            n -= p_mistat->MP.AgentsKilled[k];
        else
            n += p_mistat->MP.AgentsKilled[k];
    }
    return n;
}

void stats_mp_add_player_kills_player_agent(PlayerIdx plyr_kil, PlayerIdx plyr_vic)
{
    struct MissionStatus *p_mistat;
    p_mistat = &mission_status[plyr_kil];
    p_mistat->MP.AgentsKilled[plyr_vic]++;
}

void stats_mp_add_person_kills_person(ThingIdx killer, ThingIdx victim)
{
    struct Thing *p_killer;
    struct Thing *p_victim;

    if ((killer == 0) || (victim == 0))
        return;

    p_killer = &things[killer];
    p_victim = &things[victim];

    if (((p_victim->Flag & TngF_PlayerAgent) != 0) &&
      ((p_killer->Flag & TngF_PlayerAgent) != 0))
    {
        PlayerIdx plyr_kil, plyr_vic;

        plyr_vic = (p_victim->U.UPerson.ComCur & 0x1C) >> 2;
        plyr_kil = (p_killer->U.UPerson.ComCur & 0x1C) >> 2;
        stats_mp_add_player_kills_player_agent(plyr_kil, plyr_vic);
    }
}
/******************************************************************************/
