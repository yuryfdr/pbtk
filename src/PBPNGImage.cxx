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
#include <zlib.h>
#define PNG_SKIP_SETJMP_CHECK
#include <png.h>


PBPNGImage::PBPNGImage (const char *filename){
  load_png_(filename);
}

void PBPNGImage::load_png_(const char *name_png)
{
  int i;	  // Looping var
  FILE *fp = NULL;	  // File pointer
  int channels;	  // Number of color channels
  png_structp pp; // PNG read pointer
  png_infop info; // PNG info pointers
  png_bytep *rows;// PNG row pointers

  if ((fp = fopen(name_png, "rb")) == NULL) return;

  // Setup the PNG data structures...
  pp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (pp) info = png_create_info_struct(pp);
  if (!pp || !info) {
    if (pp) png_destroy_read_struct(&pp, NULL, NULL);
    fclose(fp);
    printf("Cannot allocate memory to read PNG file or data \"%s\".\n", name_png);
    return;
  }
  
  if (setjmp(png_jmpbuf(pp)))
  {
    png_destroy_read_struct(&pp, &info, NULL);
    fclose(fp);
    printf("PNG file or data \"%s\" contains errors!\n", name_png);
    return;
  }

  png_init_io(pp, fp); // Initialize the PNG file read "engine"...

  // Get the image dimensions and convert to grayscale or RGB...
  png_read_info(pp, info);

  if (png_get_color_type(pp, info) == PNG_COLOR_TYPE_PALETTE)
    png_set_expand(pp);

  if (png_get_color_type(pp, info) & PNG_COLOR_MASK_COLOR)
    channels = 3;
  else
    channels = 1;

  int num_trans = 0;
  png_get_tRNS(pp, info, 0, &num_trans, 0);
  if ((png_get_color_type(pp, info) & PNG_COLOR_MASK_ALPHA) || (num_trans != 0))
    channels ++;

  width=(int)(png_get_image_width(pp, info));
  height=(int)(png_get_image_height(pp, info));
  depth=channels;

  if (png_get_bit_depth(pp, info) < 8)
  {
    png_set_packing(pp);
    png_set_expand(pp);
  }
  else if (png_get_bit_depth(pp, info) == 16)
    png_set_strip_16(pp);

  if (png_get_valid(pp, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(pp);

  array = new unsigned char[width * height * depth];

  rows = new png_bytep[height];

  for (i = 0; i < height; i ++)
    rows[i] = (png_bytep)(array + i * width * depth);

  // Read the image, handling interlacing as needed...
  for (i = png_set_interlace_handling(pp); i > 0; i --)
    png_read_rows(pp, rows, NULL, height);

#if 0 //def WIN32
  // Some Windows graphics drivers don't honor transparency when RGB == white
  if (channels == 4) {
    // Convert RGB to 0 when alpha == 0...
    unsigned char *ptr = (unsigned char *)array;
    for (i = width * height; i > 0; i --, ptr += 4)
      if (!ptr[3]) ptr[0] = ptr[1] = ptr[2] = 0;
  }
#endif // WIN32

  delete[] rows;

  png_read_end(pp, info);
  png_destroy_read_struct(&pp, &info, NULL);

  fclose(fp);
  if(depth==4){ // PB not support alpha
    unsigned char *ptr = (unsigned char *)array;
    unsigned char* new_array = new unsigned char[width * height * 3];
    unsigned char* new_ptr = (unsigned char *)new_array;
    for (i = width * height; i > 0; i --, ptr += 4, new_ptr+=3){
      if (!ptr[3]) new_ptr[0] = new_ptr[1] = new_ptr[2] = 255;
      else {
      new_ptr[0] = ptr[0];
      new_ptr[1] = ptr[1];
      new_ptr[2] = ptr[2];
      }
    }
    depth=3;
    delete[] array;
    array = new_array;
  }
}

