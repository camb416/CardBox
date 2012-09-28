//
//  CardUI.cpp
//  CardBox
//
//  Created by Cameron Browning on 9/19/12.
//
//

#include "CardUI.h"

CardUI::CardUI(){
    // empty constructor
    alpha = 0.0f;
    curtainsAlpha = 0.0f;
    
    
}

CardUI::~CardUI(){
    // empty destructor
}

float CardUI::getLowerBound(){
    return caption_middle.y;
}

void CardUI::update(CardModel cm, Rectf _r){
    // do something to update the text
    cardModel = cm;
    if(cm.location.length()>0 && cm.lastInitial.length()>0){
        updateByline(cm.firstName+ " "+cm.lastInitial+", "+cm.location);
    } else if(cm.location.length()>0 && cm.lastInitial.length()<=0) {
        updateByline(cm.firstName+", "+cm.location);
    } else if(cm.location.length()<=0){
        updateByline(cm.firstName+ " "+cm.lastInitial);
    }
    updateCaption(cm.caption);
    
    // move the close button to the upper right corner of the big card
    closeButton.moveTo(Vec2f(_r.x2,_r.y1));


    
}

void CardUI::handleMouse(Vec2f cursorPos){
    closeButton.isOver(cursorPos);
    prevButton.isOver(cursorPos);
    nextButton.isOver(cursorPos);
}

void CardUI::hide(){
    closeButton.hide();
    prevButton.hide();
    nextButton.hide();
    timeline().apply(&alpha,0.0f,0.8f,EaseInOutSine());
    timeline().apply(&curtainsAlpha, 0.0f,1.0f,EaseInOutSine());
}
void CardUI::show(){
    // show the UI
    timeline().apply(&alpha,1.0f,1.0f,EaseInOutSine());
    timeline().apply(&curtainsAlpha, 0.8f,1.0f,EaseInOutSine());
    closeButton.show();
    nextButton.show();
    prevButton.show();

}
void CardUI::draw(){
    
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
  //  gl::translate(getWindowCenter());
  

    if(byline_tex) gl::draw(byline_tex,byline_leftmiddle);
    

    
   if(caption_tex)   gl::draw(caption_tex, caption_middle);
  //  gl::drawStrokedCircle(caption_middle,10);
    gl::popMatrices();
    
    
    
}
void CardUI::drawOverlay(){
    closeButton.draw();
    nextButton.draw();
    prevButton.draw();
}
void CardUI::mouseDown(MouseEvent evt){
    if(closeButton.isOver(evt.getPos())){
        // shrinkAll();
        //cui.hide();
        closeButton.down();
        console() << "you pressed while over the close button" << endl;
        
    }else if(prevButton.isOver(evt.getPos())){
        prevButton.down();
        console() << "you pressed while over the prev button" << endl;
    } else if(nextButton.isOver(evt.getPos())){
        nextButton.down();
        console() << "you pressed while over the next button" << endl;
        
    }
}
int CardUI::mouseUp(MouseEvent evt){
    if(closeButton.isOver(evt.getPos()) && closeButton.isDown()){
       // shrinkAll();
        hide();
        closeButton.hide();
        prevButton.hide();
        nextButton.hide();
        closeButton.up();
        console() << "you released while over the close button" << endl;
        return 0;
    }else if(prevButton.isOver(evt.getPos()) && prevButton.isDown()){
        prevButton.up();
       // prevCard();
        console() << "you released while over the prev button" << endl;
        return 1;
    } else if(nextButton.isOver(evt.getPos()) && nextButton.isDown()){
        nextButton.up();
       // nextCard();
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
    Font mFont = Font( "Amasis MT Pro", 32 );
    Vec2f mSize = Vec2f(getWindowWidth() - 100,75);

	TextBox tbox = TextBox().alignment( TextBox::CENTER ).font( mFont ).size( Vec2i( mSize.x, TextBox::GROW ) ).text( _text );
	tbox.setColor( Color( 1.0f,1.0f,1.0f ) );
	caption_tex = gl::Texture( tbox.render() );
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
    
    string normalFont( "HouseMovements-Sign" );
    
    TextLayout layout;
   // layout.clear( ColorA( 0.0f, 0.0f, 0.0f, 0.8f ) );
    layout.setFont( Font( normalFont, 36 ) );
    layout.setColor( Color( 1, 1, 1 ) );
    layout.addLine( _text);
    Surface8u rendered = layout.render( true, false );
    byline_tex = gl::Texture( rendered );
    
        byline_leftmiddle = Vec2f(getWindowWidth()/2-byline_tex.getWidth()/2,getWindowHeight()-75-byline_tex.getHeight()/2.0f);
}


void CardUI::setup(){
    
    // necessary to generate the texture?
    // updateByline("");
   // updateCaption("");

        // need to drop this back until the window is ready
        // not in the JSON?
        closeButton = Button("closeButton.png",Vec2f(getWindowWidth()-75,75));
        prevButton = Button("leftArrow.png",Vec2f(75,getWindowHeight()/2));
        nextButton = Button("rightArrow.png",Vec2f(getWindowWidth()-75,getWindowHeight()/2));
    
   
}