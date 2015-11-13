#!/usr/bin/python

from PyxUtils import *
from LinFitter import *

def lsdat(fname):
    lns = open(fname,"r").readlines()
    print lns[0]
    d = [[float(x) for x in l.split()] for l in lns if l[0] != "#"]
    return [(x[0]+130, x[1]) for x in d]

if __name__=="__main__":
    dbase = "IMG_%i.tiff_slice.txt"
    ds = [(0,lsdat(dbase%4695)),
          (5,lsdat(dbase%4699)),
          (10,lsdat(dbase%4703)),
          (25,lsdat(dbase%4712)),
          (140,lsdat(dbase%4717))]
    
    g = graph.graphxy(width=15,height=10,
        x=graph.axis.lin(title="position (arb)", min=0, max=500),
        y=graph.axis.lin(title="transmission", min=0, max=1.1),
	key = graph.key.key(pos="tc",columns=2))
    
    tcols = rainbowDict([d[0] for d in ds])
    for d in ds:
        gtitle = "%i minutes"%d[0]
        pstyle = [graph.style.symbol(symbol.circle, size = 0.04, symbolattrs=[style.linewidth.THin, tcols[d[0]]]), graph.style.line([tcols[d[0]]])]
        g.plot(graph.data.points([x for x in d[1] if 50 < x[0] < 450],x=1,y=2,title = gtitle), pstyle)
    
    g.writetofile("bubbled.pdf")


    timeseq = [(0, 0.562781), (5, 0.595457), (10, 0.609974), (25, 0.617297), (140, 0.627664)]
    g = graph.graphxy(width=10,height=6,
        x=graph.axis.lin(title="bubbling time [minutes]"),
        y=graph.axis.lin(title="transmission"))
    g.plot(graph.data.points(timeseq,x=1,y=2), [graph.style.symbol(symbol.diamond), graph.style.line([style.linestyle.dashed])])
    g.writetofile("clarification.pdf")
