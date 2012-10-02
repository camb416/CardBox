//
//  CardUI.h
//  CardBox
//
//  Created by Cameron Browning on 9/19/12.
//
//

#ifndef __CardBox__CardUI__
#define __CardBox__CardUI__

#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Timeline.h"
#include "cinder/Text.h"
#include "cinder/Font.h"
#include "Card.h"
#include "Button.h"
#include <string>
#include <iostream>

using namespace std;
using namespace ci;
using namespace ci::app;

class CardUI{

public:
    
    CardUI();
    ~CardUI();
    void setup();
    
    void updateModel(CardModel cm);
    void updatePositioning(Rectf _r);
    
    void drawBackground();
    void drawForeground();
    void resize(ResizeEvent evt);
    void show();
    void hide();
    float getLowerBound();
    bool isOpen();
    void handleMouse(Vec2f cursorPos);
    void mouseDown(MouseEvent evt);
    int mouseUp(MouseEvent evt);
        // mouse up returns:
        // -1   nothing
        // 0    shrink everything
        // 1    prev card
        // 2    next card
    
private:
    
    void updateCaption(string _text = "");
    void updateByline(string _text = "");
    Anim<float> alpha;
    gl::Texture caption_tex;
    gl::Texture byline_tex;
    CardModel cardModel;
    Vec2f byline_leftmiddle;
    Vec2f caption_middle;
    
    Anim<float>curtainsAlpha;
    Button closeButton;
    Button prevButton;
    Button nextButton;

};

#endif /* defined(__CardBox__CardUI__) */
