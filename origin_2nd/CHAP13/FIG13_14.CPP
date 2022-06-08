void depthSort(Face faces[])
{
  for(P = last face in list; list not empty; P = next face in list)
  {
	hopeful = 1;          // have definite action to take
    for(each Q overlapping P in z) && hopeful == 1)
	{
		if(  (x-extents of P and Q are disjoint)
	      ||(y-extents of P and Q are disjoint)
	      ||(P is on opposite side of Q’s plane)
	      ||(Q is on the same side of P’s plane)		
	      ||(the projected faces P and Q don’t overlap))
               continue; //try next Q
	    // P might obscure this Q; see if Q is marked or behind P
      if (Q is marked)  
		  {
		         split Q by plane of P; 
                    mark and insert pieces of Q;
          hopeful = 0;
       }	
       else if(Q on opposite side of P’s plane ||
               P on same side as Q’s plane)
       {
			   <mark Q, put Q at end of list>;
           hopeful = 0;
			}
	     else // can’t tell: they overlap too much
		  {  
                     split Q by plane of P; 
                    mark and insert pieces of Q;
          hopeful = 0;
       }
    }// end.. for each q..
    if(hopeful)  
       drawAndRemove(P); 
  } // end of for each P 
}
