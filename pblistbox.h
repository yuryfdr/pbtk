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
#ifndef PBLISTBOX_H
#define PBLISTBOX_H

#include "pbwidget.h"

class PBListBoxItem:public PBWidget {
protected:
  std::string _marker;
  int _maxHeight;
  int _align;
public:
  PBListBoxItem(const std::string & name, PBWidget * parent, int al = 1):PBWidget(name, parent)
      , _align(al) {
  }
  virtual bool eventInside(int X, int Y) {
    if (!_visible)
      return false;
    return PBWidget::eventInside(X,Y);
  }
  virtual void draw();

  virtual int h() const;
  int getMaxHeight() const { return _maxHeight; }
  void setMaxHeight(int val) { _maxHeight = val; }

  std::string getMarker()const { return _marker; }
  virtual void setMarker(const std::string & val) { _marker = val; }
};

typedef std::vector < PBListBoxItem * >::iterator lbitem_it;
typedef std::vector < PBListBoxItem * >::const_iterator lbitem_cit;

class PBListBox:public PBWidget {
protected:
  std::vector < PBListBoxItem * >_items;
  bool _graylinks;
  int totalHeight;
  int scrollDelta;
protected:
  std::string _caption;

  virtual int getCaptionHeight() const;
  void focusFirstIfCan();
public:
  virtual void placeWidgets();
  PBListBox(const std::string & name, PBWidget * parent):PBWidget(name, parent)
  , _graylinks(false), scrollDelta(0) { }
  ~PBListBox();

  bool isGray() const { return _graylinks; }

  virtual int handle(int type, int par1, int par2);
  virtual void draw();

  virtual std::string getCaption() const { return _caption;}
  virtual void setCaption(const std::string & val) { _caption = val; };
  virtual void selectItem(PBListBoxItem *);
  virtual void selectItem(int nmb);
  int itemsCount()const{return _items.size();}
  virtual PBListBoxItem *addItem(const std::string & text, const std::string & tag = "", int align = 1);
  virtual PBListBoxItem *addItem(boost::shared_ptr<PBImage> image, const std::string & tag = "",int align =1);
  PBListBoxItem *insertBefore(const PBListBoxItem* itm,const std::string & text);
  PBListBoxItem *insertAfter(const PBListBoxItem* itm,const std::string & text);
  PBListBoxItem *erase(const PBListBoxItem* itm,bool delete_item=true);

  template<typename Function>
  void forEachItem(Function itmz){
    for(lbitem_it it = _items.begin();it!=_items.end();++it){
      itmz( *it );
    }
  }
  template<typename Function>
  int findItem(int ib,Function itmz){
    if(ib<0||ib>=(int)_items.size())return -1;
    for(lbitem_it it = _items.begin()+ib;it!=_items.end();++it){
      if(itmz( *it ))return it-_items.begin();
    }
    return -1;
  }
  template<typename Function>
  int findItemRev(int ib,Function itmz){
    if(ib<0||ib>=(int)_items.size())return -1;
    for(lbitem_it it = _items.begin()+ib;it!=_items.begin();--it){
      if(itmz( *it ))return it-_items.begin();
    }
    return -1;
  }
  virtual void clear();
  virtual int getSelectedIndex() const;
  PBListBoxItem* getItem(int i){
    if(i>=0 && i<(int)_items.size()){
      return _items[i];
    }
    return NULL;
  }
};

#endif
