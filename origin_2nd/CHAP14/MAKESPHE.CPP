void makeSphereExtent(PointCluster& clust,SphereInfo& sph)
{
   float sum_x=0.,sum_y=0.,sum_z=0.; 
   float distance_far=0.;

   for (int i= 0; i < clust.num; i++)
       {
        sum_x+=clust.pt[i].x ;
        sum_y+=clust.pt[i].y ;
        sum_z+=clust.pt[i].z ;
        }

   sph.center.x= sum_x/clust.num;
   sph.center.y= sum_y/clust.num;
   sph.center.z= sum_z/clust.num;

   for (int i= 0; i < clust.num; i++)
        if(dot3D(sph.center, clust.pt[i])> distance_far)
              distance_far= dot3D(sph.center, clust.pt[i]);

   sph.radius=sqrt(distance_far);
}