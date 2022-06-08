void drawCircle(Point2 center, float radius)
{
      const int numVerts = 50;     // use larger for a better circle 
      ngon(numVerts, center.getX(), center.getY(), radius, 0); 
}
