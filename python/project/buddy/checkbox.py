#!/usr/bin/python
# checkbox.py

import sys
from PyQt4 import QtGui, QtCore

class CheckBox(QtGui.QWidget):
    def __init__(self, parent = None):
        QtGui.QWidget.__init__(self, parent)
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Checkbox')
        
        self.cb = QtGui.QCheckBox('Show title', self)
        self.cb.setFocusPolicy(QtCore.Qt.NoFocus)
        self.cb.move(10, 10)
        self.cb.toggle()
        self.connect(self.cb, QtCore.SIGNAL('stateChanged(int)'), self.changeTitle)
    
    def changeTitle(self, value):
        if self.cb.isChecked():
            self.setWindowTitle('Checkbox')
        else:
            self.setWindowTitle('Unchecked')
    
app = QtGui.QApplication(sys.argv)
w = CheckBox()
w.show()
sys.exit(app.exec_())
