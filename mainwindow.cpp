
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include<QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{

    ui->setupUi(this);

    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/QmlMap.qml")));
    ui->quickWidget->show();


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
    ui->graphicsView_AttitudeIndicator->redraw();
}

void MainWindow::on_doubleSpinBoxPitch_valueChanged(double arg1)
{
//    ui->graphicsEADI->setPitch(arg1);
//    ui->graphicsEADI->redraw();
//    ui->doubleSpinBoxPitch->setValue(25);
    ui->graphicsView_AttitudeIndicator->setPitch(arg1);
    ui->graphicsView_AttitudeIndicator->redraw();
    ui->statusbar->showMessage("Pitch value is changing");

}


void MainWindow::on_doubleSpinBoxRoll_valueChanged(double arg1)
{
    ui->graphicsView_AttitudeIndicator->setRoll(arg1);
    ui->graphicsView_AttitudeIndicator->redraw();
    ui->statusbar->showMessage("Pitch value is changing");
}


void MainWindow::on_doubleSpinBoxAltitude_valueChanged(double arg1)
{
    ui->graphicsView_AttitudeIndicator->setAltitude(arg1);
    ui->graphicsView_AttitudeIndicator->redraw();
    ui->statusbar->showMessage("Altitude is changing");
}


void MainWindow::on_doubleSpinBoxHeading_valueChanged(double arg1)
{
    ui->graphicsView_AttitudeIndicator->setHeading(arg1);
//    ui->graphicsView->setAirspeed(arg1);
    ui->graphicsView_AttitudeIndicator->redraw();
    ui->statusbar->showMessage("Plane is moving forward");
}


void MainWindow::on_doubleSpinBoxHeadingSpeed_valueChanged(double arg1)
{
    ui->graphicsView_AttitudeIndicator->setAirspeed(arg1);
//    ui->graphicsView->set
    ui->graphicsView_AttitudeIndicator->redraw();
    ui->statusbar->showMessage("Plane speed is changing");
}



void MainWindow::on_doubleSpinBoxTR_valueChanged(double arg1)
{
    ui->graphicsView_TurnCoord->setTurnRate(arg1);
    ui->graphicsView_TurnCoord->redraw();
//    ui->graphicsView_TC

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

        accelerationX = (signed int)(((signed int)accX) * 3.9);
        accelerationY = (signed int)(((signed int)accY) * 3.9);
        accelerationZ = (signed int)(((signed int)accZ) * 3.9);

//        Note: M_PI = 3.14159265358979323846 it is constant defined in math.h
        pitch = 180 * atan (accelerationX/sqrt(accelerationY*accelerationY + accelerationZ*accelerationZ))/M_PI;

        roll = atan2(accelerationY, accelerationZ) * 180.0 / M_PI;

        yaw = atan2(accelerationZ, accelerationX)*180.0 / M_PI - 90.0;

        double gyro_sensitivity_x = 250;
        double gyro_bias_x = 0.5;
/*       yaw = (gyroX+gyroY+gyroZ)*0.2;
        currYaw = prevYaw + yaw;
        turnRate = yaw/0.2;
        prevYaw = currYaw;
*/

//        turnRate = (gyroZ/gyro_sensitivity_x) - gyro_bias_x;
        // Update UI elements here with the received data

        ui->doubleSpinBoxPitch->setValue(pitch);
        ui->doubleSpinBoxRoll->setValue(roll);
        ui->doubleSpinBoxHeading->setValue(yaw);

        ui->doubleSpinBoxTR->setValue(gyroX);
        ui->doubleSpinBoxHeadingSpeed->setValue(accelerationX);

//            ui->doubleSpinBoxTR->setValue(turnRate);



    }
}

/*
void MainWindow::on_doubleSpinBox_Yaw_valueChanged(double arg1)
{
    ui->doubleSpinBoxheading->setValue(arg1);
    ui->graphicsView->redraw();
    ui->statusbar->showMessage("Yaw is changing");
}

*/


