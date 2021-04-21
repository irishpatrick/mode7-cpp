#!/usr/bin/python3

import sys
import os
import math
import re

class v2:
    
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def length(self):
        return math.sqrt(self.x * self.x + self.y * self.y)

    def normal(self):
        m = self.length()
        return v2(self.x / m, self.y / m)

    def __add__(self, other):
        return v2(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return v2(self.x - other.x, self.y - other.y)

    def __mul__(self, other):
        return self.x * other.x + self.y * other.y

    def scale(self, a):
        return v2(self.x * a, self.y * a)

    def __str__(self):
        return "<{}, {}>".format(self.x, self.y)

class Line:
    
    def __init__(self, ):
        self.p = v2()
        self.e = v2()
        self.v = v2()

    def create(self, p, v):
        self.p = p
        self.e = p
        self.v = v

    def connect(self, a, b):
        self.v = b - a
        self.p = a
        self.e = b

    def normal(self):
        return v2(-self.v.y, self.v.x).normal()

    def solve(self, t):
        return self.p + self.v.scale(t)

    def distTo(self, p):
        q = p - self.p
        return q * self.normal()

    def length(self):
        return (self.p - self.e).length()

    def angle(self):
        theta = math.atan2(self.v.y, self.v.x)
        if theta < 0.0:
            theta += 2 * math.pi

        return theta

def rad(deg):
    return deg * math.pi / 180.0

def deg(rad):
    return rad * 180.0 / math.pi

def print_usage():
    pass

def parse(line):
    return [float(i) for i in re.findall(r"[-+]?\d*\.\d+|\d+", line)]

def main():
    argc = len(sys.argv)
    if argc < 2:
        print_usage()
        return 0

    centerline = []
    racingline = []

    current = None
    last = None

    with open(sys.argv[1], "r") as fp:
        for line in fp:
            if current == None:
                if line.startswith(".centerline"):
                    current = centerline
                
                elif line.startswith(".racingline"):
                    current = racingline
                
                else:
                    print("bad op")
            
            else:
                if line.startswith("beg"):
                    pts = parse(line)
                    last = v2(pts[0], pts[1])
                
                elif line.startswith("lin"):
                    l = Line()
                    p = parse(line)
                    e = v2(p[0], p[1])
                    l.connect(last, e)
                    current.append(l)
                    last = e

                elif line.startswith("arc"):
                    pts = parse(line)
                    radius = pts[0]
                    degs = pts[1]
                    hat = int(pts[2])

                    last_line = current[-1]
                    norm = last_line.normal().normal()

                    theta0 = last_line.angle()

                    if hat:
                        theta0 += -math.pi / 2.0
                    else:
                        norm = norm.scale(-1)
                        theta0 += math.pi / 2.0

                    center = last_line.e + norm.scale(radius)
                    steps = degs / 8
                    part = rad(degs) / float(steps)
                    if not hat:
                        part *= -1.0

                    prev = last
                    cur = v2()
                    for i in range(0, int(steps + 1)):
                        theta = theta0 + part * float(i)
                        cur.x = radius * math.cos(theta)
                        cur.y = radius * math.sin(theta)
                        cur = center + cur
                        l = Line()
                        l.connect(prev, cur)
                        current.append(l)
                        prev = cur

    with open(sys.argv[2], "w") as fp:
        for line in centerline:
            fp.write("{},{},{},{}\n".format(line.p.x, line.p.y, line.e.x, line.e.y))


if __name__ == "__main__":
    main()
