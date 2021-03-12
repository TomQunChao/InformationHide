from matplotlib import pyplot as plt
import numpy as np


def read_all(filename):
    f = open(filename)
    x = []
    line = f.readline()
    line_s = line.split()
    for i in range(0, len(line_s)):
        x.append([])
    while line:
        line_s = line.split()
        for i in range(0, len(line_s)):
            x[i].append(float(line_s[i]))

        line = f.readline()
    f.close()
    return x


def draw_noise():
    x = read_all('atkgbc_d8_k8.hex')
    plt.xlabel('noise')
    plt.ylabel('error rate')
    plt.plot(x[0], x[1], label="none")
    plt.plot(x[0], x[2], label="guass")
    plt.plot(x[0], x[3], label="bright")
    plt.plot(x[0], x[4], label="contrast")
    plt.legend()
    plt.show()

    pass


def draw_jpeg():
    x = read_all('atkj_d8_k8.hex')
    plt.xlabel('jpeg')
    plt.ylabel('error rate')
    plt.plot(x[0], x[1], label="none")
    plt.plot(x[0], x[2], label="jpeg")
    plt.legend()
    plt.show()
    pass


def draw_delta():
    x = read_all('atk_n25_5_dd.hex')
    plt.xlabel('delta')
    plt.ylabel('error rate')
    plt.plot(x[0], x[1], label='none')
    plt.plot(x[0], x[2], label='noise STD=2.5')
    plt.plot(x[0], x[3], label='noise STD=5')
    plt.legend()
    plt.show()
    pass


if __name__ == '__main__':
    draw_noise()
    draw_jpeg()
    draw_delta()
