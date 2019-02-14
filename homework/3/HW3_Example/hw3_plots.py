import numpy as np
import matplotlib.pyplot as plt

num_threads = [1,2,4,8,16]
times = np.array([[14*60+2.84, 13*60+30.957, 13*60+3.016],\
                  [5*60+52.539, 6*60+13.882, 5*60+55.575],\
                  [3*60+41.678, 3*60+45.492, 3*60+38.665],\
                  [6*60+2.714, 6*60+58.164, 6*60+15.862],
                  [11*60+54.823, 11*60+9.245, 11*60+39.192]])

avg_times = np.sum(times, 1) / 3
plt.plot(num_threads, avg_times/60)
plt.xticks(num_threads)
plt.xlabel("Number of Threads")
plt.ylabel("Runtime (Minutes)")
plt.title("Parallelized Scaling of BWA Alignment")