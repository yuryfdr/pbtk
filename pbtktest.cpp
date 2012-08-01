#include "pbvaluator.h"

class TestW : public PBWidget {
  PBHSlider hs;
  public:
  TestW():PBWidget("",NULL),hs("Horizontal",this){
    addWidget(&hs);
  }
  void placeWidgets(){
    setSize(0,0,ScreenWidth(),ScreenHeight());
    hs.setSize(10,10,w()-20,20);
  }
} *mw;


int mainh(int t,int p1,int p2){
  if(t==EVT_INIT){
    mw=new TestW();
    mw->setFocused(true);
    mw->update();
  }
  return mw->handle(t,p1,p2);
}

int main(){
  InkViewMain(mainh);
  return 0;
}
