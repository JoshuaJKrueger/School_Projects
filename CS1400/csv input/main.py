'''
Project Name: Stock Exchange Data
Author: Joshua J. Krueger
Due Date: 10/31/2020

This program takes a csv file input and creates a summary of the data.
'''


def sort(company, lineList):
    inserted = False

    for i, line in enumerate(company):
        if float(lineList[1]) > float(line[1]):
            company.insert(i, lineList)
            inserted = True

        if inserted:
            break

    if not inserted:
        company.append(lineList)


def calcMean(list):
    sum = 0.0

    for i in list:
        sum += float(i[1])

    return sum / len(list)


def main():
    aapl = []
    ibm = []
    msft = []

    try:
        with open('stocks_data.csv') as f:
            for line in f:
                lineList = line.rstrip().split(',')

                if lineList[0] == 'AAPL':
                    sort(aapl, lineList[1:3])
                elif lineList[0] == 'IBM':
                    sort(ibm, lineList[1:3])
                elif lineList[0] == 'MSFT':
                    sort(msft, lineList[1:3])
    except IOError:
        print('File stocks_data.csv does not exist.')
        return

    greatest = ''
    lowest = ''

    if aapl[0][1] > ibm[0][1] and aapl[0][1] > msft[0][1]:
        greatest = (f'AAPL ${str(round(float(aapl[0][1]), 2))} '
                    f'{str(aapl[0][0])}')
    elif ibm[0][1] > aapl[0][1] and ibm[0][1] > msft[0][1]:
        greatest = f'IBM ${str(round(float(ibm[0][1]), 2))} {str(ibm[0][0])}'
    else:
        greatest = (f'MSFT ${str(round(float(msft[0][1]), 2))} '
                    f'{str(msft[0][0])}')

    if aapl[-1][1] < ibm[-1][1] and aapl[-1][1] < msft[-1][1]:
        lowest = (f'AAPL ${str(round(float(aapl[-1][1]), 2))} '
                  f'{str(aapl[-1][0])}')
    elif ibm[-1][1] < aapl[-1][1] and ibm[-1][1] < msft[-1][1]:
        lowest = f'IBM ${str(round(float(ibm[-1][1]), 2))} {str(ibm[-1][0])}'
    else:
        lowest = (f'MSFT ${str(round(float(msft[-1][1]), 2))} '
                  f'{str(msft[-1][0])}')

    output = (f'AAPL\n----\nMax: ${str(round(float(aapl[0][1]), 2))} '
              f'{str(aapl[0][0])}\nMin: ${str(round(float(aapl[-1][1]), 2))} '
              f'{str(aapl[-1][0])}\nAve: ${str(round(calcMean(aapl), 2))}\n\n'
              f'IBM\n----\nMax: ${str(round(float(ibm[0][1]), 2))}'
              f' {str(ibm[0][0])}\nMin: ${str(round(float(ibm[-1][1]), 2))} '
              f'{str(ibm[-1][0])}\nAve: ${str(round(calcMean(ibm), 2))}\n\n'
              f'MSFT\n----\nMax: ${str(round(float(msft[0][1]), 2))} '
              f'{str(msft[0][0])}\nMin: ${str(round(float(msft[-1][1]), 2))}'
              f' {str(msft[-1][0])}\nAve: ${str(round(calcMean(msft), 2))}\n\n'
              f'Highest: {greatest}\nLowest: {lowest}')

    print(output)

    with open('stock_summary.txt', 'w') as f:
        f.write(output)

if __name__ == "__main__":
    main()
