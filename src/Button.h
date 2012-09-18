//
//  Button.h
//  CardBox
//
//  Created by Cameron Browning on 9/18/12.
//
//

#ifndef __CardBox__Button__
#define __CardBox__Button__

#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Timeline.h"
#include <string>
#include <iostream>


using namespace std;
using namespace ci;
using namespace ci::app;


class Button{
public:
    Button();
    Button(string path_str,Vec2f pos);
    bool isOver(Vec2f mousePos);
    Vec2f pos;
    Anim<float> alpha;
    void draw();
    void update();
private:
    gl::Texture tex;
    
};

#endif /* defined(__CardBox__Button__) */
