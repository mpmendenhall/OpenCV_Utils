/// \file FilterStack.hh Base classes for image transformation "stacks"
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#ifndef FILTERSTACK_HH
#define FILTERSTACK_HH

#include "BaseTypes.hh"

/// Base class for one layer in a filter stack
class FilterLayer {
public:
    /// Constructor
    FilterLayer() { }
    /// Destructor
    virtual ~FilterLayer() { }
    
    /// Apply transform to create dst output
    virtual void apply(const Mat& src) = 0;
    /// Forward position transform
    virtual DPoint src2dst(DPoint p) const { return p; }
    /// Backwards position transform
    virtual DPoint dst2src(DPoint p) const { return p; }
    
    Mat dst;    ///< output image
};

/// Stack of FilterLayers, which may itself be treated as one filter
class FilterStack: public FilterLayer {
public:
    /// Constructor
    FilterStack() { }
    /// Destructor
    ~FilterStack() { for(auto l: layers) delete l; }
    
    /// Partial update
    void update(const Mat& src, size_t i0) { for(size_t i=i0; i<layers.size(); i++) layers[i]->apply(i? layers[i-1]->dst : src); }
    /// Apply transform to create dst output
    void apply(const Mat& src) override { update(src,0); }
    /// Forward position transform
    DPoint src2dst(DPoint p) const override { for(auto l: layers) p = l->src2dst(p); return p; }
    /// Backwards position transform
    DPoint dst2src(DPoint p) const override { for(auto it = layers.rbegin(); it != layers.rend(); it++) p = (*it)->dst2src(p); return p; }
    
    vector<FilterLayer*> layers;
};

#endif
