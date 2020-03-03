#include "X3_motor.h"

X3_motor::X3_motor()
{
 X3MotorDataLen=AT_MOTOR_INBUFLEN-AT_MODBUS_REQUNAG-2;//2-crc

 licznik=0;
 this->set_default_size(1000, 1000);
 buttonUp.set_label("Up");
 buttonUp.signal_clicked().connect( sigc::mem_fun(*this, &X3_motor::onButtonClickedUp));

 buttonDn.set_label("Down");
 buttonDn.signal_clicked().connect( sigc::mem_fun(*this, &X3_motor::onButtonClickedDown));
 buttonEN.set_label("EN");
 buttonEN.signal_clicked().connect( sigc::mem_fun(*this, &X3_motor::onButtonClickedEN));
 buttonFR_up.set_label("FREQ+");
 buttonFR_up.signal_clicked().connect( sigc::mem_fun(*this, &X3_motor::onButtonClickedFR_up));
 buttonFR_down.set_label("FREQ-");
 buttonFR_down.signal_clicked().connect( sigc::mem_fun(*this, &X3_motor::onButtonClickedFR_down));

 buttonDEST.set_label("Jedź do DEST");
 buttonDEST.signal_clicked().connect( sigc::mem_fun(*this, &X3_motor::onButtonClickedDEST));

 input_dbw.set_width_chars(35);
 input_dbr.set_width_chars(35);
 input_error.set_width_chars(35);
 input_di.set_width_chars(35);
 input_do.set_width_chars(35);
 input_up.set_width_chars(35);
 input_down.set_width_chars(35);
 grid.attach(input_dbw,0, 0,1,1);
 grid.attach(input_do, 0, 1,1,1);

 grid.attach(input_dbr,1,0,1,1);
 grid.attach(input_di ,1,1,1,1);

 grid.attach(input_poz ,1,2,1,1);

 grid.attach(buttonUp, 0,3,1,1);
 grid.attach(input_up ,1,3,1,1);

 grid.attach(buttonDn,  0,4,1,1);
 grid.attach(input_down  ,1,4,1,1);
 grid.attach(buttonEN,    0,5,1,1);
 check_button_en.set_label("EN");
 grid.attach(check_button_en,1,5,1,1);

 grid.attach(input_error,0,6,1,1);
 grid.attach(input_licznik,0,7,1,1);
 grid.attach(input_freq  ,0,8,1,1);
 grid.attach(buttonFR_up, 1,8,1,1);
 grid.attach(buttonFR_down,1,9,1,1);
 grid.attach(buttonDEST,0,12,1,1);

 check_button_upstop.set_label("Krańcówka UP");
 check_button_dnstop.set_label("Krańcówka DN");
 grid.attach(check_button_upstop,1,10,1,1);
 grid.attach(check_button_dnstop,1,11,1,1);

 input_dest.set_text("0");
 grid.attach(input_dest,1,12,1,1);
 grid.attach(text1,0,13,1,1);

 text1.set_label("aaaa");

 input_dbr.set_text("V9");


 this->add(grid);
 this->show_all_children();

 cnf=new at_Konfiguracja("config/config");
 if(cnf->getError()) input_error.set_text("Błąd pliku konfiguracyjnego: "+to_string(cnf->getError()));

 int logLevel=cnf->getInt("logLevel");
 string logPath=cnf->getTag("logPath");
 if(cnf->getError()) input_error.set_text("Brak ścieżki raportu: "+to_string(cnf->getError()));

 FileRap= new atFileLog(logPath,"x3_motor",".log",logLevel);
 FileRap->RapDate("Start programu",AT_LOG_ZAWSZE);
 X3Motor_freq=cnf->getInt("X3Motor_freq");


 X3Motor   = new atI2C_X3("MOTOR",AT_MODBUS_ID_X3,AT_REGADRES_MOTOR1);
 X3Motor->SetFrameLen(AT_MOTOR_OUTBUFLEN);
 X3Motor->confRap(FileRap);
 ati2c  = new atI2C(I2C_PI_ID);
 ati2c->confRap(FileRap);
 ati2c->setParams(cnf);
 ati2c->addDev( (atI2C_dev*)X3Motor );
 ati2c->Init();
 if(ati2c->IsError()){
     input_error.set_text("Błąd : "+ati2c->getErrorOpis() );
     FileRap->Rap( ati2c->getErrorOpis(),AT_LOG_ZAWSZE );
 }
 Glib::signal_timeout().connect( sigc::mem_fun(*this, &X3_motor::onTimeOut ), 10 );
 signal_hide().connect( sigc::mem_fun(*this, &X3_motor::onClose ) );

}
//------------------------------------------------------
X3_motor::~X3_motor(){
    delete ati2c;
    delete X3Motor;
    delete FileRap;
    delete cnf;
}

//------------------------------------------------------
void X3_motor::onButtonClickedUp(){
 dir = 1;
 go= !go;
 dest=0;
 //if(go && dir==1 )      {go=false; buttonEN.set_b _active(false); }
 //else if(go && dir==0)  {dir=1;    buttonUp.set_active(true); buttonDn.set_active(false); }
 //else {go=true; dir=1; buttonUp.set_active(true); buttonDn.set_active(false);}

 input_up.set_text("dir/go "+to_string(dir)+"/"+to_string(go));
 input_down.set_text("");

 upStart();
}
//------------------------------------------------------
void X3_motor::onButtonClickedDown(){
 dir = 0;
 go= !go;
 dest=0;
 input_down.set_text("dir/go "+to_string(dir)+"/"+to_string(go));
 input_up.set_text("");

 upStart();
}
//------------------------------------------------------
void X3_motor::onButtonClickedEN(){
 if(en==0) en=1;
 else en=0;
 upStart();
}
//------------------------------------------------------
void X3_motor::onButtonClickedFR_up(){
    uint32_t podbij=X3Motor_freq+10;
    X3Motor_freq=podbij;
    input_freq.set_text(to_string(podbij) );
    if(go) {
      upStart(); //bez tego wywołania nie zwiększał płynnie obrotów
      cnf->setTag("X3Motor_freq",to_string(X3Motor_freq));
      cnf->ZapiszPlikBak();
      //setTag
      //nie umiem zapisac do pliki konfiguracji zmienionej zmiennej X3Motor

}
}

//------------------------------------------------------
void X3_motor::onButtonClickedFR_down(){
    uint32_t podbij=X3Motor_freq-10;
    X3Motor_freq=podbij;
    input_freq.set_text(to_string(podbij) );
    if(go) {
      upStart(); //bez tego wywołania nie zwiększał obrotów
    }
}

//------------------------------------------------------
void X3_motor::onButtonClickedDEST(){
    string s=input_dest.get_text();
    X3Motor_dest=stoi(s);
    dest=!dest;
    upStart();
}


//------------------------------------------------------
void X3_motor::upStart(){
    //FileRap("..upStart ("+to_string(stat_up)+")",AT_LOG_DEBUG);
    atDataBuffer* dbr=X3Motor->getDbr();
    atDataBuffer* dbw=X3Motor->getDbw();

    dbw->Clear();
    dbw->buforSetNag(AT_MODBUS_SET,X3Motor->getAddrDev(),0,X3MotorDataLen);
    dbw->buforAddReg(0);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_GO,go);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_DIR,dir);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_EN,en);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_ONOFF,1);
    dbw->modbusSetRegBit(AT_MOTOR_DI,AT_MOTOR_DI_DEST,dest);
    dbw->buforAddReg32(X3Motor_dest);
    dbw->buforAddReg32(X3Motor_freq);
    dbw->buforAddCRC();
    X3Motor->setSendIsHot();
    //Rap("..upStart "+ dbw->charToHex()+">>",AT_LOG_DEBUG);
    input_dbw.set_text(dbw->charToHex());
    input_do.set_text(int16ToBit(dbw->modbusGetReg(AT_MOTOR_DI)) );
    if( X3Motor->go() ){

        input_dbr.set_text(dbr->charToHex());
        input_di.set_text(int16ToBit(dbr->modbusGetReg(AT_MOTOR_DO)) );
    }
}
//--------------------------------------------------------------------
bool X3_motor::onTimeOut(){
 input_licznik.set_text(to_string(licznik));
 licznik++;
 atDataBuffer* dbr=X3Motor->getDbr();
 atDataBuffer* dbw=X3Motor->getDbw();

 dbw->Clear();
 dbw->buforSetNag(AT_MODBUS_READ,X3Motor->getAddrDev(),0,0);
 dbw->buforAddCRC();
 X3Motor->setSendIsHot();
 //Rap("..upStart "+ dbw->charToHex()+">>",AT_LOG_DEBUG);
 //input_dbw.set_text(dbw->charToHex());
 //input_do.set_text(int16ToBit(dbw->modbusGetReg(AT_MOTOR_DI)) );
 if( X3Motor->go() ){
     input_dbr.set_text(dbr->charToHex());
     input_di.set_text(int16ToBit(dbr->modbusGetReg(AT_MOTOR_DO)) );
     input_poz.set_text(to_string(dbr->modbusGetInt32(AT_MOTOR_AO_POS)) );

     //input_en.set_text(to_string(dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_EN)));
     //input_freq.set_text(to_string(X3Motor_freq));
//     input_upstop.set_text(to_string(dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_UPSTOP)));
  //   input_dnstop.set_text(to_string(dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_DNSTOP)));
     check_button_en.set_active(dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_EN));
     check_button_upstop.set_active(dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_UPSTOP));
     check_button_dnstop.set_active(dbr->modbusGetRegBit(AT_MOTOR_DO,AT_MOTOR_DO_DNSTOP));

 }


 return true;
}
//--------------------------------------------------------------------
void X3_motor::onClose(){
 go=false; en=false; dest=false; upStart();
}

