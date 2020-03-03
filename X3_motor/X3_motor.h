#ifndef X3_MOTOR_H
#define X3_MOTOR_H
#include <gtkmm.h>

#include <atI2C.h>
#include <atI2C_X3.h>


#define I2C_PI_ID 0x56

class X3_motor : public Gtk::Window

{
private:
  Gtk::Grid   grid;
  Gtk::Button buttonUp;
  Gtk::Button buttonDn;
  Gtk::Button buttonEN;
  Gtk::Button buttonFR_up;
  Gtk::Button buttonFR_down;
  Gtk::Button buttonDEST;
  Gtk::CheckButton check_button_en;
  Gtk::CheckButton check_button_upstop;
  Gtk::CheckButton check_button_dnstop;
  Gtk::Entry  input_dbw;
  Gtk::Entry  input_dbr;
  Gtk::Entry  input_error;
  Gtk::Entry  input_di;
  Gtk::Entry  input_do;
  Gtk::Entry  input_licznik;
  Gtk::Entry  input_poz;
  Gtk::Entry  input_up;
  Gtk::Entry  input_down;
  Gtk::Entry  input_freq;
  Gtk::Entry  input_do_dest;
  Gtk::Entry  input_dest;
  Gtk::Frame  text1;

  int licznik;
  bool dir;
  bool en;
  bool go;
  bool dest;

  atI2C* ati2c;
  atI2C_X3* X3Motor;
  uint16_t X3MotorDataLen;
  uint32_t X3Motor_freq;   //częstotliwość pracy motora
  int32_t X3Motor_dest;

  atFileLog *FileRap;
  at_Konfiguracja *cnf;
public:
    X3_motor();
    ~X3_motor();
    void onButtonClickedUp();
    void onButtonClickedDown();
    void onButtonClickedEN();
    void onButtonClickedFR_up();
    void onButtonClickedFR_down();
    void onButtonClickedDEST();
    bool onTimeOut();
    void onClose();
    void upStart();
 };

#endif // X3_MOTOR_H
