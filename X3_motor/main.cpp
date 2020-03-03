#include <gtkmm.h>
#include <iostream>
#include <X3_motor.h>
using namespace std;


int main(int argc, char **argv)
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "Device");

    X3_motor *w = new X3_motor();
    return app->run(*w);

}

