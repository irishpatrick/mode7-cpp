#!/usr/bin/python3

import tkinter as tk
from tkinter import filedialog
import pickle
import re

class Piece:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.name = "Unknown"
        self.type = -1
        self.limits = []
        self.verts = []
        self.joints = []

    def load(self, fn):
        pass

    def assemble(self, fn):
        fp = open(fn, "r")
        for line in fp:
            line = line[:-1]
            parts = re.split(",| |\t", line)
            if line.startswith("name"):
                self.name = parts[1]
            elif line.startswith("v"):
                val = [int(i) for i in parts[1:]]
                self.verts.append((val[0], val[1]))

    def draw(self, canvas, origin):
        pass

class Track:
    def __init__(self):
        self.name = "Untitled"
        self.pieces = []
        self.origin = [0, 0]

    def draw(self):
        pass

class TrackCreator:
    def __init__(self, master):
        top = self.top = tk.Toplevel(master)
        top.title("New Track Dialogue")
        #tk.Grid.rowconfigure(top, 0, weight=1)
        tk.Grid.columnconfigure(top, 1, weight=1)
        self.l = tk.Label(top, text="Name")
        self.l.grid(row=0, column=0, sticky="W")
        self.e = tk.Entry(top)
        self.e.grid(row=0, column=2, sticky="EW")
        self.b = tk.Button(top, text="Ok", command=self.cleanup)
        self.b.grid(row=1, column=0, columnspan=3, sticky="EW")

        self.value = "Bad Value"

        top.transient(master)
        top.grab_set()
        master.wait_window(top)

    def cleanup(self):
        self.value = self.e.get()
        self.top.destroy()

class App(tk.Frame):
    def __init__(self):
        super().__init__()
        self.initui()

        self.camera_origin = [0,0]
        self.current_track = None

    def initui(self):
        root = self.master
        root.title("Track Editor")
        self.pack(fill=tk.BOTH, expand=1)

        self.canvas = tk.Canvas(self)
        self.canvas.bind('<Button-1>', self.leftclick_cb)
        self.canvas.bind('<Motion>', self.mousemove_cb)
        self.canvas.pack(fill=tk.BOTH, expand=1)

        menubar = tk.Menu(root)
        filemenu = tk.Menu(menubar, tearoff=0)
        filemenu.add_command(label="New Track", command=self.new_track)
        filemenu.add_command(label="Open Track", command=self.open_track)
        filemenu.add_command(label="Save Track", command=self.save_track)
        filemenu.add_command(label="Import Piece", command=self.import_piece)
        menubar.add_cascade(label="File", menu=filemenu)

        root.config(menu=menubar)

    def new_track(self):
        w = TrackCreator(self.master)
        self.current_track = Track()
        self.current_track.name = w.value

    def open_track(self):
        pass

    def save_track(self):
        fp = open("{}.trk".format(self.current_track.name), "wb")
        pickle.dump(self.current_track, fp)

    def import_piece(self):
        fn = filedialog.askopenfilename(
            initialdir="./", title="Import Piece",
            filetypes=(
                ("Track Pieces", "*.tp"),
                ("All Files", "*")))

        if fn == "":
            return

        p = Piece()
        p.assemble(fn)
        fp = open("pieces/{}.tpb".format(p.name), "wb")
        pickle.dump(p, fp)
        

    def leftclick_cb(self, event):
        pass

    def rightclick_cb(self, event):
        pass

    def mousemove_cb(self, event):
        pass#print(event)

def main():
    root = tk.Tk()
    root.geometry("800x600+30+30")
    app = App()
    root.mainloop()

if __name__ == "__main__":
    main()
