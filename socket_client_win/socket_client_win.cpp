#include "socket_client_win.h"

socket_client_win::socket_client_win()
{
 this->set_default_size(1000, 1000);
 button_send.set_label("Send");
 button_send.signal_clicked().connect( sigc::mem_fun(*this, &socket_client_win::onButtonClickedSend));
 grid.attach(input_hostname,0,1,1,1);
 grid.attach(input_port,1,1,1,1);
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




 string   hostname= "192.168.2.77";
 uint16_t port=5000;
 dbr  = new atDataBuffer( AT_MODBUS_ID_PI, AT_MODBUS_FRAME_LEN,AT_MODBUS_RESPNAG );
 dbw  = new atDataBuffer( AT_MODBUS_ID_PI, AT_MODBUS_FRAME_LEN,AT_MODBUS_REQUNAG );
 atSC = new atSocketClient( dbr,dbw,hostname,port);

}
//------------------------------------------------------
socket_client_win::~socket_client_win(){
    delete dbr;
    delete dbw;
    delete atSC;
}
//------------------------------------------------------
void socket_client_win::onButtonClickedSend(){
 if( atSC->isError() ){ label_err.set_label("błąd"); }
 int    id  =stoi(input_id.get_text());
 int    fun =stoi(input_fun.get_text());
 int    dev = stoi(input_dev.get_text());
 int    addr=stoi(input_addr.get_text());

 string data=input_data.get_text();

 dbw->buforSetNag(id,fun,dev,addr,data.size());
 for (unsigned i=0; i<data.size(); i++){
   dbw->buforAddByte(data[i]);
 }
 //dbw->buforAddStr(buf);
 dbw->buforAddCRC();
 label_dbw.set_label(dbw->frameToHex());
 //--------------------------wysyłamy i odbieramy zwrotkę
 atSC->go();
 label_dbr.set_label(dbr->frameToHex());



}
