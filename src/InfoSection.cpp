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
    gl::draw(pageOne);
}

