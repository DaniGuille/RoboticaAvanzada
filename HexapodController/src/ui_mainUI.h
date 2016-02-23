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
#include <QtGui/QScrollBar>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_guiDlg
{
public:
    QSpinBox *spinBox;
    QScrollBar *horizontalScrollBar;

    void setupUi(QWidget *guiDlg)
    {
        if (guiDlg->objectName().isEmpty())
            guiDlg->setObjectName(QString::fromUtf8("guiDlg"));
        guiDlg->resize(400, 300);
        spinBox = new QSpinBox(guiDlg);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(250, 50, 91, 31));
        horizontalScrollBar = new QScrollBar(guiDlg);
        horizontalScrollBar->setObjectName(QString::fromUtf8("horizontalScrollBar"));
        horizontalScrollBar->setGeometry(QRect(60, 60, 171, 20));
        horizontalScrollBar->setOrientation(Qt::Horizontal);

        retranslateUi(guiDlg);

        QMetaObject::connectSlotsByName(guiDlg);
    } // setupUi

    void retranslateUi(QWidget *guiDlg)
    {
        guiDlg->setWindowTitle(QApplication::translate("guiDlg", "hexapodcontroller", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class guiDlg: public Ui_guiDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINUI_H
