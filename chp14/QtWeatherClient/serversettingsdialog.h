#ifndef SERVERSETTINGSDIALOG_H
#define SERVERSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class ServerSettingsDialog;
}

/**
 * @brief The ServerSettingsDialog class is attached to the server settings user
 * interface.
 */
class ServerSettingsDialog : public QDialog
{
   Q_OBJECT                           // the Qt macro required for

public:
   explicit ServerSettingsDialog(QWidget *parent = 0); //pass ref to mainwindow
   ~ServerSettingsDialog();
   quint32 virtual getIPAddress();    // return the IP address as a 32-bit int
   int virtual getTimeDelay()  { return timeDelay; }  // the sample time
   int virtual getServerPort() { return serverPortNumber; } // the port number

private slots:
   /**
    * @brief on_buttonBox_accepted sets the states of the class according to the
    * options in the user-interface fields.
    */
   void on_buttonBox_accepted();      // OK button is pressed
   /**
    * @brief on_buttonBox_rejected reverts to the previous states.
    */
   void on_buttonBox_rejected();      // Cancel button is pressed

private:
   Ui::ServerSettingsDialog *ui;      // pointer to the UI components
   int serverPortNumber;              // port number (default 5555)
   int timeDelay;                     // time delay sec (default 30)
   int address[4];                    // IP address (default 192.168.1.1)
};

#endif // SERVERSETTINGSDIALOG_H
