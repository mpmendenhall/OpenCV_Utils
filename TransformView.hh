/// \file TransformView.hh Interactive "zoomable" image viewport using transform filter
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#ifndef TRANSFORMVIEW_HH
#define TRANSFORMVIEW_HH

#include "VTransformFilter.hh"
#include "ClickGetter.hh"

/// Interactive "zoomable" image viewport using transform filter
class TransformView {
public:
    /// Constructor with window name
    TransformView(VTransformFilter& T, Size ws, const char* window_name = "OpenCV image");

    /// set source image
    void setSource(Mat s);
    /// generate view image for virtual space region (optionally expanding to preserve aspect ratio)
    void setRegion(DRect vr, bool fillAspect = true);
    
    /// update display
    void updateView() { imshow(wname, myT.dst); }
    /// refresh view, clearing over-drawing
    void refresh() { myT.apply(src); }
    
    /// convert window coordinates to source image coordinates
    DPoint srcCoords(Point p) const { return myT.dst2src(p); }
    /// Convert source coordinates to window pixels
    Point viewCoords(DPoint p) const { return myT.src2dst(p); }
    
    /// zoom to region defined by current view pixel coordinates
    void zoomSrcRegion(Rect ROI, bool fillAspect = true);
    /// zoom to region containing source image points
    void zoomBounding(const vector<DPoint>& srcpts, bool fillAspect = true);
    /// reset to full-image view
    void unzoom();
    
    VTransformFilter& myT;      ///< transform filter
    const char* wname;          ///< name of window
    ClickGetter myCG;           ///< click handler for view window
    Mat src;                    ///< underlying full-scale image
    Size maxSize;               ///< maximum dimensions (window size) for output image
};

#endif
