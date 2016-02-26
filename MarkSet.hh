/// \file MarkSet.hh Collection of mark annotations to draw over image
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#ifndef MARKSET_HH
#define MARKSET_HH

#include "ZoomView.hh"
#include <opencv2/imgproc/imgproc.hpp>

/// Base class for marks/annotations to draw on image
class ImageMark {
public:
    /// Constructor
    ImageMark() { }
    /// Destructor
    virtual ~ImageMark() {}
    /// Set mark color
    virtual void setColor(const Scalar& mc) { color = mc; }
    /// Draw to image
    virtual void draw(ZoomView& ZV) const = 0;
    
    Scalar color;       ///< draw color
};

/// Fixed-radius circle mark
class CircleMark: public ImageMark {
public:
    /// Constructor
    CircleMark(DPoint p, const Scalar& cl, int r0 = 4, int t0 = 1): c(p), r(r0), t(t0) { setColor(cl); }  
    /// Draw to image
    void draw(ZoomView& ZV) const override {
        Point p = ZV.viewCoords(c);
        circle(ZV.iview, p, r, color, t, CV_AA);
    }
    
    DPoint c;           ///< center
    int r;              ///< radius
    int t;              ///< line thickness
};

/// Collection of image marks
class MarkSet {
public:
    /// Constructor
    MarkSet() { }
    /// Destructor
    ~MarkSet() { for(auto m: marks) delete m; }
    /// Add a mark (takes ownership)
    void addMark(ImageMark* m, ZoomView* ZV = NULL) { if(m) { marks.push_back(m); if(ZV) m->draw(*ZV); } }
    /// Pop mark from list; delete
    void popMark() { if(marks.size()) { delete marks.back(); marks.pop_back(); } }
    
    /// Draw to image
    void draw(ZoomView& ZV) const { for(auto m: marks) m->draw(ZV); }
    
    vector<ImageMark*> marks;
};
    
#endif
    