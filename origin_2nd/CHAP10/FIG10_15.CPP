void RGBApixmap :: blend()
{
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND); // enable blending
   draw(); // draw this pixmap blended with the destination
}
