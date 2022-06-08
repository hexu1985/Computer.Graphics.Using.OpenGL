void drawPolyLine(GlintPointArray poly, int closed)
{
	glBegin(closed ? GL_LINE_LOOP : GL_LINE_STRIP);
        for(int i = 0; i < poly.num; i++)
		     glVertex2i(poly.pt[i].x, poly.pt[i].y);
	glEnd();
	glFlush();
}
