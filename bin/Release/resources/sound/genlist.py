import argparse
import os
import codecs
parser = argparse.ArgumentParser()
parser.add_argument("-p","--path", help="Directory path.",default=os.path.dirname(os.path.realpath(__file__)),type=str)
args = parser.parse_args()
mlist=[]
os.chdir(args.path)
for file in os.listdir():
    if os.path.isfile(file): mlist.append(file)
if args.path[-1]!= "\\": args.path+="\\"
args.path+="flist.txt"
print(args.path)
f=codecs.open(args.path, "wb",encoding="UTF-8")
for it in mlist:
   if it!="flist.txt" and it!="genlist.py": f.write(it+"\r\n")

