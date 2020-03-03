
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
prykład użycia komunikacji szeregowej w Raspberry Pi po porcie szeregowym

-łączymy 2 raspberry pi'e 3 kablami
-konfigurujemy według:
  https://spellfoundry.com/2016/05/29/configuring-gpio-serial-port-raspbian-jessie-including-pi-3/
-na jednym uruchamiamy ten program
-na drugim odpalamy:
  echo "hej" >/dev/ttyAMA0
  w programie powinno po pojawić się: hej

  teraz na drugim odpalamy:
  cat </dev/ttyAMA0
  jeśli z naszego programu wyślemy: aqq
  to drugi pi powinien wypisać aqq
*/



#define I2C_PI_ID 0x56

//------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    cnf = new atKonfiguracja("./config","config");
    socketIP=cnf->getTag("socketIP").toStdString();
    socketPort=cnf->getInt("socketPort");
    ui->lineEdit_socktetIP->setText( ((QString)socketIP.c_str())+":"+QString::number(socketPort));
    //ats   = new atSerial( AT_SERIAL_TTY, AT_SERIAL_BAUD, AT_SERIAL_TIMEOUT );
    //dbr   = new atDataBuffer(AT_REGADRES_SMM_KAPPA,AT_MODBUS_FRAME_LEN);
    //dbw   = new atDataBuffer(AT_REGADRES_SMM_KAPPA,AT_MODBUS_FRAME_LEN);

    ati2c = new atI2C();
    dbrPI  = new atDataBuffer(AT_REGADRES_SMM_KAPPA,AT_MODBUS_FRAME_LEN);
    dbwPI  = new atDataBuffer(AT_REGADRES_SMM_KAPPA,AT_MODBUS_FRAME_LEN);


    dbrSocket  = new atDataBuffer(0,AT_MODBUS_FRAME_LEN);
    dbwSocket  = new atDataBuffer(0,AT_MODBUS_FRAME_LEN);
    atSC = new atSocketClient( dbrSocket,dbwSocket,socketIP.c_str(),socketPort);
//    atSC = new atSocketClient( dbrSocket,dbwSocket,AT_SOCKET_IP,AT_SOCKET_PORTNO);

    pi    = new atI2C_pi(I2C_PI_ID,dbrPI,dbwPI);
    ati2c->addDev( (atI2C_dev*)pi );


    Timer =new QTimer();
    connect(Timer,SIGNAL(timeout()),this,SLOT(TimerGo()));
    Timer->start(10);
}

//---------------------------------------------------
MainWindow::~MainWindow() {
//    cnf->setTag("socketIP",socketIP.c_str());
//    cnf->ZapiszPlikBak(".bak");
    //delete ats;
    delete Timer;
    delete ui;
}

//-----------------------------------------------------
void MainWindow::on_pushButton_clicked() {
    //ats->puts( ui->lineEdit->text().toStdString().c_str() );
}
//-----------------------------------------------------
void MainWindow::TimerGo() {
/*
    ats->gets(dbr);
    if(dbr->getDataLen()){
        if(dbr->checkId()&&(dbr->checkFunction(AT_SMM_WRITE)||dbr->checkFunction(AT_SMM_READ))){
            ui->textEdit->append( charToHex(dbr->getFrame(),dbr->getDataLen() ) );
        }
        else {
            char * s = (char*)dbr->getFrame();
            ui->textEdit->append( (QString)s );
        }

    }
    dbr->Clear();
*/
}
//-----------------------------------------------------------
QString MainWindow::charToHex(uint8_t* f,unsigned len){
    stringstream ss;
    for(unsigned i=0;i<len;i++){
     ss << std::hex << (unsigned int) f[i];
     ss << " ";
    }
    return (QString)ss.str().c_str();

}
//----------------------------------------------------
void MainWindow::on_pushButton_2_clicked() {/*
 uint16_t i=ui->lineEdit_2->text().toInt();
 dbw->Clear();
 dbw->buforSetNag(AT_REGADRES_SMM_KAPPA,AT_SMM_WRITE,0,0,2);
 dbw->buforAddReg(i);
 dbw->buforAddCRC();
 ats->puts(dbw);*/
}
//------------------------------------------------------------
void MainWindow::on_pushButton_3_clicked() {
    /*
    uint16_t i=ui->lineEdit_2->text().toInt();
    dbw->Clear();
    dbw->buforSetNag(AT_REGADRES_SMM_KAPPA,AT_SMM_WRITE,0,0,4);
    dbw->buforAddReg(i);
    dbw->buforAddReg(i);
    dbw->buforAddCRC();
    ats->puts(dbw);*/

}
//------------------------------------------------------------
void MainWindow::on_pushButton_4_clicked() {
    QString ss=ui->lineEdit_2->text();
    dbwPI->buforSetNag(AT_MODBUS_ID_X3,AT_MODBUS_WRITE,AT_REGADRES_MOTOR1,0,ss.length());
    dbwPI->buforAddStr(ss.toStdString().c_str());
    dbwPI->buforAddCRC();
    //uint16_t i=ui->lineEdit_2->text().toInt();
    //dbwPI->buforSetNag(0x05);
    //dbwPI->buforAddByte(i);
    dbrPI->Clear();
    ati2c->go();
    QString s;
    s+=QString::number( I2C_PI_ID )+ ":\n dbw: ";
    s+=dbwPI->charToHex().c_str();
    s+="\ndbr: ";
    s+=dbrPI->charToHex().c_str();
    s+="\n";

    ui->textEdit->setText(s);
}
//-----------------------------------------------------------------------
void MainWindow::on_pushButton_5_clicked(){
    //dbwMPC->buforSetNag(0x05);
    //dbwMPC->buforAddByte(0x05);
    //dbrMPC->Clear();
    /*
    ati2c->go(1);
    QString s;

    s+=QString::number( AT_TWIMCP3424_ADDR_68 )+ ":\n dbw: ";
    s+=mcp->getDbw()->charToHex().c_str();
    s+=" - ";
    s+=mcp->getDbw()->charToBit().c_str();

    s+="\ndbr: ";
    s+=mcp->getDbr()->charToHex().c_str();
    s+=" - ";
    s+=mcp->getDbr()->charToBit().c_str();

    s+="\ni2c: ";
    //s+=" zwloka: "+QString::number( mcp->getZwloka(0) );
    //s+=" bufor: "+QString::number( mcp->getBufor(0) );
    //s+=" value: "+QString::number( mcp->getValue(0) );
    s+="\n";

    ui->textEdit->setText(s);
*/
}


//-----------------------------------------------------------------------
void MainWindow::on_pushButton_MOTOR_clicked(){
    uint32_t Dest=ui->lineEdit_MOTOR_Dest->text().toInt();
    uint32_t V=ui->lineEdit_MOTOR_V->text().toInt();

    dbwSocket->buforSetNag(AT_MODBUS_ID_X3,AT_MODBUS_SET,AT_REGADRES_MOTOR1,0,10);
    dbwSocket->buforAddReg(0x5325);
        //dbwSocket->buforRegSetBit(AT_MOTOR_DI_GO,1);
        //dbwSocket->buforRegSetBit(AT_MOTOR_DI_EN,1);
        //dbwSocket->buforRegSetBit(AT_MOTOR_DI_DEST,1);
    dbwSocket->buforAddReg32(Dest);
    dbwSocket->buforAddReg32(V);
    dbwSocket->buforAddCRC();
    atSC->go();
    QString s="> ";
    s+=dbwSocket->charToHex().c_str();
    s+=" \n<";
    s+=dbrSocket->charToHex().c_str();
    ui->textEdit->setText(s);
}
//------------------------------------------------------------------------
void MainWindow::on_pushButton_ADS1256_clicked(){
    QString s=ui->lineEdit_socketCMD->text();
    dbwSocket->buforSetNag(AT_MODBUS_ID_ADS1256,AT_MODBUS_READ,0,0,s.length());
    dbwSocket->buforAddStr(s.toStdString().c_str());
    dbwSocket->buforAddCRC();
    atSC->go();
    ui->textEdit->setText(dbrSocket->charToHex().c_str());
}
//------------------------------------------------------------------------
void MainWindow::on_pushButton_PWM_clicked(){
    QString s=ui->lineEdit_socketCMD->text();
    dbwSocket->buforSetNag(AT_MODBUS_ID_PI,AT_MODBUS_READ,0,0,s.length());
    dbwSocket->buforAddStr(s.toStdString().c_str());
    dbwSocket->buforAddCRC();
    atSC->go();
    ui->textEdit->setText( dbrSocket->charToHex().c_str());
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_pushButton_MOTOR_Stop_clicked(){
    uint32_t Dest=ui->lineEdit_MOTOR_Dest->text().toInt();
    uint32_t V=ui->lineEdit_MOTOR_V->text().toInt();

    dbwSocket->buforSetNag(AT_MODBUS_ID_X3,AT_MODBUS_SET,AT_REGADRES_MOTOR1,0,10);
    dbwSocket->buforAddReg(0x5320);
        dbwSocket->buforRegSetBit(AT_MOTOR_DI_GO,0);
        dbwSocket->buforRegSetBit(AT_MOTOR_DI_EN,1);
        dbwSocket->buforRegSetBit(AT_MOTOR_DI_DEST,0);
    dbwSocket->buforAddReg32(Dest);
    dbwSocket->buforAddReg32(V);
    dbwSocket->buforAddCRC();
    atSC->go();
    QString s="> ";
    s+=dbwSocket->charToHex().c_str();
    s+=" \n<";
    s+=dbrSocket->charToHex().c_str();
    ui->textEdit->setText(s);
}

void MainWindow::on_pushButton_MOTOR_Luz_clicked(){
    uint32_t Dest=ui->lineEdit_MOTOR_Dest->text().toInt();
    uint32_t V=ui->lineEdit_MOTOR_V->text().toInt();

    dbwSocket->buforSetNag(AT_MODBUS_ID_X3,AT_MODBUS_SET,AT_REGADRES_MOTOR1,0,10);
    dbwSocket->buforAddReg(0x5320);
        dbwSocket->buforRegSetBit(AT_MOTOR_DI_GO,0);
        dbwSocket->buforRegSetBit(AT_MOTOR_DI_EN,0);
        dbwSocket->buforRegSetBit(AT_MOTOR_DI_DEST,0);
    dbwSocket->buforAddReg32(Dest);
    dbwSocket->buforAddReg32(V);
    dbwSocket->buforAddCRC();
    atSC->go();
    QString s="> ";
    s+=dbwSocket->charToHex().c_str();
    s+=" \n<";
    s+=dbrSocket->charToHex().c_str();
    ui->textEdit->setText(s);
}

void MainWindow::on_pushButton_MOTOR_2_clicked()
{
    uint32_t Dest=ui->lineEdit_MOTOR_Dest->text().toInt();
    uint32_t V=ui->lineEdit_MOTOR_V->text().toInt();

    dbwPI->buforSetNag(AT_MODBUS_ID_X3,AT_MODBUS_SET,AT_REGADRES_MOTOR1,0,10);
    dbwPI->buforAddReg(0x5320);
        dbwPI->buforRegSetBit(AT_MOTOR_DI_GO,1);
        dbwPI->buforRegSetBit(AT_MOTOR_DI_EN,1);
        dbwPI->buforRegSetBit(AT_MOTOR_DI_DEST,1);
    dbwPI->buforAddReg32(Dest);
    dbwPI->buforAddReg32(V);
    dbwPI->buforAddCRC();
    ati2c->go();
    QString s="> ";
    s+=dbwPI->charToHex().c_str();
    s+=" \n<";
    s+=dbrPI->charToHex().c_str();
    ui->textEdit->setText(s);
}
