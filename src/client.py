from PyQt6.QtWidgets import QApplication, QWidget, QPushButton, QMainWindow, QLabel
from PyQt6.QtCore import QSize, Qt
from PyQt6 import QtGui, uic

class Canvas:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.data = [[0]*self.height for _ in range(self.width)]

    def add_grain(self, x, y):
        self.data[x][y] = 1

# Only needed for access to command line arguments
import sys
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.label = QLabel()
        canvas = QtGui.QPixmap(400, 400)
        canvas.fill(Qt.GlobalColor.white)
        self.label.setPixmap(canvas)
        self.setCentralWidget(self.label)
        self.canvas = Canvas(400, 400)
        self.canvas.add_grain(200, 150)
        # print(self.canvas.data)
        self.canvas.add_grain(220, 150)
        self.canvas.add_grain(200, 130)
        self.draw_canvas()

    def draw_canvas(self):
        canvas = self.label.pixmap()
        
        # painter.setBrush(QtGui.QColorConstants.Red)
        for x in range(0, self.canvas.width):
            for y in range(0, self.canvas.height):
                if self.canvas.data[x][y] == 1:
                    # print("Hit")
                    painter = QtGui.QPainter(canvas)
                    pen = QtGui.QPen()
                    pen.setWidth(10)
                    pen.setColor(QtGui.QColorConstants.Black)
                    painter.setPen(pen)
                    painter.drawPoint(x, y)
                    painter.end()

        self.label.setPixmap(canvas)


# You need one (and only one) QApplication instance per application.
# Pass in sys.argv to allow command line arguments for your app.
# If you know you won't use command line arguments QApplication([]) works too.
app = QApplication(sys.argv)

# Create a Qt widget, which will be our window.
window = MainWindow()
window.show()  # IMPORTANT!!!!! Windows are hidden by default.

# Start the event loop.
app.exec()
