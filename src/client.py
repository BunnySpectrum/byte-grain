from PyQt6.QtWidgets import QApplication, QWidget, QPushButton, QMainWindow, QLabel
from PyQt6.QtCore import QSize, Qt, QTimer, QThread
from PyQt6 import QtGui, uic

import numpy as np


import posix_ipc
import mmap
import os, sys
import socket

from SocketClient import Worker

class Canvas:
    def __init__(self, width, height):
        self.colCount = int(width)
        self.rowCount = int(height)
        self.data = np.zeros((self.rowCount, self.colCount), QtGui.QColor)
        self.data.fill(QtGui.QColorConstants.White)

    def add_grain(self, row, col, value=QtGui.QColorConstants.Black):
        self.data[row][col] = value

    def load_image(self, data):
        for x in range(0):
            pass

    def update(self):
        newData = np.zeros((self.rowCount, self.colCount), QtGui.QColor)
        newData.fill(QtGui.QColorConstants.White)

        for row in range(0, self.rowCount):
            for col in range(0, self.colCount):
                if self.data[row][col] != QtGui.QColorConstants.White:
                    # print(f"({x}, {y}) = {self.data[x][y]}")
                    if row+1 == self.rowCount:
                        newData[row][col] = self.data[row][col]
                    elif self.data[row+1][col] == QtGui.QColorConstants.White:
                        newData[row+1][col] = self.data[row][col]
                    else:
                        newData[row][col] = self.data[row][col]
        self.data = newData


def int_to_qcolor(val):
    if val == 1:
        return QtGui.QColorConstants.Black
    elif val == 2:
        return QtGui.QColorConstants.Red
    elif val == 3:
        return QtGui.QColorConstants.Blue
    else:
        return QtGui.QColorConstants.White

# Only needed for access to command line arguments
import sys
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.thread = QThread()
        self.worker = Worker()
        self.worker.moveToThread(self.thread)
        self.thread.started.connect(self.worker.run)
        self.worker.progress.connect(self.updateImage)
        self.thread.start()


        # self.setup_mailbox()

        self.gfxWidth = 400
        self.gfxHeight = 400
        self.grainLength = 10

        self.label = QLabel()
        pixmap = QtGui.QPixmap(600, 600)
        pixmap.fill(Qt.GlobalColor.white)
        self.label.setPixmap(pixmap)

        self.setCentralWidget(self.label)
        self.canvas = Canvas(32, 32)
        self.canvas.add_grain(20, 15, QtGui.QColorConstants.Black)
        # print(self.canvas.data)
        self.canvas.add_grain(22, 15, QtGui.QColorConstants.Black)
        self.canvas.add_grain(20, 13, QtGui.QColorConstants.Black)
        self.canvas.add_grain(20, 14, QtGui.QColorConstants.Red)
        self.draw_canvas()
        
        self.start_timer()

    def start_timer(self):
        self.timer = QTimer()
        self.timer.setInterval(int(1000/30))
        self.timer.timeout.connect(self.update)
        self.timer.start()

    def draw_canvas(self):
        canvas = self.label.pixmap()
        
        # painter.setBrush(QtGui.QColorConstants.Red)
        for row in range(0, self.canvas.rowCount):
            for col in range(0, self.canvas.colCount):
                if self.canvas.data[row][col] != None:
                    # print("Hit")
                    painter = QtGui.QPainter(canvas)
                    pen = QtGui.QPen()
                    pen.setWidth(self.grainLength)
                    colorCode = int_to_qcolor(self.canvas.data[row][col])
                    pen.setColor(colorCode)

                    painter.setPen(pen)
                    painter.drawPoint(col*self.grainLength+100, row*self.grainLength+100)
                    painter.end()

        self.label.setPixmap(canvas)

    def update(self):
        pass
        # self.canvas.update()
        # self.draw_canvas()

    def updateImage(self, value):
        # print(f"UI got {value!r}\n")
        self.canvas.data = value
        # self.canvas.add_grain(value[0], value[1], QtGui.QColorConstants.Blue)
        # self.canvas.update()
        self.draw_canvas()

    # def setup_mailbox(self):
    #     addr = "test_socket2"
    #     # data = None
    #     with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as s:
    #         s.connect(addr)
    #         s.sendall(b"Hello, world")
    #         data = s.recv(1024)

    #     print(f"Received {data!r}")

    def check_messages(self):
        pass

    # def mousePressEvent(self, e):
    #     print(e.pos())
    #     if(e.button() == Qt.MouseButton.RightButton):
    #         val = QtGui.QColorConstants.Red
    #     else:
    #         val = QtGui.QColorConstants.Black 

    #     self.canvas.add_grain(int(e.pos().x()/10), int(e.pos().y()/10), val)

# You need one (and only one) QApplication instance per application.
# Pass in sys.argv to allow command line arguments for your app.
# If you know you won't use command line arguments QApplication([]) works too.
app = QApplication(sys.argv)

# Create a Qt widget, which will be our window.
window = MainWindow()
window.show()  # IMPORTANT!!!!! Windows are hidden by default.

# Start the event loop.
app.exec()
