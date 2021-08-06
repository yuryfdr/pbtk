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
#ifndef PBPAGEDLISTBOX_H
#define PBPAGEDLISTBOX_H

#include "pblistbox.h"


class PBPagedListBox:public PBListBox {
protected:
  std::vector < PBListBoxItem * >_items;
  int _topItemIndex, _bottomItemIndex;
  bool _graylinks;
  int totalHeight;
protected:
  std::string _caption;

  virtual void placeWidgets();
  virtual int getPageHeight();
  virtual int getFooterHeight();
  virtual int getCaptionHeight() const;
public:
  PBPagedListBox(const std::string & name, PBWidget * parent):PBListBox(name, parent)
  , _topItemIndex(-1), _bottomItemIndex(-1), _graylinks(false){ }

  virtual int handle(int type, int par1, int par2);
  virtual void draw();

  virtual void selectItem(int nmb);

  virtual void getPageItems(int startItemIndex, bool direct);
  virtual void clear();
};


#endif
