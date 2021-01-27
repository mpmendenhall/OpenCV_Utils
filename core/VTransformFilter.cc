/// \file VTransformFilter.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "VTransformFilter.hh"
using std::min;

DRect VTransformFilter::vROI(vector<DPoint> srcpts) const {
    for(auto& p: srcpts) p = src2v(p);
    return boundingRect(srcpts);
}
