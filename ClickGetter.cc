/// \file ClickGetter.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

#include "ClickGetter.hh"
#include <iostream>

const int ClickGetter::EVENT_KEYBOARD = 999;

ClickGetter::ClickGetter(): received(true) {
    accept = { EVENT_LBUTTONUP, EVENT_RBUTTONDOWN };
}

ClickGetter::click ClickGetter::getClick() {
    received = false;
    int k = -1;
    while(!received) {
        k = waitKey(10);
        if(k != -1 && accept.count(EVENT_KEYBOARD)) {
            received = true;
            click c;
            c.p = mousepos;
            c.evt = EVENT_KEYBOARD;
            c.flags = k;
            clicks.push_back(c);
        }
    }
    return clicks.back();
}

Rect ClickGetter::getRectangle(bool twoclick) {
    clicks.clear();
    if(twoclick) accept = { EVENT_LBUTTONUP, EVENT_RBUTTONDOWN };
    else accept = { EVENT_LBUTTONUP, EVENT_LBUTTONDOWN };
    getClick();
    getClick();
    return Rect(clicks[0].p, clicks[1].p);
}

Mat ClickGetter::getSubregion(Mat& src, double scale, bool twoclick) {
    clicks.clear();
    if(twoclick) accept = { EVENT_LBUTTONUP, EVENT_RBUTTONDOWN };
    else accept = { EVENT_LBUTTONUP, EVENT_LBUTTONDOWN };
    getClick();
    getClick();
    Rect rregion = getRectangle(twoclick);
    rregion.x *= scale;
    rregion.y *= scale;
    rregion.width *= scale;
    rregion.height *= scale;
    rregion &= Rect(0,0,src.cols,src.rows);
    //cout << clicks[0].p << "\t" << clicks[1].p << "\t" << rregion << "\n";
    return src(rregion);
}

void clickGetterCallback(int event, int x, int y, int flags, void* params) {
    assert(params);
    ClickGetter* CG = (ClickGetter*)params;
    if(CG->received) return;
    if (event == EVENT_MOUSEMOVE) CG->mousepos = Point(x,y);
    if(CG->accept.count(event)) {
        ClickGetter::click c;
        c.p = Point(x,y);
        c.evt = event;
        c.flags = flags;
        CG->clicks.push_back(c);
        CG->received = true;
    }
}
