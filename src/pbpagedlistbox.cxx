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
#include "pbpagedlistbox.h"

#include <iostream>


void PBPagedListBox::selectItem(int nmb){
  if(nmb >= 0 && nmb < (int)_items.size()){
    _items[nmb]->setFocused(true);
    int iy=_items[nmb]->y()+_items[nmb]->h();
    if(iy > (y() + h()) || (iy< y() )){
      scrollDelta+=_items[nmb]->y();
    }
  }
}
int PBPagedListBox::handle(int type, int par1, int par2)
{
  if(PBWidget::handle(type,par1,par2))return 1;
  if (type == EVT_KEYPRESS) {
    switch (par1) {
    case KEY_DOWN:
      if (getFocusedWidget() == 0 || _children.size() == 0) {
        onLeave.emit(this, true);
      } else if (getFocusedWidget() != 0) {
        // if bottom item selected and there's next page, go to this page
        if (getFocusedWidget() == _items[_bottomItemIndex]
            && _bottomItemIndex < (int) _items.size() - 1) {
          _items[_bottomItemIndex + 1]->setFocused(true);
          getPageItems(_bottomItemIndex + 1, true);
          update();
        } else {
          widgetLeaveHandler(getFocusedWidget(), true);
          //update();
        }
      }
      return 1;
    case KEY_UP:
      if (getFocusedWidget() == 0 || _children.size() == 0) {
        onLeave.emit(this, false);
      } else if (getFocusedWidget() != 0) {
        // if top item selected and there's previous page, go to this page
        if (getFocusedWidget() == _items[_topItemIndex] && _topItemIndex > 0) {
          _items[_topItemIndex - 1]->setFocused(true);
          getPageItems(_topItemIndex - 1, false);
          update();
        } else {
          widgetLeaveHandler(getFocusedWidget(), false);
          //Update();
        }
      }
      return 1;
    case KEY_RIGHT:
      if (_children.size() == 0) {
        onLeave.emit(this, true);
      }
      // if there's no next pages, leave
      else if (_bottomItemIndex == (int) _items.size() - 1) {
        onLeave.emit(this, true);
      }
      // show next page
      else {
        getPageItems(_bottomItemIndex + 1, true);
        _items[_topItemIndex]->setFocused(true);
        update();
      }
      return 1;
    case KEY_LEFT:
      if (_children.size() == 0) {
        onLeave.emit(this, false);
      }
      // if there's no previous pages, leave
      else if (_topItemIndex == 0) {
        onLeave.emit(this, false);
      }
      // show previous page
      else {
        getPageItems(_topItemIndex - 1, false);
        _items[_topItemIndex]->setFocused(true);
        update();
      }
      return 1;
    }
  }

  return 0;
}

void PBPagedListBox::placeWidgets()
{
  if (_items.size() == 0)
    return;
  getPageItems(_topItemIndex, true);
  int top = y() + BORDER_SPACE + getCaptionHeight() + BORDER_SPACE;
  for (int i = _topItemIndex; i <= _bottomItemIndex; i++) {
    _items[i]->setMaxHeight(getPageHeight());
    _items[i]->setSize(x() + BORDER_SPACE, top, w() - BORDER_SPACE * 2, 0);
    top += _items[i]->h();
  }
}

int PBPagedListBox::getPageHeight()
{
  return h() - BORDER_SPACE * 3 - getCaptionHeight() - getFooterHeight();
}

int PBPagedListBox::getCaptionHeight() const
{
  return getCaption().size() > 0 ? TextRectHeight(w() - BORDER_SPACE * 2, (char *) getCaption().c_str(), ALIGN_CENTER) : 0;
}

int PBPagedListBox::getFooterHeight()
{
  if (_topItemIndex > 0 || _bottomItemIndex < (int) _items.size() - 1)
    return TextRectHeight(w(), "<<<", ALIGN_LEFT) + BORDER_SPACE;
  return 0;
}

void PBPagedListBox::getPageItems(int startItemIndex, bool direct)
{
  _topItemIndex = _bottomItemIndex = -1;
  int height = 0;
  int pageHeight = getPageHeight();
  if (direct) {
    _topItemIndex = startItemIndex;
    for (int i = startItemIndex; i < (int) _items.size(); i++) {
      _items[i]->setMaxHeight(pageHeight);
      _items[i]->setSize(0, 0, w() - BORDER_SPACE * 2, 0);
      height += _items[i]->h();
      if (height > pageHeight)
        break;
      _bottomItemIndex = i;
    }
  } else {
    _bottomItemIndex = startItemIndex;
    for (int i = startItemIndex; i >= 0; i--) {
      _items[i]->setMaxHeight(pageHeight);
      _items[i]->setSize(0, 0, w(), 0);
      height += _items[i]->h();
      if (height > pageHeight)
        break;
      _topItemIndex = i;
    }
  }
}

void PBPagedListBox::draw()
{
  clearRegion();
  placeWidgets();
  if (_drawBorder)
    DrawRect(x(), y(), w(), h(), BLACK);
  if (getCaption().size() > 0) {
    DrawTextRect(x() + BORDER_SPACE, y() + BORDER_SPACE, w() - BORDER_SPACE * 2, getCaptionHeight(), (char *) getCaption().c_str(), ALIGN_CENTER);
    DrawLine(x() + BORDER_SPACE, y() + BORDER_SPACE + getCaptionHeight(), x() + w() - BORDER_SPACE, y() + BORDER_SPACE + getCaptionHeight(), BLACK);
  }
  if (_topItemIndex >= 0) {
    for (int i = _topItemIndex; i <= _bottomItemIndex; i++) {
      _items[i]->drawIfVisible();
    }
  }

  if (_topItemIndex > 0)
    DrawTextRect(x() + BORDER_SPACE, y() + h() - getFooterHeight(), w() - BORDER_SPACE * 2, getFooterHeight(), "<<<", ALIGN_LEFT);
  if (_bottomItemIndex < (int) _items.size() - 1)
    DrawTextRect(x() + BORDER_SPACE, y() + h() - getFooterHeight(), w() - BORDER_SPACE * 2, getFooterHeight(), ">>>", ALIGN_RIGHT);
  if (_drawBorder && _focused)
    DrawRect(x() + BORDER_SPACE / 2, y() + BORDER_SPACE / 2, w() - BORDER_SPACE, h() - BORDER_SPACE, BLACK);
}


void PBPagedListBox::clear()
{
  PBListBox::clear();
  _topItemIndex = _bottomItemIndex = -1;
}

