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
    
    closeButton = Button("closeButton.png",Vec2f(getWindowWidth()-75,75));
    prevButton = Button("leftArrow.png",Vec2f(75,getWindowHeight()/2));
    nextButton = Button("rightArrow.png",Vec2f(getWindowWidth()-75,getWindowHeight()/2));

}
void InfoSection::draw(){
        //if(alpha>0.01f){
            gl::color(1.0f,1.0f,1.0f,alpha);
            gl::pushMatrices();
            gl::translate(getWindowCenter());

        pageOne.draw();
        pageTwo.draw();
            gl::popMatrices();
    closeButton.draw();
    nextButton.draw();
    prevButton.draw();
   // }
    
}

bool InfoSection::isOpen(){
    return (pageOne.isVisible() || pageTwo.isVisible());
}
void InfoSection::open(){
    //timeline().apply(&alpha,1.0f,0.3f,EaseNone());
    pageOne.setPos(Vec3f(0,0,0));
    pageOne.show();
    closeButton.show();
    nextButton.show();
    prevButton.show();
    
}

void InfoSection::handleMouse(Vec2f cursorPos){
    
    closeButton.isOver(cursorPos);
    prevButton.isOver(cursorPos);
    nextButton.isOver(cursorPos);
    
}

void InfoSection::close(){
    // timeline().apply(&alpha,0.0f,0.2f,EaseNone());
    pageOne.hide();
    pageTwo.hide();
    closeButton.hide();
    prevButton.hide();
    nextButton.hide();
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
int InfoSection::mouseUp(MouseEvent evt){
    // returns:
    // -1   nothing
    // 0    shrink everything
    // 1    prev card
    // 2    next card
    
    if(closeButton.isOver(evt.getPos()) && closeButton.isDown()){
    //    hide();
        close();
        console() << "you released while over the close button" << endl;
        return 0;
    }else if(prevButton.isOver(evt.getPos()) && prevButton.isDown()){
        prevButton.up();
        prev();
        console() << "you released while over the prev button" << endl;
        return 1;
    } else if(nextButton.isOver(evt.getPos()) && nextButton.isDown()){
        nextButton.up();
        next();
        console() << "you released while over the next button" << endl;
        return 2;
    } else {
        closeButton.up();
        nextButton.up();
        prevButton.up();
        return -1;
    }

}
void InfoSection::mouseDown(MouseEvent evt){
    console () << getElapsedSeconds() << "\t";
    if(closeButton.isOver(evt.getPos())){
        closeButton.down();
        console() << "you pressed while over the close button" << endl;
        
    }else if(prevButton.isOver(evt.getPos())){
        prevButton.down();
        console() << "you pressed while over the prev button" << endl;
    } else if(nextButton.isOver(evt.getPos())){
        nextButton.down();
        console() << "you pressed while over the next button" << endl;
    } else {
        console () << endl;
    }
    
}