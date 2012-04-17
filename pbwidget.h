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
#ifndef PBCONTROL_H
#define PBCONTROL_H

#include <stdio.h>
#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include <vector>
#include <sigc++/sigc++.h>
#include <boost/shared_ptr.hpp>

#undef IVSAPP
#include "inkview.h"
#include "PBImg.h"

#define BORDER_SPACE 4          // space between controls

/**
  base class
*/
class PBWidget {
  virtual int dispatchMsgToWidgets(int type, int par1, int par2);
protected:
  std::string _text;

  int _x;
  int _y;
  int _w;
  int _h;

  bool _canBeFocused;
  bool _drawBorder;
  bool _focused;
  bool _leaveOnKeys;
  bool _visible;

  static int defaultFontSize;
  static ifont* defaultFont;
  static ifont* captionFont;
  static ifont* defaultBoldFont;
  mutable ifont *_font;
  bool _useParentFont;

  PBWidget *_focusedWidget;

  PBWidget *_parent;
  boost::shared_ptr<PBImage> _image;

  std::vector <PBWidget*>_children;
  std::string _tag;

  void clearRegion();
  bool update_needed;
  virtual void placeWidgets(){};

public:

  PBWidget(const std::string & name, PBWidget * parent);
  virtual ~ PBWidget(){}

  int y() const { return _y; }//< coordinate
  void y(int val) { _y = val;update_needed=true; }//< coordinate
  int x() const { return _x; }//< coordinate
  void x(int val) { _x = val;update_needed=true; }//< coordinate

  virtual int w() const { return _w; }//< width
  void w(int val) { _w = val;update_needed=true; }
  virtual int h() const { return _h; }
  void h(int val) { _h = val;update_needed=true; }

  virtual void setSize(int X, int Y, int W, int H) {
    _x = X;
    _y = Y;
    _w = W;
    _h = H;
    update_needed=true;
  }

  bool canBeFocused() const { return _canBeFocused; }
  void setCanBeFocused(bool value);
  bool isFocused() const { return _focused; };
  void setFocused(bool value,bool update=true);
  PBWidget *getFocusedWidget() const;

  void setWidgetFont(ifont * value);//< set font
  ifont *getFont() const;

  bool isVisible() const {
    if (_parent != 0 && !_parent->isVisible())
      return false;
    return _visible;
  }
  void setVisible(bool val) { _visible = val; }

  PBWidget *getParent() const { return _parent; };

  boost::shared_ptr<PBImage> getImage() const { return _image; }
  void setImage(boost::shared_ptr<PBImage> val) { _image = val; }

  std::string getTag()const { return _tag; }
  void setTag(const std::string & val) { _tag = val; }

  virtual std::string getText()const { return _text; }
  virtual void setText(const std::string & val) { _text = val; }

  virtual int handle(int type, int par1, int par2);
  /**
    x y inside 
  */
  virtual bool eventInside(int X, int Y) {
    return (X > x() && X < (x() + w()) && Y > y() && Y < (y() + h()) );
  }
  virtual void draw();
  virtual void drawIfVisible();
  virtual void update(bool relayout=false);

  void addWidget(PBWidget * control);//< add child control
  void erase(PBWidget * control);     //< remove child control

  // signals
  sigc::signal<void,PBWidget*,bool,bool> onFocusChange;
  sigc::signal<void,PBWidget*,bool> onLeave;
  sigc::signal<void,PBWidget*> onFocusedWidgetChanged;
  virtual void widgetFocusChangeHandler(PBWidget * sender, bool focused,bool update);
  virtual void widgetLeaveHandler(PBWidget * sender, bool next);
};

typedef std::vector <PBWidget*>::iterator child_it;
typedef std::vector <PBWidget*>::const_iterator child_cit;
typedef std::vector <PBWidget*>::reverse_iterator child_rev_it;

class PBVScrollBar:public PBWidget {
  int pageHeight, totalHeight, value;
public:
  PBVScrollBar(const std::string & name, PBWidget * parent):PBWidget(name, parent), pageHeight(0), totalHeight(0), value(0) {
  }
  virtual void draw();
};

#endif
