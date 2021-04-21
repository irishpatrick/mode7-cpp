#!/usr/bin/python3

import tkinter as tk
import pickle

class Piece:
    def __init__(self):
        self.name = "Unknown"
        self.type = -1
        self.limits = []
        self.verts = []
        self.joints = []

    def load(self, fn):
        pass

    def assemble(self, fn):
        pass

    def draw(self, canvas, origin):
        pass

class Track:
    def __init__(self):
        self.name = "Untitled"
        self.pieces = []
        self.origin = [0, 0]

    def draw(self):
        pass

class App(tk.Frame):
    def __init__(self):
        super().__init__()
        self.initui()

        self.camera_origin = [0,0]
        self.current_track = None

    def initui(self):
        self.master.title("Track Editor")
        self.pack(fill=tk.BOTH, expand=1)

        self.canvas = tk.Canvas(self)
        self.canvas.bind('<Button-1>', self.leftclick_cb)
        self.canvas.bind('<Motion>', self.mousemove_cb)
        self.canvas.pack(fill=tk.BOTH, expand=1)

    def leftclick_cb(self, event):
        pass

    def rightclick_cb(self, event):
        pass

    def mousemove_cb(self, event):
        print(event)

def main():
    root = tk.Tk()
    root.geometry("800x600+30+30")
    app = App()
    root.mainloop()

if __name__ == "__main__":
    main()
