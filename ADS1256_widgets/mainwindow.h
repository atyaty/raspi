#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <atFunctions.h>
#include <atSPI_ads1256.h>
#include <atSPI.h>
#include <atFile.h>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void TimerGo();
    void on_pushButton_0_clicked();
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    //--------------------------------------
    bool started0,started1,started2;
    int started0_n,started1_n,started2_n;
    //--------------------------------------
    QTimer* Timer;
    atSPI* spi;
    atSPI_ADS1256 *ads1256_0,*ads1256_1,*ads1256_2;
    atFile *FileRap;
    //------------------------------------------
    QString disp(atSPI_ADS1256 *ads1256);
    void go_0();
    void go_1();
    void go_2();


};

#endif // MAINWINDOW_H
