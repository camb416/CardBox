#include "cinder/app/AppBasic.h"
#include "cinder/Json.h"
#include "Card.h"
#include "cinder/gl/gl.h"

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
    
    private :
    vector<Card *> cards;
    gl::Texture bg_tex;
    gl::Texture shadow_tex;
    Vec2f cursorPos;
    void sortByOrder();
    void randomize();
    CardSettings cs;
    void selectACard(int _selectedID);
    int selectedCard;
    Vec2f myVec;
    void unselectAll();
    void shrinkAll(int _exception = -1);
    Anim<float>curtainsAlpha;
};

void CardBoxApp::setup()
{
    curtainsAlpha = 0.0f;
    selectedCard = -1;
   
   // setBorderless(true);
    
    Url apiUrl = Url( "http://localhost/json/json.js" );
    
    DataSourceRef ds = loadResource("data.js");
    console() << "data.js lives at: " << getAssetPath( "data.js" ) << std::endl;
   // JsonTree root = JsonTree( loadUrl( apiUrl ) );
    JsonTree root = JsonTree( loadResource("data.js"));
    JsonTree cardTree = root.getChild( "cards" );
    
    
    cs.basePath = root["basepath"].getValue();
    cs.background = root["background"].getValue();
    cs.shadow_path = root["shadow"].getValue();
    cs.shadow_tex = gl::Texture(loadImage(cs.basePath+"/"+cs.shadow_path));
    console() << "loading the gl texture" << endl;
    if(cs.shadow_tex){
        console() << "looks successful" << endl;
    } else {
        console() << "not looking good" << endl;
    }
    // &gl::Texture(loadImage(cs.basePath+"/"+cs.background));
    
    for( JsonTree::ConstIter cIt = cardTree.begin(); cIt != cardTree.end(); ++cIt )
    {
       // console () << "elemennt" << endl;
        JsonTree jt = (*cIt);
        // string firstname =
        
                // console() << firstname << ":" << path << std::endl;
        
        CardModel cm;
        cm.firstName = (*cIt)["firstname"].getValue();
        cm.lastInitial = (*cIt)["path"].getValue();
        cm.caption = (*cIt)["caption"].getValue();
        cm.location = (*cIt)["location"].getValue();
        cm.path = (*cIt)["path"].getValue();

        cards.push_back(new Card(&cs,cm));
    }

    // align everything
    float xCount  = 225;
    float yCount = 225;
    float largestY = 0;
    float margin = 25;
    for(int i=0;i<cards.size();i++){
        if(largestY<cards.at(i)->getSize().y) largestY = cards.at(i)->getSize().y;
        cards.at(i)->setOriginalPos(Vec3f(xCount, yCount,rand()%10));
        xCount += cards.at(i)->getSize().x+margin;
        // console() << xCount << ", " << yCount << endl;
        if(xCount>getWindowWidth()-225){
            xCount = 225;
            yCount += largestY+margin;
            largestY = 0;
        }
    }
    
    bg_tex = gl::Texture(loadImage(cs.basePath+"/"+cs.background));
    
    
    gl::Texture::Format fmt;
    fmt.enableMipmapping( true );
    fmt.setMinFilter( GL_LINEAR_MIPMAP_LINEAR );
    gl::enableAlphaBlending();
}
void CardBoxApp::resize(ResizeEvent evt){
    // setWindowSize(1920,1080);
}

void CardBoxApp::randomize(){
    // randomize everything
    for(int i=0;i<cards.size();i++){
        //if(largestY<cards.at(i)->getSize().y) largestY = cards.at(i)->getSize().y;
        cards.at(i)->setPos(Vec3f(rand() % getWindowWidth(), rand() % getWindowHeight(),rand()%10));
        cards.at(i)->setRot(rand() % 360);
    }
    shrinkAll();
}


void CardBoxApp::sortByOrder(){
    // align everything
    float xCount  = 225;
    float yCount = 225;
    float largestY = 0;
    float margin = 25;
    for(int i=0;i<cards.size();i++){
        if(largestY<cards.at(i)->getSize().y) largestY = cards.at(i)->getSize().y;
        cards.at(i)->setPos(Vec3f(xCount, yCount,rand()%10));
        cards.at(i)->setRot(0.0f);
        xCount += cards.at(i)->getSize().x+margin;
        // console() << xCount << ", " << yCount << endl;
        if(xCount>getWindowWidth()-225){
            xCount = 225;
            yCount += largestY+margin;
            largestY = 0;
        }
    }
    shrinkAll();
}

void CardBoxApp::shrinkAll(int _exception){
    for(int i=0;i<cards.size();i++){
        if(i!=_exception){
           cards.at(i)->shrink(); 
        }
    }
    if(_exception>-1){
        timeline().apply(&curtainsAlpha, 0.5f,1.0f,EaseInOutSine());
    } else {
        timeline().apply(&curtainsAlpha, 0.0f,1.0f,EaseInOutSine());
    }
}

void CardBoxApp::mouseDown( MouseEvent evt )
{
    shrinkAll(selectedCard);
    cards.at(selectedCard)->grow();
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
   sort (cards.begin(), cards.end(), sortBySize);
    for(int i=0;i<cards.size(); i++){
        cards.at(i)->update();
    }
    float nearest = 99999;
    float nearestID = -1;
    for(int i=0;i<cards.size(); i++){
        Vec2f mousePos = getMousePos();
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
    gl::color(1.0f,1.0f,1.0f,1.0f);
    gl::draw(bg_tex,getWindowBounds());
    gl::enableDepthWrite();
    gl::enableDepthRead();
    gl::enableAlphaBlending();
    for(int i=0;i<cards.size();i++){
        cards.at(i)->draw();
    }
    if(curtainsAlpha > 0.0f){
    gl::color(0.0f,0.0f,0.0f,curtainsAlpha);
        gl::pushMatrices();
        gl::translate(0,0,10.0f);
    gl::drawSolidRect(getWindowBounds());
        gl::popMatrices();
    }
    cursorPos = getMousePos();
    gl::color(0.0f,0.0f,1.0f,1.0f);
    gl::disableDepthRead();
    gl::disableDepthWrite();
    gl::drawLine(Vec3f(cursorPos.x, cursorPos.y,0.0f), Vec3f(myVec.x, myVec.y,0.0f));
   // console() << cursorPos.x << ", " << cursorPos.y << " : " << myVec.x << ", " << myVec.y << endl;
}
void CardBoxApp::prepareSettings(Settings * settings){

    
        settings->setWindowSize( 1050,1680 );
        settings->setFrameRate( 60 );
        settings->setResizable( false );
        settings->setTitle( "Card Box" );

}


CINDER_APP_BASIC( CardBoxApp, RendererGl )
