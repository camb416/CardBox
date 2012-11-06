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
    Anim<Vec2f> pos;
    Anim<float> alpha;
    void draw();
    void update();
    void down();
    void up();
    bool isDown();
    void hide(float time_in = 0.2f);
    void show(float time_in = 0.3f);
    void moveTo(Vec2f _pos);
    void slideTo(Vec2f _pos, float time_in = 1.0f);
    void addAlternate(string path_str);
    bool swap();
    bool getState();
private:
    gl::Texture tex;
    gl::Texture altTex;
    bool bOver;
    bool bDown;
    bool buttonState;
    
};

#endif /* defined(__CardBox__Button__) */
