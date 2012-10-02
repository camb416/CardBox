
#include "CardBoxApp.h"
/////////////////////////////////////////
////// Cinder Events (public) ///////////
/////////////////////////////////////////

void CardBoxApp::prepareSettings(Settings * settings){
    
    settings->setWindowSize( 1050,1050 );
    settings->setFrameRate( 60 );
    settings->setTitle( "Card Box" );
    
}

void CardBoxApp::setup(){
    
    cursorDownPos = Vec2f(-1,-1);
    debugState = 0;
    isMouseDown = false;
    drawGrid = false;
    topMargin = 100;
    bottomMargin = 150;
    sideMargin = 100;
    gutter = 15;
    maxCards = 40;
    
    cui.setup();
    cui.hide();
    
    selectedCard = -1;
    
    mParams = params::InterfaceGl( "CardBox Functions", Vec2i( 225, 200 ) );
    mParams.addButton("close card", std::bind( &CardBoxApp::closeCard, this ));
    mParams.addButton("next card", std::bind( &CardBoxApp::nextCard, this ));
    mParams.addButton("previous card", std::bind( &CardBoxApp::prevCard, this ));
    mParams.addButton("info", std::bind( &CardBoxApp::info, this ));
    mParams.addButton("close info", std::bind( &CardBoxApp::closeInfo, this ));
    mParams.addButton("next info", std::bind( &CardBoxApp::nextInfo, this ));
    mParams.addButton("prev info", std::bind( &CardBoxApp::prevInfo, this ));
    mParams.addButton("toggle grid visibility", std::bind( &CardBoxApp::toggleGridVisibility, this ));
    mParams.show();

    
    // and these images are not specified in JSON? 
    infoSection.setup("instructions1.png","instructions2.png");
    
    // PARSE THE JSON
    JsonTree root = JsonTree( loadResource("data.js"));

    cs.basePath = root["basepath"].getValue();
    cs.background = root["background"].getValue();
    cs.shadow_path = root["shadow"].getValue();
    cs.shadow_tex = gl::Texture(loadImage(cs.basePath+"/"+cs.shadow_path));

    // hmmmmm, this should probably be handled more generally...
    console() << "loading the gl texture" << endl;
    if(cs.shadow_tex){
        console() << "looks successful" << endl;
    } else {
        console() << "not looking good" << endl;
    }
    
    // parse the JSON and create the card objects
    int i = 0;
    JsonTree cardTree = root.getChild( "cards" );
    for( JsonTree::ConstIter cIt = cardTree.begin(); cIt != cardTree.end(); ++cIt )
    {
        JsonTree jt = (*cIt);
        
        CardModel cm;
        cm.firstName = (*cIt)["firstname"].getValue();
        cm.lastInitial = (*cIt)["lastinitial"].getValue();
        cm.caption = (*cIt)["caption"].getValue();
        cm.location = (*cIt)["location"].getValue();
        cm.path = (*cIt)["path"].getValue();
        cm.uid = i++;
        if(i<=maxCards) cards.push_back(new Card(&cs,cm));
    }
    
    bg_tex = gl::Texture(loadImage(cs.basePath+"/"+cs.background));
    
    // some text formatting stuff
    gl::Texture::Format fmt;
    fmt.enableMipmapping( true );
    fmt.setMinFilter( GL_LINEAR_MIPMAP_LINEAR );
    
    cui.setup();
    randomize();
}

void CardBoxApp::update()
{
    
    // manual z-sorting (sort methods in Card.h)
    sort(cards.begin(), cards.end(), sortByZ);
    
    // update the cards
    for(int i=0;i<cards.size(); i++){
        cards.at(i)->update();
    }
    
    // card selection
    if(isMouseDown && !cui.isOpen()){
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

}

void CardBoxApp::draw(){
   
    // clear and enable alpha
	gl::clear( Color( 1,1,1 ) );
    gl::enableAlphaBlending();
    
    // draw background
    gl::color(1.0f,1.0f,1.0f,1.0f);
    gl::draw(bg_tex,getWindowBounds());

    // draw all the small cards
    bigCard = NULL;
    for(int i=0;i<cards.size();i++){
        if(!cards.at(i)->getIsBig()){
            // gl::color(1.0f,1.0f,1.0f,1.0f);
            cards.at(i)->draw();
        } else {
            bigCard = cards.at(i);
        }
    }
    
    // draw the Card UI
    cui.drawBackground();
    
    // draw the big card
    if(bigCard!=NULL) bigCard->draw();

    // draws the buttons
    cui.drawForeground();

    // maybe add a flag for this to check if open
    infoSection.draw();
    
    // debug UI
    if(debugState>0){
        params::InterfaceGl::draw();
        if(drawGrid) drawAlignmentGrid();
        
        if(cursorDownPos.x>-1){
      //gl::clear( Color( 0,0,0 ) );
        gl::color(1.0f,1.0f,1.0f,1.0f);
           // gl::disableDepthRead();
          //  gl::disableDepthWrite();
            gl::drawStrokedCircle(cursorDownPos, 20);
            gl::drawLine(cursorDownPos, cursorPos);
            gl::drawSolidCircle(cursorPos, 10);
        }
        
    }
}

void CardBoxApp::mouseMove(MouseEvent evt){
    
    cursorPos = evt.getPos();
    cui.handleMouse(cursorPos);
    
}

void CardBoxApp::mouseDrag(MouseEvent evt){
    
    cursorPos = evt.getPos();
    cui.handleMouse(cursorPos);
    
}

void CardBoxApp::mouseDown( MouseEvent evt )
{
    cursorDownPos = evt.getPos();
    cursorDownTime = getElapsedSeconds();
    isMouseDown = true;
    if(cui.isOpen()) cui.mouseDown(evt);    // pass the event to the CardUI
    
}
void CardBoxApp::mouseUp(MouseEvent evt){
    
     isMouseDown = false;
    
    float timeTouched = getElapsedSeconds()-cursorDownTime;
    
    // if its a tap
    if(cursorDownPos.distance(cursorPos)<30 && timeTouched<3.0f){
    
    cursorDownPos = Vec2f(-1,-1);
   
    if(cui.isOpen()){
       // the CardUI mouseup returns what the user clicked on
        switch(cui.mouseUp(evt)){   
            case 0:
                // closed
                shrinkAll();
                break;
            case 1:
                // prevcard
                prevCard();
                break;
            case 2:
                // nextcard
                nextCard();
                break;
            default:
                // nothing
                break;
        }
        
    } else if(selectedCard>-1){
        
        shrinkAll(selectedCard);
        cui.show();
        cui.updateModel(cards.at(selectedCard)->getModel());
        Rectf theRect = cards.at(selectedCard)->grow(cui.getLowerBound());
        cui.updatePositioning(theRect);
        
    }
    } else if(timeTouched<3.0f){
        // its not a tap...
        console() << "it appears that the angle of the swipe is: " << getAngle(cursorPos, cursorDownPos) << endl;
        float swipeAngle = getAngle(cursorPos, cursorDownPos);
        float swipeRange = 45.0f;
        
       // console() << "down should be >" << -90+0.5f*swipeRange  << " and <" << -90-0.5f*swipeRange << endl;
        
        if(swipeAngle>-0.5f*swipeRange && swipeAngle<0.5f*swipeRange){
            // swipe left
            console() << "swipe left" << endl;
        } else if(swipeAngle>90-0.5f*swipeRange && swipeAngle<90+0.5f*swipeRange){
            // swipe up
            console() << "swipe up" << endl;
        } else if(swipeAngle>180-0.5f*swipeRange || swipeAngle<-180+0.5f*swipeRange){
            // swipe right
            console() << "swipe right" << endl;
        } else if(swipeAngle<-90+0.5f*swipeRange && swipeAngle>-90-0.5f*swipeRange){
            // swipe down
            console() << "swipe down" << endl;
        } else {
            console() << "no diagonal swiping please" << endl;
        }

        
    } else {
        console() << "too slow" << endl;
    }
    cursorDownPos = Vec2f(-1,-1);

}
void CardBoxApp::keyDown(KeyEvent evt){
    
    switch(evt.getChar()){
        case ' ':   // space
            alignToGrid();
            //sortByOrder();
            break;
        case 'r':
        case 'R':
            randomize();
            break;
        case 'g':
        case 'G':
            toggleGridVisibility();
            break;
        case '\t':  // tab
            break;
        case '`':
        case '~':
            if(isFullScreen()){
                showCursor();
            } else {
                hideCursor();
            }
            setFullScreen( ! isFullScreen() );
            break;
        case 'd':
        case 'D':
            debugState++;
            if(debugState>1) debugState = 0;
            break;
        default:
            console() << "KEY PRESSED: " << evt.getCode() << "( " << evt.getChar() << " )" << endl;
            break;
    }
    
}
void CardBoxApp::resize(ResizeEvent evt){
    
    cui.resize(evt);

}
/////////////////////////////////////////
// END Cinder Events (public) ///////////
/////////////////////////////////////////


void CardBoxApp::randomize(){
    
    // randomize card placement
    for(int i=0;i<cards.size();i++){
        cards.at(i)->setPos(
                            Vec3f(
                                  rand() % (getWindowWidth()-200)+100,
                                  rand() % (getWindowHeight()-200)+100,
                                  (rand() % 32000)/320000.0f),
                                  true
                            );
        cards.at(i)->setRot(rand() % 360, true);
        Rectf r = Rectf(0,0,160,160);
        cards.at(i)->fitToRect(r, true);
    }
    shrinkAll();
    cui.hide();

}


void CardBoxApp::sortByOrder(){

    vector<Card *> orderedCards = cards;
    sort(orderedCards.begin(),orderedCards.end(),sortByUID);

}

void CardBoxApp::shrinkAll(int _exception){
    
    for(int i=0;i<cards.size();i++){
        if(i!=_exception){
            cards.at(i)->shrink();
        }
    }
    
}

void CardBoxApp::alignToGrid(){
    
    shrinkAll();
    cui.hide();
    
    vector<Card *> orderedCards = cards;
    
    sort(orderedCards.begin(),orderedCards.end(),sortByUID);
    
    gl::color(1.0f,1.0f,1.0f,1.0f);
    int xRes = 5;
    int yRes = 8;
    
    int widthNoMargins = getWindowWidth()-sideMargin*2;
    int heightNoMargins = getWindowHeight()-(topMargin+bottomMargin);
    
    int boxWidth = (widthNoMargins-(gutter*(xRes-1)))/xRes;
    int boxHeight = (heightNoMargins-(gutter*(yRes-1)))/yRes;
    
    int cardIterator = 0;
    
    for(int j=0;j<yRes;j++){
        for(int i=0;i<xRes;i++){
            int x = i*boxWidth+sideMargin+gutter*i;
            int y = j*boxHeight+sideMargin+gutter*j;
            Rectf r = Rectf(x,y,x+boxWidth,y+boxHeight);
            gl::drawStrokedRect(r);
            if(cardIterator<orderedCards.size()){
                orderedCards.at(cardIterator)->fitToRect(r,true);
                orderedCards.at(cardIterator)->setPos(Vec3f(x+boxWidth/2, y+boxHeight/2,(float)cardIterator/1000.0f),true);
                orderedCards.at(cardIterator)->setRot(0.0f, true);
                cardIterator++;
            }
        }
    }
}
void CardBoxApp::toggleGridVisibility(){
    drawGrid = !drawGrid;
}
    
void CardBoxApp::drawAlignmentGrid(){
    
    gl::disableDepthRead();
    gl::disableDepthWrite();
    gl::color(1.0f,1.0f,1.0f,1.0f);
    int xRes = 5;
    int yRes = 8;
    
    int widthNoMargins = getWindowWidth()-sideMargin*2;
    int heightNoMargins = getWindowHeight()-(topMargin+bottomMargin);
    
    int boxWidth = (widthNoMargins-(gutter*(xRes-1)))/xRes;
    int boxHeight = (heightNoMargins-(gutter*(yRes-1)))/yRes;
    
    for(int j=0;j<yRes;j++){
        for(int i=0;i<xRes;i++){
            int x = i*boxWidth+sideMargin+gutter*i;
            int y = j*boxHeight+sideMargin+gutter*j;
            Rectf r = Rectf(x,y,x+boxWidth,y+boxHeight);
            gl::drawStrokedRect(r);
        }
    }
    
}


int CardBoxApp::getIDfromUID(int card_uid){
    
    // gets the id of a desired card in the cards vector
    for(int i=0;i<cards.size();i++){
        if(cards.at(i)->getUID() == card_uid){
            return i;
        }
    }
    
    console() << "ERROR! uid: " << card_uid << " not found in the cards vector." << endl;
    return -1;
    
}

void CardBoxApp::selectACard(int _selectedID){

    cards.at(_selectedID)->select();
    unselectAll();
    
}
void CardBoxApp::unselectAll(){
    
    for(int i=0;i<cards.size();i++){
        if(i!=selectedCard){
            cards.at(i)->unselect();
        }
    }
    
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
                
                cui.updateModel(cards.at(nextCard_id)->getModel());
                Rectf theRect = cards.at(nextCard_id)->grow(cui.getLowerBound());
                cui.updatePositioning(theRect);
                
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
                
                cui.updateModel(cards.at(nextCard_id)->getModel());
                Rectf theRect = cards.at(nextCard_id)->grow(cui.getLowerBound());
                cui.updatePositioning(theRect);
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

float CardBoxApp::getAngle(const Vec2f & a, const Vec2f  & b){
    return atan2( b.y-a.y,b.x-a.x )/M_PI*180.0f;
}

CINDER_APP_BASIC( CardBoxApp, RendererGl )
