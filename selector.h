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
#ifndef _PBTK_NUMERIC_SELECTOR_
#define _PBTK_NUMERIC_SELECTOR_
#include "pbdialog.h"

class PBNumericSelector: public PBDialog {
  int elw;
  int _select;
  int cbx,cby;
  void draw_cell(int,int,int);
  void hit_cell();
  public:
  PBNumericSelector(const std::string& cap):PBDialog(cap),_select(0),cbx(1),cby(2){
  }
  void placeWidgets(){
    ifont* fnt = getFont();
    SetFont(getFont(),BLACK);
    elw=CharWidth('\0');
    if(fnt->height > elw) elw=fnt->height;
    elw*=2;
    setSize((ScreenWidth()-3*elw)/2,50,elw*3,elw*5+1.2*captionFont->height+2);
  }
  void draw();
  int handle(int type,int par1,int par2);
  int selected()const{return _select;};
  void selected(int s){_select=s;cbx=1;cby=2;};
  void reset(){_select=0;cbx=1;cby=2;};
  sigc::signal<void,PBNumericSelector*,bool> onSelect;
};
#endif //_PBTK_NUMERIC_SELECTOR_

