//
//  Card.cpp
//  CardBox
//
//  Created by Cameron Browning on 9/5/12.
//
//

#include "Card.h"

Card::Card(CardSettings * _settings, CardModel _model){
    model = _model;
    settings = _settings;
    load();
    pos = Vec3f(0.0f,0.0f,0.0f);
    alpha = 1.0f;
    scale = Vec2f(0.2f,0.2f);
    rot = rand() % 360;
    isSelected = false;
    isBig = false;
    scalef = 1.0f;
    
}
void Card::load(){
    string actualPath = (settings->basePath) + string("/") + (model.path);
    // console() << "attempting to load: " << actualPath << endl;
    tex = gl::Texture( loadImage( actualPath ) ) ;
    
}
void Card::select(){

    isSelected = true;
}
void Card::unselect(){
    isSelected = false;
}

bool Card::getIsBig(){
    return isBig;
}

void Card::grow(){
    if(!isBig){
        isBig = true;
        float desiredScale;
        float maxWidth = getWindowWidth() - 300.0f;
        float maxHeight = getWindowHeight()- 500.0f;
        desiredScale = maxWidth/tex.getWidth();
        if(tex.getHeight()*desiredScale>maxHeight){
            desiredScale = maxHeight/tex.getHeight();
        }
    setScale(desiredScale);
    setRot(0.0f);
    Vec3f newPos = Vec3f(getWindowCenter().x,getWindowCenter().y,50.0f);
    setPos(newPos);
    }
}
void Card::shrink(){
   
    if(isBig){
    Vec2f scale2f = scale;
    if(scale2f.x>0.2){
         Vec3f pos3f = pos;
        setPos(originalPos);
        setRot(originalRot);
        setScale(0.2f);
    }
        isBig = false;
    }
}


void Card::update(){
    isSelected = false;
}
void Card::draw(){
    gl::pushMatrices();
    if (tex) {
        
        Vec3f vec = pos;
        gl::translate(vec.x,vec.y, vec.z);
        gl::rotate(rot);

        Vec2f scale2f = scale;
        
        Rectf texRect = Rectf(-0.5f*scale2f.x*tex.getWidth(),-0.5f*scale2f.y*tex.getHeight(),0.5f*scale2f.x*tex.getWidth(),0.5f*scale2f.y*tex.getHeight());
      
        Rectf bgRect = Rectf(texRect.x1-5,texRect.y1-5,texRect.x2+5,texRect.y2+5);
        
        Rectf shadowRect = bgRect;
        shadowRect.inflate(Vec2f(scale2f.x*10.0f,scale2f.y*10.0f));
       // gl::color(1.0f,1.0f,1.0f,1.0f);
        gl::Texture shadow = settings->shadow_tex;
        if(shadow){
            gl::draw(shadow, shadowRect);
        }
        gl::translate(0,0,0.01f);
      //  gl::drawSolidRect(bgRect);
        gl::translate(0,0,0.01f);
        if(isSelected){
         //   gl::color(1.0f,0.0f,0.0f,alpha);
            gl::drawStrokedRect(bgRect);
       //     gl::color(1.0f,1.0f,1.0f,alpha);
        }
         gl::draw(tex, texRect );
}
    gl::popMatrices();
}
Vec2f Card::getSize() const{
    Vec2f scale2f = scale;
    Vec2f effectiveSize = Vec2f(0,0);
    if(tex){
    effectiveSize = Vec2f(scale2f.x*tex.getWidth(), scale2f.y*tex.getHeight());
    }
    return effectiveSize;
}
Vec2f Card::getCenter(){
    Vec3f vec = pos;
    return Vec2f(vec.x,vec.y);// + (tex.getSize()/2.0f);
}
Vec3f Card::getPos(){
    return pos;
}
int Card::getUID(){
    return model.uid;
}
Vec3f Card::getOriginalPos(){
    return originalPos;
}
Vec2f Card::getPos2f(){
    Vec3f pos3f = pos;
    return Vec2f(pos3f.x,pos3f.y);
}
void Card::setOriginalPos(Vec3f _pos){
    pos = originalPos = _pos;
}
void Card::setPos(Vec3f _pos, bool setOrigin){
   // pos = _pos;
    timeline().apply(&alpha,0.5f, 0.5f,EaseNone());
    // CueRef cue = timeline().add(bind(&doSlide,this),getElapsedSeconds()+0.3f);
    timeline().apply(&pos,_pos,1.0,EaseInOutSine());
    timeline().apply(&alpha,1.0f, 0.5f,EaseNone());
    if(setOrigin) originalPos = _pos;
}


void Card::setScale(float _scale){
   // scale = Vec2f(_scale, _scale);
    timeline().apply(&scale,Vec2f(_scale,_scale),1.0f,EaseInOutSine());
}
void Card::setRot(float _rot, bool _setOrigin){
   // rot = _rot;
    timeline().apply(&rot, _rot, 1.5f,EaseOutElastic(0.2f,0.8f));
    if(_setOrigin) originalRot = _rot;
    
}
string Card::getPath(){
    return path;
}

bool Card::isMoving(){
    Vec3f pos3f = pos;
    if(pos3f!=originalPos){
        // console() << "moving..." << endl;
        return true;
    } else {
        return false;
    }
}

float Card::getScale(){
    Vec2f scale2f = scale;
    return scale2f.x;
}

bool sortByUID(Card *A, Card *B){
    if(A->getUID()<B->getUID()){
        return true;
    } else {
        return false;
    }
}

bool sortByZ(Card *A, Card *B){
    float za,zb;
    za = A->getPos().z;
    zb = B->getPos().z;
    if(za<zb){
        return true;
    } else {
        return false;
    }
}

bool sortBySize(Card *A, Card *B)
{
    float a,b;
    a = A->getScale();
    b = B->getScale();
   // b+=0.1f;
    if(a<b){
        return true;
    } else {
        return false;
    }
 // return (A->getSize().length() < B->getSize().length());
}
bool sortByPath(Card*A, Card *B){
    for( string::const_iterator lit = A->getPath().begin(), rit = B->getPath().begin(); lit != A->getPath().end() && rit != B->getPath().end(); ++lit, ++rit )
        if( tolower( *lit ) < tolower( *rit ) )
            return true;
        else if( tolower( *lit ) > tolower( *rit ) )
            return false;
    if( A->getPath().size() < B->getPath().size() )
        return true;
    return false;
}
