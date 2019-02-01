import numpy as np
import matplotlib.pyplot as plt

filename = "./timings.dat"
file = open(filename, 'r')

sizes = np.array([200,1000,5000,8000,10000,15000,20000,40000])
line_list = []

for line in file:
    line_list.append(np.double(line))

times = np.array([np.zeros(len(sizes)) for i in range(3)])
    
for part_idx in range(3):
    for size_idx in range(len(sizes)):
        for run_idx in range(3):
            line_idx = part_idx * len(sizes) * 3 + 3 * size_idx + run_idx
#            print(part_idx, size_idx, run_idx, "   ", line_idx, line_list[line_idx])
            times[part_idx][size_idx] += line_list[line_idx]

times = times / 3
            
plt.figure()

for part_idx in range(3):
    plt.plot(sizes, times[part_idx])

plt.legend(["Part " + str(i+1) for i in range(3)])
plt.set_title("Transpose Timing Study")
plt.set_xlabel("Matrix Size")
plt.set_ylabel("Run Time")
#fig, axes = plt.subplots(2,1)
#
#for part_idx in range(3):
#    axes[0].plot(sizes, times[part_idx])
#
#axes[0].legend(["Part " + str(i+1) for i in range(3)])
#axes[0].set_title("Transpose Timing Study")
#axes[0].set_xlabel("Matrix Size")
#axes[0].set_ylabel("Run Time")
#
#col_labels = ["Basic", "Blocked", "Cache Oblivious"]
#row_labels = ["MatSize = {0:d}".format(sizes[i]) for i in range(len(sizes))]
##axes[1].axis('tight')
#axes[1].axis('off')
#
#times_formatted = np.transpose(np.array([np.array([0.0, 0.0, 0.0]) for i in range(len(sizes))]))
#for row in range(times.shape[0]):
#    for col in range(times.shape[1]):
#        times_formatted[row][col] = "{0:5.3f}".format(times[row][col])
#
#axes[1].table(cellText=np.transpose(times_formatted), colLabels=col_labels, \
#          rowLabels=row_labels, loc='center')
#
#fig.savefig('plot_timings.png')