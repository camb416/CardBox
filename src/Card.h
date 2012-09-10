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
};

struct CardSettings {
    string basePath;
    string background;
    string shadow_path;
    gl::Texture shadow_tex;
};

class Card{
    
public:
    Card(CardSettings * _settings, CardModel _model);
    void draw();
    Vec2f getSize() const;
    Vec3f getPos();
    void setPos(Vec3f _pos);
    void setOriginalPos(Vec3f _pos);
    Vec2f getCenter();
    void setScale(float _scale);
    
    bool operator < (const Card* crd) const
    {
        return (getSize().length() < crd->getSize().length());
    }
    float alpha;
    string getPath();
private:
    float rot;
    Vec3f pos;
    Vec3f originalPos;
    Vec2f scale;
    // float alpha;
    CardSettings * settings;
    CardModel model;
    gl::Texture tex;
    void load();
    string path;

};

bool sortBySize(Card *A, Card *B); //function to sort fruits by names
bool sortByPath(Card *A, Card *B); //function to sort fruits by names


#endif /* defined(__CardBox__Card__) */
