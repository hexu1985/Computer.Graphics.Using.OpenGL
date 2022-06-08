#define numRed 6
#define numGreen 7
#define numBlue 6
#define pack(r,g,b) (256 * (r)+ 16 * (g) + (b))
 
for(rd = 0; rd < numRed; rd++)
  for(grn = 0; grn < numGreen; grn++)
    for(blu = 0; blu < numBlue; blu++)
    {
index = numRed * numGreen * rd + numBlue * grn + blu;
 LUT[index] = pack(r[rd],g[grn],b[blu]);
     }
