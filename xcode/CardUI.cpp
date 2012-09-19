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
}
CardUI::~CardUI(){
    // empty destructor
}
void CardUI::update(CardModel cm){
    // do something to update the text
    cardModel = cm;
    updateText(cm.caption);
}
void CardUI::draw(){
    gl::pushMatrices();
    gl::color(1.0f,1.0f,1.0f,1.0f);
    gl::translate(getWindowCenter());
    gl::draw(text_texture);
    gl::popMatrices();
}

void CardUI::updateText(string _text){

    string normalFont( "Arial" );
    TextLayout layout;
    layout.clear( ColorA( 0.0f, 0.0f, 0.0f, 0.8f ) );
    layout.setFont( Font( normalFont, 48 ) );
    layout.setColor( Color( 1, 1, 1 ) );
    layout.addLine( _text);
    Surface8u rendered = layout.render( true, false );
    text_texture = gl::Texture( rendered );
}

void CardUI::setup(){
    
    updateText("Hello, World!");
}