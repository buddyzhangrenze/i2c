#!/usr/bin/python
# gridlayout.py
import sys
from PyQt4 import QtGui

class GridLayout(QtGui.QWidget):
    def __init__(self, parent = None):
        QtGui.QWidget.__init__(self)
        self.setWindowTitle('gridlayout')
        
        names = ['Cls', 'Bck', 'Close', '7', '6', '5', '2']
        grid = QtGui.QGridLayout()
        j = 0;
        pos = [(0, 0), (0, 2), (0, 3), (1, 0), (1, 1), (1, 2), (1, 3)]
        for i in names:
            button = QtGui.QPushButton(i)
            if j==2:
                grid.addWidget(QtGui.QLabel(''), 0, 2)
            else:
                grid.addWidget(button, pos[j][0], pos[j][1])
            j = j + 1
        self.setLayout(grid)

app = QtGui.QApplication(sys.argv)
qb = GridLayout()
qb.show()
sys.exit(app.exec_())
