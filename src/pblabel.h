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
#ifndef PBLABEL_H
#define PBLABEL_H

#include "pbwidget.h"

class PBLabel:public PBWidget {
protected:
  int _maxWidth;
public:
  PBLabel(const std::string & name, PBWidget * parent):PBWidget(name, parent),_maxWidth(0) {
    _canBeFocused = false;
    _drawBorder=false;
  };
  virtual void draw();
  virtual int handle(int type, int par1, int par2);
  virtual int w() const;
  virtual void setMaxWidth(int val) { _maxWidth = val; }
  virtual int getMaxWidth() const { return _maxWidth; }
};

#endif
