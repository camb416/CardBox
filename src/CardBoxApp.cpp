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
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void resize(ResizeEvent evt);
    void prepareSettings(Settings * settings);
    
    private :
    vector<Card *> cards;
    gl::Texture bg_tex;
    gl::Texture shadow_tex;
    Vec2f cursorPos;
};

void CardBoxApp::setup()
{
    setWindowSize(1920,1080);
   // setBorderless(true);
    
    string mJsonStr = "    { \
    \"menu\": { \
        \"id\": \"file\", \
        \"value\": \"File\", \
        \"popup\": { \
            \"menuitem\": [ \
                         {\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"}, \
                         {\"value\": \"Open\", \"onclick\": \"OpenDoc()\"}, \
                         {\"value\": \"Close\", \"onclick\": \"CloseDoc()\"} \
                         ] \
        } \
    } \
}";
    
    Url apiUrl = Url( "http://localhost/json/json.js" );
    JsonTree root = JsonTree( loadUrl( apiUrl ) );
    JsonTree cardTree = root.getChild( "cards" );
    
    CardSettings cs;
    cs.basePath = root["basepath"].getValue();
    cs.background = root["background"].getValue();
    cs.shadow_path = root["shadow"].getValue();
    cs.shadow_tex = gl::Texture(loadImage(cs.basePath+"/"+cs.shadow_path));
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
void CardBoxApp::mouseDown( MouseEvent event )
{
    

}

void CardBoxApp::update()
{
   
        
    // set the sizes
    for(int i=0;i<cards.size();i++){
        Vec2f mouseRelative = cursorPos - cards.at(i)->getCenter();
        if(mouseRelative.length()<500.0f){
            cards.at(i)->setPos(Vec3f(cards.at(i)->getPos().x,cards.at(i)->getPos().y,(500.0f-mouseRelative.length())/1.0f));
            cards.at(i)->alpha = ((1000.0f-mouseRelative.length())/1000.0f+0.2f);
        } else {
            cards.at(i)->alpha = 0.2f;
        }
    }
    
    // sort them
   // sort(cards.begin(), cards.end(), sortBySize);
    

}

void CardBoxApp::draw()
{
   // gl::enableDepthRead();
	// clear out the window with black
	gl::clear( Color( 1,1,1 ) );
    gl::draw(bg_tex,getWindowBounds());
    gl::enableDepthWrite();
    gl::enableDepthRead();
    gl::enableAlphaBlending();
    for(int i=0;i<cards.size();i++){
        cards.at(i)->draw();
    }
    cursorPos = getMousePos();
}
void CardBoxApp::prepareSettings(Settings * settings){
    settings->setTitle("Card Box");
}


CINDER_APP_BASIC( CardBoxApp, RendererGl )
