#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#define USING_DHT11      false   // The DHT11 uses only 8 bits
#define DHT_GPIO         22      // Using GPIO 22 for this example
#define LH_THRESHOLD     26      // Low=~14, High=~38 - pick avg.


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

private:
    float temperature, humidity;
    float maxTemperature, minTemperature;
    bool isFahrenheit;
    QTimer *timer;
    void updateDisplay();
    int readDHTSensor();
    float celsiusToFahrenheit(float valueCelsius);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
