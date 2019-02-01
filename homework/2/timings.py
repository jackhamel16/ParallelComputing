import numpy as np
import matplotlib.pyplot as plt

filename = "./timings.dat"
file = open(filename, 'r')

sizes = np.array([200,1000,5000,8000])#,10000,15000,20000,40000])
line_list = []

for line in file:
    line_list.append(line)

times = np.array([np.zeros(len(sizes)) for i in range(3)])
    
for part_idx in range(3):
    for size_idx in range(len(sizes)):
        for run_idx in range(3):
            line_idx = part_idx * len(sizes) * 3 + 3 * size_idx + run_idx
#            print(part_idx, size_idx, run_idx, "   ", line_idx, line_list[line_idx])
            times[part_idx][size_idx] += np.double(line_list[line_idx])

times = times / 3
            
plt.figure()

for part_idx in range(3):
    plt.plot(sizes, times[part_idx])
    
plt.legend(["Part " + str(i) for i in range(3)])
plt.title("Transpose Timing Study")
plt.xlabel("Matrix Size")
plt.ylabel("Run Time")
plt.savefig('plot_timings.png')