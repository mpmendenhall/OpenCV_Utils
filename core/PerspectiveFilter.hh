/// \file PerspectiveFilter.hh Perspective transform filter
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#ifndef PERSPECTIVEFILTER_HH
#define PERSPECTIVEFILTER_HH

#include "VTransformFilter.hh"

class PerspectiveFilter: public VTransformFilter {
public:
    /// Constructor
    PerspectiveFilter();
    
    /// Apply transform to create dst output
    void apply(const Mat& src) override;
    
    /// Forward position transform
    DPoint src2dst(DPoint p) const override;
    /// Backwards position transform
    DPoint dst2src(DPoint p) const override;
    /// Forward virtual space transform
    DPoint src2v(DPoint p) const override;
    /// Reverse virtual space transform
    DPoint v2src(DPoint p) const override;
    /// Configure to map virtual coordinate region so dst fits within sOut
    void configureView(DRect vROI, Size sIn, Size sOut) override;
    
    /// Calculate perspective from image quadrangle to virtual (default unit square)
    void calcPerspective(const vector<DPoint>& psrc, vector<DPoint> pdst = vector<DPoint>());
    /// Reset to unity perspective
    void resetPerspective();
    
    Mat_<double> M0;    ///< src-to-virtual space perspective transform
    Mat_<double> M0i;   ///< inverse M0
    
protected:
    
    Mat_<double> M1;    ///< src-to-dst perspective transform
    Mat_<double> M1i;   ///< inverse M1
};

#endif
