// myRaytracer.cpp - fsh;  for ECE661,
// after adjusting SDL to add Ray, Intersection, etc.
// Use Scene class to draw a scene with openGL
// some pieces left to students to implement
#include "SDL.h"
#include "Camera.h"
#include <math.h>
#include <io.h> // needed to display current directory - DOS

//############################ GLOBALS ########################

Camera cam; // construct it
Scene scn;  // construct it

int screenWidth = 300, screenHeight = 300;
int doRayTrace = 0;
float lightStrength = 1.0;
float amount = 1.0;
int blockSize = 1;
//<<<<<<<<<<<<<<<<<<<<<< getFileName >>>>>>>>>>>>>>>>.
void getFileName(string& fname)
{ // display list of all .dat files in current directory - needs #include <io.h>
	struct _finddata_t dat_file;
	long hFile;
	if( (hFile = _findfirst("*.dat", &dat_file)) == -1L)
		 cout << " No *.dat files in current directory!\n";
	else
	{
			cout << "Available *.dat files\n";
			cout << "FILE NAME\t\tSIZE\n";
			cout << dat_file.name << "\t\t"<< dat_file.size << endl;
			while( _findnext( hFile, &dat_file) == 0)
					cout << dat_file.name <<"\t\t";
	}
	_findclose(hFile);
	cout << "type file name: (omit the '.dat') \n";
//	cout<<RAND_MAX<<"\n";
	cin >> fname; // user types his/her choice
	fname += ".dat"; //append suffix

}
//<<<<<<<<<<<<<<<<<<<<<<<<<< myIntensity >>>>>>>>>>>>>>>>>>>
void myIntensity(float change)
{
	Light *temp;
	temp=scn.light;
	while(temp!=NULL)
	{				
		temp->color.red+=change;
		if (temp->color.red<0.0 ) temp->color.red=0;
		if (temp->color.red>1.0 ) temp->color.red=1;
		
		temp->color.green+=change;
		if (temp->color.green<0.0 ) temp->color.green=0.0;
		if (temp->color.green>1.0 ) temp->color.green=1.0;

		temp->color.blue+=change;
		if (temp->color.blue<0.0 ) temp->color.blue=0;
		if (temp->color.blue>1.0 ) temp->color.blue=1;
		
		temp=temp->next;

	}
}//end of myIntensity
//<<<<<<<<<<<<<<<<<<<<<<<< myKeyboard >>>>>>>>>>>>>>>>>>>>>>
void myKeyboard(unsigned char key, int x, int y)
{
	// let user type 2-char string to specify a command
	static int numGot = 0; // # of char's so far typed
	static string s = "";
	if(key == 27) exit(-1);
	if(key == '.'|| key == ' '){numGot = 0; return;} // reset the 2-char string
	if(numGot % 2 == 0)	{s = ""; numGot = 0;}
	s += key; numGot++;
	if(numGot == 1) return; // still need another char for command
	//-------------------------------------------
	//now have two letter command; process it
	if(s == "LS") myIntensity(0.2);
	else if(s == "ls")myIntensity(-0.2);
	else if(s == "qu") exit(-1);
	else if(s == "AM") amount *= 2;
	else if(s == "am") amount /= 2;
	else if(s == "sf") cam.slide(0,0, -amount);  // slide camera forward
	else if(s == "sb") cam.slide(0,0,amount);  //slide camera back
	else if(s == "sl") cam.slide(-amount,0,0);  // slide camera to left
	else if(s == "sr") cam.slide( amount, 0,0);  //slide camera right
	else if(s == "su") cam.slide(0, amount,0);  //slide camera up
	else if(s == "sd") cam.slide(0,-amount,0);  //slide camera down
	else if(s == "pu") cam.pitch(-10 * amount);
	else if(s == "pd") cam.pitch( 10 * amount);
	else if(s == "rl") cam.roll( 10 * amount);  //roll CCW
	else if(s == "rr") cam.roll(-10 * amount);  //roll CW
	else if(s == "yl") cam.yaw( -10 * amount);
	else if(s == "yr") cam.yaw(  10 * amount);
	else if(s == "c1") cam.set(10.0,10.0,10.0,0,0,0,0,1,0);
  	else if(s == "c2") cam.set(0,0,10,0,0,0,0,1,0);
	else if(s == "CV"){/* open camera viewangle */}
	else if(s == "cv"){/* close camera viewangle */}
	else if(s == "r1"){ blockSize = 1; doRayTrace = 1;}
	else if(s == "r4"){ blockSize = 4; doRayTrace = 1;}
	else if(s == "r2"){ blockSize = 2; doRayTrace = 1;}
	else if(s == "r8"){ blockSize = 8; doRayTrace = 1;}
	else if(s == "gl"){ doRayTrace = 0;}//draw by OpenGL
	else if(s == "tr"){ doRayTrace = 1;}//draw by RayTracer with current block size
    else if(s == "rs")
	{ 
		string fname;
		getFileName(fname);
		scn.read(fname);
	}

	else
	{
		cout << "\nnot a valid command\n";
		s = "";//clear s after wrong commands
	}
	glutPostRedisplay();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<< myReshape >>>>>>>>>>>>>>>>>>>
void myReshape(int width, int height)
{ // adjust the camera aspect ratio to match that of the viewport
	/* students do here: update screenWidth and screenHeight;
	   update nCols, nRows in camera;
	   update camera's shape */
	if(height == 0)
		height = 1;

	float ratio = 1.0*width/height;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, width, height);

	// Set the correct perspective.
	gluPerspective(45,ratio,1.0,10000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);

    screenWidth=width;
	screenHeight=height;
	//to update nCols nRows camera's shape
	cam.setShape(20.0,screenWidth/(float)screenHeight,1.0, 10000.0); // match it to viewport
    cam.setDisplay(screenHeight, screenWidth); //sets nRows, nCols in camera

	//glViewport(0, 0, screenWidth, screenHeight); // update viewport
}
//<<<<<<<<<<<<<<<<<<<<<<< display >>>>>>>>>>>>>>>>>>>>>>>>>>
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);
	cam.drawOpenGL(scn);
	if(doRayTrace)
	{
		cam.raytrace(scn,blockSize);
		doRayTrace = 0;
	}
} // end of display
//<<<<<<<<<<<<<<<<<<<<<<<<<<< myinit >>>>>>>>>>>>>>>>>>>>>>>>>>>.
void myInit(void)
{
	string fname;
	glColor3f(0.0f,0.0f,0.0f);
	glShadeModel(GL_SMOOTH); // or could be GL_FLAT
	glEnable(GL_NORMALIZE);
	glViewport(0, 0, screenWidth, screenHeight);
	cam.set(10.0f,8.0f,9.4, 0,0.25f,0, 0,1,0); // initial eye, lookAt, up
	//cam.set(5.0f,4.0f,4.7, 0,0.25f,0, 0,1,0);
	cam.setShape(20.0,screenWidth/(float)screenHeight,1.0, 10000.0); // match it to viewport
    cam.setDisplay(screenHeight, screenWidth); //sets nRows, nCols in camera
	getFileName(fname);
	scn.read(fname);


} // end of myInit

//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB |GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("my ECE661 raytracer");
	glutKeyboardFunc(myKeyboard);
	myInit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	
	glutMainLoop();
}
