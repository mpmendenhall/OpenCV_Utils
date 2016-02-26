/// \file VTransformFilter.hh Geomtric transform filter with "virtual" coordinate space
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#ifndef VTRANSFORMFILTER_HH
#define VTRANSFORMFILTER_HH

#include "FilterStack.hh"

/// Base class geometric transform with operations on "virtual" transformed coordinate space
class VTransformFilter: public FilterLayer {
public:
    /// Constructor
    VTransformFilter() { }
    
    /// Forward virtual space transform
    virtual DPoint src2v(DPoint p) const = 0;
    /// Reverse virtual space transform
    virtual DPoint v2src(DPoint p) const = 0;
    /// Get virtual space ROI containing source ROI
    virtual DRect vROI(DRect srcROI) const;
    /// dst to virtual transform
    DPoint dst2v(DPoint p) const { return src2v(dst2src(p)); }
    /// dst to virtual transform
    DRect dst2v(DRect R) const { return DRect(dst2v(R.tl()), dst2v(R.br())); }
    /// virtual to dst transform
    DPoint v2dst(DPoint p) const { return src2dst(v2src(p)); }
    
    /// Configure to map virtual coordinate region so dst fits within sOut
    virtual void configureView(DRect vROI, Size sIn, Size sOut) = 0;
    
protected:
    Size srcSize;       ///< size of source image
    Size dstSize;       ///< intended size for dst image
};

#endif
