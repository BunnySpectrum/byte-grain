from PyQt6.QtWidgets import QApplication, QWidget, QPushButton, QMainWindow, QLabel
from PyQt6.QtCore import QSize, Qt, QTimer, QObject, QThread, pyqtSignal
from PyQt6 import QtGui, uic

from time import sleep
import socket
import numpy as np

# https://doc.qt.io/qtforpython-6/PySide6/QtCore/QThread.html#more
# but some typos, so not really

# https://realpython.com/python-pyqt-qthread/#multithreading-in-pyqt-with-qthread
class Worker(QObject):
    # finished = pyqtSignal()
    progress = pyqtSignal(np.ndarray)

    def setup(self):
        pass
        # addr = "test_socket2"

    def cleanup(self):
        pass

    def run(self):
        self.setup()

        # long-running task
        result = self.recv_message()

        self.cleanup()
        # self.finished.emit()
            
    def recv_message(self):
        addr = "test_socket2"
        # data = None
        with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as s:
            s.connect(addr)

            while(True):
                # s.sendall(b"Hello, world")
                data = s.recv(1024)
                # print(f"Worker got {data!r}")

                result = np.zeros((32, 32), int)
                col = 0
                row = 0
                for idx in range(32*32):
                    result[row][col] = data[idx] & 0x3F
                    col += 1
                    if(col == 32):
                        row += 1
                        col = 0
                    
                # result_x = data[0]#int.from_bytes(data[0], "big")
                # result_y = data[1]#int.from_bytes(data[1], "big")
                self.progress.emit(result)

            printf("Worker loop done")


