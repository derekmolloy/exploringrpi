/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLCDNumber *lcdTemperature;
    QLCDNumber *lcdHumidity;
    QLabel *label;
    QLabel *temperatureUnits;
    QRadioButton *radioButton;
    QLineEdit *minTemperature;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *maxTemperature;
    QPushButton *getSample;
    QMenuBar *menuBar;
    QMenu *menuMenu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(491, 243);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lcdTemperature = new QLCDNumber(centralWidget);
        lcdTemperature->setObjectName(QStringLiteral("lcdTemperature"));
        lcdTemperature->setGeometry(QRect(20, 10, 181, 101));
        lcdHumidity = new QLCDNumber(centralWidget);
        lcdHumidity->setObjectName(QStringLiteral("lcdHumidity"));
        lcdHumidity->setGeometry(QRect(260, 10, 181, 101));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(450, 20, 51, 31));
        QFont font;
        font.setPointSize(28);
        label->setFont(font);
        temperatureUnits = new QLabel(centralWidget);
        temperatureUnits->setObjectName(QStringLiteral("temperatureUnits"));
        temperatureUnits->setGeometry(QRect(210, 20, 31, 31));
        temperatureUnits->setFont(font);
        radioButton = new QRadioButton(centralWidget);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(260, 120, 181, 20));
        minTemperature = new QLineEdit(centralWidget);
        minTemperature->setObjectName(QStringLiteral("minTemperature"));
        minTemperature->setGeometry(QRect(90, 120, 113, 22));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 120, 41, 16));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 150, 41, 16));
        maxTemperature = new QLineEdit(centralWidget);
        maxTemperature->setObjectName(QStringLiteral("maxTemperature"));
        maxTemperature->setGeometry(QRect(90, 150, 113, 22));
        getSample = new QPushButton(centralWidget);
        getSample->setObjectName(QStringLiteral("getSample"));
        getSample->setGeometry(QRect(300, 150, 141, 22));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 491, 19));
        menuMenu = new QMenu(menuBar);
        menuMenu->setObjectName(QStringLiteral("menuMenu"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuMenu->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "RPi Weather Application", 0));
        label->setText(QApplication::translate("MainWindow", "%", 0));
        temperatureUnits->setText(QApplication::translate("MainWindow", "C", 0));
        radioButton->setText(QApplication::translate("MainWindow", "Use Fahrenheit", 0));
        label_2->setText(QApplication::translate("MainWindow", "Min:", 0));
        label_3->setText(QApplication::translate("MainWindow", "Max:", 0));
        getSample->setText(QApplication::translate("MainWindow", "Get Sample Now", 0));
        menuMenu->setTitle(QApplication::translate("MainWindow", "Menu", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
