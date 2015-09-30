/// \file ClickGetter.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

#include "ClickGetter.hh"
#include <iostream>

ClickGetter::ClickGetter(): received(true) {
    accept = { EVENT_LBUTTONUP, EVENT_RBUTTONDOWN };
}

void ClickGetter::getClick() {
    received = false;
    while(!received) waitKey(10);
}

Mat ClickGetter::getSubregion(Mat& src, bool twoclick) {
    clicks.clear();
    if(twoclick) accept = { EVENT_LBUTTONUP, EVENT_RBUTTONDOWN };
    else accept = { EVENT_LBUTTONUP, EVENT_LBUTTONDOWN };
    getClick();
    getClick();
    Rect rregion(clicks[0].p, clicks[1].p);
    rregion &= Rect(0,0,src.cols,src.rows);
    //cout << clicks[0].p << "\t" << clicks[1].p << "\t" << rregion << "\n";
    return src(rregion);
}

void clickGetterCallback(int event, int x, int y, int, void* params) {
    assert(params);
    ClickGetter* CG = (ClickGetter*)params;
    if(CG->received) return;
    if(CG->accept.count(event)) {
        ClickGetter::click c;
        c.p = Point(x,y);
        c.evt = event;
        CG->clicks.push_back(c);
        CG->received = true;
    }
}
