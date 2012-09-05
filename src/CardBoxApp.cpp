#include "cinder/app/AppBasic.h"
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
};

void CardBoxApp::setup()
{
}

void CardBoxApp::mouseDown( MouseEvent event )
{
}

void CardBoxApp::update()
{
}

void CardBoxApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}


CINDER_APP_BASIC( CardBoxApp, RendererGl )
