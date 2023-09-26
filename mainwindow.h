#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSerialPort>
#include <QMainWindow>
#include <QTimer>
#include<QGeoRoute>


//#include <QtQuick>
//#include <QtPositioning>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_doubleSpinBoxPitch_valueChanged(double arg1);

    void on_doubleSpinBoxRoll_valueChanged(double arg1);

    void on_doubleSpinBoxAltitude_valueChanged(double arg1);

    void on_doubleSpinBoxHeading_valueChanged(double arg1);

    void on_doubleSpinBoxHeadingSpeed_valueChanged(double arg1);


    void on_doubleSpinBoxTR_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    int timerId;
    QSerialPort *serialPort;
    void checkForData();
    void readData();
    double gyroX;
    double gyroY;
    double gyroZ;
    double accX;
    double accY;
    double accZ;
    
    signed int accelerationX;
    signed int accelerationY;
    signed int accelerationZ;
    double pitch;
    double roll;
    double yaw;
    double turnRate;
    double prevYaw=0.0;
    double heading;
    double currYaw;
};
#endif // MAINWINDOW_H
