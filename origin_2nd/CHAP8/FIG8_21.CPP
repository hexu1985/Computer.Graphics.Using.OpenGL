glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0); // a cutoff angle of 45o
glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, 4.0); // e = 4.0
GLfloat dir[] = {2.0, 1.0, -4.0}; // the spotlight’s direction
glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, dir);

