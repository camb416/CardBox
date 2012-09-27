//
//  CardBoxApp.h
//  CardBox
//
//  Created by Cameron Browning on 9/20/12.
//
//

#ifndef CardBox_CardBoxApp_h
#define CardBox_CardBoxApp_h

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
    void mouseUp( MouseEvent evt );
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
    bool isMouseDown;
    void drawAlignmentGrid();
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
    
    int topMargin;
    int bottomMargin;
    int sideMargin;
    int gutter;
    void alignToGrid();
    
    int maxCards;
    bool drawGrid;
    
};

#endif
