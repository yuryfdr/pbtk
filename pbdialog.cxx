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
#include "pbdialog.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <iostream>


PBDialog* PBDialog::dlg=NULL;

int PBDialog::dialog_iv_handler(int type,int par1,int par2){
  if(EVT_SHOW==type){
    dlg->update();
  }
  int ret = dlg->handle(type,par1,par2);
  if(!ret && EVT_KEYDOWN == type && KEY_BACK == par1 ){
    dlg->quit(false);
    return 1;
  }
  return ret;
}

PBDialog::PBDialog(const std::string& title):PBWidget(title,NULL),rethnd(NULL)
{
}

void PBDialog::run(pb_dialoghandler ds){
  setVisible(true);
  dlg=this;
  rethnd=ds;
  prev_handler = GetEventHandler();
  setFocused(true);
  placeWidgets();
  update(true);
  SetEventHandler(PBDialog::dialog_iv_handler);
}

void PBDialog::quit(bool isok){
  SetEventHandler(prev_handler);
  onQuit.emit(this,isok);
  setVisible(false);
}

