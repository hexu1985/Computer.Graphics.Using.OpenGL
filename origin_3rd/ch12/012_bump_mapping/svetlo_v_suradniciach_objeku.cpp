#include "scene.h"

static GLfloat LightPosition[] = { 0.0f, 0.0f, 128.0f, 1.0f};
float scene::max_emboss;

static void Nastav_svetlo(void)		// volat az po rotacii
{
	glLightfv( GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
}

static inline void Stvorec30(void)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex2i(  0,  0);
		glTexCoord2f(1,0);glVertex2i( 30,  0);
		glTexCoord2f(1,1);glVertex2i( 30, 30);
		glTexCoord2f(0,1);glVertex2i(  0, 30);
	glEnd();
}

static inline void Stvorec100(void)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);glVertex2i(  0,  0);
		glTexCoord2f(1,0);glVertex2i(100,  0);
		glTexCoord2f(1,1);glVertex2i(100,100);
		glTexCoord2f(0,1);glVertex2i(  0,100);
	glEnd();
}

static inline void Stvorec(void)
{
	glBegin(GL_QUADS);
		glNormal3f(0,0,1);
		glTexCoord2f(0,0);glVertex2i(   0,  0);
		glTexCoord2f(1,0);glVertex2i( 512,  0);
		glTexCoord2f(1,1);glVertex2i( 512,512);
		glTexCoord2f(0,1);glVertex2i(   0,512);
	glEnd();
}

inline void scene::Stvorec_lightmap(void)
{	
	float x,y;
	x = 0.5f-LightPosition[0]/1024.0f;
	y = 0.5f-LightPosition[1]/1024.0f;
	
	glDisable(GL_LIGHTING );
	// TEXTURE-UNIT #0		
	glActiveTextureARB(GL_TEXTURE0_ARB);			// aktivujeme 0 jednotku
	glEnable(GL_TEXTURE_2D);						// povolime texturovanie
	glBindTexture(GL_TEXTURE_2D,base[tex_base] );	// nastavime texturu
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// nastavime rezim na masobenie

	// TEXTURE-UNIT #1:
	glActiveTextureARB(GL_TEXTURE1_ARB);			// aktivuje 1 jednotku
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,lightmap );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// nastavime nasobenie

	glBegin(GL_QUADS);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0);	
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,x, y);
		glVertex3f(0,0,0);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1, 0);	
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,x+0.5f,y);
		glVertex3f(512,0,0);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1, 1);	
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,x+0.5f,y+0.5f);
		glVertex3f(512,512,0);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 1);	
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,x,y+0.5f);
		glVertex3f(0,512,0);
	glEnd();

	//FINISH MULTITEXTURING
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);						// vypneme 1 jednotku
	glActiveTextureARB(GL_TEXTURE0_ARB);
	//FINISH MULTITEXTURING
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE);
}

#define bump_delenie 20
static inline void Stvorec_d(void)
{	
	float x,y,krok;

	krok = 1.0f/(float)bump_delenie;
	glNormal3f(0,0,1);

	for(x = 0;x<1.0f;x+=krok)
	{
		for(y = 0;y<1.0f;y+=krok)
		{
			glBegin(GL_QUADS);
				glTexCoord2f(x, y);
				glVertex3f(x*512.0f, y*512.0f,0.f );
				
				glTexCoord2f((x+krok), y);	
				glVertex3f((x+krok)*512.0f, y*512.0f,0.f );

				glTexCoord2f((x+krok), (y+krok));	
				glVertex3f((x+krok)*512.0f, (y+krok)*512.0f,0.f );

				glTexCoord2f(x, (y+krok));	
				glVertex3f(x*512.0f, (y+krok)*512.0f,0.f );
			glEnd();
		}
	}
}

static void SetUpBumps( float* pos, float *c, float *l, float *s, float *t)
{
	float v[3];					//	vektor z aktualnej pozicie do bodu kde je svetlo
	float lenQ;
		
	v[0]=l[0]-c[0];
	v[1]=l[1]-c[1];
	v[2]=l[2]-c[2];
	lenQ = (float) sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	v[0]/=lenQ;		v[1]/=lenQ;		v[2]/=lenQ;
	// vypocitame posunutia v smere u a v pre texturu
	pos[0]=(s[0]*v[0]+s[1]*v[1]+s[2]*v[2])*scene::max_emboss;
	pos[1]=(t[0]*v[0]+t[1]*v[1]+t[2]*v[2])*scene::max_emboss;	
}

static inline void Stvorec_(void)
{
	float c[3],l[3],s[3],t[3],pos[2];

	l[0] = LightPosition[0];
	l[1] = LightPosition[1];
	l[2] = LightPosition[2];

	s[0] = 1; s[1] = 0; s[2] = 0;
	t[0] = 0; t[1] = 1; t[2] = 0;

	glBegin(GL_QUADS);
	//	glNormal3f( 0.0f, 0.0f, +1.0f);

		c[0] =   0; c[1] =   0; c[2] = 0;
		SetUpBumps(pos,c,l,s,t);
		glTexCoord2f(pos[0]+0,pos[1]+0); glVertex3fv(c);

		c[0] = 512; c[1] =   0; c[2] = 0;
		SetUpBumps(pos,c,l,s,t);
		glTexCoord2f(pos[0]+1,pos[1]+0); glVertex3fv(c);
		
		c[0] = 512; c[1] = 512; c[2] = 0;
		SetUpBumps(pos,c,l,s,t);
		glTexCoord2f(pos[0]+1,pos[1]+1); glVertex3fv(c);
		
		c[0] =   0; c[1] = 512; c[2] = 0;
		SetUpBumps(pos,c,l,s,t);
		glTexCoord2f(pos[0]+0,pos[1]+1); glVertex3fv(c);
	glEnd();
}

static inline void Stvorec__d(void)
{
	float x,y,krok;
	float c[3],l[3],s[3],t[3],pos[2];

	krok = 1.0f/(float)bump_delenie;
	l[0] = LightPosition[0];
	l[1] = LightPosition[1];
	l[2] = LightPosition[2];

	s[0] = 1; s[1] = 0; s[2] = 0;
	t[0] = 0; t[1] = 1; t[2] = 0;

	for(x = 0;x<1.0f;x+=krok)
	{
		for(y = 0;y<1.0f;y+=krok)
		{
			glBegin(GL_QUADS);
				c[0] =   x*512.0f; c[1] =   y*512.0f; c[2] = 0;
				SetUpBumps(pos,c,l,s,t);
				glTexCoord2f(pos[0]+x, pos[1]+y);
				glVertex3fv(c);
				
				c[0] = (x+krok)*512.0f; c[1] =   y*512.0f; c[2] = 0;
				SetUpBumps(pos,c,l,s,t);
				glTexCoord2f(pos[0]+(x+krok), pos[1]+y);
				glVertex3fv(c);

				c[0] = (x+krok)*512.0f; c[1] = (y+krok)*512.0f; c[2] = 0;
				SetUpBumps(pos,c,l,s,t);
				glTexCoord2f(pos[0]+(x+krok), pos[1]+(y+krok));
				glVertex3fv(c);

				c[0] =   x*512.0f; c[1] = (y+krok)*512.0f; c[2] = 0;
				SetUpBumps(pos,c,l,s,t);
				glTexCoord2f(pos[0]+x, pos[1]+(y+krok));
				glVertex3fv(c);
			glEnd();
		}
	}
}

static inline void Stvorec_m(void)
{	
	float c[3],l[3],s[3],t[3],pos[2];

	l[0] = LightPosition[0];
	l[1] = LightPosition[1];
	l[2] = LightPosition[2];

	s[0] = 1; s[1] = 0; s[2] = 0;
	t[0] = 0; t[1] = 1; t[2] = 0;

	glBegin(GL_QUADS);
		c[0] =   0; c[1] =   0; c[2] = 0;
		SetUpBumps(pos,c,l,s,t);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 0);	
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pos[0]+0, pos[1]+0);
		glVertex3fv(c);
		
		c[0] = 512; c[1] =   0; c[2] = 0;
		SetUpBumps(pos,c,l,s,t);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1, 0);	
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pos[0]+1, pos[1]+0);
		glVertex3fv(c);

		c[0] = 512; c[1] = 512; c[2] = 0;
		SetUpBumps(pos,c,l,s,t);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,1, 1);	
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pos[0]+1, pos[1]+1);
		glVertex3fv(c);

		c[0] =   0; c[1] = 512; c[2] = 0;
		SetUpBumps(pos,c,l,s,t);
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB,0, 1);	
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pos[0]+0, pos[1]+1);
		glVertex3fv(c);
	glEnd();
}

static inline void Stvorec_m_d(void)
{	
	float x,y,krok;
	float c[3],l[3],s[3],t[3],pos[2];

	krok = 1.0f/(float)bump_delenie;
	l[0] = LightPosition[0];
	l[1] = LightPosition[1];
	l[2] = LightPosition[2];

	s[0] = 1; s[1] = 0; s[2] = 0;
	t[0] = 0; t[1] = 1; t[2] = 0;

	for(x = 0;x<1.0f;x+=krok)
	{
		for(y = 0;y<1.0f;y+=krok)
		{
			glBegin(GL_QUADS);
				c[0] =   x*512.0f; c[1] =   y*512.0f; c[2] = 0;
				SetUpBumps(pos,c,l,s,t);
				glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x, y);	
				glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pos[0]+x, pos[1]+y);
				glVertex3fv(c);
				
				c[0] = (x+krok)*512.0f; c[1] =   y*512.0f; c[2] = 0;
				SetUpBumps(pos,c,l,s,t);
				glMultiTexCoord2fARB(GL_TEXTURE0_ARB,(x+krok), y);	
				glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pos[0]+(x+krok), pos[1]+y);
				glVertex3fv(c);

				c[0] = (x+krok)*512.0f; c[1] = (y+krok)*512.0f; c[2] = 0;
				SetUpBumps(pos,c,l,s,t);
				glMultiTexCoord2fARB(GL_TEXTURE0_ARB,(x+krok), (y+krok));	
				glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pos[0]+(x+krok), pos[1]+(y+krok));
				glVertex3fv(c);

				c[0] =   x*512.0f; c[1] = (y+krok)*512.0f; c[2] = 0;
				SetUpBumps(pos,c,l,s,t);
				glMultiTexCoord2fARB(GL_TEXTURE0_ARB,x, (y+krok));	
				glMultiTexCoord2fARB(GL_TEXTURE1_ARB,pos[0]+x, pos[1]+(y+krok));
				glVertex3fv(c);
			glEnd();
		}
	}
}

#define krok 0.002f

void scene::svetlo_v_suradniciach_objektu(void)
{

	if(keys[VK_LEFT])max_emboss-=krok/fps;
	else if(keys[VK_RIGHT])max_emboss+=krok/fps;

	if(keys[VK_UP])max_emboss+=krok/fps;
	else if(keys[VK_DOWN])max_emboss-=krok/fps;

	if(keys['R'])max_emboss = 0.008f;

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
	if(keys['D'])
	{
		bump_delene = !bump_delene;
		keys['D']=0;
	}
	if(keys['U'])
	{
		use_lightmap = !use_lightmap;
		keys['U']=0;
	}

	if(ciary)glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else	 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	if(keys['A']){	anim = !anim; keys['A'] = 0; }
	if(anim)
	{
		bump_angle += 50.f/fps;
		if(bump_angle>360.0f)bump_angle-= 360.0f;
		mouse_x =(int) (250.f + 256.0f + 200.f * (float)sin(PI180*bump_angle));
		mouse_y =(int) (8.0f  + 256.0f + 200.f * (float)cos(PI180*bump_angle));
	}

	glLoadIdentity();
	glTranslatef(250,8,0);

	LightPosition[0] = (float)mouse_x-250;
	LightPosition[1] = (float)mouse_y-8;
	Nastav_svetlo();
	if(multi&&multitextureSupported)
	{
		// TEXTURE-UNIT #0		
		glActiveTextureARB(GL_TEXTURE0_ARB);			// aktivujeme 0 jednotku
		glEnable(GL_TEXTURE_2D);						// povolime texturovanie
		glBindTexture(GL_TEXTURE_2D,bump_shift[tex_bump] );	// nastavime texturu
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// nastavime rezim na masobenie

		// TEXTURE-UNIT #1:
		glActiveTextureARB(GL_TEXTURE1_ARB);			// aktivuje 1 jednotku
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,bump_inv[tex_bump] );
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);	// nastavime scitavanie
		if(bump_delene)Stvorec_m_d();
		else Stvorec_m();

		//FINISH MULTITEXTURING
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);						// vypneme 1 jednotku
		glActiveTextureARB(GL_TEXTURE0_ARB);
		//FINISH MULTITEXTURING
		glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE);

		// 2-pass, vynasobenie 2* so zakladnou texturov
		glBindTexture(GL_TEXTURE_2D,base[tex_base] );
		glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);		// nasobenie 2* (kreslene*nakreslene(GL_DST_COLOR))+(nakreslene*kreslene(GL_SRC_COLOR))=2*(kreslene*nakreslene)
		glEnable(GL_BLEND);
		if(svetlo)glEnable(GL_LIGHTING );
		if(!emboss)
		{
			if(svetlo && use_lightmap)Stvorec_lightmap();
			else if(svetlo && bump_delene)Stvorec_d();
			else Stvorec();
		}
		glDisable(GL_BLEND);
		// -- END ------------ BUMP MAPPING -----------------------------
	}
	else
	{
		// -------------- BUMP MAPPING -----------------------------
		// 1-pass, vykreslenie bump_shift textury
		glBindTexture(GL_TEXTURE_2D,bump_shift[tex_bump] );
		Stvorec();

		// 2-pass, vykreslenie pricitanim posunutej bump_inv testury
		glBindTexture(GL_TEXTURE_2D,bump_inv[tex_bump] );
		glBlendFunc(GL_ONE,GL_ONE);				// scitavanie
		glEnable(GL_BLEND);
		if(bump_delene)Stvorec__d();			// kreslime posunutu texturu
		else Stvorec_();
		glDisable(GL_BLEND);
		
		// 3-pass, vynasobenie 2* so zakladnou texturov
		glBindTexture(GL_TEXTURE_2D,base[tex_base] );
		glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);		// nasobenie 2* (kreslene*nakreslene(GL_DST_COLOR))+(nakreslene*kreslene(GL_SRC_COLOR))=2*(kreslene*nakreslene)
		glEnable(GL_BLEND);
		if(svetlo)glEnable(GL_LIGHTING );
		if(!emboss)
		{
			if(bump_delene)Stvorec_d();
			else Stvorec();
		}
		glDisable(GL_BLEND);
		// -- END ------------ BUMP MAPPING -----------------------------
	}
	
	glColor3f(1,1,1);
	glDisable(GL_LIGHTING );
	if(ciary)glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glBindTexture(GL_TEXTURE_2D,t_svetlo );		// svetlo
	glLoadIdentity();
	glTranslatef((float)mouse_x-15,(float)mouse_y-15,0);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_BLEND);
	Stvorec30();
	glDisable(GL_BLEND);
	
	// textury kreslene hore
	glBindTexture(GL_TEXTURE_2D,base[tex_base] );
	glLoadIdentity();	
	glTranslatef(10,350,0);
	Stvorec100();

	glBindTexture(GL_TEXTURE_2D,bump[tex_bump] );
	glLoadIdentity();	
	glTranslatef(130,350,0);
	Stvorec100();

	glColor4f(1,1,1,1);
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	if(nastav_tex)		// bump textura
	{
		glLoadIdentity();	
		glTranslatef(130,350,0);
		glBegin(GL_LINE_LOOP);
		glVertex2i( -5, -5);
		glVertex2i(105, -5);
		glVertex2i(105,105);
		glVertex2i( -5,105);
		glEnd();

	}
	else				// base textura
	{
		glLoadIdentity();	
		glTranslatef(10,350,0);
		glBegin(GL_LINE_LOOP);
		glVertex2i( -5, -5);
		glVertex2i(105, -5);
		glVertex2i(105,105);
		glVertex2i( -5,105);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);


	font0->Begin();
	fps = 1.0f / timer0.GetTime();
	timer0.Reset();
	fps_counter.GetAvgFps(1.f/fps,0.2f);
	gcvt( (double)fps_counter.avg_fps, 4, text);
	font0->Print(0,0,text,0);				// vypisanie textu na 0 stlpec a 0 riadok

	font0->Print(0,2,"Max emboss:",0);
	gcvt( (double)100.0*max_emboss, 8, text);
	font0->Print(12,2,text,0);
	font0->Print(23,2,"%",0);


	if(english)
		font0->Print_xy_scale(160,573,"Emboss Bump-mapping with point light",0,1.5f,1.5f);
	else
		font0->Print_xy_scale(160,573,"Emboss Bump-mapping s bodovym svetlom",0,1.5f,1.5f);
		
	glColor3f(1,0,0);
	if(multitextureSupported)
	{
		font0->Print(0,3,"Multitexturing",0);
		if(multi)font0->Print(0,4,english ? "using":"pouzivany",0);
		else font0->Print(0,4,english ? "supported - 'M'" :"podporovany - 'M'",0);
		if(multi && svetlo && use_lightmap)font0->Print(0,5,english ? "using light map" : "pouzivam light map",0);
	}
	else
	{
		font0->Print(0,3,"Multitexturing",0);
		font0->Print(0,4,english ? "unssupported :-(":"nepodporovany :-(",0);
	}
	glColor3f(1,1,1);

	if(keys['H'])							// zobrazenie helpu
	{
		int i=3;
		glDisable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0,0,0,0.6f);
		glLoadIdentity();
		glBegin(GL_QUADS);
			glVertex2i(55,240);
			glVertex2i(800,240);
			glVertex2i(800,555);
			glVertex2i(55,555);
		glEnd();
		glColor4f(1,1,1,1);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_TEXTURE_2D);

		if(english)
		{
			font0->Print(10,i++,"Program: Bump-mapping - point svetlo",0);
			glColor3f(0.9f,0.9f,1.0f);
			font0->Print(5,i++,"Programmer: Marek Mizanin - www.mizanin.szm.sk",0);
			glColor3f(1.0f,1.0f,1.0f);
			font0->Print( 5,i++,"Controls:",0);
			font0->Print(10,i++,"1 - switch to explanation how work emboss bump-mapping",1);
			font0->Print(10,i++,"2 - switch to example bump-mapping with point light",1);
			font0->Print(10,i++,"3 - switch to labyrinth with bump-mapping",1);
			font0->Print(10,i++,"4 - planet with bump-mapping",1);
			font0->Print(10,i++,"T - change how texture you are setting",1);
			font0->Print_scale(10,i++,"+ a - on numerical keyboard - change texture that is in frame",1,0.85f,1);
			font0->Print(10,i++,"E - on/off emboss",1);
			font0->Print(10,i++,"M - on/off multitexturing",1);
			font0->Print(10,i++,"P - on/off drawing lines",1);
			font0->Print(10,i++,"L - on/off lighting",1);
			font0->Print(10,i++,"D - on/off divide texture",1);
			font0->Print(10,i++,"U - on/off using lightmap -only if lighting is on",1);
			glColor3f(1,0.4f,0.4f);
			font0->Print(10,i++,"A - on/off animation, if !anination light is there where is mouse",1);
			glColor3f(1,1,1);
			font0->Print(10,i++,"R - set max emboss on 0.008",1);
			font0->Print(10,i++,"arrows - set shift max emboss",1);
		}
		else
		{
			font0->Print(10,i++,"Program: Bump-mapping - bodove svetlo",0);
			glColor3f(0.9f,0.9f,1.0f);
			font0->Print(5,i++,"Naprogramoval: Marek Mizanin - www.mizanin.szm.sk",0);
			glColor3f(1.0f,1.0f,1.0f);
			font0->Print( 5,i++,"Pouzivane klavesy:",0);
			font0->Print(10,i++,"1 - prepne na vysvetlenie ako funguje emboss bump-mapping",1);
			font0->Print(10,i++,"2 - prepne na ukazku bump-mappingu so bodovym svetlom",1);
			font0->Print(10,i++,"3 - prepne bludisko s bump-mappingom",1);
			font0->Print(10,i++,"4 - planeta s bump-mappingom",1);
			font0->Print(10,i++,"T - prepina aku texturu budeme nastavovat",1);
			font0->Print_scale(10,i++,"+ a - na numerickej klavesnici - meni texturu ktora je v ramceku",1,0.85f,1);
			font0->Print(10,i++,"E - zapne/vypne emboss,t.j. nezobraziuje zakladnu texturu",1);
			font0->Print(10,i++,"M - zapne/vypne multitexturing",1);
			font0->Print(10,i++,"P - zapne/vypne zobrazovanie ciar",1);
			font0->Print(10,i++,"L - zapne/vypne svetlo",1);
			font0->Print(10,i++,"D - zapne/vypne rozdelenie textury",1);
			font0->Print(10,i++,"U - zapne/vypne pouzivanie lightmap-i -iba ked je svetlo",1);
			glColor3f(1,0.4f,0.4f);
			font0->Print(10,i++,"A - zapne/vypne animaciu, ak !animacia svetlo je tam kde mys",1);
			glColor3f(1,1,1);
			font0->Print(10,i++,"R - nastavy max emboss na 0.008",1);
			font0->Print(10,i++,"sipky - nastavuju posunutie max emboss",1);
		}

		font0->Print(10,i++,"H - help",1);
	}
	font0->End();
}