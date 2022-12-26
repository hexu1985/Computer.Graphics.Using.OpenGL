//////////////////////////////////////////////////////////////////////
// scene.cpp: implementation of the scene class.
//
//////////////////////////////////////////////////////////////////////

#include "scene.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

scene::scene()
{
	int i;
	//////////////////////////////////////
	// smerniky na instancie na triedy nastavim na NULL, aby som ich potom nemazal
	font0 = NULL;
	c = NULL;
	mars_ = NULL;
	//////////////////////////////////////
	// textury
	for(i=0;i<pocet_base_textur;i++) base[i] = 0;
	for(i=0;i<pocet_bump_textur;i++)
	{
		bump_shift[i] = 0;
		bump_inv[i] = 0;
		bump[i] = 0;				// pre ukazku
	}
	
	t_svetlo=0; lightmap=0;strela_svetlo=0;strela_dopad=0;
	//////////////////////////////////////
	// prepinace
	svetlo=0; texturovanie=1; ciary=0; anim=1; multi=0; emboss=0; bump_delene = 1; use_lightmap=0; bump_=1;

	fps = 200;							// v pripade ze nieco zavisy od fps
	bump_x = 1.0f;
	bump_y = 1.0f;
	bump_angle = 0;
	max_emboss = 0.008f;
	tex_base = 0; tex_bump = 0;			// cisla textur, ktore sa budu kreslit
	nastav_tex = 0;
	obrazovka = 3;
	//////////////////////////////////////
	// nacitanie textur
	i = 0;
	if( !LoadGLTexture_JPG("data/nehe22_base.JPG",&base[i]) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/nehe22_bump.JPG",&bump[i]) ) {error = 1;return;}
	if( !LoadTexture_JPG_bump("data/nehe22_bump.JPG",&bump_shift[i],&bump_inv[i]) ) {error = 1;return;}

	i++;
	if( !LoadGLTexture_JPG("data/3dmark_base.JPG",&base[i]) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/3dmark_bump.JPG",&bump[i]) ) {error = 1;return;}
	if( !LoadTexture_JPG_bump("data/3dmark_bump.JPG",&bump_shift[i],&bump_inv[i]) ) {error = 1;return;}

	i++;
	if( !LoadGLTexture_JPG("data/stena_base.JPG",&base[i]) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/stena_bump.JPG",&bump[i]) ) {error = 1;return;}
	if( !LoadTexture_JPG_bump("data/stena_bump.JPG",&bump_shift[i],&bump_inv[i]) ) {error = 1;return;}

	i++;
	if( !LoadGLTexture_JPG("data/strop_base.JPG",&base[i]) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/strop_bump.JPG",&bump[i]) ) {error = 1;return;}
	if( !LoadTexture_JPG_bump("data/strop_bump.JPG",&bump_shift[i],&bump_inv[i]) ) {error = 1;return;}

	i++;
	if( !LoadGLTexture_JPG("data/podlaha_base.JPG",&base[i]) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/podlaha_bump.JPG",&bump[i]) ) {error = 1;return;}
	if( !LoadTexture_JPG_bump("data/podlaha_bump.JPG",&bump_shift[i],&bump_inv[i]) ) {error = 1;return;}


	i++;
	if( !LoadGLTexture_JPG("data/white.JPG",&base[i]) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/black.JPG",&bump[i]) ) {error = 1;return;}
	if( !LoadTexture_JPG_bump("data/black.JPG",&bump_shift[i],&bump_inv[i]) ) {error = 1;return;}

	i++;
	if( !LoadGLTexture_JPG("data/my_bump.JPG",&bump[i]) ) {error = 1;return;}
	if( !LoadTexture_JPG_bump("data/my_bump.JPG",&bump_shift[i],&bump_inv[i]) ) {error = 1;return;}

	i++;
	if( !LoadGLTexture_JPG("data/my_bump_.JPG",&bump[i]) ) {error = 1;return;}
	if( !LoadTexture_JPG_bump("data/my_bump_.JPG",&bump_shift[i],&bump_inv[i]) ) {error = 1;return;}
	
	if( !LoadGLTexture_JPG("data/svetlo.JPG",&t_svetlo) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/lightmap.JPG",&lightmap) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/strela_svetlo.JPG",&strela_svetlo) ) {error = 1;return;}
	if( !LoadGLTexture_JPG("data/strela_dopad.JPG",&strela_dopad) ) {error = 1;return;}

	//////////////////////////////////////
	// vytvarame instancie pouzivanych tried
	font0 = new Font("data/font.bmp",&error);		// vytvorenie fontu
	c = new camera_bludisko;
	if(error)return;

	InitGL();									// nastavenie z-buffra,svetla,...
	//ShowCursor(0);
}

scene::~scene()
{
	int i;
	//////////////////////////////////////
	// mazeme instancie tried
	if(font0!=NULL)delete font0;
	if(mars_!=NULL)delete mars_;
	if(c!=NULL)delete c;
	//////////////////////////////////////
	// mazeme textury
	for(i=0;i<pocet_base_textur;i++) glDeleteTextures( 1, &base[i]);
	for(i=0;i<pocet_bump_textur;i++)
	{
		glDeleteTextures( 1, &bump_shift[i]);
		glDeleteTextures( 1, &bump_inv[i]);
		glDeleteTextures( 1, &bump[i]);		// pre ukazku
	}
	glDeleteTextures( 1, &t_svetlo);
	glDeleteTextures( 1, &lightmap);
	glDeleteTextures( 1, &strela_svetlo);
	glDeleteTextures( 1, &strela_dopad);
	KillGL();
	ShowCursor(1);
}

void scene::InitGL(void)
{
	if(!fullscreen)
	{
		RECT		WindowRect;					// miesto pre velkost okna
		WindowRect.left=(long)0;
		WindowRect.top=(long)0;
		GetClientRect(hWnd,&WindowRect);				//zistenie rozmerov uzivatelskej casti okna
		ReSizeGLScene(WindowRect.right,WindowRect.bottom);	//nastavenie perspektivy
	}
	else ReSizeGLScene(screen_x, screen_y);	//definovana v tomto subore

	glEnable(GL_TEXTURE_2D);		// Povolenie zobrazovanie textúr
	glShadeModel(GL_SMOOTH);		/*nastavuje tienovanie
	GL_SMOOTH - objekty su tienovane t.j. prechody medzi farbami bodov su plinule (gouradovo (s)tienovanie)
	GL_FLAT   - objekty nie su tienovane ziadne plinule prechody */

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //specifikuje hodnoty na cistenie pomocou glClear par.: red, green, blue, alpha (cierny podklad)
	glClearDepth(1.0f);  //specifikuje hodnoty na cistenie pomocou glClear par.: depth (hlbka)
	
	/* ZAPNUTIE HLBKOVEHO TESTU */
	glEnable(GL_DEPTH_TEST);		// zapne håbkový test
	glDepthFunc(GL_LEQUAL);			// typ håbkového testu
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// “reálnejšie” perspektívne výpoèty

	GLfloat LightAmbient[]	=	{ 0.3f, 0.3f, 0.3f, 1.0f };		//svetlo ktore osvetluje zo vsetkych stran rovnako (okolite svetlo)
	GLfloat LightDiffuse[]	=	{ 1.0f, 1.0f, 1.0f, 1.0f };		//smerove, alebo bodove svetlo - svetlo svieti z pozicie zadanej pomocou premennej LightPosition
	GLfloat LightPosition[4]=	{ 1.0f, 1.0f, 0.0f, 0.0f };		//smer difuzneho svetla - ak je 4 zlozka 0 - smerove svetlo, ak nie je 0 potom s jedna o bodove svetlo so suradnicami x,y,z
	
	glLightModelfv ( GL_LIGHT_MODEL_AMBIENT, LightAmbient );	// nastavenie okoliteho (Ambient) svetla
	glLightfv ( GL_LIGHT0, GL_DIFFUSE, LightDiffuse );			// nastavenie difuzneho svetla
	glLightfv ( GL_LIGHT0, GL_POSITION, LightPosition  );		// pozicia difuzneho svetla
	glEnable  ( GL_LIGHT0 );									// aktivovanie svetla 0
	
//	GLfloat mat_diffuse[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
//	GLfloat mat_specular[]	= { 0.9f, 0.9f, 0.9f, 1.0f };
//	GLfloat mat_shininess[] = { 128.0f };

//	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE , mat_diffuse);
//	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glColor3f(1.0f,1.0f,1.0f);				// biela farba	
}

void scene::KillGL()
{
}


AUX_RGBImageRec* scene::LoadBMP(char *Filename)			// Naèítanie bitmapového obrázka
{
	FILE *File=NULL;								// smernik na subor

	if (!Filename)return NULL;						// skúška èi meno súboru bolo zadané
	File=fopen(Filename,"r");						// otvorenie súboru na èítanie
	if (File)										// existuje súbor ?
	{
		fclose(File);								// Uzatvorenie súbora
		return auxDIBImageLoad(Filename);			// Naèíta obrázok a vráti pointer
	}
	return NULL;									// Ak sa obrázok nenaèítal vráti NULL
}

int scene::LoadGLTexture(char *texture_name,GLuint *texture)				// Naèíta obrázok a uloži ako textury
{
	int Status=TRUE;								// indikator
	AUX_RGBImageRec *TextureImage;					// miesto pre štruktúru

	TextureImage=NULL;
	if (TextureImage=LoadBMP(texture_name))
	{
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );		// nastavuje za textury sa v smere u (vodorovnom) neopakuju
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );		// nastavuje za textury sa v smere v (zvislom) neopakuju
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
	
	}
	else 
	{	MessageBox(NULL,"Chyba naèítania obrázka.",texture_name,MB_ICONERROR);Status=false;}

	if (TextureImage)									// Ak textura existuje
	{
		if (TextureImage->data)free(TextureImage->data);// ak existuju data uvo¾ni z pamäte
		free(TextureImage);								// uvo¾ni štruktúru
	}
	return Status;
}

void scene::Kurzor(void)			// zisti poziciu mysi a zobrazy kurzor
{
/*	GetCursorPos(&MouseBod);		//nacitanie pozicie mysi
	glBegin(GL_LINES);
		glVertex2i(MouseBod.x+velkost_kurzoru+1,screen_y-1-MouseBod.y);
		glVertex2i(MouseBod.x-velkost_kurzoru+1,screen_y-1-MouseBod.y);
		glVertex2i(MouseBod.x+1,screen_y-1-MouseBod.y+velkost_kurzoru);
		glVertex2i(MouseBod.x+1,screen_y-1-MouseBod.y-velkost_kurzoru);
	glEnd();
*/
	glBegin(GL_LINES);
		glVertex2i(mouse_x+velkost_kurzoru+1,mouse_y);
		glVertex2i(mouse_x-velkost_kurzoru+1,mouse_y);
		glVertex2i(mouse_x+1,mouse_y+velkost_kurzoru);
		glVertex2i(mouse_x+1,mouse_y-velkost_kurzoru);
	glEnd();
}

void scene::ReSizeGLScene(int width, int height)
{
	sirka = width;
	vyska = height;
	ReSizeGLScene_2D(width, height);
}

void scene::ReSizeGLScene_3D(int width, int height)		// funkcia pre zmenu velkosti okna
{										// vola sa v pripade zmeny velkosti okna, zmenu z(na) fullscreen
	if (height==0) height=1;
	glViewport(0,0,width,height);		// Viewport (pohlad)
	glMatrixMode(GL_PROJECTION);		// projekcia
	glLoadIdentity();					// Reset pohladu - nacita jednotkovu maticu
	gluPerspective(uhol_kamery,(GLfloat)width/(GLfloat)height,blizka_orezavacia_rovina,vzdialena_orezavacia_rovina);
	/* gluPerspective - nastavuje perspektivu kamery, ako bude vypadat priestor z pohladu kamery.
	1. parameter urcuje zorný uhol kamery. 2.param. - gldAspect je pomer výšky a šírky zobrazovanej plochy.
	3. par. udava vzdialenost od ktorej je vidiet vykreslovane objekty
	4. par. udava vzdialenost do ktorej je vidiet vykreslovane objekty */
	glMatrixMode(GL_MODELVIEW);			// Modelview
	glLoadIdentity();					// Reset The Modelview Matrix
}

void scene::ReSizeGLScene_2D(int width, int height)		// funkcia pre zmenu velkosti okna
{
	if (height==0) height=1;
	glViewport(0,0,width,height);		// Viewport (pohlad)
	glMatrixMode(GL_PROJECTION);		// projekcia
	glLoadIdentity();					// Reset pohladu - nacita jednotkovu maticu
//	glOrtho(0,screen_x,0,screen_y,-1,1);	// nastavuje standardne pravouhle premietanie
	glOrtho(0,screen_x_font,0,screen_y_font,-1,1);	// nastavuje standardne pravouhle premietanie
	glMatrixMode(GL_MODELVIEW);			// Modelview
	glLoadIdentity();					// Reset The Modelview Matrix
}

void scene::Prepni_do_3D(void)			// rychle prepnutie do 3D
{
	glEnable(GL_DEPTH_TEST );

	glMatrixMode(GL_PROJECTION);		// projekcia
	glLoadIdentity();					// Reset pohladu - nacita jednotkovu maticu
	gluPerspective(uhol_kamery,(float)sirka/(float)vyska,blizka_orezavacia_rovina,vzdialena_orezavacia_rovina);
	/* gluPerspective - nastavuje perspektivu kamery, ako bude vypadat priestor z pohladu kamery.
	1. parameter urcuje zorný uhol kamery. 2.param. - gldAspect je pomer výšky a šírky zobrazovanej plochy.
	3. par. udava vzdialenost od ktorej je vidiet vykreslovane objekty
	4. par. udava vzdialenost do ktorej je vidiet vykreslovane objekty */
	glMatrixMode(GL_MODELVIEW);			// Modelview
	glLoadIdentity();
}

void scene::Prepni_do_2D(void)			// rychle prepnutie do 2D
{
	if(svetlo)glDisable(GL_LIGHTING);					// vypne svetlo
	if(ciary)glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);	// vypne ciarovy rezim
	if(!texturovanie)glEnable(GL_TEXTURE_2D);			// zapne texturovanie
	glDisable(GL_DEPTH_TEST );

	glMatrixMode(GL_PROJECTION);		// projekcia
	glLoadIdentity();					// Reset pohladu - nacita jednotkovu maticu
//	glOrtho(0,screen_x,0,screen_y,-1,1);// nastavuje standardne pravouhle premietanie
	glOrtho(0,screen_x_font,0,screen_y_font,-1,1);	// nastavuje standardne pravouhle premietanie
	glMatrixMode(GL_MODELVIEW);			// Modelview
	glLoadIdentity();					// Reset The Modelview Matrix
}

int scene::LoadTexture_bump(char *texture_name, GLuint *shift, GLuint *inv)
{
	int Status=TRUE;								// indikator
	AUX_RGBImageRec *TextureImage;					// miesto pre štruktúru


	TextureImage=NULL;
	if (TextureImage=LoadBMP(texture_name))
	{
		int i;
		unsigned char *t = new unsigned char[TextureImage->sizeX*TextureImage->sizeY*3];

		for( i=0;i<3*TextureImage->sizeX*TextureImage->sizeY;)
		{
			t[i] = TextureImage->data[i];		// red
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
			t[i] = TextureImage->data[i];		// green
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
			t[i] = TextureImage->data[i];		// blue
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
		}

		glGenTextures(1, shift);
		glBindTexture(GL_TEXTURE_2D, *shift);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );		// nastavuje za textury sa v smere u (vodorovnom) neopakuju
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );		// nastavuje za textury sa v smere v (zvislom) neopakuju
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, t);

		for( i=0;i<3*TextureImage->sizeX*TextureImage->sizeY;)
		{
			t[i] = TextureImage->data[i];		// red
			t[i] = ~t[i];				// invertovanie
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
			t[i] = TextureImage->data[i];		// green
			t[i] = ~t[i];				// invertovanie
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
			t[i] = TextureImage->data[i];		// blue
			t[i] = ~t[i];				// invertovanie
			t[i] = t[i] >> 1;					// bitovy posun do prava o 1
			i++;
		}

		glGenTextures(1, inv);
		glBindTexture(GL_TEXTURE_2D, *inv);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );		// nastavuje za textury sa v smere u (vodorovnom) neopakuju
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );		// nastavuje za textury sa v smere v (zvislom) neopakuju
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8/*3*/, TextureImage->sizeX, TextureImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, t);
		delete []t;	
	}
	else 
	{	MessageBox(NULL,"Chyba naèítania obrázka.",texture_name,MB_ICONERROR);Status=false;}

	if (TextureImage)									// Ak textura existuje
	{
		if (TextureImage->data)free(TextureImage->data);// ak existuju data uvo¾ni z pamäte
		free(TextureImage);								// uvo¾ni štruktúru
	}
	return Status;
}


void scene::kresli_mars(void)
{
	if(keys['P'])
	{
		ciary = !ciary;
		keys['P']=0;
	}
	
	if(keys['E'])
	{
		emboss = !emboss;
		keys['E']=0;
	}

	if(keys['B'])
	{
		bump_ = !bump_;
		keys['B']=0;
	}
	if(keys['A']){	anim = !anim; keys['A'] = 0; }
	if(ciary)glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else	 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	if(mars_==NULL)mars_ = new mars(this);
	if(error == 1)
	{
		keys[VK_ESCAPE] = 1;
		return;
	}
	Prepni_do_3D();

	mars_->Kresli();

	Prepni_do_2D();
	font0->Begin();
	fps = 1.0f / timer0.GetTime();
	timer0.Reset();
	fps_counter.GetAvgFps(1.f/fps,0.2f);
	gcvt( (double)fps_counter.avg_fps, 4, text);
	font0->Print(0,0,text,0);				// vypisanie textu na 0 stlpec a 0 riadok

//	glColor3f(1,0,0);
//	if(multitextureSupported&&multi)font0->Print(0,1,"Multitexturing",0);
//	glColor3f(1,1,1);

	if(keys['H'])							// zobrazenie helpu
	{
		int i=3;
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0,0,0,0.3f);
		glLoadIdentity();
		glBegin(GL_QUADS);
			glVertex2i(55,295);
			glVertex2i(800,295);
			glVertex2i(800,555);
			glVertex2i(55,555);
		glEnd();
		glColor4f(1,1,1,1);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_TEXTURE_2D);

		if(english)
		{
			font0->Print(10,i++,"Program: Bump-mapping - planet mars",0);
			glColor3f(0.9f,0.9f,1.0f);
			font0->Print(5,i++,"Programmer: Marek Mizanin - www.mizanin.szm.sk",0);
			glColor3f(1.0f,1.0f,1.0f);
			font0->Print(5,i++,"Controls:",0);
			font0->Print(10,i++,"1 - switch to explanation how work emboss bump-mapping",1);
			font0->Print(10,i++,"2 - switch to example bump-mapping with point light",1);
			font0->Print(10,i++,"3 - switch to labyrinth with bump-mapping",1);
			font0->Print(10,i++,"4 - planet with bump-mapping",1);
			i++;
		//	font0->Print(10,i++,"  - pre kazde cislo su odlisne klavesi (odlisne helpi)",1);
			font0->Print(10,i++,"E - on/off emboss, don't show base texture",1);
			font0->Print(10,i++,"B - off/on bump-mapping",1);
		//	font0->Print(10,i++,"M - on/off multitexturing",1);
			font0->Print(10,i++,"P - on/off draw lines",1);
			font0->Print(10,i++,"L - on/off lighting",1);
			font0->Print(10,i++,"S - switch between locally and rotate light",1);
			font0->Print(10,i++,"A - on/off rotating light",1);
		}
		else
		{
			font0->Print(10,i++,"Program: Bump-mapping - planeta mars",0);
			glColor3f(0.9f,0.9f,1.0f);
			font0->Print(5,i++,"Naprogramoval: Marek Mizanin - www.mizanin.szm.sk",0);
			glColor3f(1.0f,1.0f,1.0f);
			font0->Print(5,i++,"Pouzivane klavesy:",0);
			font0->Print(10,i++,"1 - prepne na vysvetlenie ako funguje emboss bump-mapping",1);
			font0->Print(10,i++,"2 - prepne na ukazku bump-mappingu so bodovym svetlom",1);
			font0->Print(10,i++,"3 - prepne bludisko s bump-mappingom",1);
			font0->Print(10,i++,"4 - planeta s bump-mappingom",1);
			font0->Print(10,i++,"  - pre kazde cislo su odlisne klavesi (odlisne helpi)",1);
			font0->Print(10,i++,"E - zapne/vypne emboss,t.j. nezobraziuje zakladnu texturu",1);
			font0->Print(10,i++,"B - vypne/zapne bump-mapping",1);
		//	font0->Print(10,i++,"M - zapne/vypne multitexturing",1);
			font0->Print(10,i++,"P - zapne/vypne zobrazovanie ciar",1);
			font0->Print(10,i++,"L - zapne/vypne svetlo",1);
			font0->Print(10,i++,"S - prepina medzi lokalnym a rotujucim svetlom",1);
			font0->Print(10,i++,"A - zapne/vypne rotaciu svetla",1);
		}

		font0->Print(10,i++,"H - help",1);
	}
	font0->End();
}
