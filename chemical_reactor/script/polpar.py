data =[
["#H1:P4" , "CNST 29" , "4" , "2" , "Regenerate back pressure "],
["go" , "CNST 30" , "1" , "2" , "Magnetic field offset, stray field of the NMR magnet"],
["#H1:GA" , "CNST 31" , "4" , "2" , "Regenerate Field "],
["#H1:TO" , "CNST 32" , "4" , "2" , "Timeout while back pressure building up"],
["#T1:P2" , "CNST 33" , "4" , "2" , "Transfer Phase 1, transfer pressure"],
["#T1:P1" , "CNST 34" , "4" , "2" , "Transfer Phase 1, back pressure"],
["#T1:TO" , "CNST 35" , "4" , "2" , "Transfer Phase 1, timeout"],
["#T2:P2" , "CNST 36" , "4" , "2" , "Transfer Phase 2, transfer pressure"],
["#T2:P1" , "CNST 37" , "4" , "2" , "Transfer Phase 2, back pressure"],
["#T2:TO" , "CNST 38" , "4" , "2" , "Transfer Phase 2, timeout"],
["#T3:P2" , "CNST 39" , "4" , "2" , "Transfer Phase 3, transfer pressure"],
["#T3:P1" , "CNST 40" , "4" , "2" , "Transfer Phase 3, back pressure"],
["#T3:TO" , "CNST 41" , "4" , "2" , "Transfer Phase 3, timeout"],
["#T5:GA" , "CNST 42" , "4" , "2" , "Field at the end of transfer"],
["#R2:TO" , "CNST 43" , "4" , "2" , "Return timeout with high pressure (from T3:P1)"],
["#R3:P1" , "CNST 44" , "4" , "2" , "Return back pressure"],
["#R3:TO" , "CNST 45" , "4" , "2" , "Return timeout"],
["MP" , "CNST 46" , "2" , "2" , "Maximum pressure difference for VR3 and VR4 during regeneration"],
["mp" , "CNST 47" , "2" , "2" , "Minimum pressure difference for VR3 and VR4 during regeneration"]]

for line in data:
    varName = 'polPar_' + line[0].replace('#','').replace(':','_')
    print 'define delay', varName
    print '"' + varName + '= abs(' + line[1].lower().replace(' ', '') + ')"'

for line in data:
    print line[1].lower().replace(' ', '')+': <'+ line[0]+ '>' +' '*(7-len(line[0]))+ line[4]
