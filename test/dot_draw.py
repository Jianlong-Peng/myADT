'''
#=============================================================================
#     FileName: dot_draw.py
#         Desc: 
#       Author: jlpeng
#        Email: jlpeng1201@gmail.com
#     HomePage: 
#      Created: 2014-08-16 10:30:37
#   LastChange: 2014-08-16 10:31:56
#      History:
#=============================================================================
'''
import sys
import os
from glob import glob

def main(argv=sys.argv):
    if len(argv) < 2:
        print "\n  Usage: %s in.dot[...]"%argv[0]
        sys.exit(1)

    for name in argv[1:]:
        for each in glob(name):
            cmd = "dot -Tpng %s -o %s"%(each, each.replace(".dot",".png"))
            os.system(cmd)

main()

