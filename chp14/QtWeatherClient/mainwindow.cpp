#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <unistd.h>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->dialog = new ServerSettingsDialog(this);
    this->isFahrenheit = false;
    statusBar()->showMessage("Sensor Application Started");
    this->maxTemperature = -100.0f;
    this->minTemperature = 100.0f;
    this->temperature = 0.0f;
    this->humidity = 0.0f;
    this->updateDisplay();
    this->timer = new QTimer(this);
    //when the timer times out, call the on_timerUpdate() function
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timerUpdate()));
    this->timer->start(30000);             // time out after 5 sec
    this->createActions();
}

float MainWindow::celsiusToFahrenheit(float valueCelsius) {
   return ((valueCelsius * (9.0f/5.0f)) + 32.0f);
}

void MainWindow::createActions(){
    QAction *exit = new QAction("&Exit", this);
    exit->setShortcut(QKeySequence(tr("Ctrl+X")));
    QAction *settings = new QAction("&Settings", this);
    settings->setShortcut(QKeySequence(tr("Ctrl+S")));
    QMenu *menu = menuBar()->addMenu("&Menu");
    menu->addAction(settings);
    menu->addAction(exit);
    connect(exit, SIGNAL(triggered()), qApp, SLOT(quit()));  //quit application
    connect(settings, SIGNAL(triggered()), this, SLOT(on_openSettings()));
}

void MainWindow::on_getSample_clicked() {
    QDateTime local(QDateTime::currentDateTime()); // display sample time
    statusBar()->showMessage(QString("Last update: ").append(local.toString()));
    this->updateDisplay();
}

void MainWindow::on_timerUpdate() {
    this->on_getSample_clicked();
    this->updateDisplay();
}

void MainWindow::updateDisplay() {
    getSensorReading();
    if(this->isFahrenheit) {
       ui->lcdTemperature->display((double)celsiusToFahrenheit(temperature));
       ui->temperatureUnits->setText("F");
    }
    else {
       ui->lcdTemperature->display((double)temperature);
       ui->temperatureUnits->setText("C");
    }
    ui->lcdHumidity->display((double)humidity);
    ui->minTemperature->setText(QString::number(minTemperature));
    ui->maxTemperature->setText(QString::number(maxTemperature));
}

void MainWindow::on_radioButton_toggled(bool checked) {
    this->isFahrenheit = checked;
    this->updateDisplay();
}

void MainWindow::on_openSettings(){
    this->dialog->exec();                             // display the dialog box
    this->timer->start(1000*this->dialog->getTimeDelay()); //update timer delay
}

int MainWindow::getSensorReading(){
   // Get the server address and port from the settings dialog box
   int serverPort = this->dialog->getServerPort();  // get from the dialog box
   quint32 serverAddr = this->dialog->getIPAddress();   // from the dialog box
   QTcpSocket *tcpSocket = new QTcpSocket(this);    // create socket
   tcpSocket->connectToHost(QHostAddress(serverAddr), serverPort); // connect
   if(!tcpSocket->waitForConnected(1000)){    //wait up to 1s for a connection
      statusBar()->showMessage("Failed to connect to server...");
      return 1;
   }
   // Send the message "getReading" to the server
   tcpSocket->write("getReading");
   if(!tcpSocket->waitForReadyRead(3000)){    // wait up to 3s for the server
      statusBar()->showMessage("Server did not respond...");
      return 1;
   }
   // If the server has sent bytes back to the client
   if(tcpSocket->bytesAvailable()>0){
      int size = tcpSocket->bytesAvailable(); // how many bytes are ready?
      char data[200];                         // upper limit of 200 chars
      tcpSocket->read(&data[0],(qint64)size); // read the number of bytes rec.
      data[size]='\0';                        // terminate the string
      cout << "Received the data [" << data << "]" << endl;
      this->parseJSONData(QString(data));
      if(temperature<=minTemperature) minTemperature = temperature;
      if(temperature>=maxTemperature) maxTemperature = temperature;
   }
   else{
      statusBar()->showMessage("No data available...");
   }
   return 0;    // the on_updateTemperature() slot will update the display
}

int MainWindow::parseJSONData(QString str){
   QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
   QJsonObject obj = doc.object();
   QJsonObject sample = obj["sample"].toObject();
   this->temperature = (float) sample["temperature"].toDouble();
   this->humidity = (float) sample["humidity"].toDouble();
   cout << "The temperature is " << temperature << " and humidity is " << humidity << endl;
   return 0;
}

MainWindow::~MainWindow() {
    delete ui;
}

