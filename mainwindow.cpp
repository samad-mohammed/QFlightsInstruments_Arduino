#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include<QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{

    ui->setupUi(this);



    serialPort = new QSerialPort(this);


    //     Find the correct serial port name for your Arduino Mega
    foreach(const QSerialPortInfo &port, QSerialPortInfo::availablePorts()) {
        qDebug() <<  port.description();
        if (port.description().contains("Arduino Mega 2560")) {

            serialPort->setPort(port);
            serialPort->setBaudRate(QSerialPort::Baud57600);
            break;
        }
    }


    if (serialPort->open(QIODevice::ReadOnly)) {
        qDebug() << "Serial port opened successfully.";
    } else {
        qDebug() << "Failed to open serial port: " << serialPort->errorString();

    }


    // Create a timer to check for data every 1000 milliseconds (1 second)
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::checkForData);
    timer->start(200); // Start the timer
}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow :: timerEvent(QTimerEvent *event)
{
    QMainWindow :: timerEvent(event);
    ui->graphicsView->redraw();
}

void MainWindow::on_doubleSpinBoxPitch_valueChanged(double arg1)
{
//    ui->graphicsEADI->setPitch(arg1);
//    ui->graphicsEADI->redraw();
//    ui->doubleSpinBoxPitch->setValue(25);
    ui->graphicsView->setPitch(arg1);
    ui->graphicsView->redraw();
    ui->statusbar->showMessage("Pitch value is changing");

}


void MainWindow::on_doubleSpinBoxRoll_valueChanged(double arg1)
{
    ui->graphicsView->setRoll(arg1);
    ui->graphicsView->redraw();
    ui->statusbar->showMessage("Pitch value is changing");
}


void MainWindow::on_doubleSpinBoxAltitude_valueChanged(double arg1)
{
    ui->graphicsView->setAltitude(arg1);
    ui->graphicsView->redraw();
    ui->statusbar->showMessage("Altitude is changing");
}


void MainWindow::on_doubleSpinBoxheading_valueChanged(double arg1)
{
    ui->graphicsView->setHeading(arg1);
//    ui->graphicsView->setAirspeed(arg1);
    ui->graphicsView->redraw();
    ui->statusbar->showMessage("Plane is moving forward");
}


void MainWindow::on_doubleSpinBoxHeadingSpeed_valueChanged(double arg1)
{
    ui->graphicsView->setAirspeed(arg1);
    ui->graphicsView->redraw();
    ui->statusbar->showMessage("Plane speed is changing");
}

void MainWindow::checkForData()
{
    if (serialPort->bytesAvailable() > 0) {
        // Data is available to be read, so call readData to process it
        readData();
    }
}



void MainWindow::readData()
{


    //    =====================================================================================================
    QByteArray data = serialPort->readLine();
    QString dataString = QString::fromUtf8(data);

//    bool ok;
//    double gyroValue = dataString.toDouble(&ok);                           //  This is for only one value
    qDebug() << dataString;

//    if (ok) {
//        // Update the QDoubleSpinBox with the received gyroValue
//        ui->graphicsView->setHeading(gyroValue);
//    }
    //    ========================================================================================================

    QList<QByteArray> values = data.split(',');

    if (values.size() == 6) {
        gyroX = values[0].toDouble();
        gyroY = values[1].toDouble();
        gyroZ = values[2].toDouble();
        accX = values[3].toDouble();
        accY = values[4].toDouble();
        accZ = values[5].toDouble();

//                        qDebug() << "GyroX:" << gyroX << "GyroY:" << gyroY << "GyroZ:" << gyroZ
//                                 << "AccX:" << accX << "AccY:" << accY << "AccZ:" << accZ;

//                        ui->graphicsView->setHeading(accX);
                        // Update UI elements here with the received data

                        ui->doubleSpinBoxPitch->setValue(gyroX);
                        ui->doubleSpinBoxRoll->setValue(gyroY);
                        ui->doubleSpinBoxheading->setValue(gyroZ);


                        ui->doubleSpinBoxHeadingSpeed->setValue(accX);
                        ui->doubleSpinBoxAltitude->setValue(accZ);
//                        ui->doubleSpinBoxheading->setValue(gyroZ);
                        // You can also update your graphics view if needed

                        // ui->graphicsView->setPitch(gyroX);
                        // ui->graphicsView->setRoll(gyroY);
                        // ui->graphicsView->setAltitude(gyroZ);

                        // ui->graphicsView->redraw();
//                        ui->doubleSpinBoxHeadingSpeed->setValue(accX);

    }
}
