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

//struct ibitmap_s;
//typedef struct ibitmap_s ibitmap;

#include <inkview.h>
/**
  base class for different images format
*/
class PBImage {
protected:
  int width,height,depth;
  const unsigned char* array;
public:
  int h()const{return height;} //< height of the image
  int w()const{return width;}  //< width of the image
  int d()const{return depth;}  //< bity color depth
  PBImage():array(NULL){}
  virtual ~PBImage(){
    if(array)delete[] array;
  }
  /**
    draw scaled image
  */
  virtual void draw(int x,int y,int w,int h,bool save_aspect=false);
  /**
    draw image
  */
  void draw(int x,int y){
    draw(x,y,width,height);
  }
  /** create image from file
     create corresponding image from file
  */
  static PBImage* createFromFile(const char* fname);
};
/**
  png image
*/
class PBPNGImage : public PBImage {
  void load_png_(const char* fname);
public:
  PBPNGImage(const char* fname);
};
/**
  gif image
*/
class PBGIFImage : public PBImage {
  void load_png_(const char* fname);
public:
  PBGIFImage(const char* fname);
};
/**
  ibitmap image 
  ibitmap image is the single image that cannot be created from file.
*/
class PBIbitmapImage : public PBImage {
  ibitmap* bm;
public:
  virtual void draw(int x,int y,int w,int h,bool save_aspect=false);
  void draw(int x,int y){
    draw(x,y,width,height);
  }
  /**
    create 
  */
  PBIbitmapImage(ibitmap* bmp){
    bm = bmp;
    width = bm->width;
    height = bm->height;
    depth = bm->depth;
  }
  ~PBIbitmapImage(){}
};
/**
  bmp image
*/
class PBBMPImage : public PBImage {
  ibitmap* bm;
public:
  virtual void draw(int x,int y,int w,int h,bool save_aspect=false);
  void draw(int x,int y){
    draw(x,y,width,height);
  }
  PBBMPImage(const char* fname);
  ~PBBMPImage(){free(bm);}
};
/**
  jpeg image
*/
class PBJPEGImage : public PBImage {
public:
  PBJPEGImage(const char* fname);
};

