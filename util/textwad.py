#!/usr/bin/python3
# -*- coding: utf-8 -*-
# vim:sw=4

""" Text WAD/IDX conversion tool for Syndicate Wars Port.

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
}


def dict_key_for_value(d, v):
    try:
        idx = list(d.values()).index(v)
    except ValueError:
        return None
    return list(d.keys())[idx]


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
    elif lang == "jap":
        polang = "ja_JP"
    else:
        polang = "UNKNOWN"
    polist.metadata = {
      'Project-Id-Version': 'Menu text for SW Port',
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
      " Syndicate Wars Port, source port of the classic strategy game from Bullfrog.\n" + \
      "*****************************************************************************\n" + \
      "  @file " + pofname + "\n" + \
      "     Menu text for SW Port translation file\n" + \
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
                else:
                    campgn = campaign_names[0]
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
            e.occurrences.append( ('message.win',k,) )
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
                e.occurrences.append( (f'{comment2}.description',idx,) )
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
            n += 1
            k = 0
            continue
        if True:
            ctxt = "mission brief netscan information"
            e = polib.POEntry(msgstr=ln, msgctxt=ctxt)
            missi = dict_key_for_value(church_missions, (mapno,level,))
            if missi is None:
                campgn = campaign_names[0]
            else:
                campgn = campaign_names[1]
            e.occurrences.append( (f'mission.brief.map{mapno}.level{level}',k,) )
            podict[campgn].append(e)
            n += 1
            k += 1
            continue
        break
    return


def prep_po_entries_miss(podict, lines, sourceid):
    n = 0
    k = 0
    if sourceid < 24:
        campgn = campaign_names[0]
        mailid = sourceid
    else:
        campgn = campaign_names[1]
        mailid = sourceid - 23
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
            e.occurrences.append( (f'{refstart}',n,) )
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
        if campgn == campaign_names[1]:
            sourceid = mailid + 23
        elif campgn == campaign_names[2]:
            sourceid = mailid + 50
        else:
            sourceid = mailid
        return f"miss{sourceid:03d}.txt"
    match = re.match(r'^mission[.]title$', place)
    if match:
        return "names.txt"
    match = re.match(r'^mission[.]brief[.]map[0-9]+[.]level[0-9]+$', place)
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
    lines.append("# == syndicate & church map screen text ==")

    lines.append("# syndicate & church map screen title")
    if True:
        e = pomdict[('COMM','mapscreen.title','',)]
        lines.append(f"#{e.msgstr}")

    for entryno in range(0,2):
        campgn = campaign_names[entryno]
        lines.append(f"# {campgn} map screen headings")
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

    for occurrence, e in pomdict.items():
        campgn, place, num = occurrence
        pass
    return


def create_lines_for_per_line(lines, pomdict):
    # sort by key
    pomdict = dict(sorted(pomdict.items()))
    for occurrence, e in pomdict.items():
        campgn, place, num = occurrence
        pass
    return


def create_lines_for_miss(lines, pomdict):
    for occurrence, e in pomdict.items():
        campgn, place, num = occurrence
        pass
    return


def create_lines_for_names(lines, pomdict):
    for occurrence, e in pomdict.items():
        campgn, place, num = occurrence
        pass
    return


def create_lines_for_netscan(lines, pomdict):
    for occurrence, e in pomdict.items():
        campgn, place, num = occurrence
        pass
    return


def create_lines_for_outro(lines, pomdict):
    for occurrence, e in pomdict.items():
        campgn, place, num = occurrence
        pass
    return


def create_lines_for_wms(lines, pomdict):
    for occurrence, e in pomdict.items():
        campgn, place, num = occurrence
        pass
    return


def textwad_create_from_po(lines, podict, txtfname):
    pomdict = {}
    for campgn, polist in podict.items():
        for e in polist:
            # Find entries matching for current txt file
            for place, num in e.occurrences:
                fname = po_occurrence_to_fname(campgn, place, num)
                if fname == txtfname:
                    pomdict[ (campgn,place,num,) ] = e
    match = None
    if not match:
        match = re.match(r'^(city)[.]txt$', txtfname)
        if match:
            create_lines_for_city(lines, pomdict)
    if not match:
        match = re.match(r'^(lost)[.]txt$', txtfname)
        if match:
            create_lines_for_per_line(lines, pomdict)
    if not match:
        match = re.match(r'^(miss([0-9]+))[.]txt$', txtfname)
        if match:
            create_lines_for_miss(lines, pomdict)
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
            create_lines_for_per_line(lines, pomdict)
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
        txtfname = e.Filename.decode("utf-8").lower()
        n = 0
        wadfh.seek(e.Offset, os.SEEK_SET)
        with open(txtfname, 'wb') as txtfh:
            copy_buffer = wadfh.read(e.Length)
            n += len(copy_buffer)
            txtfh.write(copy_buffer)
    return


def textwad_extract(po, wadfh, idxfh, lang):
    # Prepare empty dict
    podict = {}
    for campgn in campaign_names:
        polist = polib.POFile()
        podict[campgn] = polist
        pofile_set_default_metadata(polist, lang)
    # Fill with files from WAD
    e = WADIndexEntry()
    while idxfh.readinto(e) == sizeof(e):
        txtfname = e.Filename.decode("utf-8").lower()
        wadfh.seek(e.Offset, os.SEEK_SET)
        txt_buffer = wadfh.read(e.Length)
        lines = txt_buffer.split(b'\n')
        # TODO support SW code page
        lines = [ln.decode("utf-8").rstrip("\r\n") for ln in lines]
        textwad_extract_to_po(podict, txtfname, lines)
        basename = os.path.splitext(os.path.basename(txtfname))[0]
    if lang == "eng":
        poext = "pot"
    else:
        poext = "po"
    if poext == "pot":
        for campgn in campaign_names:
            for e in podict[campgn]:
                e.msgid = e.msgstr
                e.msgstr = ""
    merge_same_po_entries(podict)
    for campgn in campaign_names:
        pofname = "text_" + campgn.lower() + "_" + lang + "." + poext
        polist = podict[campgn]
        pofile_set_default_head_comment(polist, pofname)
        polist.save(pofname)
    return


def textwad_create(po, wadfh, idxfh, lang):
    # Load PO files
    if lang == "eng":
        poext = "pot"
    else:
        poext = "po"
    podict = {}
    for campgn in campaign_names:
        pofname = "text_" + campgn.lower() + "_" + lang + "." + poext
        polist = polib.pofile(pofname)
        podict[campgn] = polist
    if poext == "pot":
        for campgn in campaign_names:
            for e in podict[campgn]:
                e.msgstr = e.msgid
                e.msgid = ""
    txtfiles = list_txtfiles_from_po_content(podict)
    for txtfname in txtfiles:
        lines = []
        e = WADIndexEntry()
        e.Filename = txtfname.encode("utf-8")
        e.Offset = wadfh.tell()
        textwad_create_from_po(lines, podict, txtfname)
        # TODO support SW code page
        lines = [ln.encode("utf-8") for ln in lines]
        wadfh.write(b'\r\n'.join(lines))
        e.Length = wadfh.tell() - e.Offset
        idxfh.write((c_ubyte * sizeof(e)).from_buffer_copy(e))


def main():
    """ Main executable function.

    Its task is to parse command line options and call a function which performs requested command.
    """
    parser = argparse.ArgumentParser(description=__doc__.split('.')[0])

    parser.add_argument('-w', '--wadfile', type=str, required=True,
          help="Name for WAD/IDX files")

    parser.add_argument('-v', '--verbose', action='count', default=0,
          help="Increases verbosity level; max level is set by -vvv")

    subparser = parser.add_mutually_exclusive_group(required=True)

    subparser.add_argument('-x', '--extract', action='store_true',
          help="Extract to text files")

    subparser.add_argument('-c', '--create', action='store_true',
          help="Create WAD/IDX from text files")

    subparser.add_argument('--version', action='version', version="%(prog)s {version} by {author}"
            .format(version=__version__, author=__author__),
          help="Display version information and exit")

    po = parser.parse_args()

    po.wadbase = os.path.splitext(os.path.basename(po.wadfile))[0]
    assert len(po.wadbase) > 0, "Provided WAD file name base is too short"
    po.wadfile = po.wadbase + ".wad"
    po.idxfile = po.wadbase + ".idx"

    po.lang = "eng"

    if po.extract:
        if (po.verbose > 0):
            print("{}: Opening for extract".format(po.wadfile))
        with open(po.wadfile, 'rb') as wadfh:
            with open(po.idxfile, 'rb') as idxfh:
                textwad_extract(po, wadfh, idxfh, po.lang)

    elif po.create:
        if (po.verbose > 0):
            print("{}: Opening for creation".format(po.wadfile))
        with open(po.wadfile, 'wb') as wadfh:
            with open(po.idxfile, 'wb') as idxfh:
                textwad_create(po, wadfh, idxfh, po.lang)

    else:
        raise NotImplementedError("Unsupported command.")


if __name__ == '__main__':
    try:
        main()
    except Exception as ex:
        print("Error: "+str(ex), file=sys.stderr)
        if 1: raise
        sys.exit(10)
