#!/usr/bin/python3
# -*- coding: utf-8 -*-
# vim:sw=4

""" Text DAT conversion tool for Syndicate Wars Fan Expansion.

Prepares DAT files accepted by game engine.
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
from ctypes import memmove, addressof, sizeof, Array


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


def datitem_string_national_adjust(stri):
    stro = ""
    for c in stri:
        if c in "êô":
            stro = stro + unicodedata.normalize('NFKD', c).encode('ascii', 'ignore').decode('ascii', errors='ignore')
        elif c in "ÍÓÚ": # TODO is that really needed? remove, if it will not cause regression in these chars
            stro = stro + c.lower()
        else:
            stro = stro + c
    return stro


def datitem_string_to_bytes(po, s):
    s = datitem_string_national_adjust(s)
    b = enctable_string_to_bytes(po.chartable_d_encode, s)
    return b


def datitem_bytes_to_string(po, b):
    s = enctable_bytes_to_string(po.chartable_d_decode, b)
    return s


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
      'Project-Id-Version': 'GUI text for SyndWarsFX',
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
      "     GUI text for SyndWarsFX translation file\n" + \
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


def prep_po_entries_per_line(polist, lines, refstart, comment):
    n = 0
    for ln in lines[n:]:
        if len(ln) < 1:
            n += 1
            continue
        if True:
            text = '\n'.join(ln.split('\\n'))
            if (text.isupper()):
                text = text[0].upper() + text[1:].lower()
            e = polib.POEntry(msgstr=text, msgctxt=comment)
            e.occurrences.append( (f'{refstart}',f'{n+1}',) )
            polist.append(e)
            n += 1
            continue
        break
    return


def textdat_extract_to_po(podict, lines):
    prep_po_entries_per_line(podict, lines, 'guitext', "Menu interface text")
    return

def textdat_part_of_po_entry_upper(k, e):
    if (k >= 565) and (k <= 580):
        sublns = e.msgstr.split('\n')
        if (len(sublns) > 1) and (' ' not in sublns[0]):
            sublns[0] = sublns[0].upper()
        s = '\\n'.join(sublns)
    else:
        s = e.msgstr.upper()
    return s


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
            lines.append(textdat_part_of_po_entry_upper(k, e))
    lines.append("")
    return


def textdat_create_from_po(lines, polist, txtfname):
    pomdict = {}
    if True:
        campgn = "COMM"
        for e in polist:
            # Find entries matching for current txt file
            for place, num in e.occurrences:
                pomdict[ (campgn,place,num,) ] = e
    create_lines_for_per_line(lines, pomdict, 'guitext')
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


def merge_same_po_entries(polist):
    if True:
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


def textdat_empty_polist(po, lang):
    # Prepare empty list
    polist = polib.POFile()
    pofile_set_default_metadata(polist, lang)
    return polist


def textdat_read_to_polist(po, polist, datfh):
    # Fill with DAT file content
    if True:
        datfh.seek(0, os.SEEK_SET)
        txt_buffer = datfh.read()
        lines = txt_buffer.split(b'\0')
        lines = [datitem_bytes_to_string(po, ln) for ln in lines]
        textdat_extract_to_po(polist, lines)
    return


def textdat_extract(po):
    polist = textdat_empty_polist(po, po.lang)
    engpolist = textdat_empty_polist(po, "eng")
    if True:
        # Input language PO
        with open(po.datfile, 'rb') as datfh:
            textdat_read_to_polist(po, polist, datfh)

    if po.lang != "eng":
        # Temporary English PO
        with open(po.engdatfile, 'rb') as datfh:
            textdat_read_to_polist(po, engpolist, datfh)

    if po.lang == "eng":
        if True:
            for e in polist:
                e.msgid = e.msgstr
                e.msgstr = ""
    else:
        if True:
            engpolist = engpolist
            for e in polist:
                ene = find_occurence_in_polist(engpolist, e.occurrences[0])
                if ene is not None:
                    e.msgid = ene.msgstr
    merge_same_po_entries(polist)
    if True:
        pofname = po.pofile
        pofile_set_default_head_comment(polist, pofname)
        polist.save(pofname)
    return


def textdat_create(po, datfh):
    # Load PO files
    pofname = po.pofile
    polist = polib.pofile(pofname)

    if True:
        for e in polist:
            if e.msgstr == "":
                e.msgstr = e.msgid
    if True:
        txtfname = po.datfile;
        lines = []
        textdat_create_from_po(lines, polist, txtfname)
        lines = [datitem_string_to_bytes(po, ln) for ln in lines]
        datfh.write(b'\0'.join(lines))


def main():
    """ Main executable function.

    Its task is to parse command line options and call a function which performs requested command.
    """
    parser = argparse.ArgumentParser(description=__doc__.split('.')[0])

    parser.add_argument('pofile', type=str,
          help="PO/POT file name")

    parser.add_argument('-d', '--datfile', type=str, required=True,
          help="Name for DAT file")

    parser.add_argument('-e', '--engdatfile', type=str, default="",
          help="Name for english language DAT file, required to extract non-english DATs to PO")

    parser.add_argument('-l', '--lang', type=str, default="eng",
          help="Language of the processed files, required to extract non-english DATs to PO")

    parser.add_argument('-v', '--verbose', action='count', default=0,
          help="Increases verbosity level; max level is set by -vvv")

    parser.add_argument('-t', '--enctable', type=str,
          help="Character encoding table file name for directly converted strings")

    subparser = parser.add_mutually_exclusive_group(required=True)

    subparser.add_argument('-x', '--extract', action='store_true',
          help="Extract to text files")

    subparser.add_argument('-c', '--create', action='store_true',
          help="Create DAT from text files")

    subparser.add_argument('--version', action='version', version="%(prog)s {version} by {author}"
            .format(version=__version__, author=__author__),
          help="Display version information and exit")

    po = parser.parse_args()

    (po.chartable_d_decode, po.chartable_d_encode) = read_enctable(po, po.enctable)

    if po.extract:
        if (po.verbose > 0):
            print("{}: Opening for extract".format(po.datfile))
        textdat_extract(po)

    elif po.create:
        if (po.verbose > 0):
            print("{}: Opening for creation".format(po.datfile))
        with open(po.datfile, 'wb') as datfh:
            textdat_create(po, datfh)

    else:
        raise NotImplementedError("Unsupported command.")


if __name__ == '__main__':
    try:
        main()
    except Exception as ex:
        print("Error: "+str(ex), file=sys.stderr)
        if 1: raise
        sys.exit(10)
