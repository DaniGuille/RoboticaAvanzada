/********************************************************************************
** Form generated from reading UI file 'mainUI.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINUI_H
#define UI_MAINUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QScrollBar>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_guiDlg
{
public:
    QSpinBox *spinBox;
    QScrollBar *horizontalScrollBar;
    QPushButton *legButton1;
    QPushButton *legButton2;

    void setupUi(QWidget *guiDlg)
    {
        if (guiDlg->objectName().isEmpty())
            guiDlg->setObjectName(QString::fromUtf8("guiDlg"));
        guiDlg->resize(400, 300);
        spinBox = new QSpinBox(guiDlg);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(250, 50, 91, 31));
        spinBox->setMinimum(-200);
        spinBox->setMaximum(200);
        horizontalScrollBar = new QScrollBar(guiDlg);
        horizontalScrollBar->setObjectName(QString::fromUtf8("horizontalScrollBar"));
        horizontalScrollBar->setGeometry(QRect(60, 60, 171, 20));
        horizontalScrollBar->setMinimum(-200);
        horizontalScrollBar->setMaximum(200);
        horizontalScrollBar->setOrientation(Qt::Horizontal);
        legButton1 = new QPushButton(guiDlg);
        legButton1->setObjectName(QString::fromUtf8("legButton1"));
        legButton1->setGeometry(QRect(100, 100, 104, 28));
        legButton2 = new QPushButton(guiDlg);
        legButton2->setObjectName(QString::fromUtf8("legButton2"));
        legButton2->setGeometry(QRect(100, 130, 104, 28));

        retranslateUi(guiDlg);

        QMetaObject::connectSlotsByName(guiDlg);
    } // setupUi

    void retranslateUi(QWidget *guiDlg)
    {
        guiDlg->setWindowTitle(QApplication::translate("guiDlg", "hexapodcontroller", 0, QApplication::UnicodeUTF8));
        legButton1->setText(QApplication::translate("guiDlg", "Pata 1", 0, QApplication::UnicodeUTF8));
        legButton2->setText(QApplication::translate("guiDlg", "Pata 2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class guiDlg: public Ui_guiDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
