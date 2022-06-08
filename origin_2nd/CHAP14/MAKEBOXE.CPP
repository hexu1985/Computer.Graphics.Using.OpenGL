void makeBoxExtent(PointCluster& clust,Cuboid& cub)
{
       cub.left= clust.pt[0].x;
       cub.right= clust.pt[0].x;
       cub.front= clust.pt[0].y;
       cub.back = clust.pt[0].y;
       cub.top= clust.pt[0].z;
       cub.bottom= clust.pt[0].z;

       for (int i=1; i < clust.num; i++)
       {
             if(clust.pt[i].x > cub.right)
                     cub.right = clust.pt[0].x;
             if(clust.pt[i].x < cub.left)
                     cub. left = clust.pt[0].x;
             if(clust.pt[i].y > cub.front)
                     cub. front = clust.pt[0].y;
             if(clust.pt[i].y < cub.back)
                     cub. back = clust.pt[0].y;
             if(clust.pt[i].z > cub.top)
                     cub. top = clust.pt[0].z;
             if(clust.pt[i].z < cub. bottom)
                     cub. bottom = clust.pt[0].z;

       }
}