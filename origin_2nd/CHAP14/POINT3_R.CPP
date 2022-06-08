Point3 rayPos(Ray r, float t)
{
      Point3 tempPoint;
      tempPoint.x = r.start.x + t* r.dir.x;
      tempPoint.y = r.start.y + t* r.dir.y;
      tempPoint.z = r.start.z + t* r.dir.z;
      return tempPoint;
};