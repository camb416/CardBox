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
}
CardUI::~CardUI(){
    // empty destructor
}

float CardUI::getLowerBound(){
    return caption_middle.y;
}

void CardUI::update(CardModel cm){
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
}
void CardUI::hide(){
    // hide the UI
    timeline().apply(&alpha,0.0f,0.8f,EaseInOutSine());
}
void CardUI::show(){
    // show the UI
    timeline().apply(&alpha,1.0f,1.0f,EaseInOutSine());
}
void CardUI::draw(){
    gl::pushMatrices();
    gl::color(1.0f,1.0f,1.0f,alpha);
  //  gl::translate(getWindowCenter());
  

    
    gl::draw(byline_tex,byline_leftmiddle);
    

    
      gl::draw(caption_tex, caption_middle);
  //  gl::drawStrokedCircle(caption_middle,10);
    gl::popMatrices();
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
    
     updateByline("World!");
    updateCaption("Hello,");
   
}