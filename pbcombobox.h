#ifndef __PBCOMBOBOX_H__
#define __PBCOMBOBOX_H__

#include "pbbutton.h"
#include "pbinput.h"

class PBComboBox : public PBWidget {
  PBInput inp;
  PBButton but;
  std::vector<std::string> items;
  void on_edit(PBInput*){
    const std::string& t=inp.getText();
    std::vector<std::string>::iterator it=std::find(items.begin(),items.end(),t);
    if(it>=items.end())items.push_back(t);
    onChange.emit(this);
  }
  static imenu* menu;
  static PBComboBox* box;
  static void menu_hnd(int p){
    if(p>=0&&p<(int)box->items.size()){
      box->inp.setText(box->items[p]);
      box->onChange.emit(box);
      box->update();
    }
  }
  void on_but(PBWidget*){
    box=this;
    if(menu)delete[] menu;
    int ms=items.size();
    menu = new imenu[ms+1];
    std::vector<std::string>::iterator it=items.begin();
    for(int i=0;i<(int)items.size();++i,++it){
      menu[ms-i-1].type=ITEM_ACTIVE;
      menu[ms-i-1].index=i;
      menu[ms-i-1].text=(char*)it->c_str();
    }
    menu[ms].type=0;
    menu[ms].index=0;
    menu[ms].text=NULL;
    OpenMenu(menu,0,x(),y()+h(),menu_hnd);
  }
  //
  public:
  PBComboBox(std::string nm,PBWidget*p):PBWidget(nm,p),inp("",this),but(">",this){
    _drawBorder=false;
    addWidget(&inp);
    addWidget(&but);
    inp.endEdit.connect(sigc::mem_fun(this,&PBComboBox::on_edit));
    but.onPress.connect(sigc::mem_fun(this,&PBComboBox::on_but));
  }
  void setSize(int X,int Y,int W,int H){
    PBWidget::setSize(X,Y,W,H);
    inp.setSize(X,Y,W-H-1,H);
    but.setSize(X+W-H,Y,H,H);
  }
  virtual std::string getText()const{
    return inp.getText();
  }
  virtual void setText(const std::string& t){
    inp.setText(t);
    inp.endEdit.emit(&inp);
    update();
  }
  void setItems(const std::vector<std::string>& v){
    items=v;
  }
  ///return copy of items
  std::vector<std::string> getItems()const{ return items;}
  
  bool editable()const{return inp.canBeFocused();}
  void editable(bool cbf){inp.setCanBeFocused(cbf);}
  sigc::signal<void,PBComboBox*> onChange;
};
#endif //__PBCOMBOBOX_H__

