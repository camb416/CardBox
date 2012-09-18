//
//  Button.cpp
//  CardBox
//
//  Created by Cameron Browning on 9/18/12.
//
//

#include "Button.h"


Button::Button(){
    pos = Vec2f(0,0);
    alpha = 1.0f;
}

Button::Button(string path_str, Vec2f _pos){
        // eventually this shouldnt be loading from resources...
    try{
        tex = loadImage(loadResource(path_str));
    } catch(Exception e){
        console() << "figure i got a problem loading a button..." << endl;
    }
    
}

bool Button::isOver(){
    
    // TO DO: implement me!
    
    return false;
}

void Button::draw(){
    gl::pushMatrices();
    gl::translate(pos-tex.getSize()/2);
    gl::draw(tex);
    
    gl::popMatrices();
}
void Button::update(){
    // do i need to update anything here?
}
