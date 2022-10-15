
import csv
from msilib.schema import tables
import matplotlib.pyplot as plt
from prettytable import PrettyTable

def CsvToList(x, y, columnName, filename):
    with open(filename, 'r', encoding='UTF-8') as f:
        data = csv.reader(f, delimiter=';')
        i = 0
        for row in data:
            if i == 0:
                columnName.append(row[0])
                for j in row[1:-1]:
                        x.append(float(j))
            else:
                y.append([])
                columnName.append(row[0])
                for j in row[1:-1]:
                        y[i - 1].append(float(j))
            i+=1


def PrintTable(tableName, columnNames, columns):
    print("\n",tableName)
    myTable = PrettyTable()
    myTable.field_names = columnNames

    for i in range(len(columns[0])):
        rowList = []
        for j in range(len(columns)):
            rowList.append(columns[j][i])
        myTable.add_row(rowList)
    print(myTable)



def CreateGraphic(filename, grName, xLabel, yLabel, subplot):
    x = []
    y = []
    columnName = []
    colors = ['b', 'g', 'r', 'c', 'm', 'k']
    plt.subplot(subplot)
    plt.title(grName)
    CsvToList(x, y, columnName, filename)
    plt.ylabel(yLabel, color='gray')
    plt.xlabel(xLabel, color='gray')
    for i in range(len(y)):
        plt.plot(x, y[i], colors[i%len(colors)])
    plt.grid(True)
    plt.legend(columnName[1:], loc=7)
    dopList = y
    dopList.insert(0, x)
    PrintTable(grName, columnName, dopList)


CreateGraphic("BandWr.csv", "WriteBandwidth", 'Block Size', 'Bandwidth', 221)
CreateGraphic("BandR.csv", "ReadBandwidth", 'Block Size', 'Bandwidth', 222)
CreateGraphic("Err.csv", "Error", 'LaunchNum', 'Error (%)', 223)


plt.show()