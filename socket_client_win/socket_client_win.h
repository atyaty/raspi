#ifndef SOCKET_CLIENT_WIN_H
#define SOCKET_CLIENT_WIN_H
#include <gtkmm.h>
#include <atSocketClient.h>
using namespace std;
class socket_client_win : public Gtk::Window
{
private:
    Gtk::Grid   grid;
    Gtk::Entry  input_hostname;
    Gtk::Entry  input_port;
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
    atSocketClient * atSC ;

public:
    socket_client_win();
    ~socket_client_win();
    void onButtonClickedSend();
};

#endif // SOCKET_CLIENT_WIN_H
