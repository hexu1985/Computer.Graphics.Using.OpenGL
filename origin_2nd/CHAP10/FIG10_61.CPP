glClear(GL_ACCUM_BUFFER_BIT); // clear the accumulation buffer
for(int i=0; i < 8; i++)
{
	cam.slide(f * jitter[i].x, f * jitter[i].y,0); //slide the camera
	display(); // draw the scene
glAccum(GL_ACCUM, 1/8.0); //add to the accumulation buffer
}
glAccum(GL_RETURN, 1.0);//copy accumulation buffer into frame buffer
