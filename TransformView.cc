/// \file TransformView.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "TransformView.hh"
#include <stdio.h>

TransformView::TransformView(VTransformFilter& T, Size ws, const char* window_name):
myT(T), wname(window_name), maxSize(ws) { 
    namedWindow(wname, WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    setMouseCallback(wname, clickGetterCallback, &myCG);
    resizeWindow(wname, maxSize.width, maxSize.height);
}

void TransformView::setRegion(DRect vr, bool fillAspect) {
    if(fillAspect)  vr = expandAspect(vr,maxSize);
    myT.configureView(vr, src.size(), maxSize);
    vROI = vr;
    srcCenter = myT.v2src(0.5*(vROI.tl() + vROI.br()));
    refresh();
}

void TransformView::zoomBounding(const vector<DPoint>& srcpts, bool fillAspect) {
    vector<DPoint> vpts;
    for(auto p: srcpts) vpts.push_back(myT.src2v(p));
    setRegion(boundingRect(vpts), fillAspect);
}

void TransformView::updateTransform(txUpdateView u) {
    if(u == VIEW_HOLD_CENTER) {
        DPoint c0 = 0.5*(vROI.tl() + vROI.br());
        vROI += myT.src2v(srcCenter) - c0;
        setRegion(vROI);
    } else if(u == VIEW_HOLD_KEY) zoomBounding(srcKey);
    else setRegion(vROI); // default VIEW_HOLD_VROI
}
