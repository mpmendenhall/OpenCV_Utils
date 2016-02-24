/// \file CollectPoints.cc Record selected points in image
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "ClickGetter.hh"
#include "Downsample.hh"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <cassert>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
using std::string;

using namespace std;
using namespace cv;

/// test ClickGetter response
void clicktest(const string& fname) {
    
    const char* window_name = "OpenCV image";
    namedWindow(window_name, CV_WINDOW_AUTOSIZE );
    ClickGetter CG;
    CG.accept.insert(ClickGetter::EVENT_KEYBOARD);
    setMouseCallback(window_name, clickGetterCallback, &CG);
    
    Mat src = imread(fname.c_str());
    Mat dst;
    resize(src, dst, Size(0,0), 0.25, 0.25, INTER_AREA);
    imshow(window_name, dst);
    
    while(1) {
        auto c = CG.getClick();
        printf("click: %i:%i\t%i, %i\n", c.evt, c.flags, c.p.x, c.p.y);
    }
}

int main(int argc, char** argv) {
    
    if(argc != 2) {
        printf("Usage: CollectPoints <image name>\n");
        return EXIT_FAILURE;
    }
    string imname = argv[1];
    //clicktest(imname);
    
    string outnm = imname+"_points.txt";
    
    // display window with mouse click tracking
    const char* window_name = "OpenCV image";
    namedWindow(window_name, CV_WINDOW_AUTOSIZE );
    ClickGetter CG;
    setMouseCallback(window_name, clickGetterCallback, &CG);
    
    // load image
    Mat src = imread(imname.c_str());
    Mat dst;
    resize(src, dst, Size(0,0), 0.25, 0.25, INTER_AREA);
    
    // Zoom in to ROI
    printf("Select ROI for points collection\n");
    imshow(window_name, dst);
    Mat roi = CG.getSubregion(src, 4.);
    resize(roi, dst, Size(0,0), 0.5, 0.5, INTER_AREA);
    
    // collect click points
    printf("Point selection commands:\n");
    printf("\tLeft-click to add new point;\n");
    printf("\t[Backspace] to remove previous point;\n");
    printf("\t[Enter] to commit points to file and start new goup;\n");
    printf("\t[Esc] to save remaining points and exit.\n");
    FILE* fout = fopen(outnm.c_str(),"w");
    imshow(window_name, dst);
    int gp = 0;
    vector<Point> cpts;
    CG.accept = { EVENT_LBUTTONUP, ClickGetter::EVENT_KEYBOARD };
    while(1) {
        auto c = CG.getClick();
        
        if(c.evt == ClickGetter::EVENT_KEYBOARD) {
            
            if(c.flags == 10) {
                printf("Saving group %i with %zu points.\n", gp, cpts.size());
                for(auto& p: cpts) fprintf(fout, "%i\t%i\t%i\n", gp, p.x, p.y);
                cpts.clear();
                gp++;
            } else if(c.flags == 65288) {
                if(cpts.size()) {
                    auto p = cpts.back();
                    printf("Deleting point %i:\t%i\t%i\n", gp, p.x, p.y);
                    cpts.pop_back();
                } else printf("No points left to delete.\n");
            } else if(c.flags == 27) break;
            else printf("Unknown key %i\n", c.flags);
            
        } else if(c.evt == EVENT_LBUTTONUP) {
            
            cpts.push_back(c.p);
            printf("Adding point %i:\t%i\t%i\n", gp, c.p.x, c.p.y);
            
        } else printf("unknown event: %i:%i\t%i, %i\n", c.evt, c.flags, c.p.x, c.p.y);
    }
    if(cpts.size()) printf("Saving group %i with %zu points.\n", gp, cpts.size());
    for(auto& p: cpts) fprintf(fout, "%i\t%i\t%i\n", gp, p.x, p.y);
    printf("Done!\n");
    fclose(fout);
    
    return EXIT_SUCCESS;
}
