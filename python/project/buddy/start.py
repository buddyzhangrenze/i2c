#!/usr/bin/python
# buddy01.py
import sys
from PyQt4 import QtGui
app = QtGui.QApplication(sys.argv)
widget = QtGui.QWidget()
widget.resize(200, 300)
widget.setWindowTitle('Buddy test')
widget.show()
sys.exit(app.exec_())
