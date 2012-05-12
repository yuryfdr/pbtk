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
#ifndef PBFILECHOOSER_H
#define PBFILECHOOSER_H

#include "pbwidget.h"
#include "pblistbox.h"
#include "pbbutton.h"
#include "pbdialog.h"
#include "pbinput.h"
#include "pbcombobox.h"


#ifdef __EMU__
#define KBDOPTS KBD_SCREENTOP
#else
#define KBDOPTS 0
#endif
#include <iostream>
/**
  
*/
class PBDirList : public PBListBox{
  void load(const std::string&);
  void select(const std::string&);
  int handle(int type, int par1, int par2);
  std::string fname;
  std::string dname;
  public:
  PBDirList(const std::string nm,PBWidget*p):PBListBox(nm,p){}
  /**
    change dirrectory and select file
  */
  void setPath(const std::string&);
  /**
    return selected path
  */
  std::string getPath() const {
    if(!fname.empty())return dname+"/"+fname;
    return dname;
  }
  /**
    return current directory
  */
  std::string getDir() const {
    return dname;
  }
  /** 
    return relative file name
  */
  std::string getFname() const {
    return fname;
  }
};

class PBFileChooser : public PBDialog {
  PBDirList fileList;
public:
  PBButton bt_ok;
private:
  PBButton bt_cancel;
  PBComboBox cbx_pattern;
  PBInput lb_path;
  void placeWidgets();
  void on_pattern_changed(PBComboBox*);
public:
  void onButton(PBWidget*);
  enum OpenMode {PBFC_OPEN=0,PBFC_SAVE=1} omode;
  PBFileChooser(const char* title,OpenMode mode = PBFC_OPEN);
  std::string getPath()const{
    return fileList.getPath();
  }
  void setPath(const std::string& s){
    fileList.setPath(s);
    update();
  }
  void setMode(OpenMode mode){
    omode = mode;
    lb_path.setCanBeFocused(omode==PBFC_SAVE);
  }
  void setPattern(const char*);
  const char* getPattern(){return cbx_pattern.getText().c_str();}
  void setInitialName(const char*);
  void update(bool relayuot=false);
};

void OpenFileChooser(const char* title,const char* path,const char* pattern,int mode,pb_dialoghandler hproc);

#endif //PBFILECHOOSER_H

