"""
Project Name: Random Walk
Author: Joshua J. Krueger
Due Date: 12/06/2020
Course: CS1400-005

This program simulates random walk in the form of characters roaming a field.
Valid participants all, a, pa, a, mi-ma, mi, ma, m-m, mm, m, reg, r
Example input: python(3) main.py 100,1000 50 p
"""
from math import sqrt
from random import choice, seed
from statistics import stdev
from turtle import Screen, Turtle
from sys import argv


def move(vDirs, pos):
    """Returns a new position based off valid directions"""
    rDir = choice(vDirs)

    if rDir == 'n':
        pos[1] += 1
    elif rDir == 'e':
        pos[0] += 1
    elif rDir == 's':
        pos[1] -= 1
    else:
        pos[0] -= 1
    return pos


def output(name, wL, fDists):
    """Generates an out put string"""
    mean = sum(fDists) / len(fDists)
    return (f"{name} random walk of {str(wL)} steps\n"
            f"Mean = {str('{:.1f}'.format(mean))} "
            f"CV = {str('{:.1f}'.format(stdev(fDists) / mean))}\n"
            f"Max = {str('{:.1f}'.format(max(fDists)))} "
            f"Min = {str('{:.1f}'.format(min(fDists)))}")


def getInput():
    """Gets and interprets input from argv"""
    if len(argv) != 4:
        print("Three parameters are required")
        return -1

    wLs = argv[1].split(',')
    trials = argv[2]
    part = argv[3]
    for i, wL in enumerate(wLs):
        if not (wL.isdigit()):
            print("Walk lengths must be integers")
            return -1
        else:
            if int(wL) < 1:
                print("Walk length must be positive")
                return -1
            else:
                wLs[i] = int(wL)

    if not(trials.isdigit()):
        print("Number of trials must be an integer greater than 1")
        return -1
    else:
        if int(trials) < 2:
            print("Number of trials must be an integer greater than 1")
            return -1
        else:
            trials = int(trials)

    if not(part.isalpha()):
        print('Enter a valid participant identifier')
        return -1
    else:
        part = part.lower()
        if part in ['a', 'all']:
            part = ['p', 'm', 'r']
        else:
            if not(part in ['pa', 'p', 'mi-ma', 'mi', 'ma',
                            'm-m', 'mm', 'm', 'reg', 'r']):
                print("Enter a valid participant identifier")
                return -1
            else:
                part = [part]
    return [wLs, trials, part]


def createParts(parts):
    """Creates participants"""
    if 'p' in parts:
        parts[parts.index('p')] = {
            'name': 'Pa',
            'pos': [0, 0],
            'vDirs': ['n', 'e', 's', 'w'],
            'fPoses': [],
            'fDists': [],
            'shape': 'circle',
            'color': 'black'
        }
    if 'm' in parts:
        parts[parts.index('m')] = {
            'name': 'Mi-Ma',
            'pos': [0, 0],
            'vDirs': ['n', 'e', 's', 's', 'w'],
            'fPoses': [],
            'fDists': [],
            'shape': 'square',
            'color': 'green'
        }
    if 'r' in parts:
        parts[parts.index('r')] = {
            'name': 'Reg',
            'pos': [0, 0],
            'vDirs': ['e', 'w'],
            'fPoses': [],
            'fDists': [],
            'shape': 'triangle',
            'color': 'red'
        }
    return parts


def main():
    """
        Entry point of program
        Checks getInput()
        Initializes seed for random.choice and participants(createParts())
        Simulates the random walk printing outputs text
        Initializes turtle then plots data
    """

    pInput = getInput()
    wLs = []
    trials = 0
    parts = []

    if pInput == -1:
        return -1
    else:
        wLs = pInput[0]
        trials = pInput[1]
        parts = pInput[2]

    # seed(123456789)

    parts = createParts(parts)

    for part in parts:
        for wL in wLs:
            for _ in range(trials):
                for _ in range(wL):
                    move(part['vDirs'], part['pos'])
                part['fPoses'].append(part['pos'].copy())
                dist = sqrt(part['pos'][0]**2 + part['pos'][1]**2)
                part['fDists'].append(dist)
                part['pos'] = [0, 0]
            print(output(part['name'], wL, part['fDists']))

    screen = Screen()
    # screen.screensize(400, 600)
    screen.title("Random Walk")
    screen.tracer(0)

    turtle = Turtle()
    turtle.ht()
    turtle.pu()
    scale = 8

    for part in parts:
        turtle.shape(part['shape'])
        turtle.fillcolor(part['color'])

        for i, pos in enumerate(part['fPoses']):
            if i > 100:
                break
            tPos = [pos[0] * scale, pos[1] * scale]
            turtle.goto(tPos)
            turtle.stamp()

    screen.update()
    screen.exitonclick()


if __name__ == "__main__":
    main()
