/// \file VTransformFilter.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "VTransformFilter.hh"
using std::min;
    
DRect VTransformFilter::vROI(DRect srcROI) const {
    
    DPoint c0 = srcROI.tl();
    DPoint c1 = srcROI.br();
    DPoint c2 = DPoint(c0.x,c1.y);
    DPoint c3 = DPoint(c1.x,c0.y);
    vector<DPoint> bpts = { c0, c1, c2, c3 };
    for(auto& p: bpts) p = src2v(p);
    
    return boundingRect(bpts);
}
