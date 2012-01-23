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
#ifndef PBINPUT_H
#define PBINPUT_H

#include "pbwidget.h"
#include "pblabel.h"

#ifdef __EMU__
#define KBDOPTS KBD_SCREENTOP
#else
#define KBDOPTS 0
#endif
#include <iostream>

class PBInput : public PBLabel {
  static PBInput* inp;
  static void inp_kbd_hndl(char*s){
    if(s){
      inp->setText(s);
      inp->update();
    }
    if(inp->customh)inp->customh(s);
    inp->endEdit.emit(inp);
  }
  public:
  iv_keyboardhandler customh;
  private:
  int handle(int type, int par1, int par2){
    if( ((EVT_KEYDOWN==type && par1==KEY_OK) || 
        ( EVT_POINTERDOWN ==type && eventInside(par1,par2) ) )
        && canBeFocused() ){
      inp=this;
      static char buff[1024];
      strcpy(buff,getText().c_str());
      OpenKeyboard("",buff,1024,0|KBDOPTS,inp_kbd_hndl);
      return 1;
    }
    return PBLabel::handle(type,par1,par2);
  }
  public:
  PBInput(const std::string nm,PBWidget*p):PBLabel(nm,p),customh(NULL){
    setCanBeFocused(true);
    _drawBorder=true;
  }
  sigc::signal<void,PBInput*> endEdit;
};


#endif //PBFILECHOOSER_H

