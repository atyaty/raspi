#include "mainwindow.h"
#include "ui_mainwindow.h"


//-----------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :    QMainWindow(parent),    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ads1256_0  = new atSPI_ADS1256( ADS1256_PINCE_15 );
    ads1256_0->Config(SCANMODE_UNIPOLAR, ADS1256_GAIN_1, ADS1256_15SPS);
    ads1256_1  = new atSPI_ADS1256( ADS1256_PINCE_16 );
    ads1256_1->Config(SCANMODE_UNIPOLAR, ADS1256_GAIN_1, ADS1256_15SPS);
    ads1256_2  = new atSPI_ADS1256( ADS1256_PINCE_18 );
    ads1256_2->Config(SCANMODE_UNIPOLAR, ADS1256_GAIN_1, ADS1256_15SPS);
    spi      = new atSPI(ADS1256_DEV00);
    spi->addDev( (atSPI_dev*)ads1256_0 );
    spi->addDev( (atSPI_dev*)ads1256_1 );
    spi->addDev( (atSPI_dev*)ads1256_2 );
    spi->Init();

    if(ads1256_0->getError()!=0){ ui->lineEdit_0->setText(QString::number( ads1256_0->getError())); ui->lineEdit_0->setStyleSheet("background:red");}
    if(ads1256_1->getError()!=0){ ui->lineEdit_1->setText(QString::number( ads1256_1->getError())); ui->lineEdit_1->setStyleSheet("background:red");}
    if(ads1256_2->getError()!=0){ ui->lineEdit_2->setText(QString::number( ads1256_2->getError())); ui->lineEdit_2->setStyleSheet("background:red");}

    double a=(PT1000_R2-PT1000_R1)/(PT1000_T2-PT1000_T1);
    for(int i=0;i<8;i++) {
        ads1256_0->setParams(i,ADS1256_VREF,0.988,a,PT1000_T1,PT1000_R1);
        ads1256_1->setParams(i,ADS1256_VREF,0.988,a,PT1000_T1,PT1000_R1);
        ads1256_2->setParams(i,ADS1256_VREF,0.988,a,PT1000_T1,PT1000_R1);
    }

    //---------------------------------------------------------------
    FileRap= new atFile("./raport","raport",".csv");

    //---------------------------------------------------------------
    started0=false;
    started1=false;
    started2=false;

    //---------------------------------------------------------------
    Timer =new QTimer();
    connect(Timer,SIGNAL(timeout()),this,SLOT(TimerGo()));
    Timer->start(100);

}
//--------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ads1256_0;
    delete ads1256_1;
    delete ads1256_2;
    delete ui;
}
//-----------------------------------------------------
void MainWindow::TimerGo() { go_0(); go_1(); go_2(); }
//-----------------------------------------------------
void MainWindow::go_1() {
    if(started1){
        started1_n++;
        ui->lineEdit_1->setText( QString::number( started1_n ) );
        ads1256_1->scan();
        if(ads1256_1->getBuforyN()>0){ ads1256_1->next(); ui->textEdit_1->setText(disp(ads1256_1)); }
    }
}
//-----------------------------------------------------
void MainWindow::go_0() {
    if(started0){
        started0_n++;
        ui->lineEdit_0->setText( QString::number( started0_n ) );
        ads1256_0->scan();
        if(ads1256_0->getBuforyN()>0){ ads1256_0->next(); ui->textEdit_0->setText(disp(ads1256_0)); }
    }
}
//-----------------------------------------------------
void MainWindow::go_2() {
    if(started2){
        started2_n++;
        ui->lineEdit_2->setText( QString::number( started2_n ) );
        ads1256_2->scan();
        if(ads1256_2->getBuforyN()>0){ ads1256_2->next(); ui->textEdit_2->setText(disp(ads1256_2)); }
    }
}

//--------------------------------------------------------
void MainWindow::on_pushButton_0_clicked(){
    started0= !started0;
    started0_n=0;
    ui->pushButton_0->setText(started0?"Stop":"Start");
    if(!started0) FileRap->ZapiszPlik();
    else {
        QString s1="nr;";
        s1+="\n";
        FileRap->Append(s1);
    }
}
//--------------------------------------------------------
void MainWindow::on_pushButton_1_clicked() {
    started1= !started1;
    started1_n=0;
    ui->pushButton_1->setText(started1?"Stop":"Start");
    if(!started1) FileRap->ZapiszPlik();
    else {
        QString s1="nr;";
        s1+="\n";
        FileRap->Append(s1);
    }
}
//-------------------------------------------------------
void MainWindow::on_pushButton_2_clicked(){
    started2= !started2;
    started2_n=0;
    ui->pushButton_2->setText(started1?"Stop":"Start");
    if(!started2) FileRap->ZapiszPlik();
    else {
        QString s1="nr;";
        s1+="\n";
        FileRap->Append(s1);
    }
}



//-----------------------------------------------------
QString MainWindow::disp(atSPI_ADS1256 *ads1256) {
    QString s="";
    for (uint8_t i = 0; i < ads1256->getLen(); i++){
        double v=ads1256->getADC_value(i);
        s+=QString::number( i )+" ";
        s+=QString::number( ads1256->getADC_value(i) )+"     ";
        s+=QString::number( ads1256->getADC_U_V(i) )+"      ";
        s+=QString::number( ads1256->getADC_T_C(i) )+" \n";

    }
    return s;
}


