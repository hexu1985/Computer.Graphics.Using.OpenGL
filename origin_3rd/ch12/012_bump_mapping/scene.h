//////////////////////////////////////////////////////////////////////
// Naprogramoval: Marek Mizanin, mizanin@szm.sk, mizanin@stonline.sk
//                www.mizanin.szm.sk, ICQ: 158283635
//
//
//////////////////////////////////////////////////////////////////////
//	Pouzivane klavesy:
//	
//	H - help
//
//////////////////////////////////////////////////////////////////////
//
// scene.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCENE_H
#define _SCENE_H

#include "init.h"				// zakladne nastavenia a zakladne hlavickove subory
								// vazby na init.cpp

#include <stdlib.h>				// standardna kniznica
#include <stdarg.h>				// --
#include <stdio.h>				// --
#include <math.h>				// matematicka kniznica

#include "Timer.h"				// trieda Timer pre pracu z casovacom
#include "bitmap_Font.h"		// trieda pre vypisovanie textu pomocou bitmapoveho fontu
#include "MultiTexture.h"		// trieda obsahujuca incializaciu multitexturingu
#include "camera.h"				// tireda pre transformacie
#include "camera_bludisko.h"
#include "mars.h"
#include "Util.h"


#ifndef PI180
#define PI180	0.0174532925199432957692369076848861f	// pi / 180
#endif

extern int		chyba;
#define pocet_base_textur 6
#define pocet_bump_textur 8

class scene  : public MultiTexture
{
	friend class mars;
	mars	*mars_;
	Font	*font0;
	Timer	timer0;
	camera_bludisko	*c;
	GLuint	base[pocet_base_textur];			// zakladne textury
	GLuint	bump_shift[pocet_bump_textur],bump_inv[pocet_bump_textur],bump[pocet_bump_textur];		// bump mapping textury
	GLuint	t_svetlo,lightmap,strela_svetlo,strela_dopad;	// textury
	
	float	fps;
	uFPS_counter fps_counter;
	float	bump_x,bump_y,bump_angle;
public:	static float max_emboss;
private:
	int		tex_base,tex_bump,nastav_tex;		// nastavenie, ktore textury sa budu kreslit
	int		obrazovka;
	char	text[30];					// text pre konverziu z float na text
	bool	svetlo,texturovanie,ciary,anim,multi,emboss,bump_,bump_delene,use_lightmap;
	
	POINT MouseBod;
	int mouse_x,mouse_y,sirka,vyska;

	AUX_RGBImageRec *LoadBMP(char *Filename);			// Naèítanie bitmapového obrázka
	int LoadGLTexture(char *texture_name,GLuint *texture);	// nacita obrazok z menom texture_name do OpenGL s cislom texture
	int LoadGLTexture_JPG(char *texture_name,GLuint *texture);	// nacita obrazok z menom texture_name do OpenGL s cislom texture
	void bump_mapping_smerove_svetlo(void);
	void vysvetlenie_2pass(void);
	void vysvetlenie_3pass(void);
	bool doMesh1TexelUnits(void);
	void svetlo_v_suradniciach_objektu(void);
	inline void Stvorec_lightmap(void);
	void bludisko_stena(float *p, float *s, float *t, float *n, float *l,int textura);
	void bludisko(void);
	void kresli_mars(void);
public:
	int LoadTexture_bump(char *texture_name,GLuint *shift, GLuint *inv);
	int LoadTexture_JPG_bump(char *texture_name,GLuint *shift, GLuint *inv);
	void ReSizeGLScene( int width, int height);
	void ReSizeGLScene_2D(int width, int height);	// funkcia pro zmenu velkosti okna
	void ReSizeGLScene_3D(int width, int height);	// funkcia pro zmenu velkosti okna
	void DrawGLScene(void);
	scene();
	~scene();
private:
	void KillGL(void);
	void InitGL(void);
	void Prepni_do_2D(void);
	void Prepni_do_3D(void);
	void Kurzor(void);			// zisti poziciu mysi a zobrazy kurzor
};

#define velkost_kurzoru 5

#endif
