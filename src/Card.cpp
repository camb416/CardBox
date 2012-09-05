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
    pos.set(0.0f,0.0f);
    alpha = 1.0f;
    scale.set(0.2f,0.2f);
    
}
void Card::load(){
    string actualPath = (settings->basePath) + string("/") + (model.path);
    console() << "attempting to load: " << actualPath << endl;
    tex = gl::Texture( loadImage( actualPath ) ) ;
    
}
void Card::draw(){
    gl::pushMatrices();
    if (tex) {
        
        
        gl::translate(pos.x-scale.x*tex.getWidth()/2,pos.y-scale.y*tex.getHeight()/2);
       /*
        gl::rotate(deg);
        */
         
        gl::color(ColorA(255, 255, 255, alpha));

        
        Rectf texRect = Rectf(0.0f,0.0f,scale.x*tex.getWidth(),scale.y*tex.getHeight());
        
      //  GalleryHelper::alignElement(_align,Area(texRect));
        //  gl::draw( texture, Vec2f( 0, 0 ) );
        gl::color(1.0f,1.0f,1.0f,alpha);
        
        gl::draw(tex, texRect );
}
    gl::popMatrices();
}
Vec2f Card::getSize() const{
    Vec2f effectiveSize = Vec2f(scale.x*tex.getWidth(), scale.y*tex.getHeight());
    return effectiveSize;
}
Vec2f Card::getCenter(){
    return pos;// + (tex.getSize()/2.0f);
}
void Card::setPos(Vec2f _pos){
    pos = _pos;
}
void Card::setScale(float _scale){
    scale = Vec2f(_scale, _scale);
}
string Card::getPath(){
    return path;
}
bool sortBySize(Card *A, Card *B)
{
    return (A->alpha < B->alpha);
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
