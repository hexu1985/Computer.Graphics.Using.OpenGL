class RGBColor{public: double r, g, b;};
class HLSColor{public: double h, l, s;};

RGB_to_HLS (RGBColor rgb, HLSColor& hls)
{// convert (r, g, b), each in [0, 1], to h, l, s
     double mx, mn;
     RGBColor tmp;
     mx = MAX(rgb.r, rgb.g, rgb.b);
     mn = MIN(rgb.r, rgb.g, rgb.b);
     hls.l =(mx + mn) / 2.0; 	 // compute lightness
     if (mx == mn) // compute saturation 
hls.s = 0.0; 	// color is gray 
     else {				// color is chromatic
if(hls.l <= 0.5)  hls.s = (mx - mn)/(mx + mn);
      else hls.s = (mx - mn)/(2 - mx + mn);
// compute hue
      tmp.r =(mx - rgb.r)/(mx - mn);   
      tmp.g =(mx - rgb.g)/(mx - mn);
      tmp.b =(mx - rgb.b)/(mx - mn);
      if (rgb.r == mx) hls.h = tmp.b - tmp.g;
      else if (rgb.g == mx) hls.h  = 2 + tmp.r - tmp.b;
      else if (rgb.b == mx) hls.h = 4 + tmp.g - tmp.r:
      hls.h * = 60;
      if(hls.h < 0.0) 
hls.h += 360;
  	}  
}
