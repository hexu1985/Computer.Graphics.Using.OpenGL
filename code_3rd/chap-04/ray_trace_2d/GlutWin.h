#ifndef _GLUTWIN_H_
#define _GLUTWIN_H_

//#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

////////////////////////////////////////////////////////////
//                       GlutWin                          //
//                                                        //
// our class for general purpose GLUT initialization      //
////////////////////////////////////////////////////////////
class GlutWin {

public:

	// constructor
	GlutWin( int windowHeight, int windowWidth,
			  int windowPosX, int windowPosY,
			  unsigned int displayMode,
			  const char * windowTitle );

	~GlutWin() {};


private:
	
	const char *	windowTitle;
	int				windowHeight, windowWidth;
	int				windowPosX,   windowPosY;
	int				windowID;
	unsigned int	displayMode;
	bool			fullScreen;

};

#endif // _CGLUTWIN_H_
