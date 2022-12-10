'''
Program Name: Data Visualization
Programmed By: Joshua Krueger
Last Modified: 4/7/2021

Overall Program Desc.
    This program finds spikes in data and gives the area for the spike

Function and Class Descs.
    Smooth smooths the data for processing
    findSpikes finds data that jumps up in value
    calcAreas finds the area under the curve in a range
'''
import numpy
import matplotlib.pyplot as plt
import glob

def smooth(arr):
    '''Bring the data closer to remove some small spikes'''
    a = numpy.copy(arr)

    for i in range(3, len(a) - 3):
        a[i] = (arr[i-3]+2*arr[i-2]+3*arr[i-1]+3*arr[i]+3*arr[i+1]+2*arr[i+2]+arr[i+3]) // 15
    
    return a

def findSpike(a):
    '''Find sudden increased values'''
    starts = []
    skip = False

    for i, val in enumerate(a):
        if skip:
            if i+1 <= len(a) - 1:
                if a[i+1] < val:
                    skip = False
                continue
        if i+2 <= len(a) - 1:
            if a[i+2] - val >= 100:
                starts.append(i)
                skip = True
    
    return starts

def calcAreas(s, d):
    '''Find the area under the curve from spike to 50 values after or to next spike'''
    areas = []
    for i, val in enumerate(s):
        area = 0
        for j in range(50):
            if i + 1 <= len(s) - 1:
                if val + j == s[i + 1]:
                    break
            if val + j <= len(d) - 1:
                area += d[val + j]
        areas.append(area)
    return areas


def analyze(fname):
    '''
        Loads data and processes it
        Plots the data saving a pdf
        outputs results using .out
    '''
    data = numpy.loadtxt(fname)
    dataSmoothed = smooth(data)
    spikeStarts = findSpike(dataSmoothed)
    areas = calcAreas(spikeStarts, data)

    fig, axs = plt.subplots(2)
    fig.suptitle(fname)
    axs[0].plot(data)
    axs[1].plot(dataSmoothed)
    plt.setp(axs[0], ylabel='Raw')
    plt.setp(axs[1], ylabel='Smoothed')
    plt.savefig(f'{fname.split(".")[0]}.pdf')

    with open(f'{fname.split(".")[0]}.out', 'w+') as f:
        f.write(f'{fname}:\n')

        for i, val in enumerate(spikeStarts):
            f.write(f'Pulse {i+1}: {val} ({int(areas[i])})\n')

def main():
    for fname in glob.glob('*.dat'):
        analyze(fname)

if __name__ == '__main__':
    main()
