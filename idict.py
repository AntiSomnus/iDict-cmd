import requests
import argparse
import json
sourceDict={"CAMBRIDGE":"剑桥高阶英汉双解词典","LONGMAN":"朗文当代高级英语词典","COLLINS":"柯林斯英汉双解大词典","ONLINE":"金山词霸"}
parser = argparse.ArgumentParser(description='manual to this script')
parser.add_argument('word', type=str, help="The word you want to query")
parser.add_argument('--detail','-d', action='store_true', default=False,  help="Show the detailed meaning of the word")
parser.add_argument('--brief','-b',  action='store_true', default=True, help="Show the brief meaning of the word", )

args = parser.parse_args()

def parseBrief(brief):
    print ("输入的单词: "+ brief['wordIn'])
    print ("输出的单词: "+brief['wordOut'])
    if 'relation' in brief['lemma']:
        print ("%s为%s的%s"%(brief['wordOut'],brief['wordIn'],brief['lemma']['relation']))
    if 'usPron' in brief or 'ukPron' in brief:
          print ("\n音标")
    if 'usPron' in brief:
        print ("  美式发音: "+brief['usPron']['ps'])
    if 'ukPron' in brief:
        print ("  英式发音: "+brief['ukPron']['ps'])
    if 'chnDefinitions' in brief:
        print ("\n中文释义")
        for chn_def in brief['chnDefinitions']:
            if 'pos' in chn_def:
                print ("  "+chn_def['pos']+" "+chn_def['meaning'])
            else:
                print ("  "+chn_def['meaning'])
    if 'engDefinitions' in brief:
        print ("\n英文释义")
        for eng_def in brief['engDefinitions']:
            if 'pos' in eng_def:
                print ("  "+eng_def['pos']+" "+eng_def['meaning'])
            else:
                print ("  "+eng_def['meaning'])
def parseSource(sentenceGroup):
    if 'source' not in sentenceGroup:
        return "牛津高阶英汉双解词典"
    else:
        return sourceDict[sentenceGroup['source']]
def parseDetail(detail):
    parseBrief(detail['wordBrief'])
    if 'derivatives' in detail:
        print ("\n词形变换")
        for derivative in detail['derivatives']:
            print("  "+derivative['relation']+": "+derivative['word'])
    if 'sentenceLists' in detail:
        print ("\n双语释义")
        for sentenceGroup in detail['sentenceLists']:
            count=1
            print ("".ljust(2)+parseSource(sentenceGroup))
            for sentence in sentenceGroup['sentences']:
                print("".ljust(4)+str(count).ljust(3)+sentence['eng'])
                print("".ljust(4)+"".ljust(3)+sentence['chn'])
                count+=1


if args.detail:
    detail=json.loads(requests.get("https://ireading.site/word/detail?json=true&word="+args.word).text)
    parseDetail(detail)
else:
    brief=json.loads(requests.get("https://ireading.site/word/brief?json=true&word="+args.word).text)
    parseBrief(brief)
