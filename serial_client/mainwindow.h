#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
//#include <atSerial.h>
#include <atSocketClient.h>
#include <atI2C.h>
//#include <atI2C_pi.h>
#include <atI2C_mcp.h>
#include <atDataBuffer.h>
#include <QTextStream>
#include <sstream>
#include <atKonfiguracja.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString charToHex(uint8_t* f,unsigned len);

private slots:

    void TimerGo();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_MOTOR_clicked();



    void on_pushButton_ADS1256_clicked();

    void on_pushButton_PWM_clicked();

    void on_pushButton_MOTOR_Stop_clicked();

    void on_pushButton_MOTOR_Luz_clicked();

    void on_pushButton_MOTOR_2_clicked();

private:
    Ui::MainWindow *ui;

//    atSerial* ats;
//    atDataBuffer* dbr;
//    atDataBuffer* dbw;

    atI2C* ati2c;
    atDataBuffer* dbrPI;
    atDataBuffer* dbwPI;

    atDataBuffer* dbrSocket;
    atDataBuffer* dbwSocket;
    atSocketClient * atSC;
    atI2C_pi* pi;

    atKonfiguracja* cnf;
    string socketIP;
    int socketPort;

    QTimer* Timer;
};

#endif // MAINWINDOW_H
