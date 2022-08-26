/// \file RawLight.cc Average light in RAW image regions
// - Michael P. Mendenhall, 2021

/*
export IMDAT=$HOME/Work/PROSPECT/Presentations/2021-01-25_XTalk/
./bin/RawLight ${IMDAT}/P2_lift_1_mm.NEF ${IMDAT}/P2_lift_BG.NEF
*/

#ifdef WITH_LIBRAW

#include <stdlib.h>
#include <cmath>
#include "QuadLayers.hh"
#include "CodeVersion.hh"
#include "ClickGetter.hh"

int main(int argc, char** argv) {
    CodeVersion::display_code_version();
    if(argc < 3) return EXIT_FAILURE;

    // display window with mouse click tracking
    const char* window_name = "SumLight image";
    namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    ClickGetter CG;
    setMouseCallback(window_name, clickGetterCallback, &CG);

    // open FG, BG files

    LibRaw i0;
    i0.open_file(argv[1]);
    QuadLayers QL(i0);

    i0.recycle();
    i0.open_file(argv[2]);
    QuadLayers QD(i0);

    // select blue channel in FG, BG
    size_t colorchan = 0;
    auto& DatImg = QL.M[colorchan];
    auto& BGImg = QD.M[colorchan];

    cv::GaussianBlur(DatImg, DatImg, {}, 5);
    cv::GaussianBlur(BGImg, BGImg, {}, 5);

    double minVal;
    double maxVal;
    cv::Point minLoc;
    cv::Point maxLoc;
    minMaxLoc(DatImg, &minVal, &maxVal, &minLoc, &maxLoc);
    printf("Raw image range %g -- %g\n", minVal, maxVal);

    // subtract darkframe
    DatImg -= BGImg;

    // preview image (normalized, nonlinear boosted brightness)
    auto ViewImg = DatImg.clone();
    auto DarkView = BGImg.clone();
    for(auto& p: ViewImg) p = p > 0? pow(p/maxVal, 0.5) : 0;
    for(auto& p: DarkView) p = p > 0? pow(p/maxVal, 0.5) : 0;
    cv::imwrite("lightcells.jpg", ViewImg * 256);
    cv::imwrite("darkcells.jpg", DarkView * 256);
    cv::imshow(window_name, ViewImg);

    // average values in selected rectangles
    while(true) {
        printf("\nSelect integration region with two clicks\n");
        auto R = CG.getRectangle(DatImg);
        double lmean = cv::mean(DatImg(R))[0];
        double bmean = cv::mean(BGImg(R))[0];
        printf("Mean value: %g (subtracted background: %g)\n", lmean, bmean);
    }

    return EXIT_SUCCESS;
}

#else

int main(int, char**) { return 0; }

#endif
