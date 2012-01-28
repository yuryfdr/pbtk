/* Copyright (C) 2011-2012 Yury P. Fedorchenko (yuryfdr at users.sf.net)  */
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
#include "selector.h"

static const char* grid[][3]={
{"<<","x",">>"},
{"1","2","3"},
{"4","5","6"},
{"7","8","9"},
{"<-","0","ok"},
};
void PBNumericSelector::draw_cell(int y0,int i,int j){
  int cx=x()+i*elw;
  int cy=y0+j*elw;
  if(cbx==i && cby==j){
    SetFont(getFont(),WHITE);
    FillArea(cx,cy,elw,elw,BLACK);
  }else{
    SetFont(getFont(),BLACK);
  }
  DrawTextRect(cx,cy,elw,elw,grid[j][i],ALIGN_CENTER|VALIGN_MIDDLE);
}
void PBNumericSelector::draw(){
    PBWidget::draw();
    SetFont(captionFont,WHITE);
    
    DrawLine(x()+elw,y(),x()+elw,y()+h()-2,BLACK);
    DrawLine(x()+2*elw,y(),x()+2*elw,y()+h()-2,BLACK);
    
    FillArea(x(),y(),w(),1.2*captionFont->height,BLACK);
    char buff[128];
    sprintf(buff,"%s %d",getText().c_str(),_select);
    DrawTextRect(x(),y(),w(),1.2*captionFont->height,buff,ALIGN_LEFT|VALIGN_MIDDLE);
    SetFont(getFont(),BLACK);
    int cy=y()+1.2*captionFont->height;

    DrawLine(x()+2,cy+1*elw,x()+w()-4,cy+1*elw,BLACK);
    DrawLine(x()+2,cy+2*elw,x()+w()-4,cy+2*elw,BLACK);
    DrawLine(x()+2,cy+3*elw,x()+w()-4,cy+3*elw,BLACK);
    DrawLine(x()+2,cy+4*elw,x()+w()-4,cy+4*elw,BLACK);

    SetFont(getFont(),BLACK);
    for(int i=0;i<3;++i)
      for(int j=0;j<5;++j)
        draw_cell(cy,i,j);
}
void PBNumericSelector::hit_cell(){
  if(cby==0){
    if(cbx==1){
      quit(false);
      onSelect.emit(this,false);
    }else if(cbx==0){
      _select=0;
      quit(true);
      onSelect.emit(this,true);
    }else if(cbx==2){
      _select=-1;
      quit(true);
      onSelect.emit(this,true);
    }
    return;
  }
  if(cby==4 && cbx==2){
    quit(true);
    onSelect.emit(this,true);
    return;
  }
  if(cby==4 && cbx==1){
    _select*=10;
  } else if(cby==4 && cbx==0){
    _select/=10;
  }else{
    int nmb=1+cbx+3*(cby-1);
    _select*=10;
    _select+=nmb;
  }
  update();
}
int PBNumericSelector::handle(int type,int par1,int par2){
  if(EVT_KEYUP == type){
    switch(par1){
      case KEY_OK:
        hit_cell();
      return 1;
      case KEY_LEFT:
      --cbx;if(cbx<0)cbx=2;
      update();
      return 1;
      case KEY_RIGHT:
      ++cbx;if(cbx>2)cbx=0;
      update();
      return 1;
      case KEY_UP:
      --cby;if(cby<0)cby=3;
      update();
      return 1;
      case KEY_DOWN:
      ++cby;if(cbx>3)cby=0;
      update();
      return 1;
    }
  }
  if(EVT_POINTERUP == type){
    int cx=(par1-x())/elw;
    if(cx>=0 && cx<3){
      int cy=(par2-y()-1.2*captionFont->height)/elw;
      if(cy>=0&& cy<5){
        cbx=cx;cby=cy;
        hit_cell();
        update();
        return 1;
      }
    }
  }
  return 0;
}
