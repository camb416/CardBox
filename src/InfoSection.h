//
//  InfoSection.h
//  CardBox
//
//  Created by Cameron Browning on 9/19/12.
//
//

#ifndef __CardBox__InfoSection__
#define __CardBox__InfoSection__

#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Timeline.h"
#include <string>
#include <iostream>


using namespace std;
using namespace ci;
using namespace ci::app;

class InfoSection{
public:
    void setup(string p1_str,string p2_str);
    InfoSection();
    Anim<Vec3f> pos;
    Anim<float> alpha;
    gl::Texture pageOne;
    gl::Texture pageTwo;
    void open();
    void close();
    void draw();
    bool isVisible();
private:
    
    
};



#endif /* defined(__CardBox__InfoSection__) */
