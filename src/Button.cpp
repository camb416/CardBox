//
//  Button.cpp
//  CardBox
//
//  Created by Cameron Browning on 9/18/12.
//
//

#include "Button.h"


Button::Button(){
    pos = Vec2f(0,0);
    alpha = 0.0f;
    bDown = bOver = false;
}

void Button::moveTo(Vec2f _pos){
    pos = _pos;
}
void Button::slideTo(Vec2f _pos, float time_in){
    timeline().apply(&pos,_pos,time_in,EaseInOutAtan());
}

Button::Button(string path_str, Vec2f _pos){
        // eventually this shouldnt be loading from resources...
    pos = (_pos);
    alpha = 0.0f;
    try{
        tex = loadImage(loadResource(path_str));
    } catch(Exception e){
        console() << "figure i got a problem loading a button..." << endl;
    }
    bDown = bOver = false;
}
void Button::down(){
    bDown = true;
    alpha = 1.0f;
    // timeline().apply(&alpha,1.0f,0.1f,EaseNone());
}
void Button::up(){
    bDown = false;
    alpha = 0.8f;
  //  timeline().apply(&alpha,0.8f,0.1f,EaseNone());
}
bool Button::isDown(){
    return bDown;
}

void Button::hide(float time_in){
      timeline().apply(&alpha,0.0f,time_in,EaseNone());
}
void Button::show(float time_in){
    timeline().apply(&alpha,0.8f,time_in,EaseNone());
}

bool Button::isOver(Vec2f mousePos){
    
    float dist = mousePos.distance(pos);
    //if(dist<200)
    //console() << "distance to button: " << dist << endl;
    if(dist<tex.getWidth()/2){
        if(!bOver){
            //timeline().apply(&alpha,1.0f,0.3f,EaseNone());
        }
        bOver = true;
        //console() << "over" << endl;
        return true;
    } else {
        if(bOver){
           // timeline().apply(&alpha,0.8f,0.3f,EaseNone());
            bOver = false;
        }
        //console() << " not over" << endl;
        return false;
    }
    }

void Button::draw(){
    gl::pushMatrices();
    gl::translate(Vec2f(pos)-tex.getSize()/2);
   // if(bDown){
       // gl::color(1.0f,1.0f,1.0f,alpha);
   // } else if(bOver){
  //      gl::color(1.0f,1.0f,1.0f,alpha);
  //  } else {
        gl::color(1.0f,1.0f,1.0f,alpha);
  //  }
    gl::draw(tex);
    
    gl::popMatrices();
}
void Button::update(){
    // do i need to update anything here?
}
