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
#include "pblabel.h"

#include <iostream>

void PBLabel::draw()
{
  PBWidget::draw();
  DrawTextRect(x() + 4, y(), w() - 4, h(), (char *) _text.c_str(), ALIGN_CENTER | VALIGN_MIDDLE);
}

int PBLabel::handle(int type, int par1, int par2)
{
  if (type == EVT_KEYPRESS) {
    switch (par1) {
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
  return 0;
}

int PBLabel::w() const
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
