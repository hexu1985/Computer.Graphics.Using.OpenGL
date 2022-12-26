//////////////////////////////////////////////////////////////////////
// init.cpp - obsahuje funkcie pre vytvorenie okna, ktore bude pracovat
// s OpenGL
// Vazba na ostatne je iba cez init.h a instanciu triedy scene
//////////////////////////////////////////////////////////////////////

#include "scene.h"
#include "resource.h"

HDC			hDC=NULL;		// privátny GDI Device Context
HGLRC		hRC=NULL;		// Rendering Context
HWND		hWnd=NULL;		// Handle na okno
HINSTANCE	hInstance;		// Inštalacia programu

char	MenoApplikacie[]="Bump-mapping";
bool	keys[256];			// pole - práca s klavesnicou
bool	fullscreen=1;		// indikátor pre fullscreen
bool	active=1;			// indikator aktivy (negacia priznaku minimalizacie)
int		error=0;			// ak je 1 nastala chyba a program sa ma ukoncit
int		screen_x=800;		// horizontalne rozlisenie
int		screen_y=600;		// vertikalne rozlisenie
int		screen_bit=16;		// pocet bitov na 1 pixel
int		english=1;
scene *scene_=NULL;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// deklarácia WndProc
void KillGLWindow(void);								// deklaracia KillGLWindow
int Nastav_rozlisenia();

//////////////////////////////////////////////////////////////////////
/*	Parametry pri tvorbe OpenGL okna		*
 *	title			- titulek okna			*
 *	width			-  sirka okna			*
 *	height			- vyska okna			*
 *	bits			- pocet bitu na pixel	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreen_)
{
	GLuint		PixelFormat;
	WNDCLASSEX	wc;		// miesto pre strukturu pouzitu na registraciu triedy okna
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		WindowRect;					// miesto pre velkost okna
	WindowRect.left=(long)0;
	WindowRect.right=(long)width;
	WindowRect.top=(long)0;
	WindowRect.bottom=(long)height;

	fullscreen=fullscreen_;
	wc.cbSize			= sizeof(WNDCLASSEX); //velkost struktury WINDOWCLASSEX
	wc.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  /*nastavenie stylu okna
				CS_HREDRAW - prekreslenie okna pri zmene horizontalne velkosti okna
				CS_VREDRAW - -----------""-------------- vertikalnej  ------""-----
				CS_OWNDC - vyhradi jedinecny kontext zariadenia pre kazde okno  */
	wc.lpfnWndProc		= (WNDPROC) WndProc; //smernika na funkciu na spracovanie sprav
	wc.cbClsExtra		= 0; //specifikuje pocet extra bytov na konci struktury triedy okien na ukladanie informacii
	wc.cbWndExtra		= 0; //-------""--------------------, ktore sa maju vyhradit po vyskyte okna
	wc.hInstance		= hInstance;		//handle (ukazovatel) na instalaciu (paremeter z WinMain)
	wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINLOGO)); //handle ikony  ktora sa ma pouzit pre danu truedu okien
	wc.hCursor			= NULL;		// LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR_MAIN)); //handle kurzora ktory ----------------""----------------
	wc.hbrBackground	= NULL;		//handle stetca pouziteho na vyfarbenie pozadia okna
	wc.lpszMenuName		= NULL;		//ukazovatel na sz(StringZero) v ktorom je nazov pre menu (!)triedy
	wc.lpszClassName	= "OpenGL";	//ukaz na sz, ktory obsahuje nazov triedy. Tento nazov sa pouzie v parametry lpClassName vo funkcii CreateWindowEx
	wc.hIconSm			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TITLE));	//handle k ikone, ktora sa ma pouzit v titulkovom pruhu okien, vytvorenych touto triedou. Ak je NULL pouzije sa hIcon

	if(!RegisterClassEx(&wc))		//registracia novej tiredy okna, pouzivanych na vytavaranie okien
	{	MessageBox(NULL,"Chyba pri registracii triedy okna.","ERROR",MB_OK|MB_ICONEXCLAMATION);return 0;}

	if(fullscreen)	//ZMENA DO FULLSCREEN
	{	
		DEVMODE dmScreenSettings;	//struktura popisujuca graficky mod
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		//vynuluje dmScreenSettings
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			//nastavy velkost
		dmScreenSettings.dmPelsWidth	= width;		//vertikalne roazlisenie
		dmScreenSettings.dmPelsHeight	= height;		//horizontalne rozlisenie
		dmScreenSettings.dmBitsPerPel	= bits;			//pocet bitov na pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT; //specifikuje ktore polozky v strukture su menene ??
		/**** zmena modu !!! ****/
		{	int mod;
			mod=ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN);
			if(mod!=DISP_CHANGE_SUCCESSFUL)
			{	
				switch(mod)
				{	
					case(DISP_CHANGE_RESTART):MessageBox(NULL,"The computer must be restarted in order for the graphics mode to work.","Chyba pri inicializacii grafickeho rezimu",MB_ICONERROR);break;
					case(DISP_CHANGE_BADFLAGS):MessageBox(NULL,"An invalid set of flags was passed in.","Chyba pri inicializacii grafickeho rezimu",MB_ICONERROR);break;
					case(DISP_CHANGE_FAILED):MessageBox(NULL,"The display driver failed the specified graphics mode.","Chyba pri inicializacii grafickeho rezimu",MB_ICONERROR);break;
					case(DISP_CHANGE_BADMODE):MessageBox(NULL,"The graphics mode is not supported.","Chyba pri inicializacii grafickeho rezimu",MB_ICONERROR);break;
				}
				if(MessageBox(NULL,"Chyba pri inicializacii grafickeho rezimu. Chcete pokracovat v okne?","Chyba",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{	fullscreen=0;}  //pokracovanie v okne
				else
				{	MessageBox(NULL,"Program se uzavrie.","ERROR",MB_OK|MB_ICONSTOP);return FALSE;}
			}
		}
	}
	
	ShowCursor(0);

	if (fullscreen)
	{	
		dwExStyle=WS_EX_APPWINDOW; //Forces a top-level window onto the taskbar when the window is minimized.
		dwStyle=WS_POPUP;		//vytvara prekryte okno
	//	ShowCursor(0);			//skryje kurzor
	}
	else
	{	
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; //okno ktore ma ohranicenie s vyvysenou hranou (WS_EX_WINDOWEDGE)
		dwStyle=WS_OVERLAPPEDWINDOW; //vytvara okno s titulkovym pruhom, ohranicenim, okno je ohraniceny tenkou ciarov
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); //vypocitava velkost okna
	/*vytvara okno, generuje: WM_GETMINMAXINFO,WM_NCCREATE,WM_NCCALCSIZE,WM_CREATA,WM_SHOWWINDOW */
	hWnd = CreateWindowEx( dwExStyle,	//rozsireny styl okna
					"OpenGL",		//ukazovatel na sz obsahujuci platny nazov triedy pre okno (registrovanej triedy)
					title,			//ukazovatel na sz obsahujuci meno okna (zobrazuje sa titulkovom pruhu)
					dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,  //...
					0, 0,				//pozicia okna na obrazovke (x,y)
					WindowRect.right-WindowRect.left,	//sirka okna
					WindowRect.bottom-WindowRect.top,	//vyska okna
					NULL,				//handle k rodicovskemu oknu
					NULL,				//handle k menu okna. Ak sa ma pouzit trieda menu pouzite hodnotu NULL
					hInstance,		//handle vyskytu aplikacie, ktora vytvara okno
					NULL);			//ukazovatel na data ktora budu odovzdane oko lParam spravy WM_CREATE
	if (!hWnd)		//ak sa okno nepodarilo vytvorit koniec
	{	
		KillGLWindow();MessageBox(NULL,"Chyba pri tvorbe okna.","ERROR",MB_OK|MB_ICONEXCLAMATION);return FALSE;
	}
/*Abychom mohli pracovat s OpenGL musíme premostit vykreslování na rozhraní OpenGL tím,
ze nastavíme tzv. PixelFormatDescriptor, který slouzí jako komunikátor mezi výstupními
rutinami windowsu a výstupními rutinami knihovny OpenGL. Poukazuje na to, jaké rozdíly
jsou v zobrazování barev atd. Toto nastavení najdete ve funkci bSetupPixelFormat.
Pokud vás zajímají jednotlivé polozky ve strukture PIXELFORMATDESCRIPTOR, najdete to v helpu.*/
	static	PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),	//velkost PIXELFORMATDESCRIPTOR
		1,							//verzia tato hodnota ma byt nastavena na 1
		PFD_DRAW_TO_WINDOW |		//support window  	-(The buffer can draw to a window or device surface.)
		PFD_SUPPORT_OPENGL |	//support OpenGL  	-(The buffer supports OpenGL drawing.)
		PFD_DOUBLEBUFFER,		//double buffered  	-(The buffer is double-buffered. This flag and PFD_SUPPORT_GDI are mutually exclusive in the current generic implementation
		PFD_TYPE_RGBA,			//RGBA type  			-(RGBA pixels. Each pixel has four components in this order: red, green, blue, and alpha.)
		(BYTE)bits,  		//<bits> -bit color depth  -(For RGBA pixel types, it is the size of the color buffer, excluding the alpha bitplanes.)
		0, 0, 0, 0, 0, 0,	//color bits ignored  		-(cRedBits,cRedShift,cGreenBits,cGreenShift,cBlueBits,cBlueShift)
		0,	0,					//no alpha buffer, shift bit ignored ,  -(cAlphaBits,cAlphaShift)
		0,					//no accumulation buffer  	-(cAccumBits)
		0, 0, 0, 0,   		//accum bits ignored  		-(cAccumRedBits,cAccumGreenBits,cAccumBlueBits,cAccumAlphaBits)
		16,					//16-bit z-buffer,  			-(cDepthBits - Specifies the depth of the depth (z-axis) buffer.)
		0,					//no stencil buffer  		-(cStencilBits - Specifies the depth of the stencil buffer.)
		0, 					//no auxiliary buffer  		-(cAuxBuffers)
		PFD_MAIN_PLANE,		//main layer  -(iLayerType - Ignored. Earlier implementations of OpenGL used this member, but it is no longer used.)
		0,					//reserved  					-(bReserved)
		0, 0, 0				//layer masks ignored  		-(dwLayerMask,dwVisibleMask,dwDamageMask)
	};

	hDC=GetDC(hWnd); 		//ziskanie handla obrazoveho kontextu zariadenia (DC) klienskej casti okna (s handlom hWnd)
	if (!hDC)
	{	
		KillGLWindow();
		MessageBox(NULL,"Nejde vytvorit GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	/*The ChoosePixelFormat function attempts to match an appropriate pixel format supported by a device context to a given pixel format specification.*/
	PixelFormat=ChoosePixelFormat(hDC,&pfd); //device context to search for a best pixel format match, pixel format for which a best match is sought
	if (!PixelFormat)
	{	
		KillGLWindow();
		MessageBox(NULL,"Nejde nájst PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	/*The SetPixelFormat function sets the pixel format of the specified device context to the format specified by the iPixelFormat index.*/
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))
	{	
		KillGLWindow();
		MessageBox(NULL,"Nejde nastavit PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	/*The wglCreateContext function creates a new OpenGL rendering context,
	which is suitable for drawing on the device referenced by hdc.
	The rendering context has the same pixel format as the device context.*/
	hRC=wglCreateContext(hDC);  // create a rendering context
	if (!hRC)                //return value is a valid handle to an OpenGL rendering context.
	{	
		KillGLWindow();
		MessageBox(NULL,"Nejde vytvorit GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
   /*The wglMakeCurrent function makes a specified OpenGL rendering context the calling
   thread's current rendering context. All subsequent OpenGL calls made by the thread are
   drawn on the device identified by hdc. You can also use wglMakeCurrent to change the
   calling thread's current rendering context so it's no longer current. */
	if(!wglMakeCurrent(hDC,hRC))  // make it the calling thread's current rendering context
	{	
		KillGLWindow();
		MessageBox(NULL,"Nejde aktivovat GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	ShowWindow(hWnd,SW_SHOW);		//zvyditelnie okna, posiela spravu WM_SHOWWINDOW
	SetForegroundWindow(hWnd);		//nastavuje okno na to s ktorym pracuje uzivatel
	SetFocus(hWnd);					//nastavy vstup klavesnice na toto okno
	// inicializacia sceny
	scene_ = new scene;				// vytvarame scenu
	if(scene_==NULL || error)		// ak sa nevytvorila, alebo pri vytvarani vznikla chyba
	{
		if(scene_!=NULL)delete scene_;	// ak sa vytvorila, uvolnime
		KillGLWindow();					// vypneme OpenGL
		MessageBox(NULL,"Chyba pri vytvarani sceny.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;						// koniec, aplikacia sa ukonci
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////
void KillGLWindow(void)					// rušíme okno
{	
	ShowCursor(1);
	if (fullscreen)						// Fullscreen (ano/nie)
	{	
		ChangeDisplaySettings(NULL,0);	// zmena grafickeho rezimu na prednastavene rozlisenie
	}
	if (hRC)
	{	
		if(!wglMakeCurrent(NULL,NULL))	// make the rendering context not current
		{	MessageBox(NULL,"DC a RC chyba.","ERROR",MB_OK | MB_ICONINFORMATION);}
		if(!wglDeleteContext(hRC))		// delete the rendering context
		{	MessageBox(NULL,"Chyba: Delete Rendering Context.","ERROR",MB_OK | MB_ICONINFORMATION);}
		hRC=NULL;
	}
	if (hDC && !ReleaseDC(hWnd,hDC))		// uvolnuje kontext zariadenia
	{	
		MessageBox(NULL,"Chyba: Uvolnenia Device Context.","ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;
	}
	if (hWnd && !DestroyWindow(hWnd))   // ukoncuje aplikaciu, posiela spravu WM_DESTROY
	{	
		MessageBox(NULL,"Chyba: hWnd.","ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;
	}
	if (!UnregisterClass("OpenGL",hInstance)) //removes a window class, freeing the memory required for the class.
	{	
		MessageBox(NULL,"Nejde odregistrovat okno.","ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;
	}
}


/*********************************************************************************************/
/*Funkcia na spracovanie sprav. Spravy su posielane do funkcie pomocou DispatchMessage(&msg);*/

LRESULT CALLBACK WndProc(HWND	hWnd,UINT	uMsg,WPARAM	wParam,LPARAM	lParam)
{
	switch (uMsg)
	{	
		case WM_ACTIVATE:			//generovane ak je okno aktivizovane, alebo deaktivizovane
		{	
			if (!HIWORD(wParam))	//HIWORD(wParam) -priznak minimalizovaneho okna
			{active=TRUE;}			//okno nie je minimalizovane
			else active=FALSE;		//okno     je minimalizovane
			return 0;
		}
		case WM_SYSCOMMAND:{  switch (wParam){  case SC_SCREENSAVE: case SC_MONITORPOWER: return 0;}break;}
		case WM_CLOSE:{PostQuitMessage(0);return 0;}	//generuje WM_QUIT a wParam=0
		case WM_KEYDOWN:{keys[wParam] = 1;return 0;}	//stlacenie klavesy
		case WM_KEYUP:	{keys[wParam] = 0;return 0;}	//pustenie klavesy
		case WM_LBUTTONDOWN:{keys[VK_LBUTTON] = TRUE;return 0;}
		case WM_LBUTTONUP:{keys[VK_LBUTTON] = FALSE;return 0;}
		case WM_RBUTTONDOWN:{keys[VK_RBUTTON] = TRUE;return 0;}
		case WM_RBUTTONUP:{keys[VK_RBUTTON] = FALSE;return 0;}
		case WM_SIZE:	{if(scene_!=NULL)scene_->ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));return 0;}  //zmena velkosti, funkcia je definovana v tomto subore
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

//////////////////////////////////////////////////////////////////////
/**********VSTUPNA FUNCKIA*****************************************************/
int WINAPI WinMain(HINSTANCE hInstance_,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{	
	MSG		msg;		//mesto pre strukturu MESSAGE - pre spravy
	BOOL	koniec=0;
	hInstance=hInstance_;

	if( Nastav_rozlisenia())
	if( ( english ? 
		MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION) :
		MessageBox(NULL,"Cela obrazovka?", "Nastavenie obrazovky (pocas behu pomocou F1)",MB_YESNO|MB_ICONQUESTION)
			) ==IDNO )
	{	fullscreen=FALSE;}
	// vytvarame okno a incializujeme scenu
	if (!CreateGLWindow(MenoApplikacie,screen_x,screen_y,screen_bit, fullscreen))return 0;
	
	while(!koniec)		//pokial nie je koniec
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	 /*ak nie je pristupna sprave vrati 0
		ak je prisupna sparava vrati nenulu a zapise ju do struktury msg,
		NULL - HWND spracuva spravy z aktualneho vlakna, PM_REMOVE - presuva spravu z radu sprav
		na rozdiel od GetMessage, PeekMessage necaka na pijatie dalsiej sprave pred vratenim (hodnoty)*/
		{				//sprava je pristupna - treba ju spracovat
			if (msg.message==WM_QUIT)koniec=1;
			else
			{	
				TranslateMessage(&msg);	//preklada virtualne spravy (VK_*) na znakove spravy (WM_CHAR)
				DispatchMessage(&msg);	//posiela spravy aplikacie prislusnej funkcie (WndProc)na spracovanie
			}							//- "" -	  vracia to co vracia WndProc
		}
		else	//ziadna sprava - cinnost ktoru ma vykonavat mimo spracovavania sprav
		{
			if (active) scene_->DrawGLScene();	//ak je program aktivny vykresluj
			if (keys[VK_ESCAPE]) koniec=1;		//ak je stlaceny Esc ukonc
			else SwapBuffers(hDC);		//prehodi buffer
			if (keys[VK_F1])			//cez F1 prepinanie z Fullscreen do okna
			{	
				keys[VK_F1]=FALSE;
				if(scene_!=NULL)delete scene_;
				KillGLWindow();
				fullscreen=!fullscreen;
				if (!CreateGLWindow(MenoApplikacie,screen_x,screen_y,screen_bit,fullscreen))return 0;
			}
		}
	}

	if(scene_!=NULL)delete scene_;
	KillGLWindow();
	return (msg.wParam);		//funkcia WinMain ma vratit hodnotu, ktora bola pouzita z funkciou PostQuitMessage
}

int GetVariable(char *temp, FILE *sub, char* varname, char* varvalue, int size_strings)
{
	if(fgets(temp, size_strings, sub) == NULL)
	{
		temp[0] = NULL;
		return 0;
	}
	varname[0]=NULL;
	varvalue[0]=NULL;
	char rovnasa=0;
	for(int i=0,j=0,k=0; temp[i]!=NULL&&temp[i]!='\n'&&temp[i]!='/'; i++)		// prejdeme vsetky znaky
	{
		if(temp[i]==' ' || temp[i]=='\t')continue;
		if(temp[i]=='='){ rovnasa=1;continue;}
		if(rovnasa)
		{
			varvalue[k]=temp[i]; k++;
		}
		else
		{
			varname[j]=temp[i]; j++;
		}
	}
	varname[j]=NULL;
	varvalue[k]=NULL;
	return 1;
}

int Nastav_rozlisenia()
{
	FILE* sub=NULL;
	char temp[100];
	char varname[100],varvalue[100];
	int ret=0;

	sub = fopen( "data/init.txt", "rb");
	if(sub == NULL)return 1;

	while(GetVariable( temp, sub, varname, varvalue, 100))
	{
		float fl=(float)atof(varvalue);
		int in=atoi(varvalue);
		strlwr(varname);				// Convert a string to lowercase
		if(!strcmp("width",varname))screen_x=in;
		else if(!strcmp("height",varname))screen_y=in;
		else if(!strcmp("colorbit",varname))screen_bit=in;
		else if(!strcmp("fullscreen",varname))
		{
			switch(varvalue[0])
			{
				case 'A':	case 'a':	case 'Y':	case 'y':	fullscreen = 1;	break;
				case 'N':	case 'n':	fullscreen = 0;	break;
				case 'M':	case 'm':	ret=1; break;
			}
		}
		else if(!strcmp("language",varname))
		{
			if(in==0)english=1;
			else if(in==1)english=0;
		}
	}
	fclose(sub);
	if(screen_x==0)screen_x=800;
	if(screen_y==0)screen_y=600;
	if(screen_bit==0)screen_bit=16;
	return ret;
}