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
void MovingImage::draw(){
    gl::pushMatrices();
    gl::translate(pos);
    gl::draw(tex,Vec2f(-0.5f*tex.getWidth(), -0.5f*tex.getHeight()));
    gl::popMatrices();
}

void MovingImage::setPos(Vec3f _pos){
    pos = _pos;
}
void MovingImage::moveTo(Vec3f _pos, float _s){
    timeline().apply(&pos,_pos,1.0f,_s,EaseOutAtan());
}
void MovingImage::setAlpha(float _alpha){
    alpha = _alpha;
}
void MovingImage::show(float _s){
    timeline().apply(&alpha,1.0f,_s,EaseOutAtan());
}
void MovingImage::hide(float _s){
    timeline().apply(&alpha,0.0f,_s,EaseOutAtan());

}