#ifndef __PBVALUATOR_H__
#define __PBVALUATOR_H__

#include "pbbutton.h"
#include "pbinput.h"

class PBValuator : public PBWidget {
 protected:
  PBButton butb,bute;
  void on_but(PBWidget* b){
    if(b==&butb){value_-=step_;if(value_<min_)value_=min_;}
    else if(b==&bute){value_+=step_;if(value_>max_)value_=max_;}
    update();
    onChange.emit(this);
  }
  double value_;
  double min_,max_;
  double step_;
  //
 public:
  PBValuator(const std::string& nm,PBWidget*p):PBWidget(nm,p),butb("<",this),bute(">",this),
    value_(0),min_(0),max_(100),step_(10){
    _drawBorder=false;
    addWidget(&butb);
    addWidget(&bute);
    butb.onPress.connect(sigc::mem_fun(this,&PBValuator::on_but));
    bute.onPress.connect(sigc::mem_fun(this,&PBValuator::on_but));
  }
  void value(double v){value_=v;}
  double value()const{return value_;}
  void min(double v){min_=v;}
  double min()const{return min_;}
  void max(double v){max_=v;}
  double max()const{return max_;}
  void step(double v){step_=v;}
  double step()const{return step_;}
  void limits(double mn,double mx,double stp=-1){
    min_=mn,max_=mx;
    if(stp<0)step_=(mx-mn)/100.;
    else step_=stp;
  }
  /** emited when item is selected or if editable new item is added*/
  sigc::signal<void,PBValuator*> onChange;
};

class PBHSlider : public PBValuator {
 double x2v(int xx)const{
   double ret=(max_-min_)*(xx-x()-h())/(w()-3.*h());
   if( ret>max_)ret=max_;
   return ret;
 }
 int v2x()const{
   return int(x()+h()+(w()-3.*h())*value_/(max_-min_));
 }
 public:
  PBHSlider(const std::string& nm,PBWidget*p):PBValuator(nm,p){}
  int handle(int e,int p1,int p2){
    int ret=PBValuator::handle(e,p1,p2);
    if(ret==0){
      switch(e){
        case EVT_KEYUP:
        case EVT_KEYREPEAT:
        break;
        case EVT_POINTERUP:
          if(eventInside(p1,p2)){
            value_=x2v(p1);
            onChange.emit(this);
            update();
            return 1;
          }
        break;
      }
    }
    return ret;
  }
  void placeWidgets(){
    butb.setSize(x(),y(),h(),h());
    bute.setSize(x()+w()-h(),y(),h(),h());
  }
  void draw(){
    PBValuator::draw();
    DrawLine(x()+h(),y(),x()+w()-h(),y(),BLACK);
    DrawLine(x()+h(),y()+h(),x()+w()-h(),y()+h(),BLACK);
    FillArea(v2x(),y(),h(),h(),BLACK);
  }
};
#endif //__PBCOMBOBOX_H__

