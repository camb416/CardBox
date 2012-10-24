//
//  CardUI.cpp
//  CardBox
//
//  Created by Cameron Browning on 9/19/12.
//
//

#include "CardUI.h"

CardUI::CardUI(){
    
    // initialize properties
    alpha = 0.0f;
    curtainsAlpha = 0.0f;
    
}

void CardUI::setup(){
    
    // might want to save the window width get stuff... save a few cycles
    closeButton = Button("closeButton.png",Vec2f(getWindowWidth()-75,75));
    prevButton = Button("leftArrow.png",Vec2f(75,getWindowHeight()/2));
    nextButton = Button("rightArrow.png",Vec2f(getWindowWidth()-75,getWindowHeight()/2));
    hide();
   
    closeButton.slideTo(Vec2f(getWindowWidth()+20,-20));
    
    Vec2f tempPos = prevButton.pos;
    prevButton.slideTo(Vec2f(-20,tempPos.y));
    
    tempPos = nextButton.pos;
    nextButton.slideTo(Vec2f(getWindowWidth()+20,tempPos.y));
    }

void CardUI::updateModel(CardModel cm){
    
    // receives the CardModel of the selected Card
    
    // update the text based on the CardModel received from the App
    cardModel = cm;
    if(cm.location.length()>0 && cm.lastInitial.length()>0){
        updateByline(cm.firstName+ " "+cm.lastInitial+", "+cm.location);
    } else if(cm.location.length()>0 && cm.lastInitial.length()<=0) {
        updateByline(cm.firstName+", "+cm.location);
    } else if(cm.location.length()<=0){
        updateByline(cm.firstName+ " "+cm.lastInitial);
    }
    updateCaption(cm.caption);
    
}

void CardUI::updatePositioning(Rectf _r){
    
    
    caption_middle = Vec2f(byline_leftmiddle.x-caption_tex.getWidth()/2+byline_tex.getWidth()/2,_r.y2+20);
    byline_leftmiddle = Vec2f(byline_leftmiddle.x,caption_middle.y+caption_tex.getHeight()+10);
    // move the close button to the upper right corner of the big card
    closeButton.slideTo(Vec2f(_r.x2,_r.y1));
    // move the prev/next buttons to the middle of the big card
    prevButton.slideTo(Vec2f(_r.x1,_r.getY2()-_r.getHeight()/2));
    nextButton.slideTo(Vec2f(_r.x2,_r.getY2()-_r.getHeight()/2));
    closeButton.alpha = nextButton.alpha = prevButton.alpha = 0;
    closeButton.show(1.0f);
    nextButton.show(1.0f);
    prevButton.show(1.0f);
    
}

void CardUI::drawBackground(){
    
    if(curtainsAlpha > 0.0f){
        gl::color(0.0f,0.0f,0.0f,curtainsAlpha);
        gl::pushMatrices();
        gl::translate(0,0,0);
        
        Rectf rect = getWindowBounds();
        gl::disableDepthRead();
        gl::disableDepthWrite();
        gl::drawSolidRect(rect);
        gl::popMatrices();
    }
    
    gl::pushMatrices();
    gl::color(1.0f,1.0f,1.0f,alpha);
    
    if(byline_tex) gl::draw(byline_tex,byline_leftmiddle);
    
    if(caption_tex)   gl::draw(caption_tex, caption_middle);
    gl::popMatrices();
    
    gl::color(1.0f,1.0f,1.0f,1.0f);
    
}

void CardUI::drawForeground(){
    closeButton.draw();
    nextButton.draw();
    prevButton.draw();
}

void CardUI::mouseDown(MouseEvent evt){
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
int CardUI::mouseUp(MouseEvent evt){
    
    // returns:
    // -1   nothing
    // 0    shrink everything
    // 1    prev card
    // 2    next card
    
    if(closeButton.isOver(evt.getPos()) && closeButton.isDown()){
        hide();
        closeButton.hide(0.5f);
        closeButton.slideTo(Vec2f(getWindowWidth()+20,-20));
        prevButton.hide(0.5f);
        Vec2f tempPos = prevButton.pos;
        prevButton.slideTo(Vec2f(-20,tempPos.y));
        nextButton.hide(0.5f);
        tempPos = nextButton.pos;
        nextButton.slideTo(Vec2f(getWindowWidth()+20,tempPos.y));
        closeButton.up();
        console() << "you released while over the close button" << endl;
        return 0;
    }else if(prevButton.isOver(evt.getPos()) && prevButton.isDown()){
        prevButton.up();
        console() << "you released while over the prev button" << endl;
        return 1;
    } else if(nextButton.isOver(evt.getPos()) && nextButton.isDown()){
        nextButton.up();
        console() << "you released while over the next button" << endl;
        return 2;
    } else {
        closeButton.up();
        nextButton.up();
        prevButton.up();
        return -1;
    }
    
}

void CardUI::updateCaption(string _text){
  
    // cgcontexterror if text is empty....
    if(_text.length()>0){
        Font mFont = Font( "Amasis MT Pro", 32 );
        Vec2f mSize = Vec2f(getWindowWidth() - 100,75);
        
        TextBox tbox = TextBox().alignment( TextBox::CENTER ).font( mFont ).size( Vec2i( mSize.x, TextBox::GROW ) ).text( _text );
        tbox.setColor( Color( 1.0f,1.0f,1.0f ) );
        caption_tex = gl::Texture( tbox.render() );
    } else {
        caption_tex = gl::Texture(0,0);
    }
    caption_middle = Vec2f(byline_leftmiddle.x-caption_tex.getWidth()/2+byline_tex.getWidth()/2,byline_leftmiddle.y-10-caption_tex.getHeight());
    
}

bool CardUI::isOpen(){
    
    if(alpha>0.9f){
        return true;
    } else{
        return false;
    }
    
}

void CardUI::updateByline(string _text){
    if(_text.length()>0){
    string normalFont( "HouseMovements-Sign" );
    TextLayout layout;
    layout.setFont( Font( normalFont, 36 ) );
    layout.setColor( Color( 1, 1, 1 ) );
    layout.addLine( _text);
    Surface8u rendered = layout.render( true, false );
    byline_tex = gl::Texture( rendered );
    } else {
        byline_tex = gl::Texture(0,0);
    }
    byline_leftmiddle = Vec2f(getWindowWidth()/2-byline_tex.getWidth()/2,getWindowHeight()-75-byline_tex.getHeight()/2.0f);
    
}

void CardUI::hide(){
    
    // hide the UI
    
    closeButton.hide();
    prevButton.hide();
    nextButton.hide();
    timeline().apply(&alpha,0.0f,0.8f,EaseInOutSine());
    timeline().apply(&curtainsAlpha, 0.0f,1.0f,EaseInOutSine());
    
}
void CardUI::show(){
    
    // show the UI
    
    timeline().apply(&alpha,1.0f,1.0f,EaseInOutSine());

    // could likely keep this at 1.0f and just use the alpha property of the UI
    timeline().apply(&curtainsAlpha, 0.8f,1.0f,EaseInOutSine());
    
    closeButton.show();
    nextButton.show();
    prevButton.show();
    
}

void CardUI::handleMouse(Vec2f cursorPos){
    
    closeButton.isOver(cursorPos);
    prevButton.isOver(cursorPos);
    nextButton.isOver(cursorPos);
    
}



float CardUI::getLowerBound(){
    
    // helper method to tell the card how big it can get
    
    return caption_middle.y;
    
}

void CardUI::resize(ResizeEvent evt){
    
    // this is all handled in the CardUI::update method.
    // Easier that way, saves some cycles
    
}

CardUI::~CardUI(){
    
    // empty destructor
    
}
