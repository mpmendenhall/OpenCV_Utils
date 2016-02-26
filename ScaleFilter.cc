/// \file ScaleFilter.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "ScaleFilter.hh"
#include <opencv2/imgproc/imgproc.hpp>

void ScaleFilter::apply(const Mat& src) {
    resize(src(srcROI), dst, dstSize, 0, 0, INTER_AREA);
}
    
void ScaleFilter::configureView(DRect vROI, Size sIn, Size sOut) {
    srcSize = sIn;
    srcROI = v2src(vROI); // integer-rounded source ROI
    srcROI &= Rect(Point(0,0), srcSize); // clip to exist within source
    // force minimum 1x1 pixel size
    srcROI.width = max(1,srcROI.width);
    srcROI.height = max(1,srcROI.height);
    // re-calculate adjusted vROI
    vROI = DRect(src2v(srcROI.tl()), src2v(srcROI.br()));
    // determine output size for correct aspect ratio
    dstSize = fitAspect(vROI.size(), sOut);
    
    // calculate view transform parameters
    dsc.x = double(dstSize.width)/srcROI.width;
    dsc.y = double(dstSize.height)/srcROI.height;
    dp0 = srcROI.tl();
}
