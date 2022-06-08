void Sphere:: xfrmRay(Ray& genRay, Matrix4 invTransf, Ray r){
// Contruct homogeous point and dir
Point4 start4= Point4(r.start.x, r.start.y, r.start.z,1);
Vector4 dir4=Vector4 (r.dir.x, r.dir.y, r.dir.z, 0);

//Multpy with inverse matrix
Start4.mult(invTransf);
dir4.mult(invTransf);

// Drop the forth component and set the genray
genRay.setStart(start4.x, start4.y, start4.z);
genRay.setDir(dir4.x, dir4.y, dir4.z);

}
