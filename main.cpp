#include <X11/Xlib.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <cmath>

#include "vroot.h"

#include "QTree.h"
#include "PointDist.h"
#include "PointNorm.h"
#include "PointUni.h"


const unsigned int numDist = 2;

unsigned long int CreateColor(int red, int green, int blue) {
    return (red << 16) + (green << 8) + blue;
}


int main(int argc, char* argv[]) {
    // Seed random
    srand(time(0));
    
    unsigned int width = 800;
    unsigned int height = 800;
       
    // Open the display
    Display* pDisplay = XOpenDisplay(getenv("DISPLAY"));

    // Get a handle to the root window
    Window root = DefaultRootWindow(pDisplay);

    // Create graphics context
    GC gc = XCreateGC(pDisplay, root, 0, NULL);

    Window windowReturn;
    int x;
    int y;
    unsigned int border;
    unsigned int depth;
    XGetGeometry(pDisplay, root, &windowReturn, &x, &y, &width, 
                      &height, &border, &depth);

    width = width - border - x - 2;
    height = height - border - y - 2;

    QTree<PointDist::Point> qt(0, 0, width, height, 4);
    
    std::vector<PointDist::Point> points;
    
    // Set screen to black
    XSetForeground(pDisplay, gc, WhitePixelOfScreen(DefaultScreenOfDisplay(pDisplay))); 

    // Create QTree color
    unsigned long int qtColor = CreateColor(167, 30, 130);

    PointDist* pPointGen;

    // Choose a random distribution
    unsigned int distro = rand() % numDist;
    switch (distro) {
        case 0:
            pPointGen = new PointNorm();
        break;
        case 1:
            pPointGen = new PointUni();
        break; 
    }
    pPointGen->setXRange(0, width);
    pPointGen->setYRange(0, height);

    while (true) {
       
        
        // Clear screen 
        // XClearWindow(pDisplay, root);
     
        // Generate point
        PointDist::Point p = pPointGen->Generate();
        points.push_back(p);       
 
        // Create point color 
        unsigned long int ptColor = CreateColor(rand() % 255, rand() % 255, rand() % 255);
 
        unsigned int last = points.size() - 1;
        qt.Insert(points[last].x, points[last].y, &points[last]);
    
        // Draw points
        XSetForeground(pDisplay, gc, ptColor); 
        XDrawArc(pDisplay, root, gc, points[last].x, points[last].y, 2, 2, 0, 6.262);

        // Draw QTree
        XSetForeground(pDisplay, gc, qtColor); 
        qt.Draw(pDisplay, &root, &gc);

        // Swap buffers
        XFlush(pDisplay);

        // Pause
        usleep(100000); 
    } 

    if (pPointGen != nullptr) {
        delete pPointGen;
        pPointGen = nullptr;
    } 

    XCloseDisplay(pDisplay);
 
    return 0;
}
