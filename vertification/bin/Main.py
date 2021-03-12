from matplotlib import pyplot as plt
import numpy as np


def read_arr5(filename):
    arr1 = []
    arr2 = []
    arr3 = []
    arr4 = []
    arr5 = []
    arr6 = []
    f = open(filename)
    n = int(f.readline())
    for i in range(0, n):
        line = f.readline().split()
        arr1.append(float(line[0]))
        arr2.append(float(line[1]))
        arr3.append(float(line[2]))
        arr4.append(float(line[3]))
        arr5.append(float(line[4]))
        arr6.append(float(line[5]))
    return np.asarray(arr1), np.asarray(arr2), np.asarray(arr3), \
           np.asarray(arr4), np.asarray(arr5), np.asarray(arr6)


def read_arr2(filename):
    exp_x = []
    exp_y = []
    the_x = []
    the_y = []
    f = open(filename)
    n = int(f.readline())
    for i in range(0, n):
        line = f.readline()
        line = line.split()
        exp_x.append(float(line[0]))
        exp_y.append(float(line[1]))
    for i in range(0, n):
        line = f.readline()
        # print(line, end=" ")
        line = line.split()
        # print(i, line)
        the_x.append(float(line[0]))
        the_y.append(float(line[1]))
    f.close()
    return np.asarray(exp_x), np.asarray(exp_y), np.asarray(the_x), np.asarray(the_y)


def plot_monte():
    x_t = ["1e-6", "1e-5", "1e-4", "1e-3", "1e-2", "1e-1"]
    plt.xticks(np.arange(-6, -1), x_t)
    plt.yticks(np.arange(-6, -1), x_t)
    # x~GGD(0,10),a=1.8,c=2
    x1 = read_arr2("noatk_a18_c20.hex")
    plt.plot(np.log10(x1[0]), np.log10(x1[1]), ".", label="Experiment,c=2,a=1.8")
    plt.plot(np.log10(x1[2]), np.log10(x1[3]), label="Theory,c=0.5,a=1.8")
    # # c=1
    x2 = read_arr2("noatk_a18_c10.hex")
    plt.plot(np.log10(x2[0]), np.log10(x2[1]), ".", label="Experiment,c=1,a=1.8")
    plt.plot(np.log10(x2[2]), np.log10(x2[3]), label="Theory,c=1,a=1.8")
    # # c=0.5
    x3 = read_arr2("noatk_a18_c10.hex")
    plt.plot(np.log10(x3[0]), np.log10(x3[1]), ".", label="Experiment,c=0.5,a=1.8")
    plt.plot(np.log10(x3[2]), np.log10(x3[3]), label="Theory,c=0.5,a=1.8")
    #
    # # a=1.5,c=2
    x4 = read_arr2("noatk_a15_c20.hex")
    plt.plot(np.log10(x4[0]), np.log10(x4[1]), ".", label="Experiment,c=2,a=1.5")
    plt.plot(np.log10(x4[2]), np.log10(x4[3]), label="Theory,c=2,a=1.5")

    # x~N(0,10), a=1.8, no noise, theory
    exp_x, exp_y, the_x, the_y = read_arr2("noatk_a18_n25.hex")
    plt.plot(np.log10(the_x), np.log10(the_y), label="Theory,std=10,c=2")
    exp_x, exp_y, the_x, the_y = read_arr2("noatk_a18_n50.hex")
    plt.plot(np.log10(the_x), np.log10(the_y), label="Theory,std=10,c=2")
    # x~N(0,10),a=1.8,noise~N(0,2.5)
    exp_x, exp_y, the_x, the_y = read_arr2("atk_a18_n25.hex")
    plt.plot(np.log10(exp_x), np.log10(exp_y), ".", label="Experiment,nStd=2.5,std=10")
    plt.plot(np.log10(the_x), np.log10(the_y), label="Theory,nStd=2.5,std=10")
    # x~N(0,10),a=1.8,noise~N(0,5)
    exp_x, exp_y, the_x, the_y = read_arr2("atk_a18_n50.hex")
    plt.plot(np.log10(exp_x), np.log10(exp_y), ".", label="Experiment,nStd=5,std=10")
    plt.plot(np.log10(the_x), np.log10(the_y), label="Theory,nStd=5,std=10")
    plt.legend()
    plt.show()


def plot_wmemb_achange():
    plt.xlabel("a")
    plt.ylabel("error rate")
    x = read_arr5("achange_c02_b02_g05.hex")
    plt.plot(x[0], x[1], label="mse")
    plt.legend()
    plt.show()
    plt.xlabel("a")
    plt.ylabel("error rate")
    plt.plot(x[0], x[2], label="no atk")
    plt.plot(x[0], x[3], label="bright=0.2")
    plt.plot(x[0], x[4], label="contrast=0.2")
    plt.plot(x[0], x[5], label="gaussian std=5")
    plt.legend()
    plt.show()
    plt.xlabel("a")
    plt.ylabel("error rate")
    x = read_arr5("achange_c05_b05_g10.hex")
    plt.plot(x[0], x[1], label="mse")
    plt.legend()
    plt.show()
    plt.xlabel("a")
    plt.ylabel("error rate")
    plt.plot(x[0], x[2], label="no atk")
    plt.plot(x[0], x[3], label="bright=0.5")
    plt.plot(x[0], x[4], label="contrast=0.5")
    plt.plot(x[0], x[5], label="gaussian std=10")
    plt.legend()
    plt.show()


def plot_wmemb_bchange():
    plt.xlabel("bright")
    plt.ylabel("error rate")
    x = read_arr2("a18_bchange.hex")
    plt.plot(x[0], x[1])
    plt.show()


def plot_wmemb_cchange():
    plt.xlabel("contrast")
    plt.ylabel("error rate")
    x = read_arr2("a18_cchange.hex")
    plt.plot(x[0], x[1])
    plt.show()


def plot_wmemb_gchange():
    plt.xlabel("std of gaussian")
    plt.ylabel("error rate")
    x = read_arr2("a18_gchange.hex")
    plt.plot(x[0], x[1])
    plt.show()


def plot_wmemb_jchange():
    plt.xlabel("level of compress")
    plt.ylabel("error rate")
    x = read_arr2("a18_jchange.hex")
    plt.plot(x[0], x[1])
    plt.show()


if __name__ == "__main__":
    plot_monte()

    plot_wmemb_achange()
    plot_wmemb_bchange()
    plot_wmemb_cchange()
    plot_wmemb_gchange()
    plot_wmemb_jchange();
