import numpy as np
import matplotlib.pyplot as plt

def average_times(times, count):
    new_times = np.zeros(int(times.size/3))
    idx = 0
    for i in range(int(times.size/3)):
        for j in range(count):
           new_times[i] += times[idx]
           idx += 1
        new_times[i] = new_times[i] / 3
    return(new_times)

serial_times = np.array([0.000902886, 0.000743002, 0.000744135, 
                         0.000654542, 0.000618088, 0.000619634, 
                         0.00183482, 0.00168141, 0.00175805, 
                         0.0124489, 0.0120123, 0.0126368, 
                         0.16348, 0.171822, 0.169072, 
                         1.72008, 1.76485, 1.7505,
                         22.6827, 22.6694, 22.74])

slow_times = np.array([0.000167126, 0.000180864, 0.000167815,
                       0.0152307, 0.0145129, 0.0142199,
                       1.69664, 1.70233, 1.68734])

threads_times_size = np.array([0.157985, 0.15098, 0.102942, 
                               0.155979, 0.155984, 0.153977,
                               0.156897, 0.155887, 0.155898,
                               0.15499, 0.155065, 0.00844887,
                               0.168334, 0.146471, 0.181982,
                               0.615002, 0.615095, 0.684443,
                               5.73746, 5.88548, 5.93916 ])

threads_times_threads = np.array([1.30633, 1.28342, 1.28482,
                                  0.948329, 0.948913, 0.94632,
                                  0.75124, 0.758817, 0.749403,
                                  0.715758, 0.660551, 0.658003,
                                  0.613325, 0.615091, 0.684527,
                                  0.589025, 0.578569, 0.579898 ])

parallel_times_size = np.array([0.00104388, 0.00097004, 0.000909028,
                                0.00191623, 0.00179881, 0.00162591,
                                0.00188216, 0.00189109, 0.00184886,
                                0.0042321, 0.00408425, 0.00410144,
                                0.0170202, 0.0172042, 0.0170817,
                                0.18676, 0.18693, 0.189776,
                                1.40521, 1.37543, 1.38158])

parallel_times_threads = np.array([1.26786, 1.27042, 1.29171,
                                   0.69908, 0.702698, 0.696268,
                                   0.409785, 0.41706, 0.415099,
                                   0.25667, 0.256403, 0.255578,
                                   0.182889, 0.187277, 0.18516,
                                   0.278361, 0.138068, 0.134589])

fft_sizes = np.logspace(2,8,7)
slow_sizes = np.array([100, 1000, 10000])
threads = [2**i for i in range(6)]

slow_times = average_times(slow_times, 3)
serial_times = average_times(serial_times, 3)
threads_times_size = average_times(threads_times_size, 3)
threads_times_threads = average_times(threads_times_threads, 3)
parallel_times_size = average_times(parallel_times_size, 3)
parallel_times_threads = average_times(parallel_times_threads, 3)

plt.loglog(slow_sizes, slow_times, fft_sizes, serial_times, fft_sizes, \
           threads_times_size, fft_sizes, parallel_times_size)
#plt.loglog(fft_sizes, serial_times, fft_sizes, \
#           threads_times_size, fft_sizes, parallel_times_size)
plt.legend(["Slow", "Serial", "Parallel FFTs Only", "Full Parallel"])
plt.title("Timing Study Over Various Matrix Sizes")
plt.xlabel("N, in NxN Matrix size")
plt.ylabel("Time (Seconds)")

plt.figure()
plt.loglog(threads, threads_times_threads, threads, parallel_times_threads)
plt.legend(["Parallel FFTs Only", "Full Parallel"])
plt.title("Timing Study Over Number of Threads")
plt.xlabel("Number of Threads")
plt.xticks(threads, [str(i) for i in threads])
plt.ylabel("Time (Seconds)")