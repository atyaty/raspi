#ifndef SERIAL_CLIENT_WIN_H
#define SERIAL_CLIENT_WIN_H
#include <gtkmm.h>
#include <atSerial.h>

using namespace std;
class serial_client_win : public Gtk::Window
{
private:
    Gtk::Grid   grid;
    Gtk::Entry  input_id;
    Gtk::Entry  input_fun;
    Gtk::Entry  input_dev;
    Gtk::Entry  input_addr;
    Gtk::Entry  input_data;
    Gtk::Label  label_dbr;
    Gtk::Label  label_dbw;
    Gtk::Label  label_err;
    Gtk::Button button_send;

    atDataBuffer* dbr  ;
    atDataBuffer* dbw  ;
    atSerial* uart;



public:
    serial_client_win();
};

#endif // SERIAL_CLIENT_WIN_H
