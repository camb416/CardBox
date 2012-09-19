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
void CardUI::update(CardModel cm){
    // do something to update the text
    cardModel = cm;
    updateCaption(cm.caption);
    
    updateByline(cm.firstName+ " "+cm.lastInitial+", "+cm.location);
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
    gl::translate(getWindowCenter());
    gl::draw(caption_tex, Vec2f(caption_tex.getWidth()/-2.0f,getWindowHeight()/2-150-caption_tex.getHeight()/2.0f));
    gl::draw(byline_tex, Vec2f(byline_tex.getWidth()/-2.0f,getWindowHeight()/2-75-byline_tex.getHeight()/2.0f));
    gl::popMatrices();
}

void CardUI::updateCaption(string _text){
/*
    string normalFont( "Amasis MT Pro" );
    TextLayout layout;
   // layout.clear( ColorA( 0.0f, 0.0f, 0.0f, 0.8f ) );
    layout.setFont( Font( normalFont, 48 ) );
    layout.setColor( Color( 1, 1, 1 ) );
    layout.addLine( _text);
    Surface8u rendered = layout.render( true, false );
    caption_tex = gl::Texture( rendered );
  */  
    
   // string txt = "Here is some text that is larger than can fit naturally inside of 100 pixels.\nAnd here is another line after a hard break.";
    Font mFont = Font( "Amasis MT Pro", 32 );
    Vec2f mSize = Vec2f(getWindowWidth() - 100,75);

	TextBox tbox = TextBox().alignment( TextBox::CENTER ).font( mFont ).size( Vec2i( mSize.x, TextBox::GROW ) ).text( _text );
	tbox.setColor( Color( 1.0f,1.0f,1.0f ) );
	// tbox.setBackgroundColor( ColorA( 0.5, 0, 0, 1 ) );
	Vec2i sz = tbox.measure();
	//console() << "Height: " << sz.y << endl;
	caption_tex = gl::Texture( tbox.render() );
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
}


void CardUI::setup(){
    
    updateCaption("Hello,");
    updateByline("World!");
}