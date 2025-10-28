#!/usr/bin/python3
# -*- coding: utf-8 -*-
# vim:sw=4

""" Text WAD/IDX conversion tool for Syndicate Wars Fan Expansion.

Prepares WAD/IDX files accepted by game engine.
"""

# Copyright (C) 2023 Mefistotelis <mefistotelis@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

__version__ = "0.0.1"
__author__ = "Mefistotelis"
__license__ = "GPL"

import sys
import argparse
import os
import re
import io
import polib
import textwrap
import unicodedata

from ctypes import c_char, c_int, c_ubyte, c_ushort, c_uint, c_ulonglong, c_float
from ctypes import memmove, addressof, sizeof, Array, LittleEndianStructure


class WADIndexEntry(LittleEndianStructure):
    _pack_ = 1
    _fields_ = [
      ('Filename', c_char * 12),
      ('Offset', c_uint),
      ('Length', c_uint),
    ]


campaign_names = ['SYNDCT', 'CHURCH', 'PUNKS', 'COMM']

syndct_missions = {
   1 : (30,16,),  3 : (32,16,),  4 : (21,31,),  5 : (28,46,),  7 : (79,31,),  8 : (30,46,),  9 : (22,16,), 20 : (27,2,),
  21 : (10,1,),  22 : (1,1,),   23 : (3,1,),   24 : (5,1,),   25 : (6,1,),   26 : (7,1,),   28 : (8,1,),   29 : (2,1,),
  34 : (9,1,),   35 : (4,1,),   36 : (47,1,),  37 : (50,1,),  38 : (44,1,),  41 : (40,46,), 42 : (46,1,),  46 : (46,60,),
  49 : (35,1,),  50 : (26,1,),  55 : (1,15,),  56 : (10,15,), 57 : (28,15,), 58 : (32,15,), 59 : (36,1,),  63 : (27,3,),
  74 : (40,1,),  76 : (47,1,),  83 : (45,2,),  84 : (46,15,), 88 : (11,1,),  89 : (12,1,),  92 : (41,3,),  96 : (47,31,),
  97 : (47,15,),101 : (65,2,), 104 : (70,11,),105 : (47,11,),
}

church_missions = {
   2 : (25,16,),  6 : (28,31,), 10 : (21,46,), 11 : (22,2,),  12 : (25,46,), 13 : (30,2,),  14 : (20,16,), 15 : (32,2,),
  16 : (27,1,),  17 : (22,3,),  18 : (79,2,),  19 : (22,4,),  27 : (7,3,),   30 : (2,3,),   31 : (1,3,),   32 : (8,3,),
  33 : (7,5,),   39 : (50,47,), 40 : (41,1,),  43 : (41,47,), 44 : (45,1,),  45 : (44,2,),  47 : (46,14,), 48 : (35,2,),
  51 : (26,2,),  52 : (26,3,),  53 : (36,2,),  54 : (36,3,),  71 : (6,3,),   72 : (4,3,),   73 : (9,3,),   79 : (44,7,),
  85 : (3,2,),   86 : (4,15,),  87 : (44,3,),  98 : (50,2,), 100 : (46,2,), 102 : (65,3,), 108 : (3,10,),
}

weapon_mod_names_to_code = {
    "uzi" : 'UZI',
    "minigun" : 'MINIGUN',
    "electron mace" : 'ELLASER',
    "pulse laser" : 'LASER',
    "plasma lance" : 'BEAM',
    "launcher" : 'RAP',
    "nuclear grenade" : 'NUCLGREN',
    "persuadertron" : 'PERSUADER',
    "indoctrinator" : 'PERSUADER',
    "flamer" : 'FLAMER',
    "disrupter" : 'H2HTASER',
    "psycho gas" : 'CRAZYGAS',
    "knockout gas" : 'KOGAS',
    "ion mine" : 'ELEMINE',
    "high explosive" : 'EXPLMINE',
    "long range rifle" : 'LONGRANGE',
    "satellite rain" : 'AIRSTRIKE',
    "graviton gun" : 'QDEVASTATOR',
    "razor wire" : 'RAZORWIRE',
    "persuadertron ii" : 'PERSUADER2',
    "stasis field" : 'STASISFLD',
    "chromotap" : 'SOULGUN',
    "displacertron" : 'TIMEGUN',
    "cerberus iff" : 'CEREBUSIFF',
    "medikit" : 'MEDI1',
    "automedikit" : 'MEDI2',
    "trigger wire" : 'EXPLWIRE',
    "clone shield" : 'CLONESHLD',
    "legs 1" : 'LEGS1',
    "legs 2" : 'LEGS2',
    "legs 3" : 'LEGS3',
    "arms 1" : 'ARMS1',
    "arms 2" : 'ARMS2',
    "arms 3" : 'ARMS3',
    "arm 1" : 'ARMS1',
    "arm 2" : 'ARMS2',
    "arm 3" : 'ARMS3',
    "body 1" : 'CHEST1',
    "body 2" : 'CHEST2',
    "body 3" : 'CHEST3',
    "brain 1" : 'BRAIN1',
    "brain 2" : 'BRAIN2',
    "brain 3" : 'BRAIN3',
    "hard skin" : 'SKIN1',
    "flame skin" : 'SKIN2',
    "energy skin" : 'SKIN3',
    "stealth skin" : 'SKIN4',
    # French names
    "miniflingue" : 'MINIGUN',
    "masse a electrons" : 'ELLASER',
    "laser a impulsions" : 'LASER',
    "lance a plasma" : 'BEAM',
    "lanceur" : 'RAP',
    "grenade nucleaire" : 'NUCLGREN',
    "persuatron" : 'PERSUADER',
    "endoctrineur" : 'PERSUADER',
    "lance-flammes" : 'FLAMER',
    "deprogrammeur" : 'H2HTASER',
    "psychogaz" : 'CRAZYGAS',
    "gaz inhibiteur" : 'KOGAS',
    "mine a ions" : 'ELEMINE',
    "explosif" : 'EXPLMINE',
    "fusil long.portee" : 'LONGRANGE',
    "pluie d'acier" : 'AIRSTRIKE',
    "canon atomiseur" : 'QDEVASTATOR',
    "barbeles" : 'RAZORWIRE',
    "persuatron ii" : 'PERSUADER2',
    "ch. ralentisseur" : 'STASISFLD',
    "bioextracteur" : 'SOULGUN',
    "transp. temporel" : 'TIMEGUN',
    "trans. temporel" : 'TIMEGUN',
    "cerbere iff" : 'CEREBUSIFF',
    "automedic" : 'MEDI2',
    "cable declencheur" : 'EXPLWIRE',
    "morphobouclier" : 'CLONESHLD',
    "jambes 1" : 'LEGS1',
    "jambes 2" : 'LEGS2',
    "jambes 3" : 'LEGS3',
    "bras 1" : 'ARMS1',
    "bras 2" : 'ARMS2',
    "bras 3" : 'ARMS3',
    "corps 1" : 'CHEST1',
    "corps 2" : 'CHEST2',
    "corps 3" : 'CHEST3',
    "cerveau 1" : 'BRAIN1',
    "cerveau 2" : 'BRAIN2',
    "cerveau 3" : 'BRAIN3',
    "peau renforcee" : 'SKIN1',
    "peau anti-brulures" : 'SKIN2',
    "peau energetique" : 'SKIN3',
    "peau furtive" : 'SKIN4',
    # German names
    "mp" : 'UZI',
    "maschinenkanone" : 'MINIGUN',
    "elektrokeule" : 'ELLASER',
    "pulslaser" : 'LASER',
    "plasmalanze" : 'BEAM',
    "raketen-werfer" : 'RAP',
    "atomgranate" : 'NUCLGREN',
    "überzeugstrahler" : 'PERSUADER',
    "indoktrinator" : 'PERSUADER',
    "flammenwerfer" : 'FLAMER',
    "trenner" : 'H2HTASER',
    "psychogas" : 'CRAZYGAS',
    "ko-gas" : 'KOGAS',
    "ionenmine" : 'ELEMINE',
    "sprengstoff" : 'EXPLMINE',
    "fernwaffe" : 'LONGRANGE',
    "satellitenregen" : 'AIRSTRIKE',
    "gravitonkanone" : 'QDEVASTATOR',
    "monomol-draht" : 'RAZORWIRE',
    "überzeuger beta" : 'PERSUADER2',
    "stasisschild" : 'STASISFLD',
    "vampirette" : 'SOULGUN',
    "zr-tron" : 'TIMEGUN',
    "zerberus" : 'CEREBUSIFF',
    "zerberus iff" : 'CEREBUSIFF',
    "medipack" : 'MEDI1',
    "auto-medipack" : 'MEDI2',
    "monomol-zünder" : 'EXPLWIRE',
    "tarnschild" : 'CLONESHLD',
    "beine 1" : 'LEGS1',
    "beine 2" : 'LEGS2',
    "beine 3" : 'LEGS3',
    "arme 1" : 'ARMS1',
    "arme 2" : 'ARMS2',
    "arme 3" : 'ARMS3',
    "körper 1" : 'CHEST1',
    "körper 2" : 'CHEST2',
    "körper 3" : 'CHEST3',
    "gehirn 1" : 'BRAIN1',
    "gehirn 2" : 'BRAIN2',
    "gehirn 3" : 'BRAIN3',
    "gehrin 3" : 'BRAIN3',
    "harthaut" : 'SKIN1',
    "flammenhaut" : 'SKIN2',
    "energiehaut" : 'SKIN3',
    "tarnhaut" : 'SKIN4',
    # Italian names
    "pistola" : 'MINIGUN',
    "mazza elettron." : 'ELLASER',
    "laser a impulsi" : 'LASER',
    "lancia al plasma" : 'BEAM',
    "lancia granate" : 'RAP',
    "granata nucleare" : 'NUCLGREN',
    "indottrinatore" : 'PERSUADER',
    "lanciafiamme" : 'FLAMER',
    "distruttore" : 'H2HTASER',
    "gas allucinogeno" : 'CRAZYGAS',
    "gas nervino" : 'KOGAS',
    "mina a ioni" : 'ELEMINE',
    "esplos. alto pot." : 'EXPLMINE',
    "fuc. lungo raggio" : 'LONGRANGE',
    "pioggia satellite" : 'AIRSTRIKE',
    "pistola gravit." : 'QDEVASTATOR',
    "filo spinato" : 'RAZORWIRE',
    "persuadertron ii" : 'PERSUADER2',
    "campo statico" : 'STASISFLD',
    "ricarica energia" : 'SOULGUN',
    "dislocatore" : 'TIMEGUN',
    "guardiano" : 'CEREBUSIFF',
    "kit medico" : 'MEDI1',
    "kit automedico" : 'MEDI2',
    "mina esplosiva" : 'EXPLWIRE',
    "schermo clonat." : 'CLONESHLD',
    "gambe 1" : 'LEGS1',
    "gambe 2" : 'LEGS2',
    "gambe 3" : 'LEGS3',
    "braccia 1" : 'ARMS1',
    "braccia 2" : 'ARMS2',
    "braccia 3" : 'ARMS3',
    "corpo 1" : 'CHEST1',
    "corpo 2" : 'CHEST2',
    "corpo 3" : 'CHEST3',
    "cervello 1" : 'BRAIN1',
    "cervello 2" : 'BRAIN2',
    "cervello 3" : 'BRAIN3',
    "pelle indurita" : 'SKIN1',
    "pelle anti-fiamma" : 'SKIN2',
    "pelle energetica" : 'SKIN3',
    "pelle segreta" : 'SKIN4',
    # Spanish names
    "maza electrones" : 'ELLASER',
    "laser de pulsos" : 'LASER',
    "lanza de plasma" : 'BEAM',
    "lanzador" : 'RAP',
    "granada nuclear" : 'NUCLGREN',
    "persuasor" : 'PERSUADER',
    "indoctrinador" : 'PERSUADER',
    "lanzallamas" : 'FLAMER',
    "perturbador" : 'H2HTASER',
    "gas psiquico" : 'CRAZYGAS',
    "gas k.o." : 'KOGAS',
    "mina ionica" : 'ELEMINE',
    "gran explosivo" : 'EXPLMINE',
    "rifle precision" : 'LONGRANGE',
    "lluvia satelite" : 'AIRSTRIKE',
    "cañon graviton" : 'QDEVASTATOR',
    "cable cortante" : 'RAZORWIRE',
    "persuasor ii" : 'PERSUADER2',
    "blindaje stasis" : 'STASISFLD',
    "chromotap" : 'SOULGUN',
    "displacertron" : 'TIMEGUN',
    "cerberus iff" : 'CEREBUSIFF',
    "auto kit medico" : 'MEDI2',
    "cable disparador" : 'EXPLWIRE',
    "blindaje clon" : 'CLONESHLD',
    "piernas 1" : 'LEGS1',
    "piernas 2" : 'LEGS2',
    "piernas 3" : 'LEGS3',
    "brazos 1" : 'ARMS1',
    "brazos 2" : 'ARMS2',
    "brazos 3" : 'ARMS3',
    "brazo 1" : 'ARMS1',
    "brazo 2" : 'ARMS2',
    "brazo 3" : 'ARMS3',
    "cuerpo 1" : 'CHEST1',
    "cuerpo 2" : 'CHEST2',
    "cuerpo 3" : 'CHEST3',
    "cerebro 1" : 'BRAIN1',
    "cerebro 2" : 'BRAIN2',
    "cerebro 3" : 'BRAIN3',
    "piel dura" : 'SKIN1',
    "piel llama" : 'SKIN2',
    "piel energía" : 'SKIN3',
    "piel de energía" : 'SKIN3',
    "piel de camuflaje" : 'SKIN4',
    # Swedish names
    "minigevär" : 'MINIGUN',
    "minivapen" : 'MINIGUN',
    "elektronspruta" : 'ELLASER',
    "pulslaser" : 'LASER',
    "plasmalans" : 'BEAM',
    "avfyrare" : 'RAP',
    "kärngranat" : 'NUCLGREN',
    "™vertalartron" : 'PERSUADER',
    "eldkastare" : 'FLAMER',
    "avbrytare" : 'H2HTASER',
    "psykogas" : 'CRAZYGAS',
    "jonmina" : 'ELEMINE',
    "sprängladdning" : 'EXPLMINE',
    "långdistansgevär" : 'LONGRANGE',
    "satellitregn" : 'AIRSTRIKE',
    "gravitonvapen" : 'QDEVASTATOR',
    "rakbladswire" : 'RAZORWIRE',
    "övertalartron ii" : 'PERSUADER2',
    "stassk™ld" : 'STASISFLD',
    "kromot™mmare" : 'SOULGUN',
    "f™flyttotron" : 'TIMEGUN',
    "vakt-iff" : 'CEREBUSIFF',
    "läkepaket" : 'MEDI1',
    "auto läkepaket" : 'MEDI2',
    "avfyrningswire" : 'EXPLWIRE',
    "klonsk™ld" : 'CLONESHLD',
    "ben 1" : 'LEGS1',
    "ben 2" : 'LEGS2',
    "ben 3" : 'LEGS3',
    "kropp 1" : 'CHEST1',
    "kropp 2" : 'CHEST2',
    "kropp 3" : 'CHEST3',
    "hjärna 1" : 'BRAIN1',
    "hjärna 2" : 'BRAIN2',
    "hjärna 3" : 'BRAIN3',
    "hårdskinn" : 'SKIN1',
    "eldskinn" : 'SKIN2',
    "energiskinn" : 'SKIN3',
    "smygskinn" : 'SKIN4',
    "smygarskinn" : 'SKIN4',
}

def dict_key_for_value(d, v):
    try:
        idx = list(d.values()).index(v)
    except ValueError:
        return None
    return list(d.keys())[idx]


def find_occurence_in_polist(polist, occur):
    for e in polist:
        if occur in e.occurrences:
            return e
    match = re.match(r'^(.+)[.][a-z0-9]+$', occur[0])
    if match:
        findplace_start = match.group(1)
        for e in polist:
            for place, num in e.occurrences:
                if place.startswith(findplace_start):
                    return e
    return None


def read_enctable(po, fname):
    chartable_decode = [''] * 256
    chartable_encode = {}
    # Fill initial data
    ctd = chartable_decode
    cte = chartable_encode
    for n in range(32):
        c = n.to_bytes(1, 'big').decode('utf-8', errors='ignore')
        ctd[n] = c
        if len(c) > 0:
            cte[c] = n.to_bytes(1, 'big')
    # Read char table
    with open(fname, 'r', encoding='utf-8') as fh:
        lines = fh.readlines()
    for i,ln in enumerate(lines):
        ln = ln.lstrip()
        if len(ln) < 2:
            continue
        if ln[0] == '#':
            continue
        match = re.match(r'^([0-9]+|0x[0-9a-fA-F]+)[\t](.)$', ln)
        assert match, f"{fname} line {i}: Invalid entry"
        n = int(match.group(1),0)
        c = match.group(2)
        if ctd[n] == '':
            ctd[n] = c
        cte[c] = n.to_bytes(1, 'big')
    return chartable_decode, chartable_encode


def enctable_bytes_to_string(ctd, b):
    s = ""
    for c in b:
        s = s + ctd[c]
    return s


def enctable_string_to_bytes(cte, s):
    b = b""
    for i,c in enumerate(s):
        assert c in cte, f"Encode table lacks '{c}'at {i} from '{s}'"
        b = b + cte[c]
    return b


def waditem_string_national_to_upper(stri):
    stro = ""
    for c in stri:
        if c in "êô":
            stro = stro + unicodedata.normalize('NFKD', c).encode('ascii', 'ignore').decode('ascii', errors='ignore')
        elif c in "ìäåéöüñáàèíòúù":
            stro = stro + c.upper()
        else:
            stro = stro + c
    return stro


def waditem_string_to_bytes(po, fname, s):
    if fname.startswith("miss"):
        b = enctable_string_to_bytes(po.chartable_m_encode, s)
    else:
        s = waditem_string_national_to_upper(s)
        b = enctable_string_to_bytes(po.chartable_d_encode, s)
    return b


def waditem_bytes_to_string(po, fname, b):
    if fname.startswith("miss"):
        s = enctable_bytes_to_string(po.chartable_m_decode, b)
    else:
        s = enctable_bytes_to_string(po.chartable_d_decode, b)
    return s


def sourceid_encode(mailid, campgn):
    if campgn == campaign_names[1]:
        sourceid = mailid + 100
    elif campgn == campaign_names[2]:
        sourceid = mailid + 200
    else:
        sourceid = mailid
    return sourceid


def sourceid_decode(sourceid):
    if sourceid < 100:
        campgn = campaign_names[0]
        mailid = sourceid
    elif sourceid < 200:
        campgn = campaign_names[1]
        mailid = sourceid - 100
    else:
        campgn = campaign_names[2]
        mailid = sourceid - 200
    return mailid, campgn


def source_linear_number_to_sourceid(sourceno):
    sourceid = sourceno
    if sourceno > 34 + 30:
        mailid_sub = ""
        mailno = sourceno - 34 - 30;
        campgn = campaign_names[2]
    elif sourceno > 34:
        mailid_sub = ""
        mailno = sourceno - 34
        if mailno >= 4:
            i = mailno - 4
            if i % 2 == 0:
                mailid_sub = "a"
            else:
                mailid_sub = "b"
            mailno = (i // 2) + 4
        campgn = campaign_names[1]
    else:
        mailid_sub = ""
        mailno = sourceno
        if mailno >= 4:
            i = mailno - 4
            if i % 2 == 0:
                mailid_sub = "a"
            else:
                mailid_sub = "b"
            mailno = (i // 2) + 4
        campgn = campaign_names[0]
    sourceid = sourceid_encode(mailno, campgn)
    return sourceid, mailid_sub


def pofile_store_entry(po, pofh, e):
    pofh.write("\n")
    if len(e.tcomment) > 0:
        pofh.write("# " + e.tcomment + "\n")
    if len(e.comment) > 0:
        pofh.write("#. " + e.comment + "\n")
    if len(e.occurrences) > 0:
        pofh.write("#: " + " ".join(e.occurrences) + "\n")
    if len(e.flags) > 0:
        pofh.write("#, " + " ".join(e.flags) + "\n")
    if len(e.msgctxt) > 0:
        text = e.msgctxt
        text = text.replace("\"", "\\\"")
        pofh.write("msgctxt \"" + text + "\"\n")
    if True:
        text = e.msgid
        text = text.replace("\"", "\\\"")
        pofh.write("msgid \"" + text + "\"\n")
    if True:
        text = e.msgstr
        text = text.replace("\"", "\\\"")
        pofh.write("msgstr \"" + text + "\"\n")
    return


def pofile_set_default_metadata(polist, lang):
    if lang == "eng":
        polang = "en_GB"
    elif lang == "fre":
        polang = "fr_FR"
    elif lang == "ger":
        polang = "de_DE"
    elif lang == "ita":
        polang = "it_IT"
    elif lang == "jap":
        polang = "ja_JP"
    elif lang == "spa":
        polang = "es_ES"
    elif lang == "swe":
        polang = "sv_SE"
    else:
        polang = "UNKNOWN"
    polist.metadata = {
      'Project-Id-Version': 'Menu text for SyndWarsFX',
      'Report-Msgid-Bugs-To': 'https://github.com/swfans/swars/issues',
      'POT-Creation-Date': '2023-08-20 01:12+0200',
      'PO-Revision-Date': '2023-09-02 12:00+0100',
      'Last-Translator': 'Mefistotelis <mefistotelis@gmail.com>',
      'Language-Team': 'Syndicate Wars Fans <github.com/swfans>',
      'Language': polang,
      'MIME-Version': '1.0',
      'Content-Type': 'text/plain; charset=UTF-8',
      'Content-Transfer-Encoding': '8bit',
      'X-Poedit-SourceCharset': 'utf-8',
    }
    return


def pofile_set_default_head_comment(polist, pofname):
    polist.header = \
      "*****************************************************************************\n" + \
      " Syndicate Wars Fan Expansion, source port of the classic game from Bullfrog.\n" + \
      "*****************************************************************************\n" + \
      "  @file " + pofname + "\n" + \
      "     Menu text for SyndWarsFX translation file\n" + \
      " @par Purpose:\n" + \
      "     Contains translation of the national text in the game.\n" + \
      " @par Comment:\n" + \
      "     Use this file to improve the translation for specific language.\n" + \
      " @author   Syndicate Wars Fans\n" + \
      " @date     20 Aug 2023 - 02 Sep 2023\n" + \
      " @par  Copying and copyrights:\n" + \
      "     This program is free software; you can redistribute it and/or modify\n" + \
      "     it under the terms of the GNU General Public License as published by\n" + \
      "     the Free Software Foundation; either version 2 of the License, or\n" + \
      "     (at your option) any later version.\n" + \
      "\n" + \
      "*****************************************************************************";
    return


def prep_po_entries_city(podict, lines):
    city_prop_comments = [
        "name",
        "population size",
        "landscape",
        "language",
        "industry",
        "toxicity",
    ]

    polist = []
    # Read initial comments
    n = 0
    comment = ""
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        # Special case for translation which is commented out in original files
        if ln == "#UTOPIA":
            e = polib.POEntry(msgstr=ln[1:], msgctxt=comment)
            e.occurrences.append( ('mapscreen.title','',) )
            podict['COMM'].append(e)
            n += 1
            continue
        if ln[0] == "#":
            comment = ln[1:]
            n += 1
            continue
        break
    # Read names of city properties - entries for Synd and Church
    k = 0
    entryno = 0
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if ln[0] == "#":
            comment = ln[1:]
            n += 1
            k = 0
            entryno += 1
            continue
        if entryno >= 2:
            break
        if k < 6:
            ctxt = comment + " - " + city_prop_comments[k]
            e = polib.POEntry(msgstr=ln, msgctxt=ctxt)
            campgn = campaign_names[entryno]
            e.occurrences.append( ('mapscreen.heading',f'{k+1}',) )
            podict[campgn].append(e)
            n += 1
            k += 1
            continue
        break
    # Read the properties of cities - 6 per city
    k = 0
    entryno = 1
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if ln[0] == "#":
            comment = ln[1:]
            n += 1
            continue
        if k >= 6:
            k = 0
            entryno += 1
        if True:
            if comment == "city data":
                ctxt = "map screen location - " + city_prop_comments[k]
            else:
                ctxt = comment
            e = polib.POEntry(msgstr=ln, msgctxt=ctxt)
            e.occurrences.append( (f'mapscreen.city{entryno}',f'{k+1}',) )
            podict['COMM'].append(e)
            n += 1
            k += 1
            continue
        break
    return polist


def prep_po_entries_names(podict, lines):
    comment = "mission title"
    n = 0
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if ln[0] == "#":
            n += 1
            continue
        match = re.match(r'^([0-9]+)[.](.*)$', ln)
        if match:
            text = match.group(2)
            missi = int(match.group(1), 10)
            if len(text) > 0:
                e = polib.POEntry(msgstr=text, msgctxt=comment)
                e.occurrences.append( ('mission.title',f'{missi}',) )
                if missi in church_missions.keys():
                    campgn = campaign_names[1]
                elif missi in syndct_missions.keys():
                    campgn = campaign_names[0]
                else:
                    campgn = campaign_names[2]
                podict[campgn].append(e)
            n += 1
            continue
        break
    return


def prep_po_entries_outro(podict, lines):
    comment = "outro message"
    n = 0
    k = 0
    entryno = -1
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if ln[0] == "#":
            n += 1
            continue
        match = re.match(r'^[\[](.*)[\]]$', ln)
        if match:
            comment = match.group(1)
            n += 1
            k = 0
            entryno += 1
            continue
        if entryno < 0:
            entryno += 1
        if True:
            e = polib.POEntry(msgstr=ln, msgctxt=comment)
            campgn = campaign_names[entryno]
            e.occurrences.append( ('message.win',f'{k+1}',) )
            podict[campgn].append(e)
            n += 1
            k += 1
            continue
        break
    return


def prep_po_entries_wms(podict, lines):
    comment1 = "outro message"
    comment2 = ""
    n = 0
    k = 0
    entryno = -1
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if ln[0] == "#":
            n += 1
            continue
        match = re.match(r'^[\[](.*PLAYER)[\]]$', ln)
        if match:
            comment1 = match.group(1).lower()
            n += 1
            k = 0
            entryno += 1
            comment2 = ""
            comment3 = ""
            continue
        match = re.match(r'^[\[](.*)[\]]$', ln)
        if match:
            comment2 = match.group(1).lower()
            # Fixes for translations
            if comment2 == "armi":
                comment2 = "weapons"
            elif comment2 == "modalità":
                comment2 = "mods"
            comment3 = ""
            n += 1
            k = 0
            continue
        if (k % 2) == 0:
            comment3 = ln.lower()
            n += 1
            k += 1
            continue
        if True:
            ctxt = comment2 + " description - " + comment3
            e = polib.POEntry(msgstr=ln, msgctxt=ctxt)
            campgn = campaign_names[entryno]
            idx = (k // 2) + 1
            if comment3 in weapon_mod_names_to_code:
                codename = weapon_mod_names_to_code[comment3]
                e.occurrences.append( (f'{comment2}.{codename}.description','',) )
            else:
                e.occurrences.append( (f'{comment2}.description',f'{idx}',) )
            podict[campgn].append(e)
            n += 1
            k += 1
            continue
        break
    return


def prep_po_entries_netscan(podict, lines):
    comment = "outro message"
    n = 0
    k = 0
    level = 0
    mapno = 0
    # note that sourceno is not sourceid - sometimes one sourceid has several missions
    sourceno = 0
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if ln[0] == "#":
            n += 1
            continue
        match = re.match(r'^[\[](.*)[\]]$', ln)
        if match:
            compnum = int(match.group(1), 10)
            level = compnum % 100
            mapno = compnum // 100
            sourceno += 1
            n += 1
            k = 0
            continue
        if True:
            mailid_sub = ""
            (sourceid, mailid_sub) = source_linear_number_to_sourceid(sourceno)
            mailid, campgn = sourceid_decode(sourceid)

            ctxt = "mission brief tactical information"
            e = polib.POEntry(msgstr=ln, msgctxt=ctxt)

            e.occurrences.append( (f'mission.brief.mail{mailid}{mailid_sub}.map{mapno}.level{level}',f'{k+1}',) )
            podict[campgn].append(e)
            n += 1
            k += 1
            continue
        break
    return


def prep_po_entries_miss(podict, lines, sourceid):
    n = 0
    k = 0
    mailid, campgn = sourceid_decode(sourceid)
    entryno = 0
    msgstr = ""
    fmtchar = "c5"
    for ln in lines[n:]:
        text = ln
        text = text.replace("\\l", "<login>")
        if text.startswith("\\") and not text.startswith("\\n"):
            fmtchar = text[1]
            if fmtchar == "c":
                fmtchar += text[2]
                text = text[3:]
            else:
                text = text[2:]
        custom_fmt = None
        if (fmtchar == "c4") and re.search(r'[\\]c4[A-Z][\\]c3', text):
            custom_fmt = "expansion"
        text = text.replace("\\c1", "")
        text = text.replace("\\c2", "")
        text = text.replace("\\c3", "")
        text = text.replace("\\c4", "")
        text = text.replace("\\c5", "")
        if not text.endswith("\\n"):
            msgstr += text
            n += 1
            continue
        if True:
            text = text[:-2]
            msgstr += text
            if len(msgstr) > 0:
                if fmtchar == "h":
                    ctxt="mission brief email header"
                elif fmtchar == "c1":
                    ctxt="mission brief book quote"
                elif fmtchar == "c2":
                    ctxt="mission brief email orders"
                elif fmtchar == "c4":
                    ctxt="mission brief email sender"
                else:
                    ctxt="mission brief email paragraph"
                if custom_fmt is not None:
                    fmtchar = custom_fmt
                e = polib.POEntry(msgstr=msgstr, msgctxt=ctxt)
                e.occurrences.append( (f'mission.brief.mail{mailid}.par{k+1}.{fmtchar}','',) )
                podict[campgn].append(e)
                k += 1
            msgstr = ""
            n += 1
            continue
    return


def prep_po_entries_per_line(podict, lines, refstart, comment):
    n = 0
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if True:
            text = ln[0].upper() + ln[1:].lower()
            e = polib.POEntry(msgstr=text, msgctxt=comment)
            e.occurrences.append( (f'{refstart}',f'{n+1}',) )
            podict['COMM'].append(e)
            n += 1
            continue
        break
    return


def po_occurrence_to_fname(campgn, place, num):
    match = re.match(r'^mapscreen[.](title|heading|city[0-9]+)$', place)
    if match:
        return "city.txt"
    match = re.match(r'^sci[.]death[.]reason$', place)
    if match:
        return "lost.txt"
    match = re.match(r'^mission[.]brief[.]mail([0-9]+)[.]par[0-9]+[.].*$', place)
    if match:
        mailid = int(match.group(1),10)
        sourceid = sourceid_encode(mailid, campgn)
        return f"miss{sourceid:03d}.txt"
    match = re.match(r'^mission[.]title$', place)
    if match:
        return "names.txt"
    match = re.match(r'^mission[.]brief[.]mail([0-9]+)([a-z]?)[.]map([0-9]+)[.]level([0-9]+)$', place)
    if match:
        return "netscan.txt"
    match = re.match(r'^scanner[.]objective$', place)
    if match:
        return "obj.txt"
    match = re.match(r'^message[.]win$', place)
    if match:
        return "outtro.txt"
    match = re.match(r'^(weapons|mods)[.].*[.]description$', place)
    if match:
        return "wms.txt"
    assert False, f"unrecognized occurence \"{place}\""
    return None

def list_txtfiles_from_po_content(podict):
    txtfiles = []
    for campgn, polist in podict.items():
        for e in polist:
            for place, num in e.occurrences:
                fname = po_occurrence_to_fname(campgn, place, num)
                if fname:
                    txtfiles.append(fname)
    txtfiles = list(set(txtfiles))
    txtfiles.sort()
    return txtfiles


def textwad_extract_to_po(podict, txtfname, lines):
    match = None
    if not match:
        match = re.match(r'^(city)[.]txt$', txtfname)
        if match:
            prep_po_entries_city(podict, lines)
    if not match:
        match = re.match(r'^(lost)[.]txt$', txtfname)
        if match:
            prep_po_entries_per_line(podict, lines, 'sci.death.reason', "scientists death reason")
    if not match:
        match = re.match(r'^(miss([0-9]+))[.]txt$', txtfname)
        if match:
            sourceid = int(match.group(2), 10)
            prep_po_entries_miss(podict, lines, sourceid)
    if not match:
        match = re.match(r'^(names)[.]txt$', txtfname)
        if match:
            prep_po_entries_names(podict, lines)
    if not match:
        match = re.match(r'^(netscan)[.]txt$', txtfname)
        if match:
            prep_po_entries_netscan(podict, lines)
    if not match:
        match = re.match(r'^(obj)[.]txt$', txtfname)
        if match:
            prep_po_entries_per_line(podict, lines, 'scanner.objective', "objective text shown on scanner")
    if not match:
        match = re.match(r'^(outtro)[.]txt$', txtfname)
        if match:
            prep_po_entries_outro(podict, lines)
    if not match:
        match = re.match(r'^(wms)[.]txt$', txtfname)
        if match:
            prep_po_entries_wms(podict, lines)
    assert match, "unrecognized file type"
    return


def create_lines_for_city(lines, pomdict):
    lines.append("# == map screen text ==")

    lines.append("# map screen title")
    if True:
        e = pomdict[('COMM','mapscreen.title','',)]
        lines.append(f"#{e.msgstr}")

    for entryno in range(0,3):
        campgn = campaign_names[entryno]
        if (campgn,'mapscreen.heading','1',) in pomdict.keys():
            break

    if True:
        lines.append("# map screen headings")
        for k in range(1,7):
            try:
                e = pomdict[(campgn,'mapscreen.heading',f'{k}',)]
            except:
                e = polib.POEntry()
            lines.append(e.msgstr)

    lines.append("")
    lines.append("# city data")
    lines.append("")

    for entryno in range(1,256):
        noexist = False
        for k in range(1,8):
            try:
                e = pomdict[('COMM',f'mapscreen.city{entryno}',f'{k}',)]
            except:
                e = polib.POEntry()
            if (k == 1) and (e.msgstr == ""):
                noexist = True
                break
            lines.append(e.msgstr)
        if noexist:
            break
    return


def create_lines_for_per_line(lines, pomdict, refstart):
    if True:
        noexist = False
        for k in range(1,32768):
            try:
                e = pomdict[('COMM',refstart,f'{k}',)]
            except:
                e = polib.POEntry()
                # Finish if the next entry does not exist as well
                noexist = ('COMM',refstart,f'{k+1}',) not in pomdict
            if noexist:
                break
            lines.append(e.msgstr.upper())
    lines.append("")
    return


def create_lines_for_miss(lines, pomdict, sourceid):
    mailid, campgn = sourceid_decode(sourceid)

    paragraphs = []
    if True:
        for (ccampgn,place,num,), e in pomdict.items():
            if ccampgn != campgn:
                continue
            match = re.match(r'^mission[.]brief[.]mail([0-9]+)[.]par([0-9]+)[.](.*)$', place)
            assert match, "Invalid mission.brief.mailN.par occurrence"
            cmailid = int(match.group(1),10)
            if cmailid != mailid:
                continue
            n = int(match.group(2),10)
            fmt = match.group(3)
            while n >= len(paragraphs):
                paragraphs.append( (None,None,) )
            paragraphs[n] = (fmt,e,)

    assert len(paragraphs) > 0

    if True:
        prevfmt = ''
        for n, (fmt,e,) in enumerate(paragraphs):
            if e is None:
                continue
            if fmt == "expansion":
                custom_fmt = fmt
                fmt = "c3"
            else:
                custom_fmt = None
            if n > 1:
                text = "\\n"
                if (prevfmt != fmt) and (fmt in ('c2','c5',)) and (custom_fmt is None):
                    text = f"\\{fmt}{text}"
                    prevfmt = fmt
                lines.append(text)
            text = e.msgstr
            text = text.replace("<login>", "\\l")
            if custom_fmt == "expansion":
                for i in range(len(text)-1, -1, -1):
                    if not text[i].isupper():
                        continue
                    if (i < 1):
                        text = f"\\c4{text[i]}\\{fmt}{text[i+1:]}"
                    else:
                        text = f"{text[:i]}\\c4{text[i]}\\{fmt}{text[i+1:]}"
            elif prevfmt != fmt:
                text = f"\\{fmt}{text}"
                prevfmt = fmt
            text = f"{text}\\n"
            wraplines = textwrap.wrap(text, width=104, break_long_words=False,
              drop_whitespace=False, break_on_hyphens=False)
            for ln in wraplines:
                lines.append(ln)
        if True:
            fmt = 'c5'
            text = "\\n"
            if prevfmt != fmt:
                text = f"\\{fmt}{text}"
                prevfmt = fmt
            lines.append(text)
    lines.append("")
    return


def create_lines_for_names(lines, pomdict):
    lines.append("# Names.txt")
    lines.append("# Contains all city drop names in the mission number order.")
    lines.append("")
    pounidict = {}
    for (campgn, place, num,), e in pomdict.items():
        pounidict[ (place,num,) ] = e
    n = 0
    for missi in range(1,256):
        try:
            e = pounidict[('mission.title',f'{missi}',)]
            n += 1
        except:
            e = polib.POEntry()
        lines.append(f"{missi}.{e.msgstr}")
        if n == len(pounidict):
            break
    lines.append("")
    return


def create_lines_for_netscan(lines, pomdict):
    lines.append("# Netscan.txt")
    lines.append("# Master file containing all OBJ****.txt files.")
    lines.append("# Files indexed Sn and Cn in Miss***.txt order.")
    lines.append("# Scan entries listed by Points Of Significance (POS).")
    pounilist = {}
    for (campgn, place, num,), e in pomdict.items():
        match = re.match(r'^mission[.]brief[.]mail([0-9]+)([a-z]?)[.]map([0-9]+)[.]level([0-9]+)$', place)
        assert match, "Invalid netscan mission.brief.mailN.mapM.levelL occurrence"
        mailid = int(match.group(1), 10)
        mailid_sub = match.group(2)
        sourceid = sourceid_encode(mailid, campgn)
        mapno = int(match.group(3), 10)
        level = int(match.group(4), 10)
        num = int(num, 10) - 1
        ssourceid = f"{sourceid}{mailid_sub}"
        if ssourceid not in pounilist.keys():
            pounilist[ssourceid] = (mapno,level,[],)
        (cmapno,clevel,clist,) = pounilist[ssourceid]
        assert cmapno == mapno
        assert clevel == level
        while num >= len(clist):
            clist.append(None)
        clist[num] = e
    # Some hard-coded values; not really needed, but make the file more like original
    if "255" not in pounilist.keys():
        pounilist["255"] = (65,3,[],)
    if "21" not in pounilist.keys():
        pounilist["21"] = (79,31,[],)
    if "22" not in pounilist.keys():
        pounilist["22"] = (8,1,[],)
    if "33" not in pounilist.keys():
        pounilist["33"] = (11,1,[],)
    if "34" not in pounilist.keys():
        pounilist["34"] = (65,2,[],)
    if "59" not in pounilist.keys():
        pounilist["59"] = (79,2,[],)
    # Now generate the lines
    for ssourceid, (mapno,level,clist,) in pounilist.items():
        if mapno is None:
            (mapno,level,) = (99,ssourceid,)
            clist = []
        lines.append(f"[{mapno:02d}{level:02d}]")
        for i, e in enumerate(clist):
           assert e is not None, f"Missing netscan entry mission.brief.mailN.map{mapno}.level{level}:{i+1}"
           lines.append(e.msgstr)
    lines.append("")
    return


def create_lines_for_outro(lines, pomdict):
    for campgn in campaign_names[0:3]:
        lines.append(f"[{campgn}_win]".lower())
        noexist = False
        for k in range(1,256):
            try:
                e = pomdict[(campgn,'message.win',f'{k}',)]
            except:
                e = polib.POEntry()
                # Finish if the next entry does not exist as well
                noexist = (campgn,'message.win',f'{k+1}',) not in pomdict
            if noexist:
                break
            if k > 1:
                lines.append("")
            lines.append(e.msgstr)
    lines.append("")
    return


def create_lines_for_wms(lines, pomdict):
    poweplist = []
    pomodlist = []
    for (campgn, place, num,), e in pomdict.items():
        if campgn.upper() not in campaign_names[0:3]:
            continue
        match = re.match(r'^(weapons|mods)[.](.+)[.]description$', place)
        if not match:
            continue
        wmtype = match.group(2)
        if match.group(1) == "weapons":
            poweplist.append( (wmtype,e,) )
        else:
            pomodlist.append( (wmtype,e,) )

    lines.append(f"[weapons]".lower())
    for wmtype,e in poweplist:
        lines.append(wmtype.upper())
        lines.append(e.msgstr)

    lines.append(f"[mods]".lower())
    for wmtype,e in pomodlist:
        lines.append(wmtype.upper())
        lines.append(e.msgstr)
    lines.append("")
    return


def textwad_create_from_po(lines, podict, txtfname):
    pomdict = {}
    for campgn, polist in podict.items():
        for e in polist:
            # Find entries matching for current txt file
            for place, num in e.occurrences:
                fname = po_occurrence_to_fname(campgn, place, num)
                if fname == txtfname:
                    if (campgn,place,num,) in pomdict:
                        print("{}: Doubled definition of {} {}{}".format(txtfname,campgn,place,f":{num}" if num != "" else ""))
                    pomdict[ (campgn,place,num,) ] = e
    match = None
    if not match:
        match = re.match(r'^(city)[.]txt$', txtfname)
        if match:
            create_lines_for_city(lines, pomdict)
    if not match:
        match = re.match(r'^(lost)[.]txt$', txtfname)
        if match:
            create_lines_for_per_line(lines, pomdict, 'sci.death.reason')
    if not match:
        match = re.match(r'^(miss([0-9]+))[.]txt$', txtfname)
        if match:
            sourceid = int(match.group(2),10)
            create_lines_for_miss(lines, pomdict, sourceid)
    if not match:
        match = re.match(r'^(names)[.]txt$', txtfname)
        if match:
            create_lines_for_names(lines, pomdict)
    if not match:
        match = re.match(r'^(netscan)[.]txt$', txtfname)
        if match:
            create_lines_for_netscan(lines, pomdict)
    if not match:
        match = re.match(r'^(obj)[.]txt$', txtfname)
        if match:
            create_lines_for_per_line(lines, pomdict, 'scanner.objective')
    if not match:
        match = re.match(r'^(outtro)[.]txt$', txtfname)
        if match:
            create_lines_for_outro(lines, pomdict)
    if not match:
        match = re.match(r'^(wms)[.]txt$', txtfname)
        if match:
            create_lines_for_wms(lines, pomdict)
    assert match, "unrecognized file type"
    return


def find_duplicate_po_entry_after(polist, n):
    e1 = polist[n]
    n += 1
    while n < len(polist):
        e2 = polist[n]
        if (e1.msgctxt == e2.msgctxt) and (e1.msgid == e2.msgid) and (e1.msgstr == e2.msgstr):
            return e2
        n += 1
    return None


def merge_same_po_entries(podict):
    for campgn, polist in podict.items():
        n = 0
        while n < len(polist):
            e1 = polist[n]
            while True:
                e2 = find_duplicate_po_entry_after(polist, n)
                if e2 is None:
                    break;
                polist.remove(e2)
                e1.occurrences.extend(e2.occurrences)
                e1.flags.extend(e2.flags)
            n += 1
    return


def textwad_extract_raw(po, wadfh, idxfh):
    e = WADIndexEntry()
    while idxfh.readinto(e) == sizeof(e):
        txtfname = e.Filename.decode('utf-8').lower()
        n = 0
        wadfh.seek(e.Offset, os.SEEK_SET)
        with open(txtfname, 'wb') as txtfh:
            copy_buffer = wadfh.read(e.Length)
            n += len(copy_buffer)
            txtfh.write(copy_buffer)
    return


def textwad_empty_podict(po, lang):
    # Prepare empty dict
    podict = {}
    for campgn in campaign_names:
        polist = polib.POFile()
        podict[campgn] = polist
        pofile_set_default_metadata(polist, lang)
    return podict


def textwad_read_to_podict(po, podict,  wadfh, idxfh):
    # Fill with files from WAD
    e = WADIndexEntry()
    while idxfh.readinto(e) == sizeof(e):
        txtfname = e.Filename.decode('utf-8').lower()
        wadfh.seek(e.Offset, os.SEEK_SET)
        txt_buffer = wadfh.read(e.Length)
        lines = txt_buffer.split(b'\n')
        lines = [waditem_bytes_to_string(po, txtfname, ln).rstrip("\r\n") for ln in lines]
        textwad_extract_to_po(podict, txtfname, lines)
    return


def textwad_extract(po):
    podict = textwad_empty_podict(po, po.lang)
    engpodict = textwad_empty_podict(po, "eng")
    if True:
        # Input language PO
        with open(po.wadfile, 'rb') as wadfh:
            with open(po.idxfile, 'rb') as idxfh:
                textwad_read_to_podict(po, podict, wadfh, idxfh)

    if po.lang != "eng":
        # Temporary English PO
        with open(po.engwadfile, 'rb') as wadfh:
            with open(po.engidxfile, 'rb') as idxfh:
                textwad_read_to_podict(po, engpodict, wadfh, idxfh)

    if po.lang == "eng":
        for campgn in campaign_names:
            for e in podict[campgn]:
                e.msgid = e.msgstr
                e.msgstr = ""
    else:
        for campgn in campaign_names:
            engpolist = engpodict[campgn]
            for e in podict[campgn]:
                ene = find_occurence_in_polist(engpolist, e.occurrences[0])
                if ene is not None:
                    e.msgid = ene.msgstr
    merge_same_po_entries(podict)
    for cidx, campgn in enumerate(campaign_names):
        pofname = po.pofiles[cidx]
        if campgn.lower() not in pofname:
            print("{}: File name lacks campaign name, is there a mistake?".format(pofname))
        polist = podict[campgn]
        pofile_set_default_head_comment(polist, pofname)
        polist.save(pofname)
    return


def textwad_create(po, wadfh, idxfh):
    # Load PO files
    podict = {}
    #for cidx, campgn in enumerate(campaign_names):
    #    pofname = po.pofiles[cidx]
    for poidx, pofname in enumerate(po.pofiles):
        campgn = None
        for ccampgn in campaign_names:
            if ccampgn.lower() in pofname:
                campgn = ccampgn
        #if campgn.lower() not in pofname:
        if campgn is None:
            print("{}: File name lacks campaign name, is there a mistake?".format(pofname))
            campgn = "UNKN"
        polist = polib.pofile(pofname)
        podict[campgn] = polist

    for campgn in podict.keys():
        for e in podict[campgn]:
            if e.msgstr == "":
                e.msgstr = e.msgid
    txtfiles = list_txtfiles_from_po_content(podict)
    for txtfname in txtfiles:
        lines = []
        e = WADIndexEntry()
        e.Filename = txtfname.upper().encode('utf-8')
        e.Offset = wadfh.tell()
        textwad_create_from_po(lines, podict, txtfname)
        lines = [waditem_string_to_bytes(po, txtfname, ln) for ln in lines]
        wadfh.write(b'\r\n'.join(lines))
        e.Length = wadfh.tell() - e.Offset
        idxfh.write((c_ubyte * sizeof(e)).from_buffer_copy(e))


def main():
    """ Main executable function.

    Its task is to parse command line options and call a function which performs requested command.
    """
    parser = argparse.ArgumentParser(description=__doc__.split('.')[0])

    parser.add_argument('pofiles', type=str, nargs=2,
          help="List of PO/POT file names, in proper order (per-campaign, then common)")

    parser.add_argument('-w', '--wadfile', type=str, required=True,
          help="Name for WAD/IDX files")

    parser.add_argument('-e', '--engwadfile', type=str, default="",
          help="Name for english language WAD/IDX files, required to extract non-english WADs to PO")

    parser.add_argument('-l', '--lang', type=str, default="eng",
          help="Language of the processed files, required to extract non-english WADs to PO")

    parser.add_argument('-v', '--verbose', action='count', default=0,
          help="Increases verbosity level; max level is set by -vvv")

    subparser = parser.add_mutually_exclusive_group(required=True)

    subparser.add_argument('-r', '--raw', action='store_true',
          help="Import or export raw files (TXT) rather than .PO/POT")

    subparser.add_argument('-t', '--enctable', type=str,
          help="Character encoding table file name for directly converted strings")

    parser.add_argument('-m', '--menctable', type=str,
          help="Character encoding table file name for mission briefing strings")

    subparser = parser.add_mutually_exclusive_group(required=True)

    subparser.add_argument('-x', '--extract', action='store_true',
          help="Extract to text files")

    subparser.add_argument('-c', '--create', action='store_true',
          help="Create WAD/IDX from text files")

    subparser.add_argument('--version', action='version', version="%(prog)s {version} by {author}"
            .format(version=__version__, author=__author__),
          help="Display version information and exit")

    po = parser.parse_args()

    if True:
        if po.wadfile.lower().endswith(".wad"):
            po.wadbase = os.path.splitext(po.wadfile)[0]
        else:
            po.wadbase = po.wadfile
        assert len(po.wadbase) > 0, "Provided WAD file name base is too short"
        po.wadfile = po.wadbase + ".wad"
        po.idxfile = po.wadbase + ".idx"

    if po.extract and not po.raw:
        if po.engwadfile.lower().endswith(".wad"):
            po.engwadbase = os.path.splitext(po.engwadfile)[0]
        else:
            po.engwadbase = po.engwadfile
        po.engwadfile = po.engwadbase + ".wad"
        po.engidxfile = po.engwadbase + ".idx"

    if not po.raw:
        (po.chartable_d_decode, po.chartable_d_encode) = read_enctable(po, po.enctable)
        (po.chartable_m_decode, po.chartable_m_encode) = read_enctable(po, po.menctable)

    if po.extract:
        if (po.verbose > 0):
            print("{}: Opening for extract".format(po.wadfile))
        if po.raw:
            with open(po.wadfile, 'rb') as wadfh:
                with open(po.idxfile, 'rb') as idxfh:
                    textwad_extract_raw(po, wadfh, idxfh)
        else:
            textwad_extract(po)

    elif po.create:
        if (po.verbose > 0):
            print("{}: Opening for creation".format(po.wadfile))
        with open(po.wadfile, 'wb') as wadfh:
            with open(po.idxfile, 'wb') as idxfh:
                textwad_create(po, wadfh, idxfh)

    else:
        raise NotImplementedError("Unsupported command.")


if __name__ == '__main__':
    try:
        main()
    except Exception as ex:
        print("Error: "+str(ex), file=sys.stderr)
        if 1: raise
        sys.exit(10)
