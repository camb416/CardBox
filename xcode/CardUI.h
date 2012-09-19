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
    void update(CardModel cm);
    void draw();
    void show();
    void hide();
private:
    void updateCaption(string _text = "");
    void updateByline(string _text = "");
    Anim<float> alpha;
    gl::Texture caption_tex;
    gl::Texture byline_tex;
    CardModel cardModel;
    
    
};

#endif /* defined(__CardBox__CardUI__) */
