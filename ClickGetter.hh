/// \file ClickGetter.hh Interactive click-point-locating class
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

#ifndef CLICKGETTER_HH
#define CLICKGETTER_HH

#include <vector>
#include <set>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

/// Receives click positions in window
class ClickGetter {
public:
    /// Constructor
    ClickGetter();
    
    /// click position information
    struct click {
        Point p;        ///< click position
        int evt;        ///< event type
        int flags;      ///< event flags
    };
    
    static const int EVENT_KEYBOARD;    ///< flag for including keyboard events with mouse
    
    /// wait until one more click received
    click getClick();
    /// get rectangular region
    Rect getRectangle(bool twoclick = true);
    /// select rectangular subregion of image
    Mat getSubregion(Mat& src, double scale = 1, bool twoclick = true);
    
    volatile bool received;     ///< flag for when event has been received
    Point mousepos;             ///< moving mouse position
    vector<click> clicks;       ///< list of clicks received
    set<int> accept;            ///< click types to accept
};

/// window callback for ClickGetter
void clickGetterCallback(int event, int x, int y, int flags, void* params);

#endif
