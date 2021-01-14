from xeger import Xeger
import random

def getone():
    _x = Xeger()
    testStr = _x.xeger(r'[pqr]|-[pqr]|\([pqr][v^>][pqr]\)|-\([pqr][v^>][pqr]\)')
    return testStr

def getbc():
    _x = Xeger()
    testStr = _x.xeger(r'[v^>]')
    return testStr



reg = "[pqr]|-[pqr]|\([pqr][v^>][pqr]\)|\(-[pqr][v^>][pqr]\)"

# f = open("test/input.txt","w")
# for i in range(0,10000):
#     length = random.randint(1,50)
#     str = "";
#     for j in range(0,length):
#         str+=random.choice('-()pqrv>^')
#     str+="\n"


#     f.write(str)
def getfmla():
    a = ""
    l = random.randint(1,5)
    if l == 1 :
        a = getone()
    if l == 2 :
        a = "(" + getone() + getbc() + getone()+")"
    if l == 3 :
        b = "(" + getone() + getbc() + getone()+")"
        a = "(" + b + getbc() + getone()+")"
    if l == 4 :
        c = "(" + getone() + getbc() + getone()+")"
        b = "(" + c + getbc() + getone()+")"
        a = "(" + b + getbc() + getone()+")"
    if l == 5 :
        d = "(" + getone() + getbc() + getone()+")"
        c = "(" + d + getbc() + getone()+")"
        b = "(" + c + getbc() + getone()+")"
        a = "(" + b + getbc() + getone()+")"
    return a

f = open("test/input.txt","w")
for i in range(0,5000):
    f.write(getfmla()+"\n")




