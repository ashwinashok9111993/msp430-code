# Companion file for Oscilloscope.c
#Simple Oscilloscope 

import os, sys
from PyQt4 import QtCore, QtGui
import scipy.io as sc
import numpy as np
import pyqtgraph as pg
import serial



class Example(QtGui.QMainWindow):
    def __init__(self):
        super(Example, self).__init__()
        self.a=[]
        self.ser = serial.Serial("/dev/ttyUSB0",115200)#replace with whatever COM port
        
        #use only 115200 baud
        
        self.initUI()
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.update_plot)
        self.timer.start(50)
        

    def initUI(self):
        
        exitAction = QtGui.QAction('&Exit', self)
        exitAction.setShortcut('Ctrl+Q')
        exitAction.setStatusTip('Exit application')
        exitAction.triggered.connect(QtGui.qApp.quit)
        
        
        self.plot = pg.PlotWidget()
        self.curve = self.plot.plot(pen='y')     
        self.plot.setRange(None,None,(0,1024))
        
        menubar = self.menuBar()
        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(exitAction)
        self.setCentralWidget(self.plot)
        
        self.statusBar().showMessage('Ready')
        
        self.setGeometry(250, 100, 700, 500)
        self.setWindowTitle('Oscilloscope')
        self.show()
        
    def update_plot(self):
        
		for i in xrange(100):
			self.ser.write('y')
			self.a.append(int(self.ser.readline().rstrip("\n")))
			self.p=np.array(self.a)
		
		self.curve.setData(self.p)
		self.a=[]
		


    
    
    
def main():
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
