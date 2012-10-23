//
//  MovingImage.cpp
//  CardBox
//
//  Created by Cameron Browning on 9/19/12.
//
//

#include "MovingImage.h"

MovingImage::MovingImage(){
    // empty constructor
    pos = Vec3f(0,0,0);
    alpha = 1.0f;
    rot = 0.0f;
    path = "";
}
MovingImage::~MovingImage(){
    // empty destructor
}
void MovingImage::setup(string _path){
    path = _path;
    tex = gl::Texture(loadImage(loadResource(path)));
}
void MovingImage::update(){
    // empty update
}

bool MovingImage::isVisible(){
    return(alpha>0.0f);
}
void MovingImage::draw(){
    gl::pushMatrices();
    gl::translate(pos);
    gl::color(1.0f,1.0f,1.0f,alpha);
    gl::draw(tex,Vec2f(-0.5f*tex.getWidth(), -0.5f*tex.getHeight()));
    gl::popMatrices();
}

void MovingImage::setPos(Vec3f _pos){
    pos = _pos;
}
void MovingImage::moveTo(Vec3f _pos, float _s){
  timeline().apply(&pos,_pos,_s,EaseInOutSine());
   }
void MovingImage::setAlpha(float _alpha){
    alpha = _alpha;
}
void MovingImage::show(float _s){
    timeline().apply(&alpha,1.0f,_s,EaseInOutSine());
}
void MovingImage::hide(float _s){
    timeline().apply(&alpha,0.0f,_s,EaseInOutSine());

}
int MovingImage::getWidth(){
    if(tex){
        return tex.getWidth();
    } else {
        return -1;
    }
}
int MovingImage::getHeight(){
    if(tex){
        return tex.getHeight();
    } else {
        return -1;
    }
}