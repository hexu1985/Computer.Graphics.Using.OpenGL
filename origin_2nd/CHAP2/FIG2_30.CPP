glClearColor(1.0,1.0,1.0,0.0); // white background 
	glClear(GL_COLOR_BUFFER_BIT);  // clear the window
	glColor3f(0.6,0.6,0.6);		  // bright gray
  glRecti(20,20,100,70);
	glColor3f(0.2,0.2,0.2);		  // dark gray
	glRecti(70, 50, 150, 130);
	glFlush();
