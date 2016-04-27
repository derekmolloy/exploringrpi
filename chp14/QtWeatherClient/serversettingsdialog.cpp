#include "serversettingsdialog.h"
#include "ui_serversettingsdialog.h"

ServerSettingsDialog::ServerSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerSettingsDialog)
{
    this->serverPortNumber = 5555;
    this->timeDelay = 30;
    address[3] = 192;
    address[2] = 168;
    address[1] = 1;
    address[0] = 1;
    ui->setupUi(this);
}

ServerSettingsDialog::~ServerSettingsDialog()
{
    delete ui;
}

quint32 ServerSettingsDialog::getIPAddress(){
   //Convert the 4 bytes into a single 32-bit address
   return ((address[3]<<24)|(address[2]<<16)|(address[1]<<8)|address[0]);
}

void ServerSettingsDialog::on_buttonBox_accepted()
{
    this->serverPortNumber = ui->portSpinBox->value();
    this->timeDelay = ui->timeSampleSpinBox->value();
    this->address[0] = ui->addrBox4->value();
    this->address[1] = ui->addrBox3->value();
    this->address[2] = ui->addrBox2->value();
    this->address[3] = ui->addrBox1->value();
}

void ServerSettingsDialog::on_buttonBox_rejected()
{
    ui->portSpinBox->setValue(this->serverPortNumber);
    ui->timeSampleSpinBox->setValue(this->timeDelay);
    ui->addrBox4->setValue(this->address[0]);
    ui->addrBox3->setValue(this->address[1]);
    ui->addrBox2->setValue(this->address[2]);
    ui->addrBox1->setValue(this->address[3]);
    this->setVisible(false);
}
