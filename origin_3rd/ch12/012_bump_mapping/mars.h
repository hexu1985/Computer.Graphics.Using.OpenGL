// mars.h: interface for the mars class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MARS_H__BF2A1A39_8972_4961_B529_3E8CADD52D83__INCLUDED_)
#define AFX_MARS_H__BF2A1A39_8972_4961_B529_3E8CADD52D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "scene.h"
#define pocet_textur 8

class mars  
{
	class scene *m_scene;
	Camera  *c_mars;
	GLuint texture[pocet_textur];
	GLuint mars_bump_shift[pocet_textur],mars_bump_inv[pocet_textur];
	float l[4],lr[4];	// lr - svetlo rotovane
	bool lok_svetlo,kresli_zem;
	void Gula_textur_v_svetlo_dole(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka,int delenie_dlzky);
	void Gula_textur_v_svetlo_hore(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka,int delenie_dlzky);
	void Gula_textur_v_svetlo_dole_bump(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka,int delenie_dlzky);
	void Gula_textur_v_svetlo_hore_bump(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka,int delenie_dlzky);
	void Normalise(XYZ *p);
public:
	void Kresli(void);
	void Gula_textur_v_svetlo_bump(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka);
	void Gula_textur_v_svetlo(float gula_polomer,int delenie_guli_sirka,int delenie_guli_dlzka);
	mars();
	mars(scene *akt_scene_);
	~mars();

};

#endif // !defined(AFX_MARS_H__BF2A1A39_8972_4961_B529_3E8CADD52D83__INCLUDED_)
