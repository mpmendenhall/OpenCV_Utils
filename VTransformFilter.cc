/// \file VTransformFilter.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "VTransformFilter.hh"
using std::min;
    
DRect VTransformFilter::vROI(DRect srcROI) const {
    DPoint c0 = src2v(srcROI.tl());
    DPoint c1 = src2v(srcROI.br());
    DPoint c2(c0.x,c1.y);
    DPoint c3(c1.x,c0.y);
    
    return DRect(DPoint(min(c0.x, min(c1.x, min(c2.x, c3.x))), min(c0.y, min(c1.y, min(c2.y, c3.y)))),
                 DPoint(max(c0.x, max(c1.x, max(c2.x, c3.x))), max(c0.y, max(c1.y, max(c2.y, c3.y)))));
}
