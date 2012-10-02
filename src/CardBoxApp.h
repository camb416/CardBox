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
    /////////////////////////////////////////
    // Cinder Events ////////////////////////
    /////////////////////////////////////////
    void prepareSettings(Settings * settings);
	void setup();
    void update();
    void draw();
	void mouseMove(MouseEvent evt);
    void mouseDrag(MouseEvent evt);
    void mouseDown( MouseEvent evt );
    void mouseUp( MouseEvent evt );
    void keyDown(KeyEvent evt);
    void resize(ResizeEvent evt);
    /////////////////////////////////////////
    
private :
    /////////////////////////////////////////
    // app view and controller methods //////
    /////////////////////////////////////////
    // application view transistions
    void randomize();               // shuffle the cards
    void toggleGridVisibility();
    void drawAlignmentGrid();       // draws the destination grid of the alignToGrid()
    void alignToGrid();
    // mode-independent selection actions
    void selectACard(int _selectedID);
    void unselectAll();
    void shrinkAll(int _exception = -1);
    // helper methods
    void sortByOrder();             // sort Cards by UID
    int getIDfromUID(int card_uid);
    float getAngle(const Vec2f & a, const Vec2f  & b);
    /////////////////////////////////////////
    
    /////////////////////////////////////////
    // button handlers //////////////////////
    /////////////////////////////////////////
    // card ui
    void closeCard();   // close
    void nextCard();    // next
    void prevCard();    // prev
    //info
    void info();        
    void closeInfo();   // close
    void nextInfo();    // next
    void prevInfo();    // prev
    /////////////////////////////////////////

    /////////////////////////////////////////
    // App-Level Data ///////////////////////
    /////////////////////////////////////////
    int debugState;
    CardSettings cs;
    // App-Level Visual Elements
    gl::Texture bg_tex;
    gl::Texture shadow_tex;
    // App-Level Visual Settings
    // (these should maybe load from the JSON?)
    int topMargin;
    int bottomMargin;
    int sideMargin;
    int gutter;
    bool isMouseVisible;
    /////////////////////////////////////////
    
    /////////////////////////////////////////
    // CARDS ////////////////////////////////
    /////////////////////////////////////////
    vector<Card *> cards;
    Card * bigCard;
    int selectedCard;
    int maxCards;
    /////////////////////////////////////////
    
    // Big Card UI
    CardUI cui;

    // Info Section UI
    InfoSection infoSection;
    
    // Debugging UI
	params::InterfaceGl	mParams;
    
    // Application State
    bool isMouseDown;
    Vec2f cursorPos;
    Vec2f cursorDownPos;
    float cursorDownTime;
    bool drawGrid;
  
};

#endif
