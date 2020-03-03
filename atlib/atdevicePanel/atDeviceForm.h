#ifndef ATDEVICEFORM_H
#define ATDEVICEFORM_H

#include <QWidget>
#include <atDevicePanel.h>

//----------------------------------------------------
class atDevicePanel;
typedef atDevicePanel atDevicePanel;

//----------------------------------------------------
namespace Ui {
  class atDeviceForm;
}

//----------------------------------------------------
class atDeviceForm : public QWidget
{
  Q_OBJECT

public:
  explicit atDeviceForm(QWidget *parent = 0);
  explicit atDeviceForm(QWidget *parent = 0, atDevice *d=NULL);
  ~atDeviceForm();
  void Show();
  void ClosePanel();
  void Disp();

private:
  Ui::atDeviceForm *ui;
  atDevice *dev;
  atDevicePanel *pan;



};

#endif // ATDEVICEFORM_H
