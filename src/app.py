# https://realpython.com/python-bindings-overview/#c-or-c-source
import ctypes
import pathlib

if __name__ == "__main__":
    libname = pathlib.Path().absolute() / "libfoo.so"
    cLib = ctypes.CDLL(libname)

    cLib.cmult.restype = ctypes.c_float
    x = 2
    y = 3.2
    answer = cLib.cmult(x, ctypes.c_float(y))
    print(answer)
