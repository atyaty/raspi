#include "atDeviceForm.h"
#include "ui_atDeviceForm.h"

atDeviceForm::atDeviceForm(QWidget *parent) :  QWidget(parent),  ui(new Ui::atDeviceForm){
  ui->setupUi(this);
}
//----------------------------------------------------------------------
atDeviceForm::atDeviceForm(QWidget *parent,atDevice *d) :  QWidget(parent),  ui(new Ui::atDeviceForm){
  ui->setupUi(this);
  dev=d;
  pan=new atDevicePanel(this);
  pan->SetDev(dev);
  pan->SetName(100);
  pan->SetParts(100,30);
  pan->SetWe();
  pan->SetWy(50);
  pan->move(1,1);
  pan->Show();
}
//----------------------------------------------------------------------
atDeviceForm::~atDeviceForm()
{
  delete ui;
  delete pan;
}
//-----------------------------------------------------------------------
void atDeviceForm::Show(){  this->show();}
//-----------------------------------------------------------------------
void atDeviceForm::Disp(){  pan->Disp();}
//-----------------------------------------------------------------------
void atDeviceForm::ClosePanel(){  pan->CloseForm();  this->close();}
