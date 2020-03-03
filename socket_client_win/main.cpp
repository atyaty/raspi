#include <gtkmm.h>
#include <iostream>
#include <socket_client_win.h>
using namespace std;


int main(int argc, char **argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "Device");

    socket_client_win *w = new socket_client_win();
    return app->run(*w);

}

