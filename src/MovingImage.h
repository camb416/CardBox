//
//  MovingImage.h
//  CardBox
//
//  Created by Cameron Browning on 9/19/12.
//
//

#ifndef __CardBox__MovingImage__
#define __CardBox__MovingImage__

#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Timeline.h"
#include <string>
#include <iostream>


using namespace std;
using namespace ci;
using namespace ci::app;

class MovingImage {
public:
    MovingImage();
    ~MovingImage();
    void setup(string _path);
    void draw();
    void update();
    void setPos(Vec3f _pos);
    void moveTo(Vec3f _pos, float _s=1.0f);
    void setAlpha(float _alpha);
    void show(float _s=1.0f);
    void hide(float _s=1.0f);
    bool isVisible();
    int getWidth();
    int getHeight();
private:
    gl::Texture tex;
    string path;
    Anim<Vec3f> pos;
    Anim<float> alpha;
    Anim<float> rot;
    
};

#endif /* defined(__CardBox__MovingImage__) */
