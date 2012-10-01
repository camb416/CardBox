
#include "CardBoxApp.h"
/////////////////////////////////////////
////// Cinder Events (public) ///////////
/////////////////////////////////////////
void CardBoxApp::prepareSettings(Settings * settings){
    settings->setWindowSize( 1050,1050 );
    settings->setFrameRate( 60 );
    settings->setTitle( "Card Box" );
}
void CardBoxApp::setup()
{
    
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
    sort(cards.begin(), cards.end(), sortByZ);
    for(int i=0;i<cards.size(); i++){
        cards.at(i)->update();
    }
    
    if(isMouseDown){
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

    cui.handleMouse(cursorPos);
    
    
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
    for(int i=0;i<cards.size();i++){
        if(!cards.at(i)->getIsBig()){
            gl::color(1.0f,1.0f,1.0f,1.0f);
            cards.at(i)->draw();
        } else {
            bigCard = cards.at(i);
        }
    }
    
    cui.draw();
        
    gl::color(1.0f,1.0f,1.0f,1.0f);
    if(bigCard!=NULL){
        bigCard->draw();
    }
    cursorPos = getMousePos();
    // gl::color(0.0f,0.0f,1.0f,1.0f);
    // gl::disableDepthRead();
    // gl::disableDepthWrite();
    // gl::drawLine(Vec3f(cursorPos.x, cursorPos.y,0.0f), Vec3f(myVec.x, myVec.y,0.0f));
    // gl::color(1.0f,1.0f,1.0f,1.0f);
    
    
    cui.drawOverlay();

    
    infoSection.draw();
    
    
    
    params::InterfaceGl::draw();
    
    
    
    if(drawGrid) drawAlignmentGrid();
    
}
void CardBoxApp::mouseDown( MouseEvent evt )
{
    isMouseDown = true;
    if(cui.isOpen()){
        cui.mouseDown(evt);
        
        
    }
}
void CardBoxApp::mouseUp(MouseEvent evt){
    isMouseDown = false;
    if(cui.isOpen()){
       
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
    }
    else if(selectedCard>-1){
        shrinkAll(selectedCard);
        cui.show();
        //closeButton.show();
        //prevButton.show();
        //nextButton.show();
        

        cui.updateModel(cards.at(selectedCard)->getModel());
        Rectf theRect = cards.at(selectedCard)->grow(cui.getLowerBound());
        cui.updatePositioning(theRect);
    }

}
void CardBoxApp::keyDown(KeyEvent evt){
    switch(evt.getChar()){
        case ' ':
            alignToGrid();
            //sortByOrder();
            break;
        case 'r':
        case 'R':
            randomize();
            break;
        case 'g':
        case 'G':
            drawGrid = !drawGrid;

    
    
            
        case '\t':
            // model->useFrontPlate = !model->useFrontPlate;
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
        //    debugState++;
//if(debugState>2){
        //        debugState = 0;
         //   }
         //   if(debugState!=0){
         //       myApp->showCursor();
         //   } else {
         //       myApp->hideCursor();
          //  }
         //   updateViewStates();
            break;
        default:
            console() << "KEY PRESSED: " << evt.getCode() << "( " << evt.getChar() << " )" << endl;
            break;
    }
    
    
}
void CardBoxApp::resize(ResizeEvent evt){
    // resize event (empty so far)
    cui.resize(evt);
}
/////////////////////////////////////////
// END Cinder Events (public) ///////////
/////////////////////////////////////////



void CardBoxApp::randomize(){
    // randomize everything
    for(int i=0;i<cards.size();i++){
        //if(largestY<cards.at(i)->getSize().y) largestY = cards.at(i)->getSize().y;
        cards.at(i)->setPos(Vec3f(rand() % (getWindowWidth()-200)+100, rand() % (getWindowHeight()-200)+100,(rand() % 32000)/320000.0f), true);
        cards.at(i)->setRot(rand() % 360, true);
        Rectf r = Rectf(0,0,160,160);
        cards.at(i)->fitToRect(r, true);
    }
    shrinkAll();
    cui.hide();

}


void CardBoxApp::sortByOrder(){
    // align everything
    //  float xCount  = 225;
    //  float yCount = 225;
    // float largestY = 110;
    // float margin = 25;
    
    vector<Card *> orderedCards = cards;
    
    sort(orderedCards.begin(),orderedCards.end(),sortByUID);
    
    /*
     for(int i=0;i<orderedCards.size();i++){
     // if(largestY<orderedCards.at(i)->getSize().y && !orderedCards.at(i)->getIsBig()) largestY = orderedCards.at(i)->getSize().y;
     orderedCards.at(i)->setPos(Vec3f(xCount, yCount,(float)i/1000.0f),true);
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
     */
    
    
}

void CardBoxApp::shrinkAll(int _exception){
    for(int i=0;i<cards.size();i++){
        if(i!=_exception){
            cards.at(i)->shrink();
        }
    }
    // TODO check up on this
    /*
    if(_exception>-1){
        timeline().apply(&curtainsAlpha, 0.8f,1.0f,EaseInOutSine());
    } else {
        timeline().apply(&curtainsAlpha, 0.0f,1.0f,EaseInOutSine());
    }
     */
}





void CardBoxApp::alignToGrid(){
    // gl::disableDepthRead();
    // gl::disableDepthWrite();
    
    shrinkAll();
    cui.hide();
    //closeButton.hide();
    //prevButton.hide();
    //nextButton.hide();
    
    
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
            //gl::drawStrokedCircle(Vec2f(x,y),24);
        }
    }
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
            //gl::drawStrokedCircle(Vec2f(x,y),24);
        }
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
   // myVec = cards.at(_selectedID)->getPos2f();
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
    //closeButton.hide();
   // prevButton.hide();
   // nextButton.hide();
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



CINDER_APP_BASIC( CardBoxApp, RendererGl )
