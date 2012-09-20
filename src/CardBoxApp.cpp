#include "cinder/app/AppBasic.h"
#include "cinder/Json.h"
#include "Card.h"
#include "cinder/gl/gl.h"
#include "Button.h"
#include "cinder/params/Params.h"
#include "InfoSection.h"
#include "CardUI.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CardBoxApp : public AppBasic {
public:
	void setup();
	void mouseDown( MouseEvent evt );
    void keyDown(KeyEvent evt);
	void update();
	void draw();
    void resize(ResizeEvent evt);
    void prepareSettings(Settings * settings);
    
    // PARAMS
	params::InterfaceGl	mParams;
    Quatf mSceneRotation;
    
   // gl::Texture infoBG;
    InfoSection infoSection;
    
    private :
    vector<Card *> cards;
    vector<Card *>animatingCards;
    Card * bigCard;
    gl::Texture bg_tex;
    gl::Texture shadow_tex;
    Vec2f cursorPos;
    void sortByOrder();
    void randomize();
    CardSettings cs;
    
    CardUI cui;
    
    int getIDfromUID(int card_uid);
    void selectACard(int _selectedID);
    int selectedCard;
    Vec2f myVec;
    void unselectAll();
    void shrinkAll(int _exception = -1);
    Anim<float>curtainsAlpha;
    
    Button closeButton;
    Button prevButton;
    Button nextButton;
    
    // button handlers
    void closeCard();
    void nextCard();
    void prevCard();
    void info();
    void closeInfo();
    void nextInfo();
    void prevInfo();
    
};
void CardBoxApp::setup()
{
    cui.setup();
    cui.hide();
    
    curtainsAlpha = 0.0f;
    selectedCard = -1;

    mParams = params::InterfaceGl( "CardBox Functions", Vec2i( 225, 200 ) );
    mParams.addButton("close card", std::bind( &CardBoxApp::closeCard, this ));
    mParams.addButton("next card", std::bind( &CardBoxApp::nextCard, this ));
    mParams.addButton("previous card", std::bind( &CardBoxApp::prevCard, this ));
    mParams.addButton("info", std::bind( &CardBoxApp::info, this ));
    mParams.addButton("close info", std::bind( &CardBoxApp::closeInfo, this ));
    mParams.addButton("next info", std::bind( &CardBoxApp::nextInfo, this ));
    mParams.addButton("prev info", std::bind( &CardBoxApp::prevInfo, this ));
    mParams.show();
    
    Url apiUrl = Url( "http://localhost/json/json.js" );
    
    DataSourceRef ds = loadResource("data.js");
    console() << "data.js lives at: " << getAssetPath( "data.js" ) << std::endl;
    JsonTree root = JsonTree( loadResource("data.js"));
    JsonTree cardTree = root.getChild( "cards" );
    
    cs.basePath = root["basepath"].getValue();
    cs.background = root["background"].getValue();
    cs.shadow_path = root["shadow"].getValue();
    cs.shadow_tex = gl::Texture(loadImage(cs.basePath+"/"+cs.shadow_path));
    
    infoSection.setup("instructions1.png","instructions2.png");
    
    console() << "loading the gl texture" << endl;
    if(cs.shadow_tex){
        console() << "looks successful" << endl;
    } else {
        console() << "not looking good" << endl;
    }
    // &gl::Texture(loadImage(cs.basePath+"/"+cs.background));
    int i = 0;
    for( JsonTree::ConstIter cIt = cardTree.begin(); cIt != cardTree.end(); ++cIt )
    {
        // console () << "elemennt" << endl;
        JsonTree jt = (*cIt);
        // string firstname =
        
        // console() << firstname << ":" << path << std::endl;
        
        CardModel cm;
        cm.firstName = (*cIt)["firstname"].getValue();
        cm.lastInitial = (*cIt)["lastinitial"].getValue();
        cm.caption = (*cIt)["caption"].getValue();
        cm.location = (*cIt)["location"].getValue();
        cm.path = (*cIt)["path"].getValue();
        cm.uid = i++;
        cards.push_back(new Card(&cs,cm));
    }
    /*
    // align everything
    float xCount  = 225;
    float yCount = 225;
    float largestY = 0;
    float margin = 25;
    for(int i=0;i<cards.size();i++){
        if(largestY<cards.at(i)->getSize().y) largestY = cards.at(i)->getSize().y;
        cards.at(i)->setOriginalPos(Vec3f(xCount, yCount,0));
        xCount += cards.at(i)->getSize().x+margin;
        // console() << xCount << ", " << yCount << endl;
        if(xCount>getWindowWidth()-225){
            xCount = 225;
            yCount += largestY+margin;
            largestY = 0;
        }
    }
    */
    randomize();
    
    bg_tex = gl::Texture(loadImage(cs.basePath+"/"+cs.background));
    
    closeButton = Button("closeButton.png",Vec2f(128,128));
    prevButton = Button("leftArrow.png",Vec2f(128,512));
    nextButton = Button("rightArrow.png",Vec2f(512,512));
    
    //closeButton_tex = gl::Texture(loadImage(loadResource("closeButton.png")));
    
    gl::Texture::Format fmt;
    fmt.enableMipmapping( true );
    fmt.setMinFilter( GL_LINEAR_MIPMAP_LINEAR );
    //  gl::enableAlphaBlending();
}
void CardBoxApp::resize(ResizeEvent evt){
    // setWindowSize(1920,1080);
}

void CardBoxApp::randomize(){
    // randomize everything
    for(int i=0;i<cards.size();i++){
        //if(largestY<cards.at(i)->getSize().y) largestY = cards.at(i)->getSize().y;
        cards.at(i)->setPos(Vec3f(rand() % (getWindowWidth()-200)+100, rand() % (getWindowHeight()-200)+100,(rand() % 32000)/320000.0f), true);
        cards.at(i)->setRot(rand() % 360, true);
    }
    shrinkAll();
    cui.hide();
}


void CardBoxApp::sortByOrder(){
    // align everything
    float xCount  = 225;
    float yCount = 225;
    float largestY = 110;
    float margin = 25;
    
    vector<Card *> orderedCards = cards;
    
    sort(orderedCards.begin(),orderedCards.end(),sortByUID);
    
    
    for(int i=0;i<orderedCards.size();i++){
      // if(largestY<orderedCards.at(i)->getSize().y && !orderedCards.at(i)->getIsBig()) largestY = orderedCards.at(i)->getSize().y;
        orderedCards.at(i)->setPos(Vec3f(xCount, yCount,0),true);
        orderedCards.at(i)->setRot(0.0f, true);
        
        xCount += 85+margin;
        //xCount += orderedCards.at(i)->getSize().x+margin;
        // console() << xCount << ", " << yCount << endl;
        if(xCount>getWindowWidth()-225){
            xCount = 225;
            yCount += largestY+margin;
            //largestY = 0;
        }
    }
    shrinkAll();
    cui.hide();

}

void CardBoxApp::shrinkAll(int _exception){
    for(int i=0;i<cards.size();i++){
        if(i!=_exception){
            cards.at(i)->shrink(); 
        }
    }
    if(_exception>-1){
        timeline().apply(&curtainsAlpha, 0.8f,1.0f,EaseInOutSine());
    } else {
        timeline().apply(&curtainsAlpha, 0.0f,1.0f,EaseInOutSine());
    }
}

void CardBoxApp::mouseDown( MouseEvent evt )
{
    
    
    if(closeButton.isOver(evt.getPos())){
        shrinkAll();
        cui.hide();
    }else if(selectedCard>-1){
        shrinkAll(selectedCard);
        cui.show();
        cui.update(cards.at(selectedCard)->getModel());
        cards.at(selectedCard)->grow(cui.getLowerBound());
    }
}
void CardBoxApp::keyDown(KeyEvent evt){
    switch(evt.getChar()){
        case ' ':
            sortByOrder();
            break;
        case 'r':
        case 'R':
            randomize();
            break;
        default:
            console() << "KEY PRESSED: " << evt.getCode() << "( " << evt.getChar() << " )" << endl;
            break;
    }
}

void CardBoxApp::update()
{
    sort(cards.begin(), cards.end(), sortByZ);
    for(int i=0;i<cards.size(); i++){
        cards.at(i)->update();
    }
    
    
    float nearest = 99999;
    float nearestID = -1;
    for(int i=0;i<cards.size(); i++){
        Vec2f mousePos = cursorPos;
        Vec2f cardPos = Vec2f(cards.at(i)->getPos().x,cards.at(i)->getPos().y);
        float mouseDist = mousePos.distance(cardPos);
        if(mouseDist<nearest){
            nearestID = i;
            nearest = mouseDist;
            
        }
    }
    if(nearestID>-1){
        selectedCard = nearestID;
        selectACard(nearestID);
        
    }
}
int CardBoxApp::getIDfromUID(int card_uid){
    for(int i=0;i<cards.size();i++){
        if(cards.at(i)->getUID() == card_uid){
            return i;
        }
    }
    return -1;
}
void CardBoxApp::selectACard(int _selectedID){
    cards.at(_selectedID)->select();
    myVec = cards.at(_selectedID)->getPos2f();
    unselectAll();
    
}
void CardBoxApp::unselectAll(){
    for(int i=0;i<cards.size();i++){
        if(i!=selectedCard){
            cards.at(i)->unselect();
        }
    }
    
}

void CardBoxApp::draw()
{
    //   gl::enableDepthRead();
    
    // clear out the window with black
	gl::clear( Color( 1,1,1 ) );
    
    gl::enableAlphaBlending();
    // gl::disableDepthRead();
    // gl::disableDepthWrite();
    
    gl::color(1.0f,1.0f,1.0f,1.0f);
    

    gl::draw(bg_tex,getWindowBounds());
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    bigCard = NULL;
  //  int drawCount = 0;
  //  int reason1 = 0;
  //  int reason2 = 0;
    for(int i=0;i<cards.size();i++){
        // if(!cards.at(i)->getIsBig() && !cards.at(i)->isMoving()){
        if(!cards.at(i)->getIsBig()){
            gl::color(1.0f,1.0f,1.0f,1.0f);
            cards.at(i)->draw();
          //  drawCount++;
            
        //} //else if(!cards.at(i)->getIsBig()){
            // gl::color(1.0f,1.0f,1.0f,1.0f);
            // cards.at(i)->draw();
            //reason2++;
        } else {
          //  reason1++;
            bigCard = cards.at(i);
        }
    }
    
    if(curtainsAlpha > 0.0f){
        gl::color(0.0f,0.0f,0.0f,curtainsAlpha);
        gl::pushMatrices();
        gl::translate(0,0,0);
        
        // this here is problematic...
        Rectf rect = getWindowBounds();
       // rect.x2/=2;
        gl::disableDepthRead();
        gl::disableDepthWrite();
        gl::drawSolidRect(rect);
        gl::popMatrices();
    }
   // if(drawCount<200)
  //  console() << drawCount << " cards being drawn up front. CUZ: " << reason1 << " cards are big, and " << reason2 << " cards are moving."<< endl;
   // int drawCount2 = 0;
  //  for(int i=0;i<cards.size();i++){
      //  if(cards.at(i)->isMoving() && !cards.at(i)->getIsBig()){
      //      drawCount2++;
      //      gl::color(1.0f,1.0f,1.0f,1.0f);
      //      cards.at(i)->draw();
     //   }
  //  }
   //if(drawCount<200)
  //  console() << drawCount2 << " cards being drawn in back." << endl;

    gl::color(1.0f,1.0f,1.0f,1.0f);
    if(bigCard!=NULL){
        bigCard->draw();
       // console() << "big card being drawn" << endl;
      //  drawCount++;
    }
    cursorPos = getMousePos();
    gl::color(0.0f,0.0f,1.0f,1.0f);
    gl::disableDepthRead();
    gl::disableDepthWrite();
    gl::drawLine(Vec3f(cursorPos.x, cursorPos.y,0.0f), Vec3f(myVec.x, myVec.y,0.0f));
    gl::color(1.0f,1.0f,1.0f,1.0f);
    closeButton.draw();
    nextButton.draw();
    prevButton.draw();
    //    gl::draw(closeButton_tex,Rectf(0,0,64,64));
    // console() << cursorPos.x << ", " << cursorPos.y << " : " << myVec.x << ", " << myVec.y << endl;
     
    infoSection.draw();
   // gl::draw(infoBG);
    params::InterfaceGl::draw();
    cui.draw();

}
void CardBoxApp::prepareSettings(Settings * settings){
    
    
    settings->setWindowSize( 1050,1050 );
    settings->setFrameRate( 60 );
  //  settings->setResizable( false );
    settings->setTitle( "Card Box" );
    
}

// button handlers
void CardBoxApp::closeCard(){
    console() << "closeCard()" << endl;
    shrinkAll();
    cui.hide();
}
void CardBoxApp::nextCard(){
    int curCard_uid = -1;
    int nextCard_id = -1;
    if(bigCard!=NULL){
        curCard_uid = bigCard->getUID();
        console() << "the current card has a uid of: " << curCard_uid << endl;
        if(curCard_uid>-1){
            curCard_uid++;
            if(curCard_uid>=cards.size()) curCard_uid = 0; // loop
            nextCard_id = getIDfromUID(curCard_uid);
            console() << "the next card id is: " << nextCard_id << endl;
            if(nextCard_id>-1){
                shrinkAll(nextCard_id);
                cui.show();
                cui.update(cards.at(nextCard_id)->getModel());
                cards.at(nextCard_id)->grow(cui.getLowerBound());
            }
        }
    }
    
    console() << "nextCard()" << endl;
}
void CardBoxApp::prevCard(){
    int curCard_uid = -1;
    int nextCard_id = -1;
    if(bigCard!=NULL){
        curCard_uid = bigCard->getUID();
        console() << "the current card has a uid of: " << curCard_uid << endl;
        if(curCard_uid>-1){
            curCard_uid--;
            if(curCard_uid<0) curCard_uid = (cards.size()-1); // loop
            nextCard_id = getIDfromUID(curCard_uid);
            console() << "the next card id is: " << nextCard_id << endl;
            if(nextCard_id>-1){
                shrinkAll(nextCard_id);
                cui.show();
                cui.update(cards.at(nextCard_id)->getModel());
                cards.at(nextCard_id)->grow(cui.getLowerBound());
            }
        }
    }
    

    console() << "prevCard()" << endl;
}
void CardBoxApp::info(){
    infoSection.open();
    shrinkAll();
    cui.hide();
    console() << "info()" << endl;
}
void CardBoxApp::closeInfo(){
    infoSection.close();
    console() << "closeInfo()" << endl;
}
void CardBoxApp::nextInfo(){
    infoSection.next();
    console() << "nextInfo()" << endl;
}
void CardBoxApp::prevInfo(){
    infoSection.prev();
    console() << "prevInfo()" << endl;
}



CINDER_APP_BASIC( CardBoxApp, RendererGl )
