/// \file ScaleFilter.hh Rescaling filter
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#ifndef SCALEFILTER_HH
#define SCALEFILTER_HH

#include "VTransformFilter.hh"

class ScaleFilter: public VTransformFilter {
public:
    /// Constructor
    ScaleFilter(): vsc(1,1), dsc(1,1) { }
    
    /// Apply transform to create dst output
    void apply(const Mat& src) override;
    
    /// Forward position transform
    DPoint src2dst(DPoint p) const override { return DPoint((p.x-dp0.x)*dsc.x, (p.y-dp0.y)*dsc.y); }
    /// Backwards position transform
    DPoint dst2src(DPoint p) const override { return DPoint(p.x/dsc.x+dp0.x, p.y/dsc.y+dp0.y); }
    /// Forward virtual space transform
    DPoint src2v(DPoint p) const override { return DPoint((p.x-vp0.x)*vsc.x, (p.y-vp0.y)*vsc.y); }
    /// Reverse virtual space transform
    DPoint v2src(DPoint p) const override { return DPoint(p.x/vsc.x+vp0.x, p.y/vsc.y+vp0.y); }
    /// Reverse virtual space transform
    DRect v2src(DRect R) const { return DRect(v2src(R.tl()), v2src(R.br())); }
    /// Configure to map virtual coordinate region so dst fits within sOut
    void configureView(DRect vROI, Size sIn, Size sOut) override;
    
    DPoint vp0; ///< image point mapping to virtual 0,0
    DPoint vsc; ///< x,y image to virtual scale factors
    
protected:
    Rect srcROI;        ///< region-of-interest in source mapped to destination
    DPoint dp0;         ///< image point mapping to destination 0,0
    DPoint dsc;         ///< image to dst x,y scale factors
};

#endif
