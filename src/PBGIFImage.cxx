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

#include "PBImg.h"
#include <stdio.h>
#include <stdlib.h>
#include "gif_lib.h"
#include <vector>

static int InterlacedOffset[] = { 0, 4, 2, 1 };
static int InterlacedJumps[] = { 8, 8, 4, 2 };

PBGIFImage::PBGIFImage(const char *fname){
  GifFileType *GifFile;
	if ((GifFile = DGifOpenFileName(fname)) == NULL) {
	    PrintGifError();
	    return;
	}
  std::vector<std::vector<unsigned char> > ScreenBuffer;
  ScreenBuffer.resize(GifFile->SHeight);
  int Size = GifFile->SWidth;
  for (int i = 0; i < GifFile->SHeight; i++) {
	  ScreenBuffer[i].resize(Size);
	  for (int j = 0; j < GifFile->SWidth; j++)
	    ScreenBuffer[i][j] = GifFile->SBackGroundColor;
  }
  //printf("BG:%d\n",GifFile->SBackGroundColor);
  GifRecordType RecordType;
  int ImageNum=0;
  int ExtCode;
  GifByteType *Extension;
  do {
	  if (DGifGetRecordType(GifFile, &RecordType) == GIF_ERROR) {
	    PrintGifError();
	    return;
	  }
	  switch (RecordType) {
	    case IMAGE_DESC_RECORD_TYPE:
		  if (DGifGetImageDesc(GifFile) == GIF_ERROR) {
		    PrintGifError();
		    return;
		  }
		  {
		    int Row = GifFile->Image.Top;
		    int Col = GifFile->Image.Left;
		    int Width = GifFile->Image.Width;
		    int Height = GifFile->Image.Height;
		    //printf("Image %d at (%d, %d) [%dx%d]:\n",++ImageNum, Col, Row, Width, Height);
		    if (GifFile->Image.Left + GifFile->Image.Width > GifFile->SWidth ||
		        GifFile->Image.Top + GifFile->Image.Height > GifFile->SHeight) {
		      fprintf(stderr, "Image %d is not confined to screen dimension, aborted.\n",ImageNum);
		      return;
		    }
		    if (GifFile->Image.Interlace) {
		      /* Need to perform 4 passes on the images: */
		      for (int i = 0; i < 4; i++)
			      for (int j = Row + InterlacedOffset[i]; j < Row + Height;j += InterlacedJumps[i]) {
			        if (DGifGetLine(GifFile, &ScreenBuffer[j][Col],Width) == GIF_ERROR) {
				        PrintGifError();
				        return;
			        }
			      }
		    } else {
		      for (int i = 0; i < Height; i++) {
			      if (DGifGetLine(GifFile, &ScreenBuffer[Row++][Col],Width) == GIF_ERROR) {
			        PrintGifError();
			        return;
			      }
		      }
		    }
		  }
		  break;
	    case EXTENSION_RECORD_TYPE:
		  /* Skip any extension blocks in file: */
		  if (DGifGetExtension(GifFile, &ExtCode, &Extension) == GIF_ERROR) {
		    PrintGifError();
		    return;
		  }
		  while (Extension != NULL) {
		    if (DGifGetExtensionNext(GifFile, &Extension) == GIF_ERROR) {
			    PrintGifError();
		      return;
		    }
		  }
		  break;
	    case TERMINATE_RECORD_TYPE:
		  break;
	    default:		    /* Should be traps by DGifGetRecordType. */
		  break;
	  }
  }while (RecordType != TERMINATE_RECORD_TYPE);
  ColorMapObject* ColorMap = (GifFile->Image.ColorMap	? GifFile->Image.ColorMap
		                        : GifFile->SColorMap);
  depth = 3;
  width = GifFile->SWidth;
  height = GifFile->SHeight;
  array = new unsigned char[width*height*depth];
  unsigned char *ptr=(unsigned char *)array;
	for (int i = 0; i < height; i++) {
	  for (int j = 0; j < width; j++) {
	    /*if(ScreenBuffer[i][j]==GifFile->SBackGroundColor){
		  GifColorType *ColorMapEntry = &ColorMap->Colors[ScreenBuffer[i][j]];
		  *ptr++ = 0;//ColorMapEntry->Red;
		  *ptr++ = 0;//ColorMapEntry->Green;
		  *ptr++ = 0;//ColorMapEntry->Blue;
	    }else{*/
		  GifColorType *ColorMapEntry = &ColorMap->Colors[ScreenBuffer[i][j]];
		  *ptr++ = ColorMapEntry->Red;
		  *ptr++ = ColorMapEntry->Green;
		  *ptr++ = ColorMapEntry->Blue;
		  //}
	  }
	}
}

