// mars.cpp: implementation of the mars class.
//
//////////////////////////////////////////////////////////////////////

#include "mars.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

mars::mars()
{	
	int i;
	m_scene = NULL; c_mars = NULL;
	for(i=0;i<pocet_textur;i++) 
	{
		texture[i] = 0;
		mars_bump_shift[i] = 0;
		mars_bump_inv[i] = 0;
	}
	error = 1;
}

mars::mars(	scene *akt_scene_)
{
	int i;
	m_scene = akt_scene_;
	c_mars = NULL;
	l[0] = 0; l[1] = 0; l[2] = 40.0f; l[3] = 1.0f;
	lok_svetlo = 1;
	for(i=0;i<pocet_textur;i++) 
	{
		texture[i] = 0;
		mars_bump_shift[i] = 0;
		mars_bump_inv[i] = 0;
	}
	i=0;
	if( !m_scene->LoadGLTexture_JPG("data/mars/mars_0.JPG",&texture[i]) ) {error = 1;return;}
	if( !m_scene->LoadTexture_JPG_bump("data/mars/mars_bump_0.JPG",&mars_bump_shift[i],&mars_bump_inv[i]) ) {error = 1;return;}
	
	i++;
	if( !m_scene->LoadGLTexture_JPG("data/mars/mars_1.JPG",&texture[i]) ) {error = 1;return;}
	if( !m_scene->LoadTexture_JPG_bump("data/mars/mars_bump_1.JPG",&mars_bump_shift[i],&mars_bump_inv[i]) ) {error = 1;return;}

	i++;
	if( !m_scene->LoadGLTexture_JPG("data/mars/mars_2.JPG",&texture[i]) ) {error = 1;return;}
	if( !m_scene->LoadTexture_JPG_bump("data/mars/mars_bump_2.JPG",&mars_bump_shift[i],&mars_bump_inv[i]) ) {error = 1;return;}
	
	i++;
	if( !m_scene->LoadGLTexture_JPG("data/mars/mars_3.JPG",&texture[i]) ) {error = 1;return;}
	if( !m_scene->LoadTexture_JPG_bump("data/mars/mars_bump_3.JPG",&mars_bump_shift[i],&mars_bump_inv[i]) ) {error = 1;return;}
	
	i++;
	if( !m_scene->LoadGLTexture_JPG("data/mars/mars_4.JPG",&texture[i]) ) {error = 1;return;}
	if( !m_scene->LoadTexture_JPG_bump("data/mars/mars_bump_4.JPG",&mars_bump_shift[i],&mars_bump_inv[i]) ) {error = 1;return;}
	
	i++;
	if( !m_scene->LoadGLTexture_JPG("data/mars/mars_5.JPG",&texture[i]) ) {error = 1;return;}
	if( !m_scene->LoadTexture_JPG_bump("data/mars/mars_bump_5.JPG",&mars_bump_shift[i],&mars_bump_inv[i]) ) {error = 1;return;}
	
	i++;
	if( !m_scene->LoadGLTexture_JPG("data/mars/mars_6.JPG",&texture[i]) ) {error = 1;return;}
	if( !m_scene->LoadTexture_JPG_bump("data/mars/mars_bump_6.JPG",&mars_bump_shift[i],&mars_bump_inv[i]) ) {error = 1;return;}
	
	i++;
	if( !m_scene->LoadGLTexture_JPG("data/mars/mars_7.JPG",&texture[i]) ) {error = 1;return;}
	if( !m_scene->LoadTexture_JPG_bump("data/mars/mars_bump_7.JPG",&mars_bump_shift[i],&mars_bump_inv[i]) ) {error = 1;return;}

	c_mars = new Camera;
}

mars::~mars()
{
	int i;
	if(c_mars!=NULL)delete c_mars;
	for(i=0;i<pocet_textur;i++)
	{
		glDeleteTextures( 1, &texture[i]);
		glDeleteTextures( 1, &mars_bump_shift[i]);
		glDeleteTextures( 1, &mars_bump_inv[i]);
	}
}

void mars::Gula_textur_v_svetlo(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka)
{
	int i;

	glPushMatrix();
	for(i=0;i<pocet_textur/2;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		Gula_textur_v_svetlo_hore(gula_polomer,delenie_guli_sirka,delenie_guli_dlzka,pocet_textur/2);
		glRotatef( (720.0f/(float)pocet_textur) ,0,1,0);
	}
	glPopMatrix();

	glPushMatrix();
	for(i=0;i<pocet_textur/2;i++)
	{
		glBindTexture(GL_TEXTURE_2D, texture[i+pocet_textur/2]);
		Gula_textur_v_svetlo_dole(gula_polomer,delenie_guli_sirka,delenie_guli_dlzka,pocet_textur/2);
		glRotatef( (720.0f/(float)pocet_textur) ,0,1,0);
	}
	glPopMatrix();
}

void mars::Gula_textur_v_svetlo_hore(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka,int delenie_dlzky)
{
	float	sirka;		// sirka je vo zvislom smere, moze byt severna, alebo juzna
	float	dlzka;		// dlzka je vo vodorovnom smere, moze byt vychodna, alebo zapadna
	int		i_sirka, i_dlzka;	// index sirky a dlzky
	float	u,v0,v1;		// suradnice na texture
	XYZ		b[2],normal;	// suradnice vypocitanych bodov
	double	velkost_casti_sirky,velkost_casti_dlzky;
	double	polomer_pre_sirku0,polomer_pre_sirku1;	// polomer pre danu sirku

	velkost_casti_sirky = 1.0/(double)delenie_guli_sirka;
	velkost_casti_dlzky = 1.0/(double)delenie_guli_dlzka;
	
	for(i_sirka=0; i_sirka<delenie_guli_sirka; i_sirka++) // ideme od 0 stupnov do < ako 90.0
	{
		sirka = (float)( i_sirka*90.0*velkost_casti_sirky );	// vypocet uhlu pre dany index
		v0 = (float)( i_sirka*velkost_casti_sirky );			// vypocet prislusnej texturovej koordinacie
		v1 = (float)( (i_sirka+1)*velkost_casti_sirky );		// dalsia texturova koordinacia

		polomer_pre_sirku0 = gula_polomer*cos(PI180*sirka);
		polomer_pre_sirku1 = gula_polomer*cos(PI180*(sirka+90.0*velkost_casti_sirky));
		b[0].y = (float)( gula_polomer*sin(PI180*sirka) );
		b[1].y = (float)( gula_polomer*sin(PI180*(sirka+90.0*velkost_casti_sirky)) );

		b[0].x = 0.0f;
		b[1].x = 0.0f;

		b[0].z = (float)( polomer_pre_sirku0 );
		b[1].z = (float)( polomer_pre_sirku1 );
		u = 0.0;								// vypocet prislusnej texturovej koordinacie

		glBegin( GL_TRIANGLE_STRIP);			// kreslim pas trojuholnikov
		
		normal = b[0];
		Normalise( &normal);
		glNormal3fv((float*)&normal);
		glTexCoord2f(u,v0);
		glVertex3fv((float*)&b[0]);
		
		normal = b[1];
		Normalise( &normal);
		glNormal3fv((float*)&normal);
		glTexCoord2f(u,v1);
		glVertex3fv((float*)&b[1]);

		for(i_dlzka=1; i_dlzka<=delenie_guli_dlzka; i_dlzka++) // ideme od o jeden viac ako 0 stupnov po 90.0
		{
			dlzka = (float)( i_dlzka*(360.0f/(float)delenie_dlzky)*velkost_casti_dlzky );	// vypocet uhlu pre dany index
			u = (float)( i_dlzka*velkost_casti_dlzky );				// vypocet prislusnej texturovej koordinacie

			b[0].x = (float)( polomer_pre_sirku0*sin(PI180*dlzka) );
			b[1].x = (float)( polomer_pre_sirku1*sin(PI180*dlzka) );

			b[0].z = (float)( polomer_pre_sirku0*cos(PI180*dlzka) );
			b[1].z = (float)( polomer_pre_sirku1*cos(PI180*dlzka) );

			normal = b[0];
			Normalise( &normal);
			glNormal3fv((float*)&normal);
			glTexCoord2f(u,v0);
			glVertex3fv((float*)&b[0]);
			
			normal = b[1];
			Normalise( &normal);
			glNormal3fv((float*)&normal);
			glTexCoord2f(u,v1);
			glVertex3fv((float*)&b[1]);
		}
		glEnd();
	}
}

void mars::Gula_textur_v_svetlo_dole(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka,int delenie_dlzky)
{
	float	sirka;		// sirka je vo zvislom smere, moze byt severna, alebo juzna
	float	dlzka;		// dlzka je vo vodorovnom smere, moze byt vychodna, alebo zapadna
	int		i_sirka, i_dlzka;	// index sirky a dlzky
	float	u,v0,v1;		// suradnice na texture
	XYZ		b[2],normal;	// suradnice vypocitanych bodov
	double	velkost_casti_sirky,velkost_casti_dlzky;
	double	polomer_pre_sirku0,polomer_pre_sirku1;	// polomer pre danu sirku

	velkost_casti_sirky = 1.0/(double)delenie_guli_sirka;
	velkost_casti_dlzky = 1.0/(double)delenie_guli_dlzka;
	
	for(i_sirka=-delenie_guli_sirka; i_sirka<0; i_sirka++) // ideme od 0 stupnov do < ako 90.0
	{
		sirka = (float)( i_sirka*90.0*velkost_casti_sirky );	// vypocet uhlu pre dany index
		v0 = (float)( (i_sirka+delenie_guli_sirka)*velkost_casti_sirky );			// vypocet prislusnej texturovej koordinacie
		v1 = (float)( (i_sirka+delenie_guli_sirka+1)*velkost_casti_sirky );		// dalsia texturova koordinacia
		
		polomer_pre_sirku0 = gula_polomer*cos(PI180*sirka);
		polomer_pre_sirku1 = gula_polomer*cos(PI180*(sirka+90.0*velkost_casti_sirky));
		b[0].y = (float)( gula_polomer*sin(PI180*sirka) );
		b[1].y = (float)( gula_polomer*sin(PI180*(sirka+90.0*velkost_casti_sirky)) );

		b[0].x = 0.0f;
		b[1].x = 0.0f;

		b[0].z = (float)( polomer_pre_sirku0 );
		b[1].z = (float)( polomer_pre_sirku1 );
		u = 0.0f;										// vypocet prislusnej texturovej koordinacie

		glBegin( GL_TRIANGLE_STRIP);					// kreslim pas trojuholnikov
		normal = b[0];
		Normalise( &normal);
		glNormal3fv((float*)&normal);
		glTexCoord2f(u,v0);
		glVertex3fv((float*)&b[0]);
		
		normal = b[1];
		Normalise( &normal);
		glNormal3fv((float*)&normal);
		glTexCoord2f(u,v1);
		glVertex3fv((float*)&b[1]);

		for(i_dlzka=1; i_dlzka<=delenie_guli_dlzka; i_dlzka++) // ideme od o jeden viac ako 0 stupnov po 90.0
		{
			dlzka = (float)( i_dlzka*(360.0f/(float)delenie_dlzky)*velkost_casti_dlzky );		// vypocet uhlu pre dany index
			u = (float)( i_dlzka*velkost_casti_dlzky );				// vypocet prislusnej texturovej koordinacie

			b[0].x = (float)( polomer_pre_sirku0*sin(PI180*dlzka) );
			b[1].x = (float)( polomer_pre_sirku1*sin(PI180*dlzka) );

			b[0].z = (float)( polomer_pre_sirku0*cos(PI180*dlzka) );
			b[1].z = (float)( polomer_pre_sirku1*cos(PI180*dlzka) );

			normal = b[0];
			Normalise( &normal);
			glNormal3fv((float*)&normal);
			glTexCoord2f(u,v0);
			glVertex3fv((float*)&b[0]);
			
			normal = b[1];
			Normalise( &normal);
			glNormal3fv((float*)&normal);
			glTexCoord2f(u,v1);
			glVertex3fv((float*)&b[1]);
		}
		glEnd();
	}
}

void mars::Kresli()
{
	if(keys['S'])
	{
		lok_svetlo = !lok_svetlo;
		l[0] = 0; l[1] = 0; l[2] = 40.0f; l[3] = 1.0f;
		keys['S']=0;
	}

	c_mars->MoveWorld(m_scene->fps);					// otocenie sveta
	
	if(!lok_svetlo)
	{
		if(m_scene->anim)
		{
			float pom=l[0];
			l[0] = l[2]*(float)sin(1.0/m_scene->fps) + l[0]*(float)cos(1.0/m_scene->fps);
			l[2] = l[2]*(float)cos(1.0/m_scene->fps) - pom*(float)sin(1.0/m_scene->fps);
		}
	}
	else
	{
		l[0] = c_mars->vp.x;
		l[1] = c_mars->vp.y;
		l[2] = c_mars->vp.z;
	}

	if(m_scene->svetlo)	
	{
		GLfloat LightDiffuse[]	=	{ 1.5f,1.5f,1.5f, 1.0f };
		GLfloat LightAmbient[]	=	{ 1.0f,1.0f,1.0f, 1.0f };

		glLightfv ( GL_LIGHT0, GL_AMBIENT, LightAmbient );
		glLightfv ( GL_LIGHT0, GL_DIFFUSE, LightDiffuse );
		glLightfv( GL_LIGHT0, GL_POSITION, l);
		glEnable(GL_LIGHTING );		// preco je tu toto ? Ani sam neviem, ale bez toho mne robi same nezmyselne ciary
		glBegin(GL_TRIANGLE_STRIP );
		glEnd();
		glPushMatrix();			// tomu ale vobec nechapem, preco tu musi byt Push a Pop matrix ???
		glDisable(GL_LIGHTING );
		glPopMatrix();
	}

	Gula_textur_v_svetlo_bump(20.0f,10,10);

	if(!lok_svetlo)
	{
		glDisable(GL_LIGHTING );
		glBindTexture(GL_TEXTURE_2D, m_scene->t_svetlo);
		glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_COLOR);
		glEnable(GL_BLEND);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);glVertex3f( l[0],l[1],l[2]);
			glTexCoord2f(1,0);glVertex3f( l[0]+2.f*c_mars->vr.x,l[1]+2.f*c_mars->vr.y,l[2]+2.f*c_mars->vr.z);
			glTexCoord2f(1,1);glVertex3f( l[0]+2.f*c_mars->vr.x+2.f*c_mars->vu.x,l[1]+2.f*c_mars->vr.y+2.f*c_mars->vu.y,l[2]+2.f*c_mars->vr.z+2.f*c_mars->vu.z);
			glTexCoord2f(0,1);glVertex3f( l[0]+2.f*c_mars->vu.x,l[1]+2.f*c_mars->vu.y,l[2]+2.f*c_mars->vu.z);
		glEnd();
		glDisable(GL_BLEND);
	}

	{	// vratim nstavenie svetla pre 2 a 3 volbu
		GLfloat LightDiffuse[]	=	{ 1.0f, 1.0f, 1.0f, 1.0f };		//smerove, alebo bodove svetlo - svetlo svieti z pozicie zadanej pomocou premennej LightPosition
		GLfloat LightAmbient[]	=	{ 0.0f, 0.0f, 0.0f, 1.0f };

		glLightfv ( GL_LIGHT0, GL_AMBIENT, LightAmbient );
		glLightfv ( GL_LIGHT0, GL_DIFFUSE, LightDiffuse );			// nastavenie difuzneho svetla
	}

}

void mars::Gula_textur_v_svetlo_bump(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka)
{
	int i;
	glDisable(GL_LIGHTING );

	if(m_scene->bump_)
	{
		// 1-pass, vykreslenie bump_shift textury

		glPushMatrix();
		for(i=0;i<pocet_textur/2;i++)
		{
			glBindTexture(GL_TEXTURE_2D, mars_bump_shift[i]);
			Gula_textur_v_svetlo_hore(gula_polomer,delenie_guli_sirka,delenie_guli_dlzka,pocet_textur/2);
			glRotatef( (720.0f/(float)pocet_textur) ,0,1,0);
		}
		glPopMatrix();

		glPushMatrix();
		for(i=0;i<pocet_textur/2;i++)
		{
			glBindTexture(GL_TEXTURE_2D, mars_bump_shift[i+pocet_textur/2]);
			Gula_textur_v_svetlo_dole(gula_polomer,delenie_guli_sirka,delenie_guli_dlzka,pocet_textur/2);
			glRotatef( (720.0f/(float)pocet_textur) ,0,1,0);
		}
		glPopMatrix();
		
		// 2-pass, vykreslenie pricitanim posunutej bump_inv testury
		glPushMatrix();
		glBlendFunc(GL_ONE,GL_ONE);				// scitavanie
		glEnable(GL_BLEND);

		for(i=0;i<4;i++)lr[i]=l[i];
		for(i=0;i<pocet_textur/2;i++)
		{
			glBindTexture(GL_TEXTURE_2D, mars_bump_inv[i]);
			Gula_textur_v_svetlo_hore_bump(gula_polomer,delenie_guli_sirka,delenie_guli_dlzka,pocet_textur/2);
			glRotatef( (720.0f/(float)pocet_textur) ,0,1,0);
			{	// rotacia polohy - rotujeme naopak
				float pom=lr[0];
				lr[0] = lr[2]*(float)sin(-PI180*720.0f/(float)pocet_textur) + lr[0]*(float)cos(-PI180*720.0f/(float)pocet_textur);
			//	lr[1] = lr[1];
				lr[2] = lr[2]*(float)cos(-PI180*720.0f/(float)pocet_textur) - pom*(float)sin(-PI180*720.0f/(float)pocet_textur);
			}
		}
		glPopMatrix();

		for(i=0;i<4;i++)lr[i]=l[i];
		glPushMatrix();
		for(i=0;i<pocet_textur/2;i++)
		{
			glBindTexture(GL_TEXTURE_2D, mars_bump_inv[i+pocet_textur/2]);
			Gula_textur_v_svetlo_dole_bump(gula_polomer,delenie_guli_sirka,delenie_guli_dlzka,pocet_textur/2);
			glRotatef( (720.0f/(float)pocet_textur) ,0,1,0);
			{	// rotacia polohy - rotujeme naopak
				float pom=lr[0];
				lr[0] = lr[2]*(float)sin(-PI180*720.0f/(float)pocet_textur) + lr[0]*(float)cos(-PI180*720.0f/(float)pocet_textur);
			//	lr[1] = lr[1];
				lr[2] = lr[2]*(float)cos(-PI180*720.0f/(float)pocet_textur) - pom*(float)sin(-PI180*720.0f/(float)pocet_textur);
			}
		}
		glPopMatrix();
		glDisable(GL_BLEND);
	}

	// 3-pass, vynasobenie 2* so zakladnou texturov
	glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
	if(m_scene->bump_)glEnable(GL_BLEND);
	if(m_scene->svetlo)glEnable(GL_LIGHTING );
	if(!m_scene->emboss)Gula_textur_v_svetlo(gula_polomer,delenie_guli_sirka,delenie_guli_dlzka);
	glDisable(GL_BLEND);
}

static void SetUpBumps( float* pos, XYZ *c, float *l, XYZ *s, XYZ *t)
{
	float v[3];					//	vektor z aktualnej pozicie do bodu kde je svetlo
	float lenQ;
		
	v[0]=l[0]-c->x;
	v[1]=l[1]-c->y;
	v[2]=l[2]-c->z;
	lenQ = (float) sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	v[0]/=lenQ;		v[1]/=lenQ;		v[2]/=lenQ;
	// vypocitame posunutia v smere u a v pre texturu
	pos[0]=(s->x*v[0]+s->y*v[1]+s->z*v[2])*scene::max_emboss;
	pos[1]=(t->x*v[0]+t->y*v[1]+t->z*v[2])*scene::max_emboss;
}

void mars::Gula_textur_v_svetlo_hore_bump(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka,int delenie_dlzky)
{
	float	sirka;		// sirka je vo zvislom smere, moze byt severna, alebo juzna
	float	dlzka;		// dlzka je vo vodorovnom smere, moze byt vychodna, alebo zapadna
	int		i_sirka, i_dlzka;	// index sirky a dlzky
	float	u,v0,v1;		// suradnice na texture
	XYZ		b[2],normal;	// suradnice vypocitanych bodov
	double	velkost_casti_sirky,velkost_casti_dlzky;
	double	polomer_pre_sirku0,polomer_pre_sirku1;	// polomer pre danu sirku

	XYZ s,t;
	float pos[2];

	velkost_casti_sirky = 1.0/(double)delenie_guli_sirka;
	velkost_casti_dlzky = 1.0/(double)delenie_guli_dlzka;
	
	for(i_sirka=0; i_sirka<delenie_guli_sirka; i_sirka++) // ideme od 0 stupnov do < ako 90.0
	{
		sirka = (float)( i_sirka*90.0*velkost_casti_sirky );	// vypocet uhlu pre dany index
		v0 = (float)( i_sirka*velkost_casti_sirky );			// vypocet prislusnej texturovej koordinacie
		v1 = (float)( (i_sirka+1)*velkost_casti_sirky );		// dalsia texturova koordinacia

		polomer_pre_sirku0 = gula_polomer*cos(PI180*sirka);
		polomer_pre_sirku1 = gula_polomer*cos(PI180*(sirka+90.0*velkost_casti_sirky));
		b[0].y = (float)( gula_polomer*sin(PI180*sirka) );
		b[1].y = (float)( gula_polomer*sin(PI180*(sirka+90.0*velkost_casti_sirky)) );

		b[0].x = 0.0f;
		b[1].x = 0.0f;

		b[0].z = (float)( polomer_pre_sirku0 );
		b[1].z = (float)( polomer_pre_sirku1 );
		u = 0.0;								// vypocet prislusnej texturovej koordinacie

		glBegin( GL_TRIANGLE_STRIP);			// kreslim pas trojuholnikov
		
		normal = b[0];
		Normalise( &normal);
		glNormal3fv((float*)&normal);
		
		t.x = b[1].x - b[0].x;
		t.y = b[1].y - b[0].y;
		t.z = b[1].z - b[0].z;
		Normalise(&t);
		CROSSPROD(t,normal,s);
		Normalise(&s);
		SetUpBumps( pos, &b[0], lr, &s, &t);

		glTexCoord2f(u+pos[0],v0+pos[1]);
		glVertex3fv((float*)&b[0]);
		
		normal = b[1];
		Normalise( &normal);
		glNormal3fv((float*)&normal);
		
		CROSSPROD(t,normal,s);
		Normalise(&s);
		SetUpBumps( pos, &b[1], lr, &s, &t);

		glTexCoord2f(u+pos[0],v1+pos[1]);
		glVertex3fv((float*)&b[1]);

		for(i_dlzka=1; i_dlzka<=delenie_guli_dlzka; i_dlzka++) // ideme od o jeden viac ako 0 stupnov po 90.0
		{
			dlzka = (float)( i_dlzka*(360.0f/(float)delenie_dlzky)*velkost_casti_dlzky );	// vypocet uhlu pre dany index
			u = (float)( i_dlzka*velkost_casti_dlzky );				// vypocet prislusnej texturovej koordinacie

			b[0].x = (float)( polomer_pre_sirku0*sin(PI180*dlzka) );
			b[1].x = (float)( polomer_pre_sirku1*sin(PI180*dlzka) );

			b[0].z = (float)( polomer_pre_sirku0*cos(PI180*dlzka) );
			b[1].z = (float)( polomer_pre_sirku1*cos(PI180*dlzka) );

			normal = b[0];
			Normalise( &normal);
			glNormal3fv((float*)&normal);
			
			t.x = b[1].x - b[0].x;
			t.y = b[1].y - b[0].y;
			t.z = b[1].z - b[0].z;
			Normalise(&t);
			CROSSPROD(t,normal,s);
			Normalise(&s);
			SetUpBumps( pos, &b[0], lr, &s, &t);

			glTexCoord2f(u+pos[0],v0+pos[1]);
			glVertex3fv((float*)&b[0]);
			
			normal = b[1];
			Normalise( &normal);
			glNormal3fv((float*)&normal);
			
			CROSSPROD(t,normal,s);
			Normalise(&s);
			SetUpBumps( pos, &b[1], lr, &s, &t);

			glTexCoord2f(u+pos[0],v1+pos[1]);
			glVertex3fv((float*)&b[1]);
		}
		glEnd();
	}
}

void mars::Gula_textur_v_svetlo_dole_bump(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka,int delenie_dlzky)
{
	float	sirka;		// sirka je vo zvislom smere, moze byt severna, alebo juzna
	float	dlzka;		// dlzka je vo vodorovnom smere, moze byt vychodna, alebo zapadna
	int		i_sirka, i_dlzka;	// index sirky a dlzky
	float	u,v0,v1;		// suradnice na texture
	XYZ		b[2],normal;	// suradnice vypocitanych bodov
	double	velkost_casti_sirky,velkost_casti_dlzky;
	double	polomer_pre_sirku0,polomer_pre_sirku1;	// polomer pre danu sirku

	XYZ s,t;
	float pos[2];

	velkost_casti_sirky = 1.0/(double)delenie_guli_sirka;
	velkost_casti_dlzky = 1.0/(double)delenie_guli_dlzka;
	
	for(i_sirka=-delenie_guli_sirka; i_sirka<0; i_sirka++) // ideme od 0 stupnov do < ako 90.0
	{
		sirka = (float)( i_sirka*90.0*velkost_casti_sirky );	// vypocet uhlu pre dany index
		v0 = (float)( (i_sirka+delenie_guli_sirka)*velkost_casti_sirky );			// vypocet prislusnej texturovej koordinacie
		v1 = (float)( (i_sirka+delenie_guli_sirka+1)*velkost_casti_sirky );		// dalsia texturova koordinacia
		
		polomer_pre_sirku0 = gula_polomer*cos(PI180*sirka);
		polomer_pre_sirku1 = gula_polomer*cos(PI180*(sirka+90.0*velkost_casti_sirky));
		b[0].y = (float)( gula_polomer*sin(PI180*sirka) );
		b[1].y = (float)( gula_polomer*sin(PI180*(sirka+90.0*velkost_casti_sirky)) );

		b[0].x = 0.0f;
		b[1].x = 0.0f;

		b[0].z = (float)( polomer_pre_sirku0 );
		b[1].z = (float)( polomer_pre_sirku1 );
		u = 0.0f;										// vypocet prislusnej texturovej koordinacie

		glBegin( GL_TRIANGLE_STRIP);					// kreslim pas trojuholnikov
		normal = b[0];
		Normalise( &normal);
		glNormal3fv((float*)&normal);

		t.x = b[1].x - b[0].x;
		t.y = b[1].y - b[0].y;
		t.z = b[1].z - b[0].z;
		Normalise(&t);
		CROSSPROD(t,normal,s);
		Normalise(&s);
		SetUpBumps( pos, &b[0], lr, &s, &t);

		glTexCoord2f(u+pos[0],v0+pos[1]);
		glVertex3fv((float*)&b[0]);
		
		normal = b[1];
		Normalise( &normal);
		glNormal3fv((float*)&normal);
		CROSSPROD(t,normal,s);
		Normalise(&s);
		SetUpBumps( pos, &b[1], lr, &s, &t);
		glTexCoord2f(u+pos[0],v1+pos[1]);
		glVertex3fv((float*)&b[1]);

		for(i_dlzka=1; i_dlzka<=delenie_guli_dlzka; i_dlzka++) // ideme od o jeden viac ako 0 stupnov po 90.0
		{
			dlzka = (float)( i_dlzka*(360.0f/(float)delenie_dlzky)*velkost_casti_dlzky );		// vypocet uhlu pre dany index
			u = (float)( i_dlzka*velkost_casti_dlzky );				// vypocet prislusnej texturovej koordinacie

			b[0].x = (float)( polomer_pre_sirku0*sin(PI180*dlzka) );
			b[1].x = (float)( polomer_pre_sirku1*sin(PI180*dlzka) );

			b[0].z = (float)( polomer_pre_sirku0*cos(PI180*dlzka) );
			b[1].z = (float)( polomer_pre_sirku1*cos(PI180*dlzka) );

			normal = b[0];
			Normalise( &normal);
			glNormal3fv((float*)&normal);
			
			t.x = b[1].x - b[0].x;
			t.y = b[1].y - b[0].y;
			t.z = b[1].z - b[0].z;
			Normalise(&t);
			CROSSPROD(t,normal,s);
			Normalise(&s);
			SetUpBumps( pos, &b[0], lr, &s, &t);

			glTexCoord2f(u+pos[0],v0+pos[1]);
			glVertex3fv((float*)&b[0]);
			
			normal = b[1];
			Normalise( &normal);
			glNormal3fv((float*)&normal);
			
			CROSSPROD(t,normal,s);
			Normalise(&s);
			SetUpBumps( pos, &b[1], lr, &s, &t);

			glTexCoord2f(u+pos[0],v1+pos[1]);
			glVertex3fv((float*)&b[1]);
		}
		glEnd();
	}
}

void mars::Normalise(XYZ *p)
{
	float length;

	length = (float) sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
	if (length != 0) {
		p->x /= length;
		p->y /= length;
		p->z /= length;
	} else {
		p->x = 0;
		p->y = 0;
		p->z = 0;
	}
}
