glBegin(GL_QUADS); // define a quadrilateral face
glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 2.5, 1.5);
glTexCoord2f(0.0, 0.6); glVertex3f(1.0, 3.7, 1.5);
glTexCoord2f(0.8, 0.6); glVertex3f(2.0, 3.7, 1.5);
glTexCoord2f(0.8, 0.0); glVertex3f(2.0, 2.5, 1.5);
glEnd();
