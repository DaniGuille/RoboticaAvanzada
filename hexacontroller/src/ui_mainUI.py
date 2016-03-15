# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'src/mainUI.ui'
#
# Created: Tue Mar  8 18:45:56 2016
#      by: pyside-uic 0.2.15 running on PySide 1.2.2
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_guiDlg(object):
    def setupUi(self, guiDlg):
        guiDlg.setObjectName("guiDlg")
        guiDlg.resize(384, 148)
        self.pushAvanzar = QtGui.QPushButton(guiDlg)
        self.pushAvanzar.setGeometry(QtCore.QRect(150, 70, 94, 31))
        self.pushAvanzar.setCheckable(True)
        self.pushAvanzar.setObjectName("pushAvanzar")

        self.retranslateUi(guiDlg)
        QtCore.QMetaObject.connectSlotsByName(guiDlg)

    def retranslateUi(self, guiDlg):
        guiDlg.setWindowTitle(QtGui.QApplication.translate("guiDlg", "hexacontroller", None, QtGui.QApplication.UnicodeUTF8))
        self.pushAvanzar.setText(QtGui.QApplication.translate("guiDlg", "Avanzar", None, QtGui.QApplication.UnicodeUTF8))

