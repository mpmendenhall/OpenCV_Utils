/// \file BaseTypes.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "BaseTypes.hh"
using std::min;

Size fitAspect(Size s, Size targ) {
    if(s.width*targ.height < s.height*targ.width)
        return Size((s.width*targ.height)/s.height, targ.height);
    else return Size(targ.width, (targ.width*s.height)/s.width);
}

Size fitAspect(DSize s, Size targ) {
    if(s.width*targ.height < s.height*targ.width)
        return Size((s.width*targ.height)/s.height, targ.height);
    else return Size(targ.width, (targ.width*s.height)/s.width);
}

DRect expandAspect(DRect R, Size targ) {
    DPoint p0 = R.tl();
    DPoint p1 = R.br();
    if(R.width*targ.height < R.height*targ.width) {
        double x0 = 0.5*(p0.x + p1.x);
        double hw = 0.5*targ.width*R.height/targ.height;
        p0.x = x0 - hw;
        p1.x = x0 + hw;
    } else {
        double y0 = 0.5*(p0.y + p1.y);
        double hh = 0.5*targ.height*R.width/targ.width;
        p0.y = y0 - hh;
        p1.y = y0 + hh;
    }
    return DRect(p0,p1);
}

DRect boundingRect(const vector<DPoint>& v) {
    if(!v.size()) return DRect();
    DPoint p0(v[0]);
    DPoint p1(v[1]);
    for(auto p: v) {
        p0.x = min(p0.x,p.x);
        p0.y = min(p0.y,p.y);
        p1.x = max(p1.x,p.x);
        p1.y = max(p1.y,p.y);
    }
    return DRect(p0,p1);
}

vector<DPoint> corners(DRect R) {
    DPoint c0 = R.tl();
    DPoint c1 = R.br();
    DPoint c2 = DPoint(c0.x,c1.y);
    DPoint c3 = DPoint(c1.x,c0.y);
    return { c0, c3, c1, c2 };
}
