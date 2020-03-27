import tkinter
import serial
import threading

from tkinter import Canvas
from tkinter import Label
from tkinter import Entry
from tkinter import Button

top = tkinter.Tk()

canvGraf = Canvas(top, bg = "white", height = 310, width = 900)
canvGraf.create_line(0, 300, 900, 300, fill="lightgray")

lblP = Label(top, text = "P:")
lblI = Label(top, text = "I:")
lblD = Label(top, text = "D:")
entP = Entry(top, width = 10)
entI = Entry(top, width = 10)
entD = Entry(top, width = 10)

def sendP():
    msg = "P"+entP.get()+"\n"
    ser.write(msg.encode())

def sendI():
    msg = "I"+entP.get()+"\n"
    ser.write(msg.encode())

def sendD():
    msg = "D"+entP.get()+"\n"
    ser.write(msg.encode())

btnP = Button(top, text = "Ok", command = sendP)
btnI = Button(top, text = "Ok", command = sendI)
btnD = Button(top, text = "Ok", command = sendD)

canvGraf.pack (fill=tkinter.X)
lblP.pack(padx=5, pady=5, side=tkinter.LEFT)
entP.pack(padx=5, pady=5, side=tkinter.LEFT)
btnP.pack(padx=5, pady=5, side=tkinter.LEFT)
lblD.pack(padx=5, pady=5, side=tkinter.LEFT)
entD.pack(padx=5, pady=5, side=tkinter.LEFT)
btnD.pack(padx=5, pady=5, side=tkinter.LEFT)
lblI.pack(padx=5, pady=5, side=tkinter.LEFT)
entI.pack(padx=5, pady=5, side=tkinter.LEFT)
btnI.pack(padx=5, pady=5, side=tkinter.LEFT)

def read_from_port(ser):
    global running
    ptRef = None
    ptInp = None
    ptOut = None
    x = 0
    yRef0 = 0
    yInp0 = 0
    yOut0 = 0
    while running:
        linha = ser.readline().decode()
        partes = linha.split(" ")
        if len(partes) == 3:
            yRef1 = 300.0 - (float(partes[0])/20.0)*300.0
            yInp1 = 300.0 - (float(partes[1])/20.0)*300.0
            yOut1 = 300.0 - ((float(partes[2])+10000)/2000.0)*300.0
            try:
                canvGraf.create_line(x, yRef0, x+1, yRef1, fill="blue")
                canvGraf.create_line(x, yInp0, x+1, yInp1, fill="red")
                canvGraf.create_line(x, yOut0, x+1, yOut1, fill="green")
                yRef0 = yRef1
                yInp0 = yInp1
                yOut0 = yOut1
                # canvGraf.create_oval (x+2, yRef+2, x-2, yRef-2, fill="blue")
                # canvGraf.create_oval (x+2, yInp+2, x-2, yInp-2, fill="red")
                # canvGraf.create_oval (x+2, yOut+2, x-2, yOut-2, fill="green")
                x = x + 2
                if x >= 900:
                  x = 0
                  canvGraf.delete("all")
            except:
                print ("Fim")

ser = serial.Serial('/dev/ttyUSB0', 9600)
thread = threading.Thread(target=read_from_port, args=(ser,))
running = True
thread.start()

top.mainloop()

running = False
