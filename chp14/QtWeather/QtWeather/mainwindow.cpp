#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->isFahrenheit = false;
    statusBar()->showMessage("Sensor Application Started");
    this->maxTemperature = -100.0f;
    this->minTemperature = 100.0f;
    this->updateDisplay();
    this->timer = new QTimer(this);
    //when the timer times out, call the on_timerUpdate() function
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timerUpdate()));
    this->timer->start(5000);             // time out after 5 sec
}

float MainWindow::celsiusToFahrenheit(float valueCelsius) {
   return ((valueCelsius * (9.0f/5.0f)) + 32.0f);
}

void MainWindow::on_getSample_clicked() {
    QDateTime local(QDateTime::currentDateTime()); // display sample time
    statusBar()->showMessage(QString("Last update: ").append(local.toString()));
    this->readDHTSensor();
    if(temperature<minTemperature) minTemperature = temperature;
    if(temperature>maxTemperature) maxTemperature = temperature;
    this->updateDisplay();
}

void MainWindow::on_timerUpdate() {
    this->on_getSample_clicked();
    this->updateDisplay();
}

void MainWindow::updateDisplay() {

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

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::readDHTSensor(){
   wiringPiSetupGpio();
   piHiPri(99);
   int humid=0, temp=0;
   unsigned char data[5] = {0,0,0,0,0};
   pinMode(DHT_GPIO, OUTPUT);                 // gpio starts as output
   digitalWrite(DHT_GPIO, LOW);               // pull the line low
   usleep(18000);                             // wait for 18ms
   digitalWrite(DHT_GPIO, HIGH);              // set the line high
   pinMode(DHT_GPIO, INPUT);                  // now gpio is an input

   // need to ignore the first and second high after going low
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==HIGH);
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==LOW);
   do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==HIGH);
   // Remember the highs, ignore the lows -- a good philosophy!
   for(int d=0; d<5; d++) {       // for each data byte
      // read 8 bits
      for(int i=0; i<8; i++) {    // for each bit of data
         do { delayMicroseconds(1); } while(digitalRead(DHT_GPIO)==LOW);
         int width = 0;           // measure width of each high
         do {
            width++;
            delayMicroseconds(1);
            if(width>1000) break; // missed a pulse -- data invalid!
         } while(digitalRead(DHT_GPIO)==HIGH);    // time it!
         // shift in the data, msb first if width > the threshold
         data[d] = data[d] | ((width > LH_THRESHOLD) << (7-i));
      }
   }
   if (USING_DHT11){
      humid = data[0] * 10;            // one byte - no fractional part
      temp = data[2] * 10;             // multiplying to keep code concise
   }
   else {                              // for DHT22 (AM2302/AM2301)
      humid = (data[0]<<8 | data[1]);  // shift MSBs 8 bits left and OR LSBs
      temp = (data[2]<<8 | data[3]);   // same again for temperature
   }
   unsigned char chk = 0;   // the checksum will overflow automatically
   for(int i=0; i<4; i++){ chk+= data[i]; }
   if(chk==data[4]){
      temperature = ((float)temp)/10;
      humidity = ((float)humid)/10;
   }
   else {
      return -1;
   }
   return 0;
}

