/* Copyright (C) 2009 AI */
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
//#include <inkinternal.h>
#include "pbwidget.h"

#include <iostream>


int PBWidget::defaultFontSize=20;
ifont* PBWidget::defaultFont=NULL;
ifont* PBWidget::captionFont=NULL;
ifont* PBWidget::defaultBoldFont=NULL;

//static bool scropened=false;

PBWidget::PBWidget(const std::string & name, PBWidget * parent): _text(name),_w(10), _h(10),
_canBeFocused(true), _drawBorder(true), _focused(false), _leaveOnKeys(true), _visible(true),
_font(NULL),_parent(parent), /*_image(NULL),*/update_needed(true)//, 
{
  if (!_parent) {
/*    if(ivstate.isopen){
      OpenScreen();
    }
    if(!defaultFont){
      defaultFont=OpenFont(DEFAULTFONT,defaultFontSize,1);
    }
    if(!defaultBoldFont){
      defaultBoldFont=OpenFont(DEFAULTFONTB,defaultFontSize,1);
    }
    if(!captionFont){
      captionFont=OpenFont(DEFAULTFONTB,defaultFontSize/1.5,1);
    }*/
    _y = 0;
    _x = 0;
    _useParentFont = false;
    _font=defaultFont;
  } else {
    _y = _parent->y();
    _x = _parent->x();
    _useParentFont = true;
  }
}

int PBWidget::dispatchMsgToWidgets(int type, int par1, int par2)
{
  PBWidget *focused = getFocusedWidget();
  int ret = 0;
  if (focused != 0)
    ret = focused->handle(type, par1, par2);
  if (!ret && ISPOINTEREVENT(type) ) {
    child_it it;
    for (it = _children.begin(); it != _children.end(); it++)
      if(*it!=focused)
        if (ret = (*it)->handle(type, par1, par2)) {
          return ret;
        }
  }
  return ret;
}

void PBWidget::widgetFocusChangeHandler(PBWidget * sender, bool focused,bool upd)
{
  // only one child control can have focus
  if (focused) {
    PBWidget *fc = getFocusedWidget();
    if (fc != 0 && fc != sender){
      fc->setFocused(false,upd);
    }
    _focusedWidget = sender;

    if(upd){
      if(_parent)_parent->update();
      else update();
    }
    onFocusedWidgetChanged.emit(this);
  } else {
  }
}

void PBWidget::widgetLeaveHandler(PBWidget * sender, bool next)
{
  // check if focus already moved
  if (!sender->_focused)
    return;
  // get iterator for sender
  child_it it;
  for (it = _children.begin(); it != _children.end(); ++it)
    if (*(it) == sender)
      break;

  child_rev_it last_it = _children.rbegin();

  // cycle focus through visible controls
  if (next) {
    do {
      if ( *it == *last_it) {
        if (_leaveOnKeys) {
          onLeave.emit(this, next);
          return;
        } else
          it = _children.begin();
      } else
        ++it;

      if ( (*it)->isVisible() && (*it)->canBeFocused()) {
        (*it)->setFocused(true);
        //update();
        break;
      }
    } while ( *it != sender);
  } else {
    do {
      if (it == _children.begin()) {
        if (_leaveOnKeys) {
          onLeave.emit(this, next);
          return;
        } else {
          for (it = _children.begin(); it != _children.end(); it++)
            if ( *it == *last_it )
              break;
          //it = _children.find(last_it->first);
        }
      } else
        --it;

      if ( (*it)->isVisible() && (*it)->canBeFocused()) {
        (*it)->setFocused(true);
        //update();
        break;
      }
    }
    while (*it != sender);
  }

}

void PBWidget::setCanBeFocused(bool value)
{
  _canBeFocused = value;
  if (!_canBeFocused && _focused)
    setFocused(false);
}

void PBWidget::setFocused(bool value,bool upd)
{
  if (_focused == value)
    return;
  // if lost focus, clear focus of child controls
  if (!value) {
    for (child_it it = _children.begin(); it != _children.end(); ++it) {
      (*it)->setFocused(false,upd);
    }
  } else { // if got focus
    if (!canBeFocused())
      return;
    PBWidget *fc = getFocusedWidget();
    if (fc != 0) {
      fc->setFocused(true,upd);
    } else {
      // focus first visible child control
      for (child_it it = _children.begin(); it != _children.end(); ++it) {
        if ((*it)->isVisible() && (*it)->canBeFocused()) {
          (*it)->setFocused(true,upd);
          break;
        }
      }
    }
  }
  _focused = value;

  onFocusChange.emit(this, value,upd);
}

void PBWidget::setWidgetFont(ifont * value)
{
  if (_font == value)
    return;
  _font = value;
  _useParentFont = false;
}
void PBWidget::replaceWidgetFont(ifont*old,ifont * value)
{
  if (_font == old) _font = value;
  for (child_it it = _children.begin(); it != _children.end(); ++it) (*it)->replaceWidgetFont(old,value);
}

ifont *PBWidget::getFont() const {
  if (_useParentFont && _parent != 0)
    return _parent->getFont();
  if(!_font){
    if(!defaultFont){
      defaultFont=OpenFont(DEFAULTFONT,defaultFontSize,1);
    }
    _font=defaultFont;
  }
  if(!defaultBoldFont){
    defaultBoldFont=OpenFont(DEFAULTFONTB,defaultFontSize,1);
  }
  if(!captionFont){
    captionFont=OpenFont(DEFAULTFONTB,defaultFontSize/1.5,1);
  }
  return _font;
}
PBWidget *PBWidget::getFocusedWidget() const {
  child_cit it;
  //= _children.find(_focusedWidget);
  for (it = _children.begin(); it != _children.end(); it++)
    if (*it == _focusedWidget)
      break;

  if (it == _children.end())
    return 0;

  return *it;

  //for (child_it it = _children.begin(); it != _children.end(); it++)
  //{
  //  if(it->second->GetFocused())
  //    return it->second;
  //}

  return 0;
}
int PBWidget::handle(int type, int par1, int par2)
{
  int ret = dispatchMsgToWidgets(type, par1, par2);
  if (type == EVT_POINTERDOWN && eventInside(par1, par2) && isVisible() && canBeFocused()) {
    setFocused(true);
    //update();
  }
  return ret;
}

void PBWidget::clearRegion()
{
  FillArea(x(), y(), w(), h(), WHITE);
}

void PBWidget::draw()
{
  clearRegion();
  if(update_needed){
    placeWidgets();
    update_needed=false;
  }
  if (_drawBorder)
    DrawRect( x(), y(), w(), h(), BLACK);
  for (child_it it = _children.begin(); it != _children.end(); it++) {
    (*it)->drawIfVisible();
  }

  if (_drawBorder && _focused)
    DrawRect( x() + BORDER_SPACE / 2, y() + BORDER_SPACE / 2, w() - BORDER_SPACE, h() - BORDER_SPACE, BLACK);
}

void PBWidget::drawIfVisible()
{
  if (isVisible()) {
    SetFont(getFont(), BLACK);
    draw();
  }
}

void PBWidget::update(bool relayout)
{
  if(relayout)update_needed=true;
  drawIfVisible();
  PartialUpdate( x(), y(), w(), h());
}

void PBWidget::addWidget(PBWidget * control)
{
  _children.push_back(control);
  control->onFocusChange.connect(sigc::mem_fun(this,&PBWidget::widgetFocusChangeHandler));
  control->onLeave.connect(sigc::mem_fun(this,&PBWidget::widgetLeaveHandler));
}
void PBWidget::erase(PBWidget * itm)
{
  std::vector < PBWidget* >::iterator it = std::find(_children.begin(),_children.end(),itm);
  if(it!=_children.end()){
    std::vector < PBWidget* >::iterator ite = _children.erase(it);
  }
}


