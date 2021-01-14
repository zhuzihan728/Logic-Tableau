import difflib

A = open('test/output1.txt', 'r')

B = open('test/output2.txt', 'r')

contextA = A.read()

contextB = B.read()

s = difflib.SequenceMatcher(lambda x:x=="", contextA, contextB)

result = s.get_opcodes()

for tag, i1, i2, j1, j2 in result:
    if (tag != "equal"):
        print("%s contextA[%d:%d]=%s \ncontextB[%d:%d]=%s"%\
            (tag, i1, i2, contextA[i1:i2], j1, j2, contextB[j1:j2]))
         