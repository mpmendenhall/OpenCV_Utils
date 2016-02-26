/// \file CollectPoints.cc Record selected points in image
// This file was produced under the employ of the United States Government,
// and is consequently in the PUBLIC DOMAIN, free from all provisions of
// US Copyright Law (per USC Title 17, Section 105).
// 
// -- Michael P. Mendenhall, 2016

#include "ClickGetter.hh"
#include "TransformView.hh"
#include "ScaleFilter.hh"
#include "PerspectiveFilter.hh"
#include "MarkSet.hh"

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

string dropLast(const string& str, const string& splitchars) {
    return str.substr(0, str.find_last_of(splitchars));
}

int main(int argc, char** argv) {
    
    if(argc < 2 || argc > 3) {
        printf("Usage: CollectPoints <image name> [output filename]\n");
        return EXIT_FAILURE;
    }
    string imname = argv[1];
    string outnm = argc==3? argv[2] : dropLast(imname,".")+"_points.txt";
    
    // load, display image
    Mat src = imread(imname.c_str());
    //ScaleFilter SF;
    PerspectiveFilter PF;
    //SF.vsc = DPoint(3,2);
    TransformView TV(PF, fitAspect(src.size(), Size(1200,800)));
    TV.setSource(src);
    TV.updateView();
    
    // show instructions
    printf("Point selection commands:\n");
    printf("\tLeft-click to add new point;\n");
    printf("\tRight-click-and-drag to zoom in to selected region.\n");
    printf("\t[u] to unzoom to whole image.\n");
    printf("\t[Backspace] to remove previous point;\n");
    printf("\t[Enter] to commit points to file and start new goup;\n");
    printf("\t[Esc] to save remaining points and exit.\n");
    
    ///////////////////////
    // collect click points
    FILE* fout = fopen(outnm.c_str(),"w");
    int gp = 0;
    vector<DPoint> cpts;                // points in current group
    MarkSet MS;                         // image display annotations
    
    TV.myCG.accept = { EVENT_LBUTTONDOWN, EVENT_RBUTTONDOWN, EVENT_RBUTTONUP, ClickGetter::EVENT_KEYBOARD };
    ClickGetter::click prevclick; // previous mouse-down location
    while(1) {
        auto c = TV.myCG.getClick();
        
        if(c.evt == EVENT_RBUTTONUP && prevclick.evt == EVENT_RBUTTONDOWN) {
            
            TV.zoomSrcRegion(Rect(c.p, prevclick.p));
            MS.draw(TV.myT);
            TV.updateView();
            
        } else if(c.evt == EVENT_RBUTTONDOWN) {
            
            // nothing done here; wait for up next time around
            
        } else if(c.evt == ClickGetter::EVENT_KEYBOARD) {
            
            if(c.flags == 10 || c.flags == 13) { // [enter]
                
                printf("Saving group %i with %zu points.\n", gp, cpts.size());
                for(auto& p: cpts) fprintf(fout, "%i\t%g\t%g\n", gp, p.x, p.y);
                gp++;
                
                for(auto m: MS.marks) m->setColor(CV_RGB(0,0,255));
                cpts.clear();
                TV.refresh();
                MS.draw(TV.myT);
                TV.updateView();
                
            } else if(c.flags == 127 || c.flags == 65288) { // [delete]
                
                if(cpts.size()) {
                    auto p = cpts.back();
                    printf("Deleting point %i:\t%g\t%g\n", gp, p.x, p.y);
                    cpts.pop_back();
                    MS.popMark();
                    TV.refresh();
                    MS.draw(TV.myT);
                    TV.updateView();
                } else printf("No points left to delete.\n");
                
            } else if(c.flags == 117) { // 'u'
                TV.unzoom();
                MS.draw(TV.myT);
                TV.updateView();
            } else if(c.flags == 112) { // 'p'
                PF.calcPerspective(cpts);
                TV.zoomBounding(cpts);
                TV.refresh();
                MS.draw(TV.myT);
                TV.updateView();
            } else if(c.flags==114) { // 'r'
                PF.resetPerspective();
                TV.unzoom();
                MS.draw(TV.myT);
                TV.updateView();
            } else if(c.flags == 27) break; // [esc]
            else printf("Unknown key %i\n", c.flags);
            
        } else if(c.evt == EVENT_LBUTTONDOWN) {
            auto p = TV.srcCoords(c.p);
            cpts.push_back(p);
            printf("Adding point %i:\t%g\t%g\n", gp, p.x, p.y);
            MS.addMark(new CircleMark(p,CV_RGB(0,255,0)), &TV.myT);
            TV.updateView();
            
        } else printf("unknown event: %i:%i\t%i, %i\n", c.evt, c.flags, c.p.x, c.p.y);
        
        prevclick = c;
    }
    if(cpts.size()) printf("Saving group %i with %zu points.\n", gp, cpts.size());
    for(auto& p: cpts) fprintf(fout, "%i\t%g\t%g\n", gp, p.x, p.y);
    printf("Points output to '%s'.\n", outnm.c_str());
    fclose(fout);
    
    return EXIT_SUCCESS;
}
