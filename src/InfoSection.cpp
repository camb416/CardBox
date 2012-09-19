//
//  InfoSection.cpp
//  CardBox
//
//  Created by Cameron Browning on 9/19/12.
//
//

#include "InfoSection.h"

InfoSection::InfoSection(){
    pos = Vec3f(0,0,0);
    alpha = 0.0f;
}

void InfoSection::setup(string p1_str, string p2_str){
    pageOne = gl::Texture(loadImage(loadResource(p1_str)));
    pageTwo = gl::Texture(loadImage(loadResource(p2_str)));
}
void InfoSection::draw(){
    gl::color(1.0f,1.0f,1.0f,alpha);
    gl::pushMatrices();
    gl::translate(getWindowCenter());
    gl::draw(pageOne,Vec2f(pageOne.getWidth()/-2.0f,pageTwo.getWidth()/-2.0f));
    
    gl::popMatrices();
}

bool InfoSection::isVisible(){
    return (alpha>0.0f);
}
void InfoSection::open(){
    timeline().apply(&alpha,1.0f,0.3f,EaseNone());
}
void InfoSection::close(){
    timeline().apply(&alpha,0.0f,0.2f,EaseNone());
    
}