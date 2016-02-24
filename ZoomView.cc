/// \file ZoomView.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

#include "ZoomView.hh"
#include <opencv2/imgproc/imgproc.hpp>

ZoomView::ZoomView(const char* window_name): wname(window_name) { 
    namedWindow(wname, WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    setMouseCallback(wname, clickGetterCallback, &myCG);
    maxSize = targetSize = Size(1200,800);
    resizeWindow(wname, targetSize.width, targetSize.height);
}

Size ZoomView::fitAspect(Size s) const {
    if(s.width*maxSize.height < s.height*maxSize.width)
        return Size((s.width*maxSize.height)/s.height, maxSize.height);
    else return Size(maxSize.width, (maxSize.width*s.height)/s.width);
}

void ZoomView::setSource(Mat s) {
    src = s;
    setRegion(Rect(0,0,src.cols,src.rows));
}

void ZoomView::setRegion(Rect r) {
    rregion = r;
    rregion &= Rect(0,0,src.cols,src.rows);
    targetSize = fitAspect(rregion.size());
    resize(src(rregion), iview, targetSize, 0, 0, INTER_AREA);
}

array<double,2> ZoomView::srcCoords(Point p) const {
    return array<double,2>({{rregion.x + rregion.width*double(p.x)/targetSize.width, rregion.y + rregion.height*double(p.y)/targetSize.height}});
}

void ZoomView::zoomSelectedRegion() {
    Rect ROI = myCG.getRectangle();
    auto a0 = srcCoords(ROI.tl());
    auto a1 = srcCoords(ROI.br());
    setRegion(Rect(Point(a0[0],a0[1]), Point(a1[0],a1[1])));
}
