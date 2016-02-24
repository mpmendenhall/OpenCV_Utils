/// \file ZoomView.hh Interactive "zoomable" image viewport
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

#ifndef ZOOMVIEW_HH
#define ZOOMVIEW_HH

#include "ClickGetter.hh"
#include <opencv2/highgui/highgui.hpp>
#include <array>
using std::array;
using namespace std;
using namespace cv;

/// Interactive "zoomable" image viewport
class ZoomView {
public:
    /// Constructor
    ZoomView(const char* window_name = "OpenCV image");

    /// set source image
    void setSource(Mat s);
    /// generate re-scaled image for viewing region
    void setRegion(Rect r);
    
    /// update display
    void updateView() { imshow(wname, iview); }
    
    /// fit aspect ratio within maximum
    Size fitAspect(Size s) const;
    
    /// convert window coordinates to source image coordinates
    array<double,2> srcCoords(Point p) const;
    
    /// user input zoom rectangle selection
    void zoomSelectedRegion();
    
    const char* wname;  ///< name of window
    ClickGetter myCG;   ///< click handler for view window
    Mat src;            ///< underlying full-scale image
    Mat iview;          ///< scaled view version
    Size maxSize;       ///< view window maximum dimensions
    Size targetSize;    ///< view window dimensions
    Rect rregion;       ///< view region of source image
};

#endif
