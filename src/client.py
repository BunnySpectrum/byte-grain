from PyQt6.QtWidgets import QApplication, QWidget, QPushButton, QMainWindow, QLabel
from PyQt6.QtCore import QSize, Qt, QTimer
from PyQt6 import QtGui, uic

class Canvas:
    def __init__(self, width, height):
        self.width = int(width)
        self.height = int(height)
        self.data = [[QtGui.QColorConstants.White]*self.height for _ in range(self.width)]

    def add_grain(self, x, y, value=QtGui.QColorConstants.Black):
        self.data[x][y] = value

    def update(self):
        newData = [[QtGui.QColorConstants.White]*self.height for _ in range(self.width)]
        for x in range(0, self.width):
            for y in range(0, self.height):
                if self.data[x][y] != QtGui.QColorConstants.White:
                    # print(f"({x}, {y}) = {self.data[x][y]}")
                    if y+1 == self.height:
                        newData[x][y] = self.data[x][y]
                    elif self.data[x][y+1] == QtGui.QColorConstants.White:
                        newData[x][y+1] = self.data[x][y]
                    else:
                        newData[x][y] = self.data[x][y]
        self.data = newData


# Only needed for access to command line arguments
import sys
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.gfxWidth = 400
        self.gfxHeight = 400
        self.grainLength = 10

        self.label = QLabel()
        pixmap = QtGui.QPixmap(400, 400)
        pixmap.fill(Qt.GlobalColor.white)
        self.label.setPixmap(pixmap)

        self.setCentralWidget(self.label)
        self.canvas = Canvas(self.gfxWidth/self.grainLength, self.gfxHeight/self.grainLength)
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
        for x in range(0, self.canvas.width):
            for y in range(0, self.canvas.height):
                if self.canvas.data[x][y] != None:
                    # print("Hit")
                    painter = QtGui.QPainter(canvas)
                    pen = QtGui.QPen()
                    pen.setWidth(self.grainLength)
                    pen.setColor(self.canvas.data[x][y])
                    painter.setPen(pen)
                    painter.drawPoint(x*self.grainLength, y*self.grainLength)
                    painter.end()

        self.label.setPixmap(canvas)

    def update(self):
        self.canvas.update()
        self.draw_canvas()

    def mousePressEvent(self, e):
        print(e.pos())
        if(e.button() == Qt.MouseButton.RightButton):
            val = QtGui.QColorConstants.Red
        else:
            val = QtGui.QColorConstants.Black 

        self.canvas.add_grain(int(e.pos().x()/10), int(e.pos().y()/10), val)

# You need one (and only one) QApplication instance per application.
# Pass in sys.argv to allow command line arguments for your app.
# If you know you won't use command line arguments QApplication([]) works too.
app = QApplication(sys.argv)

# Create a Qt widget, which will be our window.
window = MainWindow()
window.show()  # IMPORTANT!!!!! Windows are hidden by default.

# Start the event loop.
app.exec()
