#include "serial_client_win.h"


serial_client_win::serial_client_win()
{
    this->set_default_size(1000, 1000);
    button_send.set_label("Send");
    button_send.signal_clicked().connect( sigc::mem_fun(*this, &serial_client_win::onButtonClickedSend));
    grid.attach(input_id,0,2,1,1);
    grid.attach(input_fun,0,3,1,1);
    grid.attach(input_dev,0,4,1,1);
    grid.attach(input_addr,0,5,1,1);
    grid.attach(input_data,0,6,1,1);

    grid.attach(button_send,0,7,1,1);

    grid.attach(label_dbw,0,8,1,1);
    grid.attach(label_dbr,0,9,1,1);
    grid.attach(label_err,0,10,1,1);


    this->add(grid);
    this->show_all_children();



}
