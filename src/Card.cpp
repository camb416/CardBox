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
    rot = rot - 180;
    isSelected = false;
    isBig = false;
    scalef = 1.0f;
    
}
void Card::load(){
    
    string actualPath = (settings->basePath) + string("/") + (model.path);
    try{ tex = gl::Texture( loadImage( actualPath ) ) ;
    } catch(Exception ex){
        console() << "busted" << endl;
        
        string normalFont( "Arial" );
        TextLayout layout;
        layout.setFont( Font( normalFont, 36 ) );
        layout.setColor( Color( 1, 1, 1 ) );
        layout.addLine(actualPath);
        layout.addLine("not loaded.");
        
        Surface8u rendered = layout.render( true, false );
        Surface8u newSurface(rendered.getWidth(),rendered.getWidth(),false);
        newSurface.copyFrom(rendered, newSurface.getBounds());
       tex = gl::Texture(newSurface );
    
    }
    /*if(tex){
        console() << actualPath << " loaded successfully." << endl;
    } else {
        console() << actualPath << " not loaded." << endl;
    }*/
    
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

void Card::fitToRect(Rectf r, bool _setOrigin){

        float desiredScale;
        float maxWidth = r.x2-r.x1;
        float maxHeight = r.y2-r.y1;
        desiredScale = maxWidth/tex.getWidth();
        if(tex.getHeight()*desiredScale>maxHeight){
            desiredScale = maxHeight/tex.getHeight();
        }
        setScale(desiredScale, _setOrigin);
    
}

Rectf Card::grow(float lowerBound){
    
    float upperBound = 75;
    float sideBounds = 75;
    
    Rectf returnRect = Rectf(sideBounds,upperBound,getWindowWidth()-sideBounds,lowerBound);
    if(!isBig){
        console() << "growing: " << model.uid << endl;
        isBig = true;
        
        float desiredScale;
        float maxWidth = getWindowWidth() - (sideBounds*2.0f);
        float maxHeight = (lowerBound-upperBound*2);
        desiredScale = maxWidth/tex.getWidth();
        if(tex.getHeight()*desiredScale>maxHeight){
            desiredScale = maxHeight/tex.getHeight();
        }
        setScale(desiredScale);
        setRot(0.0f);
        Vec3f newPos = Vec3f(getWindowCenter().x,(lowerBound-upperBound*2)/2+upperBound,2.0f);
        setPos(newPos);
        returnRect.x1 = newPos.x-tex.getWidth()/2*desiredScale;
        returnRect.y1 = newPos.y-tex.getHeight()/2*desiredScale;
        returnRect.x2 = newPos.x+tex.getWidth()/2*desiredScale;
        returnRect.y2 = newPos.y+tex.getHeight()/2*desiredScale;
    }
    
    return returnRect;
    
}
void Card::shrink(float _s){
    
    if(isBig){
        console() << "shrinking: " << model.uid << endl;
        Vec2f scale2f = scale;
        
        // removed this conditional to fix issue #16 where furious tapping
        // leads to some cards stuck big.
      //  if(scale2f.x>0.2){
            Vec3f pos3f = pos;
            setPos(originalPos);
            setRot(originalRot);
            setScale(originalScale);
            //setScale(0.2f);
       // } else {
       //     console() << "questionable: " << model.uid << endl;
      //  }
        
        isBig = false;
    } else {
        // no point in shrinking if it's not big
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
        gl::Texture shadow = settings->shadow_tex;
        if(shadow){
            gl::draw(shadow, shadowRect);
        }
        gl::translate(0,0,0.01f);
        gl::translate(0,0,0.01f);
        if(isSelected){
            gl::drawStrokedRect(bgRect);
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
    return Vec2f(vec.x,vec.y);
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
    timeline().apply(&alpha,0.5f, 0.5f,EaseNone());
    timeline().apply(&pos,_pos,1.0,EaseInOutSine());
    timeline().apply(&alpha,1.0f, 0.5f,EaseNone());
    if(setOrigin) originalPos = _pos;
}

CardModel Card::getModel(){
    return model;
}

void Card::setScale(float _scale, bool _setOrigin){
    if(_setOrigin) originalScale = _scale;
    timeline().apply(&scale,Vec2f(_scale,_scale),1.0f,EaseInOutSine());
}
void Card::setRot(float _rot, bool _setOrigin){
    timeline().apply(&rot, _rot, 1.5f,EaseOutElastic(0.2f,0.8f));
    if(_setOrigin) originalRot = _rot;
    
}
string Card::getPath(){
    return path;
}

bool Card::isMoving(){
    Vec3f pos3f = pos;
    if(pos3f!=originalPos){
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
    if(a<b){
        return true;
    } else {
        return false;
    }
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
