#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTcpSocket>
#include <QMessageBox>
#include "serversettingsdialog.h"

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
    void on_getSample_clicked();
    void on_radioButton_toggled(bool checked);
    void on_timerUpdate();
    void on_openSettings();          // opens the server settings window

private:
    float temperature, humidity;
    float maxTemperature, minTemperature;
    bool isFahrenheit;
    QTimer *timer;                    // The timer that updates
    ServerSettingsDialog *dialog;     // The Server settings dialog

    int getSensorReading();
    void updateDisplay();
    float celsiusToFahrenheit(float valueCelsius);
    int parseJSONData(QString str);
    Ui::MainWindow *ui;

    void createActions();             // sets up the menu for the window
};

#endif // MAINWINDOW_H
