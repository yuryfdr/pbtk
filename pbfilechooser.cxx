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
#include "pbfilechooser.h"
#include "convert.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

static PBFileChooser* fc=NULL;

void PBDirList::setPath(const std::string& nm){
  struct stat st;
  stat(nm.c_str(),&st);
  if(!S_ISDIR(st.st_mode)){
    std::string sdname=dname;
    size_t pspos=nm.find_last_of("/");
    if(pspos!=nm.npos && pspos!=0){
      dname=nm.substr(0,pspos);
      fname=nm.substr(pspos+1);
    }else{
      char buff[1024];
      if(getcwd(buff,1023))
        dname=buff;
        fname=nm;
    }
    if(sdname==dname){
      select(fname);
      if(_parent)_parent->update();
      return;
    }
  }else{
    dname=nm;
    fname="";
  }
  
  DIR *dir = iv_opendir(dname.c_str());
  clear();
  std::vector< std::string > dirs;
  std::vector< std::string > files;
  const char* pattern = fc->getPattern();
  if (dir != 0){
    struct dirent *ep;
    while ( (ep = iv_readdir(dir)) ){
      std::string nm(ep->d_name);
      if(nm=="." || nm=="..")continue;
      stat((dname+"/"+ep->d_name).c_str(),&st);
      if(S_ISDIR(st.st_mode))dirs.push_back(ep->d_name);
      else if(select_files){
        if(filename_match(ep->d_name,pattern))
          files.push_back(ep->d_name);
      }
    }
    iv_closedir(dir);
  }
  std::sort(dirs.begin(),dirs.end());
  std::sort(files.begin(),files.end());

  PBListBoxItem *itm = addItem("..");
  itm->setWidgetFont(defaultBoldFont);
  for(std::vector<std::string>::iterator it=dirs.begin();it!=dirs.end();++it){
    itm = addItem(*it);
    itm->setWidgetFont(defaultBoldFont);
  }
  for(std::vector<std::string>::iterator it=files.begin();it!=files.end();++it){
    itm = addItem(*it);
    if(!select_files)itm->setCanBeFocused(false);
  }
  //update(true);
  placeWidgets();
  if(!fname.empty())select(fname);
  else select("..");//selectItem((int)0,true);
  if(_parent)_parent->update();
  else update();
}
void PBDirList::select(const std::string& s){
  for(lbitem_it it=_items.begin();it!=_items.end();++it){
    if((*it)->getText()==s){selectItem(it-_items.begin(),true);return;}
  }
}

void PBDirList::load(const std::string& str){
  std::cerr<<"load:"<<str<<std::endl;
  if(str==".."){
    int ps = dname.find_last_of("/");
    if(ps!=(int)dname.npos)
      dname=dname.substr(0,ps);
    if(dname=="."){
      char buff[1024];
      if(getcwd(buff,1023))
        dname=buff;
    }
    setPath(dname);
  }else setPath(dname+"/"+str);
}
 
int PBDirList::handle(int type, int par1, int par2){
  int ret = PBListBox::handle(type,par1,par2);
  if (!ret && (type == EVT_KEYUP ||type == EVT_KEYDOWN || type == EVT_KEYREPEAT)){
    switch (par1){
      case KEY_OK:{
        PBListBoxItem *item = (PBListBoxItem *)getFocusedWidget();
        if (item == 0 || (item->y()+item->h() > y()+h()) || 
          item->y() < y() ){
          return 0;
        }
        if(type == EVT_KEYUP || type == EVT_KEYREPEAT){
          PBWidget* par = getParent();
          if(par){
            //par->update();
            if(type == EVT_KEYREPEAT){
              PBFileChooser* pfc=dynamic_cast<PBFileChooser*>(par);
              if(pfc){
                pfc->onButton(&pfc->bt_ok);
                return 1;
              }
            }
          }
          load(item->getText());
        }
        return 1;
      }
    }
  }
  if( !ret && (/*type == EVT_POINTERDOWN ||*/ type == EVT_POINTERUP)
           && eventInside(par1,par2)){
    for(lbitem_cit it = _items.begin();
                   it < _items.end();++it){
      if( (*it)->eventInside(par1,par2) ){
        if( (*it)->canBeFocused() ){
          (*it)->setFocused(true);
          //update();
          if(type == EVT_POINTERUP){
            load((*it)->getText());
          }
          //getParent()->update();
          return 1;
        }
      }
    }
  }
  return 0;
}


static void inp_h(char* s){
  if(s && fc)fc->setPath(s);
}
void PBFileChooser::on_pattern_changed(PBComboBox*bp){
  fileList.setPath(fileList.getPath());
}
PBFileChooser::PBFileChooser(const char* title,OpenMode mode):PBDialog(title),fileList("",this),bt_ok("OK",this),bt_cancel("Cancel",this),
  cbx_pattern("",this),lb_path("",this)//,
{
  cbx_pattern.editable(false);
  cbx_pattern.setText("*");
  cbx_pattern.onChange.connect(sigc::mem_fun(this,&PBFileChooser::on_pattern_changed));
  //if(mode==PBFC_OPEN)lb_path.setCanBeFocused(false);
  setMode(mode);
  lb_path.customh=inp_h;
  addWidget(&fileList);
  addWidget(&lb_path);
  addWidget(&cbx_pattern);
  addWidget(&bt_ok);
  addWidget(&bt_cancel);
  bt_ok.onPress.connect(sigc::mem_fun(this,&PBFileChooser::onButton));
  bt_cancel.onPress.connect(sigc::mem_fun(this,&PBFileChooser::onButton));
  fc=this;
}
void PBFileChooser::update(bool relayout){
  lb_path.setText(fileList.getPath());
  PBWidget::update(relayout);
}
void PBFileChooser::placeWidgets(){
  setSize(10,10,580,580);
  fileList.setSize(12,12+captionHeight(),576,470-captionHeight());
  cbx_pattern.setSize(12,485,576,25);
  lb_path.setSize(12,515,576,25);
  bt_ok.setSize(12,545,286,30);
  bt_cancel.setSize(300,545,286,30);
}

void PBFileChooser::onButton(PBWidget* but){
  if(but==&bt_ok && (!fileList.getFname().empty() || omode==PBFC_ODIR)){
    quit(true);
    rethnd(1,this);
  }
  if(but==&bt_cancel){
    quit(false);
    rethnd(0,this);
  }
}

void PBFileChooser::setPattern(const char* pat){
  if(pat && pat[0]){
    std::stringstream str(pat);
    std::vector<std::string> vpat;
    while(!str.eof() && !str.fail()){
      std::string tmp;
      std::getline(str,tmp);
      if(tmp.size()!=0)vpat.push_back(tmp);
    }
    if(!vpat.empty()){
      cbx_pattern.setItems(vpat);
      cbx_pattern.setText(vpat[0]);
    }
  }
}

void OpenFileChooser(const char* title,const char* path,const char* pattern, int mode,pb_dialoghandler hproc){
  if(!fc){
    fc=new PBFileChooser(title,(PBFileChooser::OpenMode)mode);
    fc->getFont();
  }
  else fc->setMode((PBFileChooser::OpenMode)mode);
  fc->setPattern(pattern);
  fc->setPath(path);
  fc->update(true);
  fc->run(hproc);
}

