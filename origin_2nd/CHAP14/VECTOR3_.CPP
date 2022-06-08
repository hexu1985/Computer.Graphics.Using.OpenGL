Vector3 cubeNormal(int which)
{
 Vector3 tempVector;
Switch ( which)
{
case 0:
     tempVector.set(0,1,0)
     break;
case 1:
     tempVector.set(0,-1,0)
     break;
case 2:
     tempVector.set(1,0,0)
     break;
case 3:
     tempVector.set(-1,0,0)
     break;
case 4:
     tempVector.set(0,0,1)
     break;
case 5:
     tempVector.set(0, 0,-1)
     break;
    }
    return tempVector;
}