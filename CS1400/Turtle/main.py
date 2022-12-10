'''
Project Name: Turtle Patterns II
Author: Joshua J. Krueger
Due Date: Nov 14, 2020
Course: CS1400-005

This program uses the turtle module to draw scalable scenes from basic shapes
Standard Settings:
width: 800
height: 600
monitor: 2
'''
import turtle
import math
import random


def rotate(o, p, a):
    '''
    Simple helper function to rotate vertices
    '''
    x = o[0] + math.cos(a) * (p[0] - o[0]) - math.sin(a) * (p[1] - o[1])
    y = o[1] + math.sin(a) * (p[0] - o[0]) - math.cos(a) * (p[1] - o[1])

    return (x, y)


class Polygon:
    '''
    Takes various parameters and generates an vertex polygon
    '''
    def __init__(self, nS, c, color, s=1, sL=50, o=False, rD=6, a=0):
        self.verts = []
        self.color = color

        self.genVerts(nS, c, s, sL, o, rD, a)

    def genVerts(self, nS, c, s, sL, o, rD, a):
        sL /= s
        r = sL / (2 * math.sin(math.pi / nS))
        r1 = sL / (rD * math.sin(math.pi / nS))

        for i in range(nS):
            if o and i % 2 == 0:
                dX = c[0] + r1 * math.cos(math.pi / nS * (1 + 2 * i))
                dY = c[1] + r1 * math.sin(math.pi / nS * (1 + 2 * i))
            else:
                dX = c[0] + r * math.cos(math.pi / nS * (1 + 2 * i))
                dY = c[1] + r * math.sin(math.pi / nS * (1 + 2 * i))

            self.verts.append(rotate(c, (dX, dY), a))
        self.verts.append((self.verts[0][0], self.verts[0][1]))


class Square(Polygon):
    def __init__(self, c, color, _s=1, _a=0):
        super().__init__(4, c, color, s=_s, a=_a)


class Star(Polygon):
    def __init__(self, c, color, _s=1, _a=0):
        super().__init__(10, c, color, s=_s, sL=25, o=True, rD=4, a=_a)


class Pentegon(Polygon):
    def __init__(self, c, color, _s=1, _a=0):
        super().__init__(5, c, color, s=_s, a=_a)


class Triangle(Polygon):
    def __init__(self, c, color, _s=1, _a=0):
        super().__init__(3, c, color, s=_s, a=_a)


class Rectangle():
    def __init__(self, bB, s, color):
        self.color = color
        self.verts = [
            (s * -(bB.w / 2) + bB.x, s * (bB.h / 2) + bB.y),
            (s * (bB.w / 2) + bB.x, s * (bB.h / 2) + bB.y),
            (s * (bB.w / 2) + bB.x, s * -(bB.h / 2) + bB.y),
            (s * -(bB.w / 2) + bB.x, s * -(bB.h / 2) + bB.y),
            (s * -(bB.w / 2) + bB.x, s * (bB.h / 2) + bB.y)
        ]


class bBox:
    '''
    Custom storage class
    '''
    def __init__(self, c, d):
        self.x = c[0]
        self.y = c[1]
        self.w = d[0]
        self.h = d[1]


class Scene:
    def __init__(self, bB):
        self.shapes = []
        self.bB = bB

    def draw(self, scale):
        for shape in self.shapes:
            turtle.up()
            turtle.pencolor(shape.color[0])
            turtle.fillcolor(shape.color[1])
            turtle.begin_fill()

            for vert in shape.verts:
                turtle.goto(scale * (vert[0] - self.bB.x) + self.bB.x,
                            scale * (vert[1] - self.bB.y) + self.bB.y)
                turtle.down()

            turtle.end_fill()


def genSScene(sScene, colors, scale):
    '''
    This function generates all the shapes for the scaled scene
    I put it in a function to improve readability
    '''
    t = bBox((sScene.bB.x, 170), (sScene.bB.w, sScene.bB.h))
    t1 = (colors['lightBlue'], colors['lightBlue'])
    sScene.shapes.append(Rectangle(t, 1, t1))

    # Stars
    for i in range(int(sScene.bB.w / 40)):
        x = (sScene.bB.x) + (random.randint(-sScene.bB.w / 2,
                                            sScene.bB.w / 2))
        y = (sScene.bB.y + sScene.bB.h / 2) + (
            random.randint(int(-sScene.bB.h / 7), int(sScene.bB.h / 7)))
        sScene.shapes.append(Star((x, y), (
            colors['yellow'], colors['yellow']), 4, i))

    # Sky & Ground
    x = int(sScene.bB.x - (sScene.bB.w / 2) + 25)
    y = int(sScene.bB.y - (sScene.bB.h / 2) + 25)
    oY = y
    oX = x
    f = True
    for i in range(oY + int(sScene.bB.h / 3), oY, -50):
        for j in range(oX, int(oX + sScene.bB.w), 50):
            if f:
                sScene.shapes.append(Square((j, i), (
                    colors['darkGreen'], colors['limeGreen']), .7))
            else:
                sScene.shapes.append(Square((j, i), (
                    colors['brown'], colors['lightBrown']), .7))
        f = False

    # Tree Trunk
    y = oY + int(sScene.bB.h / 3) + 25
    for i in range(int((sScene.bB.h / 3) / 20)):
        turtle.tilt(10)
        sScene.shapes.append(Pentegon((sScene.bB.x + sScene.bB.w / 4, y), (
            colors['brown'], colors['lightBrown']), 2, -(math.pi / 2)))
        y += 25

    # Tree Leaves
    angle = -(math.pi / 2)
    oX = sScene.bB.x + sScene.bB.w / 4
    oY = y
    for i in range(3):
        x = oX
        x1 = oX
        y = oY

        for j in range(6):
            x += 10 + (i * 3)
            x1 -= 10 + (i * 3)
            y -= 10 + (j - (i * 3))
            t = (colors['darkGreen'], colors['darkGreen'])
            t1 = lambda: random.uniform(1.5, 2.5)
            sScene.shapes.append(Triangle((x, y), t, t1(), angle))
            sScene.shapes.append(Triangle((x1, y), t, t1(), angle))
            angle += (math.pi / 2)
        oY += 20

    sScene.draw(scale)


def main():
    try:
        width = int(input('Enter the width: '))
        height = int(input('Enter the height: '))
        monitor = int(input(f'Enter the monitor on which to draw'
                            f'the smaller scene (1-3): '))
    except ValueError:
        print('Enter positive integers')
        return

    if width < 1 or height < 1 or monitor < 1:
        print('Enter positive integers')
        return

    if monitor > 3:
        print('Enter a number between 1 and 3 for the third input')

    screen = turtle.Screen()
    screen.screensize(width, height)
    screen.tracer(0, 0)

    turtle.ht()

    colors = {
        'lightBrown': (160 / 255, 130 / 255, 50 / 255),
        'brown': (110 / 255, 70 / 255, 0),
        'darkGreen': (0 / 255, 200 / 255, 0),
        'limeGreen': (0 / 255, 255 / 255, 0),
        'green': (0, 70 / 255, 0),
        'black': (0, 0, 0),
        'white': (1, 1, 1),
        'yellow': (1, 1, 0),
        'lightBlue': (173 / 255, 216 / 255, 230 / 255),
        'lightGrey': (211 / 255, 211 / 255, 211 / 255),
        'grey': (128 / 255, 128 / 255, 128 / 255),
        'lightBlack': (75 / 255, 75 / 255, 75 / 255)
    }

    s = Scene(bBox((0, 0), (width, height)))

    # Monitors
    t = (colors['black'], colors['black'])
    for i in range(-200, 201, 200):
        s.shapes.append(Rectangle(bBox((i, 50), (15, 100)), 1, t))
        s.shapes.append(Rectangle(bBox((i, 6), (50, 15)), 1, t))
    genMonitor = lambda c, s: [Rectangle(bBox(c, (2, 1)), s, t), Rectangle(
        bBox(c, (2, 1)), s - s / 20, (colors['green'], colors['green']))]
    for i in range(-200, 400, 200):
        s.shapes += genMonitor(((i, 100)), 100)

    # Desk
    for i in range(2):
        for j in range(2):
            s.shapes.append(Rectangle(bBox(((-300 - (
                j * 15)) if i % 2 == 0 else (300 + (j * 15)), -100 - (
                    j * 10)), (20, 200)), 1, (
                        colors['lightGrey'], colors['grey'])))

    s.shapes.append(Polygon(50, (50, 5), t, sL=1))
    s.shapes.append(Rectangle(bBox((s.bB.x, s.bB.y - 5), (
        700, 15)), 1, (colors['lightGrey'], colors['grey'])))
    s.shapes.append(Rectangle(bBox((-50, 5), (100, 5)), 1, (
        colors['lightBlack'], colors['black'])))

    s.draw(1)

    sScene = Scene(bBox(((monitor - 2) * 200, 90), (width, height)))
    genSScene(sScene, colors, .15)

    screen.update()
    screen.exitonclick()


if __name__ == "__main__":
    main()
