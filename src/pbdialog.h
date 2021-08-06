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
#ifndef PBDIALOG_H
#define PBDIALOG_H

#include "pbwidget.h"
#include "pblistbox.h"
#include "pbbutton.h"


#ifdef __EMU__
#define KBDOPTS KBD_SCREENTOP
#else
#define KBDOPTS 0
#endif
#include <iostream>

class PBDialog;
/**
  called on dialog quit.
  \param bn for button caused quit
  \param dlg dialog
*/
typedef void(*pb_dialoghandler)(int bn,PBDialog *dlg);

class PBDialog : public PBWidget {
  static PBDialog* dlg;
  static int dialog_iv_handler(int type,int par1,int par2);
  iv_handler prev_handler;
protected:
  pb_dialoghandler rethnd;
  virtual void quit(bool isok);
public:
  PBDialog(const std::string& title);
  /** return height of the caption*/
  int captionHeight(){if(captionFont)return int(1.2*captionFont->height); return int(defaultFontSize/1.5);}
  virtual void draw();
  /**
    show the dialog 
    \param hproc - quit callback (must be called in derived class)
  */
  virtual void run(pb_dialoghandler hproc=NULL);
  /**
   signal is raised when dialog is hide<br>
   \b Prototype:
    void on_quit(PBDialog* dlg,bool isok)
   \param dlg - sender dialog
   \param isok - dlg is hiden by OK (or smth like) button
  */
  sigc::signal<void,PBDialog*,bool> onQuit;
};
#endif //PBFILECHOOSER_H

