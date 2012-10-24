//
//  Card.h
//  CardBox
//
//  Created by Cameron Browning on 9/5/12.
//
//

#ifndef __CardBox__Card__
#define __CardBox__Card__


#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Timeline.h"
#include "cinder/Text.h"
#include <string>
#include <iostream>

using namespace std;
using namespace ci;
using namespace ci::app;

struct CardModel{
    string path;
    string firstName;
    string lastInitial;
    string caption;
    string location;
    int uid;
};

struct CardSettings {
    string basePath;
   // string background;
    gl::Texture shadow_tex;
};

class Card{
    
public:
    Card(CardSettings * _settings, CardModel _model);
    void draw();
    Vec2f getSize() const;
    Vec3f getPos();
    void setPos(Vec3f _pos, bool setOrigin = false);
    Vec3f getOriginalPos();
    void setOriginalPos(Vec3f _pos);
    Vec2f getCenter();
    void setScale(float _scale, bool _setOrigin = false);
    void setRot(float _rot, bool _setOrigin = false);
    void fitToRect(Rectf r, bool _setOrigin = false);
    bool operator < (const Card* crd) const
    {
        return (getSize().length() < crd->getSize().length());
    }
    
    int getUID();
    string getPath();
        void update();
    void select();
    void unselect();
    Vec2f getPos2f();
    Rectf grow(float lowerBound = 200.0f);
    void shrink(float _s = 0.2f);
    bool getIsBig();
    float getScale();
    CardModel getModel();
    bool isMoving();
    
private:
    bool isBig;
    Anim<float> rot;
    Anim<Vec3f> pos;
    Anim<float> alpha;
    Vec3f originalPos;
    float originalRot;
    float originalScale;
    Anim<Vec2f> scale;
    float scalef;
    
    // float alpha;
    CardSettings * settings;
    CardModel model;
    gl::Texture tex;
    void load();
    string path;
    bool isSelected;

};

bool sortBySize(Card *A, Card *B); //function to sort fruits by names
bool sortByPath(Card *A, Card *B); //function to sort fruits by names
bool sortByZ(Card *A, Card *B); //function to sort fruits by names
bool sortByUID(Card *A, Card *B); //function to sort fruits by names

#endif /* defined(__CardBox__Card__) */
