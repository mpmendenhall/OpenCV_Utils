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

void TransformView::setSource(Mat s) {
    src = s;
    setRegion(myT.vROI(DRect(Point(),src.size())));
}

void TransformView::setRegion(DRect vr, bool fillAspect) {
    if(fillAspect)  vr = expandAspect(vr,maxSize);
    myT.configureView(vr, src.size(), maxSize);
    refresh();
}

void TransformView::unzoom() {
    setRegion(myT.vROI(DRect(Point(),src.size())));
}

void TransformView::zoomSrcRegion(Rect ROI, bool fillAspect) {
    setRegion(myT.dst2v(ROI), fillAspect);
}

void TransformView::zoomBounding(const vector<DPoint>& srcpts, bool fillAspect) {
    vector<DPoint> vpts;
    for(auto p: srcpts) vpts.push_back(myT.src2v(p));
    setRegion(boundingRect(vpts), fillAspect);
}
