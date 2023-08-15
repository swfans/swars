/******************************************************************************/
// Syndicate Wars Port, source port of the classic strategy game from Bullfrog.
/******************************************************************************/
/** @file lvobjctv.c
 *     Routines for level objectives handling.
 * @par Purpose:
 *     Implement functions for handling and checking level objectives.
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
#include "lvobjctv.h"

#include <string.h>
#include <stdlib.h>
#include "bffile.h"
#include "bfmemory.h"
#include "bfmemut.h"
#include "bfini.h"
#include "thing.h"
#include "game.h"
#include "swlog.h"
/******************************************************************************/

enum ObjectiveDefFlags {
    ObDF_None = 0x0000,
    // What to place in Thing/UniqueId
    ObDF_ReqGroup = 0x0001,
    ObDF_ReqThing = 0x0002, // May use Thing on UniqueId, depending on LevelNo and type; so set both
    ObDF_ReqCount = 0x0004,
    // What to place in Coord/Radius
    ObDF_ReqCoord = 0x0010,
    ObDF_ReqRadius = 0x0020,
    // What to place in Arg2
    ObDF_ReqAmount = 0x0040,
    ObDF_ReqSecTng = 0x0080, // TODO this can cause bugs, use only Thing for thing index
    ObDF_ReqSecGrp = 0x0100,
};

struct ObjectiveDef {
    const char *CmdName;
    const char *DefText;
    ushort Flags;
};

struct ObjectiveDef objectv_defs[] = {
    /* Unreachable. */
    {"GAME_OBJ_NONE",		"DO NOTHING",		ObDF_None },
    /* Require the target person to reach DEAD state. */
    {"GAME_OBJ_P_DEAD",		"ASSASSINATE",		ObDF_ReqThing },
    /* Require whole group to be neutralized. */
    {"GAME_OBJ_ALL_G_DEAD",	"ELIMINATE GROUP",	ObDF_ReqGroup },
    /* Require at least specified amount of group members to reach DEAD state. */
    {"GAME_OBJ_MEM_G_DEAD",	"KILL GROUP MEM",	ObDF_ReqGroup|ObDF_ReqAmount },
    /* Unreachable. Require person near? */
    {"GAME_OBJ_P_NEAR",		"RENDEZVOUS",		ObDF_ReqThing|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Unreachable. Require specified amount of group members near? */
    {"GAME_OBJ_MEM_G_NEAR",	"RENDEZVOUS2",		ObDF_ReqGroup|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require the target person to be within given radius around given coordinates. */
    {"GAME_OBJ_P_ARRIVES",	"GOTO LOCATION",	ObDF_ReqThing|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require at least specified amount of group members to be within radius around given coords. */
    {"GAME_OBJ_MEM_G_ARRIVES", "GOTO LOCATION", ObDF_ReqGroup|ObDF_ReqCoord|ObDF_ReqRadius|ObDF_ReqAmount },
    /* Require all of group members to be within radius around given coords. */
    {"GAME_OBJ_ALL_G_ARRIVES", "ALL GOTO LOCATION", ObDF_ReqGroup|ObDF_ReqCoord|ObDF_ReqRadius },
    /* Require target person to be within the group belonging to local player. */
    {"GAME_OBJ_PERSUADE_P",	"PERSUADE",			ObDF_ReqThing },
    /* Require at least specified amount of group members to be within the local player group. */
    {"GAME_OBJ_PERSUADE_MEM_G", "PERSUADE GANG MEM", ObDF_ReqGroup|ObDF_ReqAmount },
    /* Require all of group members to be persuaded. */
    {"GAME_OBJ_PERSUADE_ALL_G", "PERSUADE ALL GANG", ObDF_ReqGroup },
    /* Require specified amount of game turns to pass. */
    {"GAME_OBJ_TIME",		"TIMER",			ObDF_ReqCount },
    /* Require specified carried item to change owner to a person belonging to local player. */
    {"GAME_OBJ_GET_ITEM",	"COLLECT ITEM",		ObDF_ReqThing },
    /* Unreachable. Require specified item to be used? */
    {"GAME_OBJ_USE_ITEM",	"USE ITEM",			ObDF_ReqThing },
    /* Unreachable. Require acquiring specified amount of funds? */
    {"GAME_OBJ_FUNDS",		"GET BULLION",		ObDF_None },
    /* Require given thing to have DESTROYED flag set.
     * Coords need to be provided, rather than UniqueId, to find the object thing in case it changed. */
    {"GAME_OBJ_DESTROY_OBJECT", "DESTROY BUILDING", ObDF_ReqThing|ObDF_ReqCoord },
    /** Require the target person to either be DESTROYED or change owner to local player group. */
    {"GAME_OBJ_PKILL_P",	"NEUTRALISE",		ObDF_ReqThing },
    /* Require FIRST group member to either be DESTROYED or change owner to local player group.
      Suspicious implementation, looks like copy-paste error. */
    {"GAME_OBJ_PKILL_G",	"NEUTRALISE MEM",	ObDF_ReqGroup|ObDF_ReqAmount },
    /* Require all of group members to either be DESTROYED or change owner to local player group. */
    {"GAME_OBJ_PKILL_ALL_G", "NEUTRALISE G",	ObDF_ReqGroup },
    /* Unreachable. Require using P.A. Network? */
    {"GAME_OBJ_USE_PANET",	"USE P.A.NET",		ObDF_None },
    /* Unreachable. */
    {"GAME_OBJ_UNUSED_21",	"UNEXPECT 21",		ObDF_None },
    /* Fail if any of group members reach DEAD state. The only negative objective. */
    {"GAME_OBJ_PROTECT_G",	"PROTECT GROUP",	ObDF_ReqGroup },
    /* Require all of group members to change owner to specified person. */
    {"GAME_OBJ_P_PERS_G",	"PEEP PERSUADE ALL", ObDF_ReqGroup|ObDF_ReqSecTng },
    /* Require all of group members to either be DEAD or within specified vehicle. */
    {"GAME_OBJ_ALL_G_USE_V", "USE VEHICLE",		ObDF_ReqThing|ObDF_ReqSecGrp },
    /* Unreachable. */
    {"GAME_OBJ_UNUSED_25",	"UNEXPECT 25",		ObDF_None },
    /* Unreachable. */
    {"GAME_OBJ_UNUSED_26",	"UNEXPECT 26",		ObDF_None },
    {NULL,					NULL,				ObDF_None },
};

/* deprecated */
const char *gameobjctv_names[] = {
    "GAME_OBJ_NONE",
    "GAME_OBJ_P_DEAD",
    "GAME_OBJ_ALL_G_DEAD",
    "GAME_OBJ_MEM_G_DEAD",
    "GAME_OBJ_P_NEAR",
    "GAME_OBJ_MEM_G_NEAR",
    "GAME_OBJ_P_ARRIVES",
    "GAME_OBJ_MEM_G_ARRIVES",
    "GAME_OBJ_ALL_G_ARRIVES",
    "GAME_OBJ_PERSUADE_P",
    "GAME_OBJ_PERSUADE_MEM_G",
    "GAME_OBJ_PERSUADE_ALL_G",
    "GAME_OBJ_TIME",
    "GAME_OBJ_GET_ITEM",
    "GAME_OBJ_USE_ITEM",
    "GAME_OBJ_FUNDS",
    "GAME_OBJ_DESTROY_OBJECT",
    "GAME_OBJ_PKILL_P",
    "GAME_OBJ_PKILL_G",
    "GAME_OBJ_PKILL_ALL_G",
    "GAME_OBJ_USE_PANET",
    "GAME_OBJ_UNUSED_21",
    "GAME_OBJ_PROTECT_G",
    "GAME_OBJ_P_PERS_G",
    "GAME_OBJ_ALL_G_USE_V",
    "GAME_OBJ_UNUSED_25",
    "GAME_OBJ_UNUSED_26",
};

enum ObjectiveConfigParam {
    ObvP_Thing = 1,
    ObvP_Group,
    ObvP_Count,
    ObvP_UniqueID,
    ObvP_Coord,
    ObvP_Radius,
    ObvP_Amount,
    ObvP_SecGroup,
    ObvP_SecThing,
    ObvP_Arg2,
    ObvP_StringIndex,
    ObvP_Flags,
    ObvP_TextId,
};

const struct TbNamedEnum missions_conf_objective_params[] = {
  {"Thing",			ObvP_Thing},
  {"Group",			ObvP_Group},
  {"Count",			ObvP_Count},
  {"UniqueID",		ObvP_UniqueID},
  {"Coord",			ObvP_Coord},
  {"Radius",		ObvP_Radius},
  {"Amount",		ObvP_Amount},
  {"SecGroup",		ObvP_SecGroup},
  {"SecThing",		ObvP_SecThing},
  {"Arg2",			ObvP_Arg2},
  {"StringIndex",	ObvP_StringIndex},
  {"Flags",			ObvP_Flags},
  {"TextId",		ObvP_TextId},
  {NULL,			0},
};


int add_used_objective(long mapno, long levelno)
{
    struct Objective *p_objectv;
    int objectv;

    objectv = next_used_objective;
    next_used_objective++;

    p_objectv = &game_used_objectives[objectv];
    LbMemorySet(p_objectv, 0, sizeof(struct Objective));
    p_objectv->Type = 0;
    p_objectv->Pri = 0;
    p_objectv->Map = mapno;
    p_objectv->Level = (levelno - 1) % 15 + 1;
    p_objectv->Status = 0;

    return objectv;
}

ubyte fix_single_objective(struct Objective *p_objectv, ushort objectv, const char *srctext)
{
#if 0
    ubyte ret;
    asm volatile ("call ASM_fix_single_objective\n"
        : "=r" (ret) : "a" (p_objectv));
    return ret;
#else
    short thing;
    ubyte ret;

    if (p_objectv->Level != (current_level - 1) % 15 + 1)
        return 1;
    if (p_objectv->Map != current_map)
        return 1;

    ret = 0;
    switch (p_objectv->Type) // TODO fix parameters based on objectv_defs[] flags
    {
    case GAME_OBJ_P_DEAD:
    case GAME_OBJ_P_ARRIVES:
    case GAME_OBJ_PERSUADE_P:
    case GAME_OBJ_GET_ITEM:
    case GAME_OBJ_PKILL_P:
        if (p_objectv->Thing > 0)
        {
            thing = search_things_for_index(p_objectv->Thing);
            if (thing <= 0) {
                thing = 0;
            }
            else if (thing > 0) {
                struct Thing *p_thing = &things[thing];
                if (p_thing->U.UPerson.UniqueID != p_objectv->UniqueID)
                    thing = 0;
            }
            if (thing == 0) {
                thing = search_things_for_uniqueid(p_objectv->UniqueID, 1);
            }
        }
        else
        {
            thing = search_things_for_index(p_objectv->Thing);
            if (thing <= 0) {
                struct SimpleThing *p_sthing = &sthings[thing];
                if (p_sthing->UniqueID != p_objectv->UniqueID)
                    thing = 0;
            }
            else if (thing > 0) {
                thing = 0;
            }
            if (thing == 0) {
                thing = search_things_for_uniqueid(p_objectv->UniqueID, 0);
            }
        }
        if (thing != 0) {
            p_objectv->Thing = thing;
            ret = 2;
        } else {
            LOGWARN("OBJV-%s-%d type=%d target Thing%d UID=%d not found",
              srctext, objectv, (int)p_objectv->Type,
              (int)p_objectv->Thing, (int)p_objectv->UniqueID);
        }
        break;
    case GAME_OBJ_ALL_G_DEAD:
    case GAME_OBJ_MEM_G_DEAD:
    case GAME_OBJ_P_NEAR:
    case GAME_OBJ_MEM_G_NEAR:
    case GAME_OBJ_MEM_G_ARRIVES:
    case GAME_OBJ_ALL_G_ARRIVES:
    case GAME_OBJ_PERSUADE_MEM_G:
    case GAME_OBJ_PERSUADE_ALL_G:
    case GAME_OBJ_TIME:
    case GAME_OBJ_USE_ITEM:
    case GAME_OBJ_FUNDS:
    case GAME_OBJ_PKILL_G:
    case GAME_OBJ_PKILL_ALL_G:
    default:
        break;
    case GAME_OBJ_DESTROY_OBJECT:
        thing = find_nearest_object2(p_objectv->X, p_objectv->Z, 0);
        if (thing != 0) {
            p_objectv->Thing = thing;
            ret = 2;
        } else {
            LOGWARN("OBJV-%s-%d type=%d target Thing%d at (%d,%d) not found",
              srctext, objectv, (int)p_objectv->Type,
              (int)p_objectv->Thing, (int)p_objectv->X, (int)p_objectv->Z);
        }
        break;
    }
    return ret;
#endif
}

void snprint_objective(char *buf, ulong buflen, ushort objectv)
{
    struct Objective *p_objectv;
    struct ObjectiveDef *p_odef;
    char *s;
    ubyte nparams;

    p_objectv = &game_used_objectives[objectv];
    p_odef = &objectv_defs[p_objectv->Type];

    s = buf;

    sprintf(s, "P%02d = %s( ", (int)p_objectv->Pri, p_odef->CmdName);
    s += strlen(s);
    nparams = 0;

    if ((p_odef->Flags & ObDF_ReqGroup) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Group(%hd)", p_objectv->Thing);
        s += strlen(s);
        nparams++;
    } else if ((p_odef->Flags & ObDF_ReqCount) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Count(%hd)", p_objectv->Thing);
        s += strlen(s);
        nparams++;
    } else if (((p_odef->Flags & ObDF_ReqThing) != 0) ||
      (p_objectv->Thing != 0) || (p_objectv->UniqueID != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Thing(%hd,%hu)", p_objectv->Thing, p_objectv->UniqueID);
        s += strlen(s);
        nparams++;
    }
    // Unexpected to have unique id withiut thing
    if (((p_odef->Flags & ObDF_ReqThing) == 0) && (p_objectv->UniqueID != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "UniqueID(%hu)", p_objectv->UniqueID);
        s += strlen(s);
    }

    if (((p_odef->Flags & ObDF_ReqCoord) != 0) ||
      (p_objectv->X|p_objectv->Y|p_objectv->Z) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Coord(%hd,%hd,%hd)", p_objectv->X, p_objectv->Y, p_objectv->Z);
        s += strlen(s);
        nparams++;
    }
    if (((p_odef->Flags & ObDF_ReqRadius) != 0) ||
      (p_objectv->Radius != 0)) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Radius(%hd)", p_objectv->Radius);
        s += strlen(s);
        nparams++;
    }
    if ((p_odef->Flags & ObDF_ReqAmount) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Amount(%d)", (int)p_objectv->Arg2);
        s += strlen(s);
    } else if ((p_odef->Flags & ObDF_ReqSecGrp) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "SecGroup(%d)", (int)p_objectv->Arg2);
        s += strlen(s);
    } else if ((p_odef->Flags & ObDF_ReqSecTng) != 0) {
        // TODO storing a ThingOffset in 8-bit value is risky; when possible, switch Arg2 with Thing
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "SecThing(%d)", (int)p_objectv->Arg2);
        s += strlen(s);
    } else if (p_objectv->Arg2 != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Arg2(%d)", (int)p_objectv->Arg2);
        s += strlen(s);
    }
    if (p_objectv->StringIndex != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "StringIndex(%hu)", p_objectv->StringIndex);
        s += strlen(s);
    }
    if (p_objectv->Flags != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "Flags(%hu)", p_objectv->Flags);
        s += strlen(s);
        nparams++;
    }
    if (p_objectv->ObjText != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "TextId(%d)", (int)p_objectv->ObjText);
        s += strlen(s);
    }
    if ((p_objectv->field_1B[0]|p_objectv->field_1B[1]|p_objectv->field_1B[2]|p_objectv->field_1B[3]) != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "field_1B(%d,%d,%d,%d)", (int)p_objectv->field_1B[0],
          (int)p_objectv->field_1B[1], (int)p_objectv->field_1B[2], (int)p_objectv->field_1B[3]);
        s += strlen(s);
    }
    if (p_objectv->field_1F != 0) {
        if (nparams) { sprintf(s, ", "); s += strlen(s); }
        sprintf(s, "field_1F(%d)", (int)p_objectv->field_1F);
        s += strlen(s);
    }

    snprintf(s, buflen - (s-buf), " )");
}

void save_objective_chain_conf(TbFileHandle fh, ushort objectv_head, char *buf, ulong buflen)
{
    ushort objectv;

    objectv = 0;
    while (objectv != objectv_head)
    {
        { // Go backwards through single-directional chain
            ushort nxobjectv;
            nxobjectv = objectv_head;
            while (game_used_objectives[nxobjectv].Next != objectv)
                nxobjectv = game_used_objectives[nxobjectv].Next;
            objectv = nxobjectv;
        }

        snprint_objective(buf, buflen, objectv);
        strncat(buf, "\n", buflen);
        LbFileWrite(fh, buf, strlen(buf));
    }
    if (objectv_head == 0) {
        sprintf(buf, "; no objectives defined\n");
        LbFileWrite(fh, buf, strlen(buf));
    }
}

int tokenize_script_func(char *olist[], char *obuf, const char *ibuf, long ibuflen)
{
    TbBool in_quotes, token_end, parse_end;
    int li;
    int opos, pos, in_parath;

    li = 0;
    pos = 0;
    in_quotes = false;
    token_end = true;
    parse_end = false;
    in_parath = 0;
    if (pos >= ibuflen) return 0;
    // Skipping spaces after previous parameter
    while ((ibuf[pos] == ' ') || (ibuf[pos] == '\t'))
    {
        pos++;
        if (pos >= ibuflen) return 0;
    }
    if (ibuf[pos] == '\"') {
        in_quotes = true;
        pos++;
        if (pos >= ibuflen) return 0;
    }
    opos = 0;
    while (!parse_end)
    {
        if (token_end) {
            olist[li] = &obuf[opos];
            li++;
            token_end = false;
        }
        for (; pos < ibuflen; pos++)
        {
            if (in_quotes) {
                if ((ibuf[pos] == '\"') ||
                     (ibuf[pos] == '\r') ||
                     (ibuf[pos] == '\n')) {
                    in_quotes = false;
                    if (in_parath == 0) {
                        token_end = true;
                        continue; // do not copy quote to output
                    }
                }
            } else if (ibuf[pos] == '\"') {
                in_quotes = true;
                if (in_parath == 0)
                    continue; // do not copy quote to output
            } else if (ibuf[pos] == '(') {
                in_parath++;
                if (in_parath == 1) {
                    token_end = true;
                    // do not copy first parenthesis to output
                    continue;
                }
            } else if (ibuf[pos] == ')') {
                in_parath--;
                if (in_parath == 0) {
                    // do not copy last parenthesis to output, and end here
                    parse_end = true;
                    break;
                }
            } else if (in_parath < 2) {
                if (ibuf[pos] == ',') {
                    token_end = true;
                    continue;
                } else if ((ibuf[pos] == ' ') ||
                     (ibuf[pos] == '\t') ||
                     ((uchar)ibuf[pos] < 7)) {
                    continue;
                } else if ((ibuf[pos] == '\r') ||
                     (ibuf[pos] == '\n')) {
                    parse_end = true;
                    break;
                } else if (token_end) {
                    // we found a char to include in a token, but that's a new one
                    break;
                }
            }
            obuf[opos] = ibuf[pos];
            opos++;
            if (pos >= ibuflen) {
                opos++;
                parse_end = true;
                break;
            }
        }
        obuf[opos] = '\0';
        opos++;
    }
    olist[li] = NULL;
    return li;
}

int parse_objective_param(struct Objective *p_objectv, const char *buf, long buflen)
{
    char *toklist[16];
    char tokbuf[128];
    int i;

    LbMemorySet(toklist, 0, sizeof(toklist));
    i = tokenize_script_func(toklist, tokbuf, buf, buflen);
    if (i < 2) {
        LOGWARN("Objective parameter consists of less than 2 tokens.");
        return -1;
    }

    // Finding parameter number
    i = 0;
    while (1)
    {
        const struct TbNamedEnum *param;

        if (missions_conf_objective_params[i].name == NULL) {
            i = -1;
            break;
        }
        param = &missions_conf_objective_params[i];
        if (strcasecmp(toklist[0], param->name) == 0) {
            i = param->num;
            break;
        }
        i++;
    }
    switch (i)
    {
    case ObvP_Group:
    case ObvP_Count:
        p_objectv->Thing = atoi(toklist[1]);
        break;
    case ObvP_Thing:
        if (toklist[2] == NULL)  {
            LOGWARN("Objective parameter \"%s\" requires 2 numbers.", toklist[0]);
            return -1;
        }
        p_objectv->Thing = atoi(toklist[1]);
        p_objectv->UniqueID = atoi(toklist[2]);
        break;
    case ObvP_UniqueID:
        p_objectv->UniqueID = atoi(toklist[1]);
        break;
    case ObvP_Coord:
        if (toklist[3] == NULL)  {
            LOGWARN("Objective parameter \"%s\" requires 3 numbers.", toklist[0]);
            return -1;
        }
        p_objectv->X = atoi(toklist[1]);
        p_objectv->Y = atoi(toklist[2]);
        p_objectv->Z = atoi(toklist[3]);
        break;
    case ObvP_Radius:
        p_objectv->Radius = atoi(toklist[1]);
        break;
    case ObvP_Amount:
    case ObvP_SecGroup:
    case ObvP_SecThing:
    case ObvP_Arg2:
        p_objectv->Arg2 = atoi(toklist[1]);
        break;
    case ObvP_StringIndex:
        p_objectv->StringIndex = atoi(toklist[1]);
        break;
    case ObvP_Flags:
        p_objectv->Flags = atoi(toklist[1]);
        break;
    case ObvP_TextId:
        p_objectv->ObjText = atoi(toklist[1]);
        break;
    default:
        LOGWARN("Objective parameter name \"%s\" not recognized.", toklist[0]);
        return -1;
    }
    return 1;
}

int parse_next_used_objective(const char *buf, long buflen, long pri, long mapno, long levelno)
{
    struct ObjectiveDef *p_odef;
    struct Objective *p_objectv;
    char *toklist[32];
    char tokbuf[256];
    int i, objectv;

    tokenize_script_func(toklist, tokbuf, buf, buflen);

    // Finding command number
    i = 0;
    while (objectv_defs[i].CmdName != NULL)
    {
        p_odef = &objectv_defs[i];
        if (strcasecmp(toklist[0], p_odef->CmdName) == 0)
            break;
        i++;
    }
    p_odef = &objectv_defs[i];
    if (p_odef->CmdName == NULL) {
        LOGWARN("Objective name not recognized.");
        return -1;
    }
    objectv = add_used_objective(mapno, levelno);
    p_objectv = &game_used_objectives[objectv];
    p_objectv->Type = i;
    p_objectv->Pri = pri;

    for (i = 1; toklist[i] != NULL; i++)
    {
        parse_objective_param(p_objectv, toklist[i], sizeof(tokbuf) - (toklist[i] - tokbuf) );
    }

    return objectv;
}

/******************************************************************************/
