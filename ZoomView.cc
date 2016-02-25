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
    _maxSize = maxSize = targetSize = Size(1200,800);
    resizeWindow(wname, targetSize.width, targetSize.height);
}

Size ZoomView::fitAspect(Size s, Size targ) {
    if(s.width*targ.height < s.height*targ.width)
        return Size((s.width*targ.height)/s.height, targ.height);
    else return Size(targ.width, (targ.width*s.height)/s.width);
}

void ZoomView::expandAspect(DPoint& p0, DPoint& p1) const {
    double dx = fabs(p1.x-p0.x);
    double dy = fabs(p1.y-p0.y);
    if(dx*maxSize.height < dy*maxSize.width) {
        double x0 = 0.5*(p0.x + p1.x);
        double hw = 0.5*maxSize.width*dy/maxSize.height;
        p0.x = x0 - hw;
        p1.x = x0 + hw;
    } else {
        double y0 = 0.5*(p0.y + p1.y);
        double hh = 0.5*maxSize.height*dx/maxSize.width;
        p0.y = y0 - hh;
        p1.y = y0 + hh;
    }
}

void ZoomView::setSource(Mat s) {
    src = s;
    maxSize = fitAspect(src.size(), _maxSize);
    setRegion(Rect(0,0,src.cols,src.rows));
}

void ZoomView::setRegion(Rect r) {
    rregion = r;
    rregion &= Rect(0,0,src.cols,src.rows); // necessary to avoid crash from out-of-image points
    targetSize = fitAspect(rregion.size(), maxSize);
    resize(src(rregion), iview, targetSize, 0, 0, INTER_AREA);
}

DPoint ZoomView::srcCoords(Point p) const {
    return DPoint(rregion.x + rregion.width*double(p.x)/targetSize.width,
                  rregion.y + rregion.height*double(p.y)/targetSize.height);
}



Point ZoomView::viewCoords(DPoint p) const {
    return Point(targetSize.width*(p.x - rregion.x)/rregion.width,
                 targetSize.height*(p.y - rregion.y)/rregion.height);
}

void ZoomView::zoomViewRegion(Rect ROI, bool fillAspect) {
    auto a0 = srcCoords(ROI.tl());
    auto a1 = srcCoords(ROI.br());
    if(fillAspect) expandAspect(a0,a1);
    Rect R(Point(a0.x,a0.y), Point(a1.x,a1.y));
    if(R.area() <= 0) {
        printf("\tNot zooming to zero-sized region.\n");
    } else {
        printf("\tZOOOM!\n");
        setRegion(R);
    }
}

void ZoomView::zoomSelectedRegion(bool fillAspect) {
    printf("Click two points to select zoom region...\n");
    zoomViewRegion(myCG.getRectangle(), fillAspect);
}
