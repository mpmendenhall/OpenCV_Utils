/// \file ClickGetter.hh Interactive click-point-locating class
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

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
    };
    
    /// wait until one more click received
    click getClick();
    /// select rectangular subregion of image
    Mat getSubregion(Mat& src, bool twoclick = true);
    
    volatile bool received;     ///< flag for when event has been received
    vector<click> clicks;       ///< list of clicks received
    set<int> accept;            ///< click types to accept
};

/// window callback for ClickGetter
void clickGetterCallback(int event, int x, int y, int, void* params);
