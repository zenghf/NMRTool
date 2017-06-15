#!/usr/bin/python
import HTML
f = open('tmp')
lines = f.readlines()
f.close
t = []
for line in lines:
    expno = line.split('/')[-4]
    f = open(line[:-1],'r')
    tlines = f.readlines()
    f.close()
    title = '<p>'.join(tlines)+'<p>'
    t.append([expno, title])
t2 = sorted(t, key=lambda r:int(r[0]))    
htmlcode = HTML.Table(t2, header_row=['ExpNo', 'Title'])
#print htmlcode
HTMLFILE = 'a.html'
h = open(HTMLFILE, 'w')
h.write(str(htmlcode))
h.close()
