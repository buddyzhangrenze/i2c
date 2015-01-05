#!/usr/bin/python
# toolbar.py
import sys
from PyQt4 import QtGui, QtCore

class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent = None):
        QtGui.QMainWindow.__init__(self)
        self.resize(250, 150)
        self.setWindowTitle('Toolbar')
        
        self.exit = QtGui.QAction(QtGui.QIcon('/home/buddy/buddy.png'), 'Exit', self)
        self.exit.setShortcut('Ctrl+Q')
        self.connect(self.exit, QtCore.SIGNAL('triggered()'), QtGui.qApp, QtCore.SLOT('quit()'))
        self.toolbar = self.addToolBar('Exit')
        bu = self.addToolBar('open')
        m = QtGui.QAction(QtGui.QIcon('/home/buddy/buddy.png'), 'open', self)
        self.toolbar.addAction(self.exit)
        bu.addAction(m)

app = QtGui.QApplication(sys.argv)
main = MainWindow()
main.show()
sys.exit(app.exec_())
