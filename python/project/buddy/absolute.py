#!/usr/bin/python
# absolute.py

import sys
from PyQt4 import QtGui

class Absolute(QtGui.QWidget):
    def __init__(self, parent = None):
        QtGui.QWidget.__init__(self)
        
        self.setWindowTitle('Communicaton')
        label = QtGui.QLabel('Couldn\t', self)
        label.move(15, 10)
        label = QtGui.QLabel('less', self)
        label.move(30, 40)
        
        self.resize(250, 150)
        
app = QtGui.QApplication(sys.argv)
qb  = Absolute()
qb.show()
sys.exit(app.exec_())
