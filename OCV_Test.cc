/// \file OCV_Test.cc
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2015

#include "ClickGetter.hh"
#include "Downsample.hh"

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

/// Calculate transmission based on image sets
class TxCalculator {
public:
    TxCalculator() { }
    
    Mat illum;          ///< illumination lightfield
    Mat illum_dark;     ///< illumination darkfield
    Mat object;         ///< object in front of illumination lightfield
    Mat object_dark;    ///< object in front of illumination darkfield
    Mat tx;             ///< calculated transmission
    
    /// calculate transmission, assuming illum and object assigned and darkfield-subtracted
    void calcTx() {
        tx = illum.clone();
        tx /= object;
    }
};

int main(int, char**) {
    // display window with mouse click tracking
    const char* window_name = "OpenCV image";
    namedWindow(window_name, CV_WINDOW_AUTOSIZE );
    ClickGetter CG;
    setMouseCallback(window_name, clickGetterCallback, &CG);
    
    // load image as greyscale
    cout << "Loading file...\n";
    Mat src = imread("IMG_4657.tiff", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_GRAYSCALE);
    cout << "Loaded data of type " << src.type() << "\n";
    //vector<Mat> bgr;
    //split(src, bgr);
    
    // downsampled floating point image; scale to display nicely.
    Mat_<float> resamp;
    // blue = 1,1
    // red = 0,0
    sumSubsample<uint16_t>(src, 2, 3, 1, 1, resamp, 1./(1<<10));
    
    // black point... actually want to use dark frame
    double minval, maxval;
    minMaxLoc(resamp, &minval, &maxval);
    cout << "Image range " << minval << " to " << maxval << "\n";
    resamp -= minval;
    
    // optional rotate
    //transpose(resamp,resamp);
    //flip(resamp, resamp, 0);
    
    imwrite("Subsampled.jpg", resamp*256.);
    
    // identify sub-region to sum
    imshow(window_name, resamp);
    Mat subregion = CG.getSubregion(resamp);
    
    //g32.convertTo(bgr[1],CV_8U);
    
    bool yproject = false;
    
    // column sum average
    Mat colSum;
    reduce(subregion, colSum, yproject, CV_REDUCE_AVG);
    //for(auto it = colSum.begin<float>(); it != colSum.end<float>(); it++)
    //    cout << int(*it * 1000) << "\n";
    
    // stretch to 200 y lines for display
    Mat linestretch;
    if(yproject) repeat(colSum, 1, 100, linestretch);
    else repeat(colSum, 100, 1, linestretch);
    imshow(window_name, linestretch);
    waitKey(0);
    
    return EXIT_SUCCESS;
}
