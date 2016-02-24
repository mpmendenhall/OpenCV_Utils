#!/usr/bin/python

from PyxUtils import *
from LinFitter import *

def lsdat(fname):
    lns = open(fname,"r").readlines()
    print lns[1]
    d = [[float(x) for x in l.split()] for l in lns if l[0] != "#"]
    return [(x[0]/10,x[1]) for x in d]

if __name__=="__main__":
    dbase = "/Users/michael/Desktop/BubbleScans/IMG_%i.tiff_slice.txt"
    #dbase = "IMG_%i.tiff_slice.txt"
    ds = [(0,lsdat(dbase%4695)),
          (5,lsdat(dbase%4699)),
          (10,lsdat(dbase%4703)),
          (25,lsdat(dbase%4712)),
          (140,lsdat(dbase%4717)),
          (140,lsdat(dbase%4748)),
          (550,lsdat(dbase%4752))]
    
    g = graph.graphxy(width=15,height=10,
        x=graph.axis.lin(title="position [cm]", min=-2, max=12),
        y=graph.axis.lin(title="transmission", min=0, max=1.1),
	key = graph.key.key(pos="tc",columns=2))
    
    tcols = rainbowDict([d[0] for d in ds])
    for d in ds:
        gtitle = "%i minutes"%d[0]
        pstyle = [graph.style.symbol(symbol.circle, size = 0.04, symbolattrs=[style.linewidth.THin, tcols[d[0]]]), graph.style.line([tcols[d[0]]])]
        g.plot(graph.data.points(d[1],x=1,y=2,title = gtitle), pstyle)
    
    g.writetofile("bubbled.pdf")


    timeseq = [(0, 0.5635), (5, 0.5934), (10, 0.6083), (25, 0.6162), (140, 0.6263), (140,.6163), (550,.6775)]
    g = graph.graphxy(width=10,height=6,
        x=graph.axis.lin(title="cumulative bubbling time [minutes]"),
        y=graph.axis.lin(title="transmission"))
    g.plot(graph.data.points(timeseq,x=1,y=2), [graph.style.symbol(symbol.diamond), graph.style.line([style.linestyle.dashed])])
    #g.plot(graph.data.function("y(x)=0.563"), [graph.style.line([style.linestyle.dotted])])
    #g.plot(graph.data.function("y(x)=0.80"), [graph.style.line([style.linestyle.dotted])])
    g.writetofile("clarification.pdf")
