import requests
import argparse
import json
import os
import configparser

from colorama import init

def get_color(color_code):
    return '\x1b[%sm' % color_code


def parse_brief(brief):
    word = WORD_COLOR + brief['wordOut'] + ": "
    if 'relation' in brief['lemma']:
        word += TEXT_COLOR + (
                "%s为%s的%s" % (brief['wordOut'], brief['lemma']['lemma'], brief['lemma']['relation']))
    print(word)
    pron = ""
    if 'usPron' in brief:
        pron += HINT_COLOR + "    美音 " + TEXT_COLOR + "/%s/" % brief['usPron']['ps']
    if 'ukPron' in brief:
        pron += HINT_COLOR + "    英音 " + TEXT_COLOR + "/%s/" % brief['ukPron']['ps']
    if pron:
        print(pron)
    if 'chnDefinitions' in brief:
        print(SECTION_COLOR + "中文释义")
        for chn_def in brief['chnDefinitions']:
            if 'pos' in chn_def:
                print("    " + HINT_COLOR + chn_def['pos'].ljust(8) + TEXT_COLOR + chn_def[
                    'meaning'])
            else:
                print("    " + "".ljust(8) + TEXT_COLOR + chn_def['meaning'])
    if 'engDefinitions' in brief:
        print(SECTION_COLOR + "英文释义")
        for eng_def in brief['engDefinitions']:
            if 'pos' in eng_def:
                print("    " + HINT_COLOR + eng_def['pos'].ljust(8) + TEXT_COLOR + eng_def[
                    'meaning'])
            else:
                print("    " + "".ljust(8) + TEXT_COLOR + eng_def['meaning'])


def parse_source(sentence_group):
    if 'source' not in sentence_group:
        return "牛津高阶英汉双解词典"
    else:
        return sourceDict[sentence_group['source']]


def parse_detail(detail):
    parse_brief(detail['wordBrief'])
    if 'sentenceLists' in detail:
        print(SECTION_COLOR + "双语例句")
        for sentenceGroup in detail['sentenceLists']:
            count = 1
            print("".ljust(4) + HINT_COLOR + parse_source(sentenceGroup))
            for sentence in sentenceGroup['sentences']:
                print(TEXT_COLOR + "".ljust(8) + ("%s." % str(count)).ljust(3) + sentence['eng'])
                print("".ljust(8) + "".ljust(3) + sentence['chn'])
                if count >= default_sent:
                    break
                count += 1


init()

sourceDict = {"CAMBRIDGE": "剑桥高阶英汉双解词典", "LONGMAN": "朗文当代高级英语词典", "COLLINS": "柯林斯英汉双解大词典", "ONLINE": "金山词霸"}
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('word', type=str, help="The word you want to query")
parser.add_argument('--detail', '-d', action='store', default=0, const=2, nargs='?', type=int, dest='detail',
                    help="Show the detailed meaning of the word")
parser.add_argument('--brief', '-b', action='store_true', default=True, help="Show the brief meaning of the word", )
'\x1b[31m'
args = parser.parse_args()

config_path = os.path.join(os.path.split(os.path.realpath(__file__))[0], "color.ini")
config = configparser.ConfigParser()
config.read(config_path)
WORD_COLOR = get_color(config.getint('COLOR', 'word_color') if config.getint('COLOR', 'word_color') else 91)
HINT_COLOR = get_color(config.getint('COLOR', 'hint_color') if config.getint('COLOR', 'hint_color') else 92)
SECTION_COLOR = get_color(config.getint('COLOR', 'section_color') if config.getint('COLOR', 'section_color') else 93)
TEXT_COLOR = get_color(config.getint('COLOR', 'text_color') if config.getint('COLOR', 'text_color') else 97)
detail = json.loads(requests.get("https://ireading.site/word/detail?json=true&word=" + args.word).text)
default_sent = args.detail
if args.detail:
    parse_detail(detail)
else:
    parse_brief(detail['wordBrief'])
