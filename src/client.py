from PyQt6.QtWidgets import QApplication, QWidget, QPushButton, QMainWindow, QLabel
from PyQt6.QtCore import QSize, Qt
from PyQt6 import QtGui, uic



# Only needed for access to command line arguments
import sys
class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.label = QLabel()
        canvas = QtGui.QPixmap(400, 300)
        canvas.fill(Qt.GlobalColor.white)
        self.label.setPixmap(canvas)
        self.setCentralWidget(self.label)
        self.draw_grain()

    def draw_grain(self):
        canvas = self.label.pixmap()
        painter = QtGui.QPainter(canvas)

        pen = QtGui.QPen()
        pen.setWidth(10)
        pen.setColor(QtGui.QColorConstants.Black)
        painter.setPen(pen)
        # painter.setBrush(QtGui.QColorConstants.Red)

        painter.drawPoint(200, 150)
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
