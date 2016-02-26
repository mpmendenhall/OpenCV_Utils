/// \file PerspectiveFilter.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "PerspectiveFilter.hh"
#include <opencv2/imgproc/imgproc.hpp>

PerspectiveFilter::PerspectiveFilter() {
    resetPerspective();
}

void PerspectiveFilter::apply(const Mat& src) {
    warpPerspective(src, dst, M1, dstSize);
}

DPoint perspectivetx(DPoint p, const Mat& M) {
    vector<DPoint> pIn = {p}; 
    vector<DPoint> pOut(1);
    perspectiveTransform(pIn, pOut, M);
    return pOut[0];
}

DPoint PerspectiveFilter::src2dst(DPoint p) const {
    return perspectivetx(p,M1);
}

DPoint PerspectiveFilter::dst2src(DPoint p) const {
    return perspectivetx(p,M1i);
}

DPoint PerspectiveFilter::src2v(DPoint p) const {
    return perspectivetx(p,M0);
}

DPoint PerspectiveFilter::v2src(DPoint p) const {
    return perspectivetx(p,M0i);
}

void PerspectiveFilter::calcPerspective(const vector<DPoint>& psrc, vector<DPoint> pdst) {
    if(psrc.size() < 4) { printf("Need 4 points to update perspective!\n"); return; }
    if(pdst.size() < 4)  pdst = vector<DPoint>({ DPoint(0,0), DPoint(1,0), DPoint(1,1), DPoint(0,1)});
    Point2f pfsrc[4] = {psrc[0], psrc[1], psrc[2], psrc[3]};
    Point2f pfdst[4] = {pdst[0], pdst[1], pdst[2], pdst[3]};
    M0 = getPerspectiveTransform(pfsrc, pfdst);
    M0i = M0.inv();
}

void PerspectiveFilter::resetPerspective() {
    M0 = Mat_<double>(3,3,0.);
    for(int i=0; i<3; i++) M0(i,i) = 1.;
    M0i = M0;
}

void PerspectiveFilter::configureView(DRect vROI, Size, Size sOut) {
    dstSize = sOut; // set output size
    
    // calculate transform from virtual-to-dst
    Point2f p0 = vROI.tl();
    Point2f p1 = vROI.br();
    Point2f psrc[4] = {p0, Point2f(p0.x,p1.y), p1, Point2f(p1.x,p0.y)};
    Point2f pdst[4] = {Point2f(0,0), Point2f(0,sOut.height), Point2f(sOut.width,sOut.height), Point2f(sOut.width,0)};
    Mat Ma = getPerspectiveTransform(psrc, pdst);
    
    // compose for total src-to-dst transformation
    M1 = Ma*M0;
    M1i = M1.inv();
}
