#!/usr/bin/python3

import sys
import math

class Point:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

class Line:
    def __init__(self):
        self.p = Point()
        self.e = Point()

def print_usage():
    print("usage: ./track.py <radius>")

def main():
    num = 16
    if len(sys.argv) < 2:
        print_usage()
        return

    r = int(sys.argv[1])
    lines = []
    last = Point(r, 0)
    part = 2.0 * math.pi / num
    for i in range(1, num + 1):
        theta = i * part
        pt = Point(r * math.cos(theta), r * math.sin(theta))
        add = Line()
        add.p = last
        add.e = pt
        last = pt
        lines.append(add)

    fp = open("circle.track", "w")
    fp.write("{}\n".format(len(lines)))
    for l in lines:
        fp.write("{},{},{},{}\n".format(l.p.x, l.p.y, l.e.x, l.e.y))
    fp.close()

if __name__ == "__main__":
    main()
