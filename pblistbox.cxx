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
#include "pblistbox.h"

#include <iostream>

void PBListBoxItem::draw()
{
  clearRegion();
  PBListBox *lb = dynamic_cast < PBListBox * >(_parent);
  if (lb && lb->isGray() && canBeFocused()) {
    FillArea(x() + 1, y() + 1, w() - 2, h() - 2, LGRAY);
  }
  int markerWidth = StringWidth((char *)getMarker().c_str());
  if (_image) {
    int W = w() - markerWidth - 3;
    int H = h() - 3;
    if (_image->w() > W)
      _image->draw(x() + 3, y() + 3, W, W / _image->w() * H);
    else if (_align == ALIGN_CENTER) {
      _image->draw(x() + (W - _image->w()) / 2, y() + 3);
    } else if (_align == ALIGN_RIGHT) {
      _image->draw(x() + 3 + W - _image->w(), y() + 3);
    } else
      _image->draw(x() + 3, y() + 3);

  } else if (!_children.empty()) {
    std::string text = getText();
    for (size_t i = 0; i < _children.size(); ++i) {
      text += _children[i]->getText();
    }
    DrawTextRect(x() + 3, y() + 3, w() - markerWidth - 3, h() - 3, (char *)text.c_str(), _align);
  } else
    DrawTextRect(x() + 3 + markerWidth, y() + 3, w() - markerWidth - 3, h() - 3,
                 (char *)getText().c_str(), _align);
  if (getMarker().size() > 0) {
    FillArea(x(), y(), markerWidth, h(), LGRAY);
    DrawTextRect(x() + 1, y() + 3, markerWidth, h() - 3, (char *)getMarker().c_str(), ALIGN_RIGHT);
  }

  if (_focused)
    DrawSelection(x() + markerWidth, y(), w() - markerWidth, h(), BLACK);
  else if(getParent() && getParent()->getFocusedWidget() == this && canBeFocused()){
    DrawLine(x() + markerWidth, y()+h()-BORDER_SPACE,x()+w(),y()+h()-BORDER_SPACE,BLACK);
  }
}

int PBListBoxItem::h() const
{
  if (_h <= 0) {
    int height;
    if (_image)
      height = _image->h();
    else {
      std::string text = getText();
      for (size_t i = 0; i < _children.size(); ++i)
        text += _children[i]->getText();
      if (text.empty())
        text = "I";
      height = TextRectHeight(w() - 6 - StringWidth((char *)getMarker().c_str()),
                              (char *)text.c_str(), _align) + 6;
    }
    //std::cerr<<"Sizes:"<<height-6<<'\t'<<getFont()->size<<std::endl;
    if (getMaxHeight() > 0 && height > getMaxHeight())
      return getMaxHeight();

    return height;
  }
  return PBWidget::h();
}

PBListBox::~PBListBox()
{
  for (lbitem_it it = _items.begin(); it != _items.end(); it++)
    delete(*it);
}

bool PBListBox::scrollTo(PBListBoxItem *itm)
{
  if(!itm)return false;
  int iy = itm->y() + itm->h();
  int H=h()-2*BORDER_SPACE;
  printf("iy %d %d %d %d\n",iy,y()+h(),itm->y(),y() + getCaptionHeight());
  if (iy > y() + H) {
    scrollDelta = iy-y()-h();//(itm->y() + scrollDelta) - y() - h();
    return true;
  } else if (itm->y() < y() + getCaptionHeight()) {
    scrollDelta = iy /*+ scrollDelta*/ - y() - getCaptionHeight()+BORDER_SPACE;
    return true;
  }
  return false;
}

void PBListBox::selectItem(PBListBoxItem * itm,bool upd)
{
  if (itm) {
    std::vector < PBListBoxItem * >::iterator it = std::find(_items.begin(), _items.end(), itm);
    if (it != _items.end()) {
      selectItem(it - _items.begin(),upd);
    }
  }
}

void PBListBox::selectItem(int nmb,bool upd)
{
  if (nmb >= 0 && nmb < (int)_items.size()) {
    bool scr=scrollTo(nmb);
    if(scr || update_needed)placeWidgets();
    _items[nmb]->setFocused(true,upd);
  }
}
void PBListBox::focusFirstIfCan(){
  placeWidgets();
  for(lbitem_it it = _items.begin();it<_items.end();++it){
    if( (*it)->y()>y() && (*it)->y()<y()+h() && (*it)->canBeFocused()){
      scrollTo(*it);
      (*it)->setFocused(true);break;
    }
  }
}
int PBListBox::handle(int type, int par1, int par2)
{
  static bool was_repeat(false);
  PBWidget::handle(type, par1, par2);
  PBListBoxItem* si=getSelectedItem();
  if(type==EVT_POINTERLONG && eventInside(par1, par2) && si && si->eventInside(par1, par2)){
    if( si->eventInside(par1,par2) )onItemAction.emit(this,getSelectedIndex());
    return 1;
  }

  if (EVT_POINTERUP == type && eventInside(par1, par2) && (x() +  w() -3.*BORDER_SPACE) < par1) {
    if (totalHeight > (h() - getCaptionHeight())
        && scrollDelta < (totalHeight - (h() - getCaptionHeight()))
        && par2 > (y() + 2. / 3. * h())) {
      scrollDelta += h() * 0.9;
      if (scrollDelta > (totalHeight - (h() - getCaptionHeight())))
        scrollDelta = (totalHeight - h() - getCaptionHeight());
      update(true);
      return 1;
    }
    if (totalHeight > h() && scrollDelta > 0 && par2 < (y() + 1. / 3. * h())) {
      scrollDelta -= h() * 0.9;
      if (scrollDelta < 0)
        scrollDelta = 0;
      update(true);
      return 1;
    }
  }
  if ( (EVT_KEYRELEASE == type && _scrollOnRL ) || (EVT_KEYREPEAT == type && par2 == 1)) {
    if (totalHeight > h()) {
      if (par1 == KEY_RIGHT && (scrollDelta < (totalHeight - h()))) {
        scrollDelta += (EVT_KEYREPEAT != type) ? getFont()->size : (h() * 0.9);
        if (scrollDelta > (totalHeight - h()))
          scrollDelta = (totalHeight - h());
        if(EVT_KEYREPEAT == type){focusFirstIfCan();was_repeat=true;}
        update(true);
        return 1;
      }
      if (par1 == KEY_LEFT && scrollDelta > 0) {
        scrollDelta -= (EVT_KEYREPEAT != type) ? getFont()->size : (h() * 0.9);
        if (scrollDelta < 0)
          scrollDelta = 0;
        if(EVT_KEYREPEAT == type){focusFirstIfCan();was_repeat=true;}
        update(true);
        return 1;
      }
      if ((par1 == KEY_PREV ) && (scrollDelta < (totalHeight - h()))) {
        scrollDelta += (h() * 0.9);
        if (scrollDelta > (totalHeight - h()))
          scrollDelta = (totalHeight - h());
        focusFirstIfCan();
        update(true);
        return 1;
      }
      if ((par1 == KEY_NEXT ) && scrollDelta > 0) {
        scrollDelta -= (h() * 0.9);
        if (scrollDelta < 0)
          scrollDelta = 0;
        focusFirstIfCan();
        update(true);
        return 1;
      }
    }
    if ( par1==KEY_OK && type==EVT_KEYREPEAT ){
      onItemAction.emit(this,getSelectedIndex());
      return 1;
    }
  }

  if (EVT_KEYRELEASE == type && !was_repeat) {
    PBWidget *fc = getFocusedWidget();
    int fc_itm = -1;
    switch (par1) {
    case KEY_DOWN:
      for (size_t i = 0; i < _items.size(); ++i) {
        if (fc && _items[i] == fc)
          fc_itm = i;
        if (_items[i]->canBeFocused()) {
          if (!fc || ((int)i > fc_itm && fc_itm != -1)) {
            if (_items[i]->y() + _items[i]->h() > y() + h()-2*BORDER_SPACE) {
              scrollDelta = (_items[i]->y() + _items[i]->h() + scrollDelta) - y() - h() + getCaptionHeight()+4*BORDER_SPACE;
              update_needed = true;
            }
            _items[i]->setFocused(true);
            return 1;
          }
        }
      }
      break;
    case KEY_UP:
      for (int i = _items.size() - 1; i >= 0; --i) {
        if (fc && _items[i] == fc)
          fc_itm = i;
        if (_items[i]->canBeFocused()) {
          if (!fc || (i < fc_itm && fc_itm != -1)) {
            if (_items[i]->y() < y() + getCaptionHeight()+BORDER_SPACE) {
              scrollDelta = _items[i]->y() + scrollDelta - y() - getCaptionHeight()-BORDER_SPACE;
              update_needed = true;
            }
            _items[i]->setFocused(true);
            return 1;
          }
        }
      }
      break;
    }
  }
  if (type == EVT_KEYRELEASE && !was_repeat) {
    switch (par1) {
    case KEY_DOWN:
      if (getFocusedWidget() == 0 || _children.size() == 0) {
        onLeave.emit(this, true);
      } else if (getFocusedWidget() != 0) {
        if (getFocusedWidget() == *(_items.rbegin())) {
          widgetLeaveHandler(getFocusedWidget(), true);
        }
      }
      return 1;
    case KEY_UP:
      if (getFocusedWidget() == 0 || _children.size() == 0) {
        onLeave.emit(this, false);
      } else if (getFocusedWidget() != 0) {
        if (getFocusedWidget() == *_items.begin()) {
          widgetLeaveHandler(getFocusedWidget(), false);
        }
      }
      return 1;
    case KEY_RIGHT:
      onLeave.emit(this, true);
      return 1;
    case KEY_LEFT:
      onLeave.emit(this, false);
      return 1;
    }
  }
  if( EVT_KEYPRESS==type)was_repeat=false;
  return 0;
}

void PBListBox::placeWidgets()
{
  if (_items.size() == 0)
    return;
  int top = y() + BORDER_SPACE + getCaptionHeight() + BORDER_SPACE;
  totalHeight = BORDER_SPACE + getCaptionHeight() + BORDER_SPACE + 10;
  top -= scrollDelta;
  printf("top %d %d\n",top,scrollDelta);
  for (size_t i = 0; i < _items.size(); ++i) {
    _items[i]->setMaxHeight(0);
    _items[i]->setSize(x() + BORDER_SPACE, top, w() - BORDER_SPACE * 4, 0);
    top += _items[i]->h();
    totalHeight += _items[i]->h();
  }
}

int PBListBox::getCaptionHeight() const
{
  return getCaption().size() > 0 ? TextRectHeight(w() - BORDER_SPACE * 2,
                                                  (char *)getCaption().c_str(), ALIGN_CENTER) : 0;
}

void PBListBox::draw()
{
  clearRegion();
  if (update_needed) {
    placeWidgets();
    update_needed = false;
  }
  int cH = getCaptionHeight();
  if (totalHeight > (h() - cH)) {
    int scrH = h() - 1.5* BORDER_SPACE - cH;
    FillArea(x() + w() - 2.5 * BORDER_SPACE,
             y() + cH + BORDER_SPACE
             , 2*BORDER_SPACE  , scrH, LGRAY);
    DrawRect(x() + w() - 2.5 * BORDER_SPACE,
             y() + cH + BORDER_SPACE
             , 2*BORDER_SPACE  , scrH, BLACK);
    FillArea(x() + w() - 2.5 * BORDER_SPACE,
             y() + BORDER_SPACE + scrollDelta * scrH / (totalHeight) + cH
             , 2 * BORDER_SPACE,
             scrH * scrH / totalHeight /*- BORDER_SPACE*/, BLACK);
  }
  if (_drawBorder)
    DrawRect(x(), y(), w(), h(), BLACK);
  if (getCaption().size() > 0) {
    DrawTextRect(x() + BORDER_SPACE, y() + BORDER_SPACE / 2,
                 w() - BORDER_SPACE * 2, cH, (char *)getCaption().c_str(), ALIGN_CENTER);
    DrawLine(x() + BORDER_SPACE,
             y() + BORDER_SPACE / 2 + cH, x() + w() - BORDER_SPACE, y() + BORDER_SPACE + cH, BLACK);
  }

  SetClip(x() + BORDER_SPACE / 2, y() + BORDER_SPACE / 2 + cH + 1,
          w() - BORDER_SPACE, h() - BORDER_SPACE - cH - 2);
  for (size_t i = 0; i < _items.size(); i++) {
    _items[i]->drawIfVisible();
  }
  SetClip(0, 0, ScreenWidth(), ScreenHeight());
  if (_drawBorder && _focused)
    DrawRect(x() + BORDER_SPACE / 2, y() + BORDER_SPACE / 2,
             w() - BORDER_SPACE, h() - BORDER_SPACE, BLACK);
}

PBListBoxItem *PBListBox::addItem(const std::string & text, const std::string & tag, int align)
{
  char itemName[20];
  sprintf(itemName, "item%d", _items.size() + 1);
  PBListBoxItem *newItem = new PBListBoxItem(itemName, this, align);
  newItem->setText(text);
  newItem->setTag(tag);
  _items.push_back(newItem);
  addWidget(newItem);
  if (getFocusedWidget() == 0 && _focused)
    newItem->setFocused(true,false);
  update_needed = true;
  return newItem;
}

PBListBoxItem *PBListBox::insertBefore(const PBListBoxItem * itm, const std::string & text)
{
  std::vector < PBListBoxItem * >::iterator it = std::find(_items.begin(), _items.end(), itm);
  if (it != _items.end()) {
    PBListBoxItem *newItem = new PBListBoxItem("", this, 1);
    newItem->setText(text);
    _items.insert(it, newItem);
    addWidget(newItem);
    newItem->setCanBeFocused(true);
    if (getFocusedWidget() == 0 && _focused)
      newItem->setFocused(true);
    update_needed = true;
    return newItem;
  } else
    return addItem(text);
}

PBListBoxItem *PBListBox::insertAfter(const PBListBoxItem * itm, const std::string & text)
{
  std::vector < PBListBoxItem * >::iterator it = std::find(_items.begin(), _items.end(), itm);
  if (it != _items.end()) {
    ++it;
    PBListBoxItem *newItem = new PBListBoxItem("", this, 1);
    newItem->setText(text);
    _items.insert(it, newItem);
    addWidget(newItem);
    newItem->setCanBeFocused(true);
    if (getFocusedWidget() == 0 && _focused)
      newItem->setFocused(true);
    update_needed = true;
    return newItem;
  } else
    return addItem(text);
}

PBListBoxItem *PBListBox::erase(const PBListBoxItem * itm, bool delete_item)
{
  std::vector < PBListBoxItem * >::iterator it = std::find(_items.begin(), _items.end(), itm);
  if (it != _items.end()) {
    PBListBoxItem *erased = *it;
    std::vector < PBListBoxItem * >::iterator ite = _items.erase(it);
    PBWidget::erase(erased);
    PBListBoxItem *itm = NULL;
    if (ite == _items.end()) {
      if (_items.size() != 0)
        itm = *(--ite);
    } else
      itm = *ite;
    if (itm) {
      if (getFocusedWidget() == 0 && _focused)
        itm->setFocused(true);
    }
    update_needed = true;
    if (delete_item) {
      delete erased;
    } else {
      return erased;
    }
  }
  return NULL;
}

PBListBoxItem *PBListBox::addItem(boost::shared_ptr<PBImage> image, const std::string & tag, int align)
{
  PBListBoxItem *newItem = addItem("", tag, align);
  newItem->setImage(image);
  update_needed = true;
  return newItem;
}

void PBListBox::clear()
{
  _children.clear();
  for (lbitem_it it = _items.begin(); it != _items.end(); ++it) {
    delete(*it);
  }
  _items.clear();
  _focusedWidget = NULL;
  scrollDelta=0;
  update_needed = true;
}

int PBListBox::getSelectedIndex() const
{
  PBWidget* s=getFocusedWidget();
  if(!s)return -1;
  for (lbitem_cit it = _items.begin(); it != _items.end(); ++it) {
    if( (*it)==s) //((*it)->isFocused())     //!!
      return it - _items.begin();
  }

  return -1;
}
