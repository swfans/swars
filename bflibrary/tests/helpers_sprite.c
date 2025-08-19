/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet, Genewars or Dungeon Keeper.
/******************************************************************************/
/** @file helpers_sprite.c
 *     Implementation of sample sprite data creation for tests.
 * @par Purpose:
 *     Unknown.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     25 Nov 2024 - 21 Mar 2025
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include <string.h>
#include "helpers_sprite.h"
#include "bfanywnd.h"
#include "bfpalette.h"
#include "bfutility.h"
#include "bftstlog.h"

static struct TbAnyWindow sprites1_list[] = {
    {  0,  0, 0, 0, NULL,}, // Torso walk blue
    {  2,  8,11,16, NULL,},
    { 18,  8,11,16, NULL,},
    { 34,  8,11,16, NULL,},
    { 50,  8,14,16, NULL,},
    { 66,  8,12,16, NULL,},
    { 82,  8,11,16, NULL,},
    { 98,  8,10,16, NULL,},
    {114,  8,13,16, NULL,},
    {129,  8,14,16, NULL,},
    {146,  8,12,16, NULL,},
    {162,  8,12,16, NULL,},
    {178,  8,13,16, NULL,},
    {193,  8,12,16, NULL,},
    {209,  8,12,16, NULL,},
    {226,  8,13,16, NULL,},
    {241,  8,13,16, NULL,},
    {256,  8,14,16, NULL,},
    {273,  8,12,16, NULL,},
    {290,  8,11,16, NULL,},
    {305,  8,12,16, NULL,},
    {  2, 32,12,16, NULL,},
    { 18, 32,12,16, NULL,},
    { 34, 32,10,16, NULL,},
    { 49, 32,13,16, NULL,},
    { 66, 32,12,16, NULL,},
    { 82, 32,12,16, NULL,},
    { 98, 32,11,16, NULL,},
    {113, 32,14,16, NULL,},
    {130, 32,13,16, NULL,},
    {146, 32,12,16, NULL,},
    {161, 32,12,16, NULL,},
    {178, 32,12,16, NULL,},
    {194, 32,11,16, NULL,}, // Torso walk red
    {210, 32,11,16, NULL,},
    {226, 32,11,16, NULL,},
    {242, 32,14,16, NULL,},
    {258, 32,12,16, NULL,},
    {274, 32,11,16, NULL,},
    {290, 32,10,16, NULL,},
    {306, 32,13,16, NULL,},
    {  1, 56,14,16, NULL,},
    { 18, 56,12,16, NULL,},
    { 34, 56,12,16, NULL,},
    { 50, 56,14,16, NULL,},
    { 65, 56,12,16, NULL,},
    { 81, 57,12,15, NULL,},
    { 99, 55,12,17, NULL,},
    {113, 57,13,15, NULL,},
    {129, 58,14,14, NULL,},
    {145, 57,12,15, NULL,},
    {162, 58,10,14, NULL,},
    {176, 58,12,14, NULL,},
    {194, 58,13,14, NULL,},
    {210, 58,12,14, NULL,},
    {227, 57, 9,15, NULL,},
    {240, 57,13,15, NULL,},
    {258, 57,11,15, NULL,},
    {275, 57,11,15, NULL,},
    {290, 55,11,17, NULL,},
    {304, 57,13,15, NULL,},
    {  3, 80,12,16, NULL,},
    { 18, 80,12,16, NULL,},
    { 33, 80,12,16, NULL,},
    { 50, 81,12,15, NULL,},
    { 65, 83,15,13, NULL,}, // Torso drowning blue
    { 81, 83,15,13, NULL,},
    { 97, 83,15,13, NULL,}, // Torso drowning red
    {113, 83,15,13, NULL,},
    {130, 77,12,19, NULL,}, // Burned body to ash
    {146, 77,12,19, NULL,},
    {162, 77,12,19, NULL,},
    {178, 77,12,19, NULL,},
    {192, 80,14,16, NULL,},
    {208, 84,14,12, NULL,},
    {223, 88,18, 8, NULL,},
    {242, 91,14, 5, NULL,},
    {257, 93,13, 3, NULL,},
    {274, 80,12,16, NULL,},
    {222, 88, 7, 8, NULL,}, // Small light
    {310, 91, 3, 5, NULL,}, // Flame
    {  5,104, 6,16, NULL,},
    { 21,110, 6,10, NULL,},
    { 36,108, 8,12, NULL,},
    { 52,108, 8,12, NULL,},
    { 68,105, 9,15, NULL,},
    { 82, 99,14,21, NULL,},
    { 99,116,10, 4, NULL,}, // Floor ring
    {114,113,13, 7, NULL,},
    {133,116, 8, 4, NULL,}, // Flag blue
    {149,115, 8, 5, NULL,},
    {165,116, 8, 4, NULL,}, // Flag red
    {181,115, 8, 5, NULL,},
    {193,107,15,13, NULL,}, // Golden wings artifact
    {210,107,13,13, NULL,}, // Medusa head
    {227,105,12,15, NULL,}, // Torso walk blue
    {245,114, 4, 6, NULL,}, // Hand up
    {262,112, 3, 8, NULL,},
    {278,110, 3,10, NULL,},
    {292,113, 4, 7, NULL,},
    {308,115, 4, 5, NULL,},
    {  6,137, 2, 7, NULL,},
    { 18,130,11,14, NULL,}, // Torso front no hands blue
    { 34,130,11,14, NULL,}, // Torso front no hands red
    { 49,124,12,20, NULL,}, // Person in red salto
    { 64,127,16,17, NULL,},
    { 80,122,16,22, NULL,},
    { 96,122,13,22, NULL,},
    {112,122,16,22, NULL,},
    {129,122,15,22, NULL,},
    {144,122,16,22, NULL,},
    {161,122, 8,22, NULL,},
    {178,125, 9,19, NULL,},
    {196,131, 8,13, NULL,}, // Red something L
    {210,129,12,15, NULL,}, // Torso punch red
    {226,128,13,16, NULL,}, // Torso swamp run blue
    {244,137,12, 7, NULL,},
    {263,138, 4, 6, NULL,}, // Hand
    {280,136, 3, 8, NULL,},
    {293,131, 8,13, NULL,}, // Red something R
    {307,132,10,12, NULL,}, // Puff smoke
    {  1,147,12,21, NULL,}, // Person in blue salto
    { 16,151,16,17, NULL,},
    { 32,146,16,22, NULL,},
    { 48,145,13,23, NULL,},
    { 64,145,17,23, NULL,},
    { 81,146,15,22, NULL,},
    { 96,147,16,21, NULL,},
    {112,156,10,12, NULL,}, // Shield
    {128,147,16,21, NULL,}, // Skeleton
    {144,147,13,21, NULL,}, // Smoke mushroom
    {161,147,14,21, NULL,},
    {177,147,12,21, NULL,},
    {192,148,14,20, NULL,},
    {210,152,12,16, NULL,}, // Firely ground burn
    {224,148,15,20, NULL,},
    {240,147,15,21, NULL,},
    {256,147,15,21, NULL,},
    {273,160,15, 8, NULL,}, // Short cloud with tail
    {292,160,12, 8, NULL,},
    {309,160, 9, 8, NULL,},
    {  0,184,15, 9, NULL,}, // Short cloud
    { 17,184,13, 9, NULL,},
    { 34,187,10, 6, NULL,},
    { 51,189, 6, 4, NULL,},
    { 63,186,18, 7, NULL,}, // Dust near ground
    { 82,187,14, 6, NULL,},
    { 97,191,15, 2, NULL,},
    {112,191,11, 2, NULL,},
    {129,191, 7, 2, NULL,},
    {150,180, 5,13, NULL,}, // Spikes
    {166,182, 5,11, NULL,},
    {182,185, 5, 8, NULL,},
    {198,187, 5, 6, NULL,},
    {214,189, 5, 4, NULL,},
    {226,188, 8, 5, NULL,}, // Head helmet red
    {242,188, 8, 5, NULL,},
    {258,188, 8, 5, NULL,}, // Head helmet blue
    {274,188, 8, 5, NULL,},
    {292,179, 9,14, NULL,}, // Red over gold
    {310,180, 8,13, NULL,},
    {  1,207,16,12, NULL,}, // Body sinking blue
    { 17,207,16,12, NULL,},
    { 33,207,16,12, NULL,},
    { 49,205,15,14, NULL,},
    { 65,205,16,14, NULL,},
    { 83,200,14,19, NULL,},
    { 99,200,12,19, NULL,},
    {113,210,14, 9, NULL,}, // Guilotine
    {129,208,16,11, NULL,}, // Body sinking red
    {145,208,16,11, NULL,},
    {164,208, 7,11, NULL,}, // Torso armored block red
    {180,208, 9,11, NULL,},
    {196,207,10,12, NULL,},
    {209,210,14, 9, NULL,}, // Guilotine
    {225,210,14, 9, NULL,},
    {241,210,14, 9, NULL,},
    {261,214, 6, 5, NULL,}, // Pipe yellow
    {277,214, 6, 5, NULL,}, // Pipe green
    {293,214, 6, 5, NULL,}, // Pipe red
    {309,214, 6, 5, NULL,}, // Pipe blue
    {  0,220,14,27, NULL,}, // Pikeman in helmet walk
    { 16,220,14,27, NULL,},
    { 33,220,13,27, NULL,},
    { 49,220,13,27, NULL,},
    { 64,220,16,27, NULL,},
    { 80,220,14,27, NULL,},
    { 96,220,14,27, NULL,},
    {112,220,15,27, NULL,},
    {127,220,16,27, NULL,},
    {144,220,15,27, NULL,},
    {161,220,16,27, NULL,},
    {177,220,16,27, NULL,},
    {194,220,15,27, NULL,},
    {209,220,16,27, NULL,},
    {225,220,16,27, NULL,},
    {241,220,16,27, NULL,},
    {257,220,16,27, NULL,},
    {273,220,16,27, NULL,},
    {289,220,15,27, NULL,},
    {304,220,16,27, NULL,},
    {  1,254,14,25, NULL,},
    { 18,254,13,25, NULL,},
    { 33,254,14,25, NULL,},
    { 48,254,15,25, NULL,},
    { 65,252,15,27, NULL,},
    { 82,252,14,27, NULL,},
    { 98,252,13,27, NULL,},
    {114,252,14,27, NULL,},
    {129,251,15,28, NULL,},
    {145,251,12,28, NULL,},
    {161,251,15,28, NULL,},
    {177,251,15,28, NULL,},
    {192,251,16,28, NULL,},
    {208,253,14,26, NULL,}, // Barbarian in pants walk
    {224,253,13,26, NULL,},
    {242,253,11,26, NULL,},
    {258,253,13,26, NULL,},
    {273,253,12,26, NULL,},
    {289,253,12,26, NULL,},
    {305,253,13,26, NULL,},
    {  1,282,12,27, NULL,},
    { 17,282,11,27, NULL,},
    { 33,282,12,27, NULL,},
    { 48,282,15,27, NULL,},
    { 64,282,13,27, NULL,},
    { 80,282,14,27, NULL,},
    { 96,282,14,27, NULL,},
    {112,282,14,27, NULL,},
    {128,282,15,27, NULL,},
    {146,282,11,27, NULL,},
    {162,282,12,27, NULL,},
    {176,282,15,27, NULL,},
    {193,282,13,27, NULL,},
    {209,282,12,27, NULL,},
    {225,282,12,27, NULL,},
    {240,282,13,27, NULL,},
    {258,282,12,27, NULL,},
    {273,282,14,27, NULL,},
    {289,282,13,27, NULL,},
    {305,282,11,27, NULL,},
    {  1,315,13,27, NULL,},
    { 17,315,13,27, NULL,},
    { 32,315,14,27, NULL,},
    { 48,315,13,27, NULL,},
    { 66,315,12,27, NULL,},
    { 80,321,16,21, NULL,}, // Barbarian in pants victory
    { 96,313,16,29, NULL,},
    {113,313,14,29, NULL,}, // Teleport person
    {130,317,12,25, NULL,},
    {147,321, 9,21, NULL,},
    {165,326, 8,16, NULL,},
    {181,331, 6,11, NULL,},
    {198,336, 3, 6, NULL,},
    {208,328,15,14, NULL,}, // Teleport star
    {228,332,10,10, NULL,},
    {247,336, 7, 7, NULL,},
    {260,333, 9, 9, NULL,},
    {274,335,10,17, NULL,}, // Head with horned helmet
    {292,335,10,17, NULL,},
    {311,333, 6, 9, NULL,},
    {  4,365,10, 7, NULL,},
    { 21,365,11, 7, NULL,},
    { 40,364, 6, 8, NULL,},
    { 53,365,11, 7, NULL,},
    { 64,362,16,10, NULL,}, // Torso laying
    { 80,362,14,10, NULL,},
    { 98,359,14,13, NULL,}, // Torso legs up
    {113,362,10,10, NULL,},
    {131,359,13,13, NULL,},
    {145,362,10,10, NULL,},
    {160,357,14,15, NULL,}, // Person in helmet hands up
    {180,360,10,12, NULL,},
    {195,363,12, 9, NULL,},
    {207,363,12, 9, NULL,},
    {224,360,10,12, NULL,},
    {240,358,14,14, NULL,},
    {256,357,14,15, NULL,},
    {276,360,10,12, NULL,},
    {292,363,11, 9, NULL,},
    {303,363,11, 9, NULL,},
    {  0,396,10,12, NULL,},
    { 16,393,14,15, NULL,},
    { 32,393,16,15, NULL,}, // Dirt cracks
    { 48,383,16,25, NULL,}, // Statue
    { 64,395,10,13, NULL,},
    { 80,384,17,24, NULL,},
    { 97,390,15,18, NULL,},
    {112,393,16,15, NULL,}, // Walls
    {128,395,16,13, NULL,},
    {144,376,16,32, NULL,}, // Water wall
    {160,376,16,32, NULL,},
    {176,376,16,32, NULL,},
    {192,376,16,32, NULL,},
    {208,376,16,32, NULL,},
    {224,376,16,32, NULL,},
    {240,376,16,32, NULL,},
    {256,376,16,32, NULL,},
    {272,376,16,32, NULL,},
    {288,376,16,32, NULL,},
    {304,376,16,32, NULL,},
    {  0,409,16,32, NULL,},
    { 16,409,16,32, NULL,},
    { 32,409,16,32, NULL,},
    { 48,409,16,32, NULL,},
    { 64,409,16,32, NULL,},
    { 80,409,16,32, NULL,},
    { 96,409,16,32, NULL,},
    {112,409,16,32, NULL,},
    {128,409,16,32, NULL,},
    {144,409,16,32, NULL,},
    {160,409,16,32, NULL,},
    {176,409,16,32, NULL,},
    {192,409,16,32, NULL,},
    {210,426,10,15, NULL,}, // Torso squat blue
    {225,427,13,14, NULL,}, // Torso bored blue
    {242,426,10,15, NULL,}, // Torso squat red
    {258,427,13,14, NULL,}, // Torso bored red
    {274,432,15, 9, NULL,}, // Lava lakes
    {289,432,14, 9, NULL,},
    {307,432,13, 9, NULL,},
    {  0,448,13, 8, NULL,},
    { 17,448,14, 8, NULL,},
    { 31,448,15, 8, NULL,},
    { 51,448,13, 8, NULL,},
    { 64,448,13, 8, NULL,},
    { 82,448,14, 8, NULL,},
    { 96,448,14, 8, NULL,},
    {115,448,13, 8, NULL,},
    {128,448,13, 8, NULL,},
    {146,448,15, 8, NULL,},
    {161,448,15, 8, NULL,},
    {181,448,12, 8, NULL,},
    {193,448,12, 8, NULL,},
    {212,448,12, 8, NULL,},
    {229,448,12, 8, NULL,},
    {241,448,11, 8, NULL,},
    {256,448,14, 8, NULL,},
    {276,448,18, 8, NULL,},
    {289,448,13, 8, NULL,},
    {308,448,12, 8, NULL,},
    {  0,464,11, 8, NULL,},
    { 26,464,20, 8, NULL,},
    { 33,464,12, 8, NULL,},
    { 50,464,28, 8, NULL,},
    { 82,464,28, 8, NULL,},
    {112,467,12, 5, NULL,},
    {133,464,22, 8, NULL,},
    {162,464,14, 8, NULL,},
    {186,466,18, 6, NULL,},
    {213,464,23, 8, NULL,},
    {242,464,19, 8, NULL,},
    {278,464,24, 8, NULL,},
    {305,464,15, 8, NULL,},
    {  2,474,13,24, NULL,}, // Blonde woman walk
    { 19,474,10,24, NULL,},
    { 37,474, 8,24, NULL,},
    { 51,474,10,24, NULL,},
    { 66,474,12,24, NULL,},
    { 84,474, 9,24, NULL,},
    { 98,474,13,24, NULL,},
    {116,474, 9,24, NULL,},
    {132,474, 9,24, NULL,},
    {148,474, 9,24, NULL,},
    {161,474,15,24, NULL,},
    {180,474, 9,24, NULL,},
    {193,474,12,25, NULL,},
    {210,474,11,24, NULL,},
    {226,474,11,24, NULL,},
    {242,474,12,24, NULL,},
    {261,474, 9,24, NULL,},
    {274,474,10,24, NULL,},
    {290,474,13,24, NULL,},
    {307,474, 9,24, NULL,},
    {  2,501,12,24, NULL,},
    { 19,501, 9,24, NULL,},
    { 32,501,15,24, NULL,},
    { 52,501, 9,24, NULL,},
    { 67,501,10,24, NULL,},
    { 83,501, 8,24, NULL,},
    { 99,501,10,24, NULL,},
    {113,501,13,24, NULL,},
    {130,501,13,24, NULL,},
    {147,501,10,24, NULL,},
    {163,501,11,24, NULL,},
    {178,501,11,24, NULL,},
    {192,505,15,20, NULL,}, // Barbarian sinking
    {209,505,15,20, NULL,},
    {224,517,14, 8, NULL,}, // Lava vein
    {246,508, 5,17, NULL,}, // Purple lightning
    {259,509, 7,16, NULL,},
    {280,517, 6, 8, NULL,}, // Lava flyer
    {294,516, 7, 9, NULL,},
    {304,518, 6, 7, NULL,}, // Blue slim thing
    {  0,527, 6, 8, NULL,},
    { 15,527, 8, 8, NULL,},
    { 32,527, 8, 8, NULL,},
    { 48,527, 8, 8, NULL,},
    { 64,527, 6, 8, NULL,},
    { 81,527, 6, 8, NULL,},
    { 96,527, 7, 8, NULL,},
    {113,527, 4, 8, NULL,},
    {128,527, 2, 8, NULL,},
    {144,527, 4, 8, NULL,},
    {159,527, 8, 8, NULL,},
    {176,527, 6, 8, NULL,},
    {192,527, 8, 8, NULL,},
    {208,527, 8, 8, NULL,},
    {224,527, 6, 8, NULL,},
    {256,527, 7, 8, NULL,},
    {272,527, 3, 8, NULL,},
    {288,527, 3, 8, NULL,},
    {304,527, 4, 8, NULL,},
    {  0,538, 5, 8, NULL,},
    { 16,537, 5, 9, NULL,}, // Blue egg
    { 32,537, 5, 9, NULL,},
    { 48,537, 4, 9, NULL,},
    { 65,537,14, 9, NULL,}, // Vulture flying
    { 79,537,11, 9, NULL,},
    { 97,537,12, 9, NULL,},
    {112,537,12, 9, NULL,},
    {128,537,15, 9, NULL,},
    {144,537,10, 9, NULL,},
    {161,537,15, 9, NULL,},
    {176,537,15, 9, NULL,},
    {192,537,15, 9, NULL,},
    {208,537,10, 9, NULL,},
    {225,537,12, 9, NULL,},
    {242,537,12, 9, NULL,},
    {257,537,15, 9, NULL,},
    {274,537,10, 9, NULL,},
    {289,537,15, 9, NULL,},
    {304,537,15, 9, NULL,},
    {  0,548,15,27, NULL,}, // Barbarian standing
    { 16,548,15,27, NULL,}, // Barbarian burned to dust
    { 32,548,15,27, NULL,},
    { 48,548,15,27, NULL,},
    { 64,548,15,27, NULL,},
    { 80,548,15,27, NULL,},
    { 95,548,15,27, NULL,},
    {111,548,15,27, NULL,},
    {127,548,15,27, NULL,},
    {144,547,16,28, NULL,}, // Barbarian zapped
    {160,556,14,19, NULL,}, // Barbarian sinking
    {177,556,15,19, NULL,},
    {192,556,16,19, NULL,},
    {209,556,15,19, NULL,},
    {224,556,16,19, NULL,},
    {241,556,15,19, NULL,},
    {256,556,16,19, NULL,},
    {272,549,15,26, NULL,}, // Barbarian muscle pose
    {291,547,13,28, NULL,}, // Barbarian stand on hands
    {304,547,16,28, NULL,}, // Barbarian throwing pose
    {  0,576,16,18, NULL,}, // Red helmet person sinking
    { 16,576,15,18, NULL,},
    { 31,576,15,18, NULL,},
    { 47,576,15,18, NULL,},
    { 63,576,15,18, NULL,},
    { 78,576,15,18, NULL,},
    { 93,576,13,18, NULL,},
    {113,576,13,18, NULL,}, // Icons for user actions
    {129,576,14,18, NULL,},
    {148,576,11,18, NULL,},
    {162,576,11,18, NULL,},
    {177,576,14,18, NULL,},
    {192,576,16,18, NULL,},
    {209,576,14,18, NULL,},
    {225,576,15,18, NULL,},
    {240,576,14,18, NULL,},
    {262,589, 3, 5, NULL,}, // Steel armor part?
    {278,587, 5, 7, NULL,},
    {294,587, 6, 7, NULL,},
    {309,587, 8, 7, NULL,},
    {  5,604, 6, 7, NULL,}, // Blue dress blond hair woman head
    { 19,600, 8,11, NULL,},
    { 36,602, 7, 9, NULL,},
    { 51,605, 7, 6, NULL,},
    { 68,598, 7,13, NULL,},
    { 83,600, 8,11, NULL,},
    { 99,600, 9,11, NULL,},
    {115,601, 9,10, NULL,},
    {133,600, 8,11, NULL,},
    {150,598, 7,13, NULL,},
    {164,605, 7, 6, NULL,},
    {181,602, 7, 9, NULL,},
    {196,600, 8,11, NULL,},
    {212,604, 7, 7, NULL,},
    {229,604, 5, 7, NULL,},
    {245,604, 6, 7, NULL,}, // Blue dress orange hair woman head
    {259,600, 8,11, NULL,},
    {277,601, 7,10, NULL,},
    {295,605, 7, 6, NULL,},
    {307,598, 8,13, NULL,},
    {  5,616, 8,11, NULL,},
    { 19,617, 9,10, NULL,},
    { 35,616, 9,11, NULL,},
    { 53,616, 8,11, NULL,},
    { 68,614, 8,13, NULL,},
    { 85,620, 7, 7, NULL,},
    {101,616, 7,11, NULL,},
    {116,620, 6, 7, NULL,},
    {133,620, 5, 7, NULL,},
    {149,623, 5, 4, NULL,}, // Blond hair man head
    {165,622, 6, 5, NULL,},
    {181,621, 5, 6, NULL,},
    {197,622, 5, 5, NULL,},
    {214,621, 5, 6, NULL,},
    {229,622, 6, 5, NULL,},
    {246,623, 5, 4, NULL,},
    {262,623, 5, 4, NULL,},
    {277,623, 5, 4, NULL,}, // Yellow hair man head
    {293,621, 6, 6, NULL,},
    {310,621, 5, 6, NULL,},
    // TODO more sprited exist in the input bitmap, should be added here
};

void get_example_sprites_file_name(int sprfile_no, char *fname)
{
    switch (sprfile_no)
    {
    case 1:
        strcpy(fname, "referenc/spr_devpac-mans16.png");
        break;
    default:
        fname[0] = '\0';
        break;
    }
}

TbScreenMode get_example_sprites_screen_mode(int sprfile_no)
{
    char locstr[64];
    ulong img_width, img_height, img_bpp;
    ulong flags;

    switch (sprfile_no)
    {
    case 1:
        img_width = 320;
        img_height = 1024;
        img_bpp = 8;
        break;
    default:
        img_width = 4;
        img_height = 4;
        img_bpp = 8;
        break;
    }

    sprintf(locstr, "%lux%lux%lu", img_width, img_height, img_bpp);
    if (img_bpp <= 8)
        flags = Lb_VF_PALETTE;
    else
        flags = Lb_VF_RGBCOLOUR;
    return LbRegisterVideoMode(locstr, img_width, img_height, img_bpp, flags);
}

int get_example_sprites_total_count(int sprfile_no)
{
    int tot_sprites;

    switch (sprfile_no)
    {
    case 1:
        tot_sprites = sizeof(sprites1_list)/sizeof(sprites1_list[0]);
        break;
    default:
        tot_sprites = 0;
        break;
    }

    return tot_sprites;
}

int generate_example_sprites_from_screen(int sprfile_no, const ubyte *pal,
  ubyte *p_dat, TbSprite *p_tab)
{
    TbSprite *p_spr;
    ubyte *p_sprdt;
    struct TbAnyWindow *p_wnd;
    int tot_sprites, i;

    switch (sprfile_no)
    {
    case 1:
        p_wnd = &sprites1_list[0];
        break;
    default:
        p_wnd = NULL;
        break;
    }
    tot_sprites = get_example_sprites_total_count(sprfile_no);

    // Convert each area into a sprite
    p_spr = p_tab;
    p_sprdt = p_dat;
    for (i = 0; i < tot_sprites; i++)
    {
        int len;

        LbScreenSetGraphicsWindow(p_wnd->x, p_wnd->y, p_wnd->width, p_wnd->height);
        len = LbScreenSpriteEncode(p_spr, p_sprdt);
        p_sprdt += len;
        p_spr++;
        p_wnd++;
    }

    return p_sprdt - p_dat;
}

void palette_remap_to_screen(TbPixel *p_remap, const TbPixel *p_altpal)
{
    int i;
    for (i = 0; i < PALETTE_8b_COLORS; i++)
    {
        ubyte r, g, b;
        r = p_altpal[3*i + 0];
        g = p_altpal[3*i + 1];
        b = p_altpal[3*i + 2];
        p_remap[i] = LbPaletteFindColour(lbDisplay.Palette, r, g, b);
    }
}

/******************************************************************************/
