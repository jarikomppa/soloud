""" Complains if some API call is not documented """

import soloud_codegen
import glob
import os

def checkfile(apifunc, fname):
    """ Checks whether the string can be found in a file """
    if apifunc in open(fname).read():
        #print apifunc + " found in " + fname
        return True
    return False
    
def checkfiles(apifunc):
    """ Goes through all multimarkdown files in doc dir and passes
        them to checkfile. If found, early out.
    """
    for file in glob.glob("../doc/*.mmd"):
        if checkfile(apifunc, file):
            return True
    return False

print "Checking for undocumented APIs.."
print

found = 0
total = 0

for func in soloud_codegen.soloud_func:
    apifunc = func[1].replace("_",".")
    if ((apifunc[-2::] != "Ex") and 
        (apifunc[-7::] != "destroy") and 
        (apifunc[-6::] != "create")):
        total += 1
        if not checkfiles(apifunc + "()"):
            print apifunc
            found += 1

if found == 0:
    print "All good! (" + str(total) + ", not counting ctor/dtor/ex)"
else:
    print
    print str(found) + " / " + str(total) + " APIs undocumented. Get to work!"
    print "(not counting ctor/dtor/ex variants)"
