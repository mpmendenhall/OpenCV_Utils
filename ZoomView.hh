/// \file ZoomView.hh Interactive "zoomable" image viewport
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

#ifndef ZOOMVIEW_HH
#define ZOOMVIEW_HH

#include "BaseTypes.hh"
#include "ClickGetter.hh"

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
    /// refresh zoom image, clearing any over-drawing
    void refresh() { setRegion(rregion); }
    
    /// expand bounding box to fit window aspect ratio
    void expandAspect(DPoint& p0, DPoint& p1) const;
    
    /// convert window coordinates to source image coordinates
    DPoint srcCoords(Point p) const;
    /// Convert source coordinates to window pixels
    Point viewCoords(DPoint p) const;
    
    /// zoom to region defined by current view pixel coordinates
    void zoomViewRegion(Rect ROI, bool fillAspect = true);
    /// user input zoom rectangle selection
    void zoomSelectedRegion(bool fillAspect = true);
    /// reset to full-image view
    void unzoom() { setSource(src); }
    
    const char* wname;  ///< name of window
    ClickGetter myCG;   ///< click handler for view window
    Mat src;            ///< underlying full-scale image
    Mat iview;          ///< scaled view version
    Size maxSize;       ///< view window maximum dimensions
    Size _maxSize;      ///< full maximum dimensions for any image
    Size targetSize;    ///< view window dimensions
    Rect rregion;       ///< view region of source image
};

#endif
