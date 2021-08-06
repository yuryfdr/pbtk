#include "pbvaluator.h"
#include "pbfilechooser.h"

class TestW : public PBWidget {
  PBHSlider hs;
  PBButton bto;
  PBButton bts;
  PBButton btd;
  static void fopen_hndl(bool isok,PBFileChooser* dlg){
    if(isok){
      fprintf(stderr,"%s\n",dlg->getPath().c_str());
    }
    tw->update();
  }
  void on_bt(PBButton*b){
    if(b==&bto){
      OpenFileChooser("Open",".","*.*",PBFileChooser::PBFC_OPEN,(pb_dialoghandler)fopen_hndl);
    }
    if(b==&bts){
      OpenFileChooser("Save",".","*.*",PBFileChooser::PBFC_SAVE,(pb_dialoghandler)fopen_hndl);
    }
    if(b==&btd){
      OpenFileChooser("Select Dir","","*.*",PBFileChooser::PBFC_ODIR,(pb_dialoghandler)fopen_hndl);
    }
  }
  public:
  static TestW* tw;
  TestW():PBWidget("",NULL),hs("Horizontal",this)
          ,bto("Open",this),bts("Save",this),btd("Open Dir",this){
    addWidget(&hs);
    addWidget(&bto);
    addWidget(&bts);
    addWidget(&btd);
    bto.onPress.connect(sigc::mem_fun(this,&TestW::on_bt));
    bts.onPress.connect(sigc::mem_fun(this,&TestW::on_bt));
    btd.onPress.connect(sigc::mem_fun(this,&TestW::on_bt));
  }
  void placeWidgets(){
    setSize(0,0,ScreenWidth(),ScreenHeight());
    hs.setSize(10,10,w()-20,20);
    bto.setSize(10,40,100,30);
    bts.setSize(10,80,100,30);
    btd.setSize(10,120,100,30);
  }
} *mw;

TestW* TestW::tw;

int mainh(int t,int p1,int p2){
  if(t==EVT_INIT){
    TestW::tw=mw=new TestW();
    mw->setFocused(true);
    mw->update();
  }
  return mw->handle(t,p1,p2);
}

int main(){
  InkViewMain(mainh);
  return 0;
}
