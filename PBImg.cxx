/* Copyright (C) 2011 Yury P. Fedorchenko (yuryfdr at users.sf.net)  */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include <stdlib.h>
#include "PBImg.h"
#include <inkview.h>
#include <algorithm>

void PBImage::draw(int x,int y,int w, int h,bool save_aspect){
  if(save_aspect){
    double coefw=(double)w/(double)width;
    double coefh=(double)h/(double)height;
    if(coefw <1. && coefh <1.){
      double coef = std::min(coefw,coefh);
      w=coef*w;
      h=coef*h;
    }else if(coefw <1.){
      h=coefw*h;
    }else if(coefh <1.){
      w=coefh*w;
    }
    
  }
  //printf("Stretch to: %d %d\n",w,h);
  Stretch(array,(depth==1)?IMAGE_GRAY8:IMAGE_RGB,width,height,width * depth,x,y,w,h,0);
}

void PBBMPImage::draw(int x,int y,int w,int h,bool save_aspect){
  DrawBitmapRect(x,y,w,h,bm,ALIGN_CENTER | VALIGN_MIDDLE);
}

PBBMPImage::PBBMPImage(const char* fname){
    bm = LoadBitmap((char*)fname);
    width = bm->width;
    height = bm->height;
    depth = bm->depth;
}

PBImage* PBImage::createFromFile(const char* fname){
  if(!fname || strlen(fname)<3)return NULL;
  const char *ext=&fname[strlen(fname)-3];
  if(strcasecmp(ext,"png")==0)
    return new PBPNGImage(fname);
  if(strcasecmp(ext,"gif")==0)
    return new PBGIFImage(fname);
  if(strcasecmp(ext,"bmp")==0)
    return new PBBMPImage(fname);
  return new PBJPEGImage(fname);
}

