/* Copyright (C) 2009 AI */
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
#ifndef PBBUTTON_H
#define PBBUTTON_H

#include "pbwidget.h"

class PBButton:public PBWidget {
protected:
  int _maxWidth;
public:
  PBButton(const std::string & name, PBWidget * parent):PBWidget(name, parent){
    setText(name);
  } 
  virtual void draw();
  virtual int handle(int type, int par1, int par2);

  virtual int w() const;
  virtual void setMaxWidth(int value) { _maxWidth = value; }
  virtual int getMaxWidth() const { return _maxWidth; };
  /** emmited on button press*/
  sigc::signal<void,PBButton*> onPress;
};

class PBCheckButton : public PBButton {
  bool _checked;
  virtual void on_check(PBWidget*){
    _checked=!_checked;
    update();
  }
 public:
  PBCheckButton(const std::string &nm,PBWidget*p) : PBButton(nm,p),_checked(false){
    onPress.connect(sigc::mem_fun(this,&PBCheckButton::on_check));
  }
  /** return true if check button is checked*/
  bool checked()const{return _checked;}
  void checked(bool c){_checked=c;update();}
  void draw();
};


#endif
