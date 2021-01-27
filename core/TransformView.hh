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
    void setSource(Mat s) { src = s; unzoom(); }
    
    /// convert window coordinates to source image coordinates
    DPoint srcCoords(Point p) const { return myT.dst2src(p); }
    /// Convert source coordinates to window pixels
    Point viewCoords(DPoint p) const { return myT.src2dst(p); }
    
    /// update display after modifying myT.dst
    void updateView() { imshow(wname, myT.dst); }
    /// refresh view, clearing over-drawing
    void refresh() { myT.apply(src); }
    
    // view window control commands
    
    /// generate view image for virtual space region (optionally expanding to preserve aspect ratio)
    void setRegion(DRect vr, bool fillAspect = true);
    /// zoom to region defined by dst coordinates
    void zoomSrcRegion(Rect ROI, bool fillAspect = true) { setRegion(myT.dst2v(ROI), fillAspect); }
    /// zoom to region containing source image points
    void zoomBounding(const vector<DPoint>& srcpts, bool fillAspect = true);
    /// zoom out to contain full source image
    void unzoom() { setRegion(myT.vROI(DRect(Point(),src.size()))); }
    
    /// Options for updating view when underlying transform changes
    enum txUpdateView {
        VIEW_HOLD_VROI,         ///< cover same virtual ROI coordinates as before
        VIEW_HOLD_CENTER,       ///< follow source centerpoint with same vROI range
        VIEW_HOLD_KEY           ///< keep srcKey keypoints in view
    };
    /// Notify of transform update and choose view window selection method
    void updateTransform(txUpdateView u);
    
    /// set current vROI as source key points
    void setvROIkey() { srcKey = corners(vROI); for(auto& p: srcKey) p = myT.v2src(p); }
    
    
    VTransformFilter& myT;      ///< transform filter
    const char* wname;          ///< name of window
    ClickGetter myCG;           ///< click handler for view window
    Mat src;                    ///< underlying full-scale image
    Size maxSize;               ///< maximum dimensions (window size) for output image
    
    vector<DPoint> srcKey;      ///< source key point locations for view range
    DRect vROI;                 ///< current virtual ROI
    DPoint srcCenter;           ///< source point at center of view
    
};

#endif
