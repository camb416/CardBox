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
}
void Button::up(){
    bDown = false;
}
bool Button::isDown(){
    return bDown;
}

void Button::hide(){
      timeline().apply(&alpha,0.0f,0.2f,EaseNone());
}
void Button::show(){
    timeline().apply(&alpha,1.0f,0.3f,EaseNone());
}

bool Button::isOver(Vec2f mousePos){
    
    float dist = mousePos.distance(pos);
    //if(dist<200)
    //console() << "distance to button: " << dist << endl;
    if(dist<tex.getWidth()/2){
        bOver = true;
        //console() << "over" << endl;
        return true;
    } else {
        bOver = false;
        //console() << " not over" << endl;
        return false;
    }
    }

void Button::draw(){
    gl::pushMatrices();
    gl::translate(pos-tex.getSize()/2);
    if(bDown){
        gl::color(1.0f,1.0f,0.0f,alpha);
    } else if(bOver){
        gl::color(0.0f,1.0f,0.0f,alpha);
    } else {
        gl::color(1.0f,1.0f,1.0f,alpha);
    }
    gl::draw(tex);
    
    gl::popMatrices();
}
void Button::update(){
    // do i need to update anything here?
}
