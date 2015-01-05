#!/usr/bin/python
# menubar.py

import sys
from PyQt4 import QtGui, QtCore

class MainWindow(QtGui.QMainWindow):
    def __init__(self, parent = None):
        QtGui.QMainWindow.__init__(self)
        
        self.resize(800, 600)
        self.setWindowTitle('menubar')
        
        exit = QtGui.QAction(QtGui.QIcon('icons/exit.png'), 'Exit', self)
        exit.setShortcut('Ctrl+Q')
        exit.setStatusTip('Exit application')
        exit.connect(exit, QtCore.SIGNAL('triggered()'), QtGui.qApp, QtCore.SLOT('quit()'))
        
        self.statusBar()
        menubar = self.menuBar()
        file = menubar.addMenu('&File')
        bu = file.addMenu('&Make').addMenu('H')
        bu.addMenu('&ll')
        bu.addMenu('&l2')
        bu.addMenu('&l3')
        save = menubar.addMenu('&Save')
        file.addAction(exit)

app = QtGui.QApplication(sys.argv)
main = MainWindow()
main.show()
sys.exit(app.exec_())
