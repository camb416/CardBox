
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
    
    lastTouched = getElapsedSeconds();
    timeout = 5.0f;
    
    isAttract = false;
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
    mParams.addParam("attract mode", &isAttract);
    mParams.addButton("enter attract", std::bind(&CardBoxApp::enterAttract, this));
    
    mParams.hide();

    
    // and these images are not specified in JSON?
    

    

    shuffleButton = Button("gridButton.png",Vec2f(75,getWindowHeight()-75));
    shuffleButton.addAlternate("shuffleButton.png");
    shuffleButton.show();
    
    // PARSE THE JSON
    string dataErrorMessage;
    string basePath = getHomeDirectory().string()+"Documents/AMNH/postcards/";
    console() << "I'd like to load a file from here: " << basePath+"data.js" << endl;
    JsonTree root;
    
    try{
        root = JsonTree( loadFile(basePath+"data.js"));
    } catch(Exception ex){
        debugState = -1;
        console() << "Problem loading " << basePath << "data.js" << endl;
         dataErrorMessage = "problem loading data.js";
    }
    
    if(debugState>-1){
        try{
    cs.basePath = basePath+root["basepath"].getValue();
    cs.shadow_tex = gl::Texture(loadImage(loadResource("shadow.png")));

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
        }catch(Exception ex){
        // problem parsing data.js
            debugState = -2;
            dataErrorMessage = "problem parsing data.js";
    }
    
            try{
                root = JsonTree( loadFile(basePath+"appsettings.js"));
            } catch(Exception ex){
                debugState = -3;
                 dataErrorMessage = "problem loading appsettings.js";
                console() << "Problem loading " << basePath << "appsettings.js" << endl;
            }

            vector<string> instructionsStrings;
            string backgroundString;
            string elementsPath;
            
            if(debugState>-1){
                try{
                   // cs.basePath = basePath+root["basepath"].getValue();
                    // cs.background = root["background"].getValue();
                    //cs.shadow_path = root["shadow"].getValue();
                    debugState = atoi(root["debugstate"].getValue().c_str());
                    timeout = atof(root["timeout"].getValue().c_str());
                    elementsPath = root["elementspath"].getValue();
                    backgroundString = root["background"].getValue();
                    console() << "setting debugstate to: " << debugState << endl;
                    updateDebugState();
                    
                    JsonTree instructionsTree = root.getChild( "instructions" );
                    for( JsonTree::ConstIter cIt = instructionsTree.begin(); cIt != instructionsTree.end(); ++cIt )
                    {
                        JsonTree jt = (*cIt);
                        string instructionsString = jt.getValue();
                        
                        //console() << "testing..." << jt.getValue() << endl;
                        instructionsStrings.push_back(instructionsString);
                    }

                }catch(Exception e){
                    debugState = -4;
                    console() << "problem parsing appsettings.js" << endl;
                     dataErrorMessage = "problem parsing appsettings.js";
                    
                }
            } 
            console() << "instructions string loaded?" << basePath+elementsPath+instructionsStrings.at(0) << endl;
        try{
            if(instructionsStrings.size()>1){
                // infoSection.setup("instructions1.png","instructions2.png");
                infoSection.setup(basePath+elementsPath+instructionsStrings.at(0), basePath+elementsPath+instructionsStrings.at(1));
            } else if(instructionsStrings.size()>0){
                // infoSection.setup("instructions1.png","instructions2.png");
                infoSection.setup(basePath+elementsPath+instructionsStrings.at(0));
            } else {
                // t'would appear there is no infoSection...
            }
        } catch(Exception e){
            debugState = -5;
            dataErrorMessage = "problem loading the instructions file.";
        }
            infoButton = Button("infoButton.png",Vec2f(getWindowWidth()-75,75));
            if(infoSection.isAvailable()) infoButton.show();
        try{
    bg_tex = gl::Texture(loadImage(basePath+elementsPath+backgroundString));
        } catch(Exception e){
            debugState = -6;
            dataErrorMessage = "problem loading " +backgroundString;
        }
    // some text formatting stuff
    gl::Texture::Format fmt;
    fmt.enableMipmapping( true );
    fmt.setMinFilter( GL_LINEAR_MIPMAP_LINEAR );
    
    cui.setup();
    randomize();
    }

        
    if(debugState<0){
        string normalFont( "Arial" );
        TextLayout layout;
        layout.setFont( Font( normalFont, 36 ) );
        layout.setColor( Color( 1,1,1 ) );
        layout.addLine(dataErrorMessage);
//        layout.addLine(basePath+"data.js");
 //       layout.addLine("parse error.");
        
        Surface8u rendered = layout.render( true, false );
        Surface8u newSurface(1050,1680,false);
        newSurface.copyFrom(rendered, newSurface.getBounds());
        bg_tex = gl::Texture(newSurface);

    }
        
}

void CardBoxApp::updateDebugState(){
    if(debugState==0){
        if(!isFullScreen()) setFullScreen(true);
        if(mParams.isVisible()) mParams.hide();
        hideCursor();
    } else {
        if(isFullScreen()) setFullScreen(false);
        if(!mParams.isVisible()) mParams.show();
        showCursor();
    }
    
}

void CardBoxApp::update()
{
    
    if(!isAttract){
        if(getElapsedSeconds()-lastTouched > timeout) enterAttract();
    }
    if(debugState>-1){
    // manual z-sorting (sort methods in Card.h)
    sort(cards.begin(), cards.end(), sortByZ);
    
    // update the cards
    for(int i=0;i<cards.size(); i++){
        cards.at(i)->update();
    }
    
      
        
    // card selection
    if(isMouseDown && !cui.isOpen() && !infoSection.isOpen() && !shuffleButton.isOver(cursorPos) && !infoButton.isOver(cursorPos)){
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
    } else if(isAttract) {
        for(int i=0;i<cards.size(); i++){
          //  console() << i << ": " << ((float)i/(float)cards.size() * M_PI*2.0f)<< " > " << M_PI*2 << endl;
        //    cards.at(i)->pulse(getElapsedSeconds()+((float)i/(float)cards.size()*M_PI*2.0f));
            cards.at(i)->pulse(getElapsedSeconds()+cards.at(i)->getPos2f().x/(float)getWindowWidth()*2.0f*M_PI);
            
        }
        // console() << "excuse for a breakpoint" << endl;
    }
    }
}

void CardBoxApp::draw(){
    gl::clear( Color( 1,1,1 ) );
    gl::enableAlphaBlending();
    
    // draw background
    gl::color(1.0f,1.0f,1.0f,1.0f);
    gl::draw(bg_tex,getWindowBounds());
    
    if(debugState>-1){
    // clear and enable alpha
	
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
    infoButton.draw();
        shuffleButton.draw();
    // draw the Card UI
    cui.drawBackground();
    
    // draw the big card
    if(bigCard!=NULL) bigCard->draw();

    // draws the buttons
    cui.drawForeground();

    // draw the info section
        if(infoSection.isOpen()){
            
             infoSection.draw();
        } 
           
        
    
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
}

void CardBoxApp::mouseMove(MouseEvent evt){
    // lastTouched = getElapsedSeconds();
    cursorPos = evt.getPos();
    cui.handleMouse(cursorPos);
    
}

void CardBoxApp::mouseDrag(MouseEvent evt){
    lastTouched = getElapsedSeconds();
    cursorPos = evt.getPos();
    cui.handleMouse(cursorPos);
    
}

void CardBoxApp::enterAttract(){
    if(infoSection.isOpen()) infoSection.close();
    isAttract = true;
    randomize();
}

void CardBoxApp::leaveAttract(){
    isAttract = false;
    shrinkAll();
}

void CardBoxApp::mouseDown( MouseEvent evt )
{
    lastTouched = getElapsedSeconds();
    if(isAttract){
        leaveAttract();
    }
    cursorDownPos = evt.getPos();
    cursorDownTime = getElapsedSeconds();
    isMouseDown = true;
    
    if(cui.isOpen()){
        cui.mouseDown(evt);
    }else if(infoSection.isOpen()){
        infoSection.mouseDown(evt);    // pass the event to the InfoSection
    } else if(infoButton.isOver(cursorDownPos)){
        infoButton.down();
    } else if(shuffleButton.isOver(cursorDownPos)){
        shuffleButton.down();
    }
    
}
void CardBoxApp::mouseUp(MouseEvent evt){
    lastTouched = getElapsedSeconds();
     isMouseDown = false;
    
    float timeTouched = getElapsedSeconds()-cursorDownTime;
    
    // if its a tap
    if(cursorDownPos.distance(cursorPos)<30 && timeTouched<1.5f){
    
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
        
    } else if(infoSection.isOpen()){
       
        switch(infoSection.mouseUp(evt)){
            case 0:
                infoButton.show();
                shuffleButton.show();
                break;
            default:
                // do nothing
                break;
        }
         console() << "mouse up when the info section is open" << infoSection.mouseUp(evt) << endl;
     
    } else if(infoButton.isOver(cursorPos) && infoSection.isAvailable()){
        infoButton.up();
        info();
        infoButton.hide();
        shuffleButton.hide();
        
    } else if(shuffleButton.isOver(cursorPos)){
        // do something here
        shuffleButton.up();
        shuffleButton.swap() ? alignToGrid() : randomize();
      
    } else if(selectedCard>-1){
        
        shrinkAll(selectedCard);
        cui.show();
        cui.updateModel(cards.at(selectedCard)->getModel());
        Rectf theRect = cards.at(selectedCard)->grow(cui.getLowerBound());
        cui.updatePositioning(theRect);
        
    }
    } else if(timeTouched<2.0f){
        // its not a tap...
        console() << "it appears that the angle of the swipe is: " << getAngle(cursorPos, cursorDownPos) << endl;
        float swipeAngle = getAngle(cursorPos, cursorDownPos);
        float swipeRange = 45.0f;
        
       // console() << "down should be >" << -90+0.5f*swipeRange  << " and <" << -90-0.5f*swipeRange << endl;
        
        if(swipeAngle>-0.5f*swipeRange && swipeAngle<0.5f*swipeRange){
            // swipe left
            if(cui.isOpen()) nextCard();
            console() << "swipe left" << endl;
        } else if(swipeAngle>90-0.5f*swipeRange && swipeAngle<90+0.5f*swipeRange){
            // swipe up
            console() << "swipe up" << endl;
        } else if(swipeAngle>180-0.5f*swipeRange || swipeAngle<-180+0.5f*swipeRange){
            // swipe right
            if(cui.isOpen())  prevCard();
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
            if(debugState>0){
                mParams.show();
            } else {
                mParams.hide();
            }
            break;
        default:
            console() << "KEY PRESSED: " << evt.getCode() << "( " << evt.getChar() << " )" << endl;
            break;
    }
    
}
void CardBoxApp::resize(ResizeEvent evt){
    
    
    
       
    shuffleButton.moveTo(Vec2f(75,getWindowHeight()-75));
    infoButton.moveTo(Vec2f(getWindowWidth()-75,75));
    cui.resize(evt);

}
/////////////////////////////////////////
// END Cinder Events (public) ///////////
/////////////////////////////////////////


void CardBoxApp::randomize(){
    
    if(shuffleButton.getState()){
        shuffleButton.swap();
    }
    
    // randomize card placement
    for(int i=0;i<cards.size();i++){
        cards.at(i)->setPos(
                            Vec3f(
                                  rand() % (getWindowWidth()-200)+100,
                                  rand() % (getWindowHeight()-200)+100,
                                  (rand() % 32000)/320000.0f),
                                  true
                            );
        float randomNumber = (rand() % 360) - 180.0f;
        cards.at(i)->setRot(randomNumber, true);
        // this should be configurable...
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
    if(_exception>-1){
        console() << "shrinking all with the exception of " << _exception << endl;
    } else {
        console() << "no exceptions." << endl;
    }
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
        console() << "the current card has ids of: " << curCard_uid << ", " << getIDfromUID(curCard_uid) << endl;
        if(curCard_uid>-1){
            curCard_uid++;
            if(curCard_uid>=cards.size()) curCard_uid = 0; // loop
            nextCard_id = getIDfromUID(curCard_uid);
            console() << "the next card id is: " << curCard_uid << ", " << nextCard_id << endl;
           
            if(nextCard_id>-1){
                shrinkAll(nextCard_id);
            //    cui.show();
                
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
