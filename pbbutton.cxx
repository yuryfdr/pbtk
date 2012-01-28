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
#include "pbbutton.h"

#include <iostream>


void PBButton::draw()
{
  clearRegion();
  DrawTextRect(x() + 4, y(), w() - 4, h(), (char *) _text.c_str(), ALIGN_LEFT | VALIGN_MIDDLE);
  if (_focused)
    DrawSelection(x(), y(), w(), h(), BLACK);
  else {
    //DrawRect(x(), y(), w(), h(), BLACK);
    DrawLine(x()+2,y()    ,x()+w()-2,y()    ,BLACK);
    DrawLine(x()+2,y()+h(),x()+w()-2,y()+h(),BLACK);
    
    DrawLine(x()    ,y()+2,x()    ,y()+h()-2,BLACK);
    DrawLine(x()+w(),y()+2,x()+w(),y()+h()-2,BLACK);

    DrawLine(x()+2,y()    ,x(),    y()+2    ,BLACK);

    DrawLine(x()+2,y()+h(),x(),    y()+h()-2,BLACK);
    
    DrawLine(x()+w(),y()+2    ,x()+w()-2,y()    ,BLACK);
    DrawLine(x()+w(),y()+h()-2,x()+w()-2,y()+h(),BLACK);
  }
}

int PBButton::handle(int type, int par1, int par2)
{
  if (type == EVT_KEYPRESS) {
    switch (par1) {
    case KEY_OK:
      onPress.emit(this);
      break;
    case KEY_LEFT:
    case KEY_UP:
      onLeave.emit(this, false);
      break;
    case KEY_RIGHT:
    case KEY_DOWN:
      onLeave.emit(this, true);
      break;
    }
  }
  if (type == EVT_POINTERUP /*&& eventInside(par1,par2) */ ) {
    //printf("eventInside(par1,par2) %d %p\n",eventInside(par1,par2),this);
    if (eventInside(par1, par2) /*&& canBeFocused()*/) {
      PBWidget* pf=(_parent)?_parent->getFocusedWidget():NULL;
      setFocused(true);
      //if(_parent && pf!=this){_parent->update();}
      onPress.emit(this);
      //if(_parent)_parent->update();
      return 1;
    }
  }
  return 0;
}

int PBButton::w() const
{
  if (_w <= 0) {
    int width;
    width = StringWidth((char *) getText().c_str()) + 8;
    if (getMaxWidth() > 0 && width > getMaxWidth())
      return getMaxWidth();
    return width;
  }
  return PBWidget::w();
}

void PBCheckButton::draw()
{
  clearRegion();
  int hg = h();
  DrawRect(x()+4, y()+4, hg-8, hg-8, BLACK);
  if(_checked){
    DrawLine(x()+6, y()+hg/2, x()+hg/2, y()+hg-5, BLACK);
    DrawLine(x()+hg/2, y()+hg-5,x()+hg-5,y()+4, BLACK);
    DrawLine(x()+7, y()+hg/2, x()+hg/2, y()+hg-6, BLACK);
    DrawLine(x()+hg/2, y()+hg-6,x()+hg-6,y()+4, BLACK);
  }
  DrawTextRect(x() + 8 + hg, y(), w() - 4, h(), (char *) _text.c_str(), ALIGN_LEFT | VALIGN_MIDDLE);
  if (_focused)
    DrawSelection(x(), y(), w(), h(), BLACK);
  //else DrawRect(x(), y(), w(), h(), BLACK);
}

