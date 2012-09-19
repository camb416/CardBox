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
    curPage = 0;
}

void InfoSection::setup(string p1_str, string p2_str){
    pageOne.setup(p1_str);
    pageTwo.setup(p2_str);
    pageTwo.setAlpha(0.0f);
    pageOne.setAlpha(0.0f);
    pageOne.setPos(Vec3f(0,0,0));
    pageTwo.setPos(Vec3f(1000,0,0));

}
void InfoSection::draw(){
        //if(alpha>0.01f){
            gl::color(1.0f,1.0f,1.0f,alpha);
            gl::pushMatrices();
            gl::translate(getWindowCenter());

        pageOne.draw();
        pageTwo.draw();
            gl::popMatrices();
   // }
    
}

bool InfoSection::isVisible(){
    return (alpha>0.0f);
}
void InfoSection::open(){
    //timeline().apply(&alpha,1.0f,0.3f,EaseNone());
    pageOne.setPos(Vec3f(0,0,0));
    pageOne.show();
    
}
void InfoSection::close(){
    // timeline().apply(&alpha,0.0f,0.2f,EaseNone());
    pageOne.hide();
    pageTwo.hide();
}
void InfoSection::next(){
    if(curPage == 0) {
    pageOne.moveTo(Vec3f(-1000,0,0));
    pageOne.hide();
    pageTwo.moveTo(Vec3f(0,0,0));
    pageTwo.show();
        curPage = 1;
    } else {
        prev();
    }
}
void InfoSection::prev(){
    // do something
    if(curPage == 1) {
        pageOne.setPos(Vec3f(-1000,0,0));
        
        pageTwo.moveTo(Vec3f(1000,0,0));
        pageTwo.hide();
        pageOne.moveTo(Vec3f(0,0,0));
        pageOne.show();
        curPage = 0;
    } else {
        next();
    }
}