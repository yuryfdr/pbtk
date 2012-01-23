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

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "PBImg.h"

extern "C" {
#include <jpeglib.h>
  struct pb_jpeg_error_mgr {
    jpeg_error_mgr	pub_;		// Destination manager...
    jmp_buf		errhand_;	// Error handler
  };

  static void
  pb_jpeg_error_handler(j_common_ptr dinfo) {	// I - Decompressor info
      (*dinfo->err->output_message) (dinfo);
      char buffer[256];
      (*dinfo->err->format_message) (dinfo,buffer);
      printf("%s\n",buffer);
    longjmp(((pb_jpeg_error_mgr *)(dinfo->err))->errhand_, 1);
  }

  static void
  pb_jpeg_output_handler(j_common_ptr dinfo) {	// I - Decompressor info (not used)
    printf("err %d\n",dinfo->err->msg_code);
  }
}

PBJPEGImage::PBJPEGImage(const char *filename){
  FILE				*fp;	// File pointer
  jpeg_decompress_struct	dinfo;	// Decompressor info
  pb_jpeg_error_mgr		jerr;	// Error handler info
  JSAMPROW			row;	// Sample row pointer
  
  // the following variables are pointers allocating some private space that
  // is not reset by 'setjmp()'
  char* max_finish_decompress_err;      // count errors and give up afer a while
  char* max_destroy_decompress_err;     // to avoid recusion and deadlock
  
  // Open the image file...
  if ((fp = fopen(filename, "rb")) == NULL) return;
  
  // Setup the decompressor info and read the header...
  dinfo.err                = jpeg_std_error((jpeg_error_mgr *)&jerr);
  jerr.pub_.error_exit     = pb_jpeg_error_handler;
  jerr.pub_.output_message = pb_jpeg_output_handler;
  
  // Setup error loop variables
  max_finish_decompress_err = (char*)malloc(1);   // allocate space on the frame for error counters
  max_destroy_decompress_err = (char*)malloc(1);  // otherwise, the variables are reset on the longjmp
  *max_finish_decompress_err=10;
  *max_destroy_decompress_err=10;
  
  if (setjmp(jerr.errhand_))
  {
    // JPEG error handling...
    // if any of the cleanup routines hits another error, we would end up 
    // in a loop. So instead, we decrement max_err for some upper cleanup limit.
    if ( ((*max_finish_decompress_err)-- > 0) && array)
      jpeg_finish_decompress(&dinfo);
    if ( (*max_destroy_decompress_err)-- > 0)
      jpeg_destroy_decompress(&dinfo);
    
    fclose(fp);
    
    width = height = depth = 0;
    
    if (array) {
      delete[] array;
      array = 0;
    }
    
    free(max_destroy_decompress_err);
    free(max_finish_decompress_err);
    return;
  }
  
  jpeg_create_decompress(&dinfo);
  jpeg_stdio_src(&dinfo, fp);
  jpeg_read_header(&dinfo, 1);
  
  dinfo.quantize_colors      = (boolean)FALSE;
  dinfo.out_color_space      = JCS_RGB;
  dinfo.out_color_components = 3;
  dinfo.output_components    = 3;
  
  jpeg_calc_output_dimensions(&dinfo);
  
  width=dinfo.output_width; 
  height=dinfo.output_height;
  depth=dinfo.output_components;
  
  array = new unsigned char[width * height * depth];
  
  jpeg_start_decompress(&dinfo);
  
  while (dinfo.output_scanline < dinfo.output_height) {
    //printf("%d %d\n",dinfo.output_scanline,dinfo.output_height);
    row = (JSAMPROW)(array +
                     dinfo.output_scanline * dinfo.output_width *
                     dinfo.output_components);
    jpeg_read_scanlines(&dinfo, &row, (JDIMENSION)1);
  }
  
  jpeg_finish_decompress(&dinfo);
  jpeg_destroy_decompress(&dinfo);
  
  free(max_destroy_decompress_err);
  free(max_finish_decompress_err);
  
  fclose(fp);
}

