import hw1
import numpy as np
import time
import matplotlib.pyplot as plt

num_of_runs = 6

nx_vec = np.linspace(100, 500, 5, dtype=int)
nt_vec = np.logspace(2, 6, 5, dtype=int)

time_matrix = np.zeros((5,5))

for run_idx in range(num_of_runs):
    for nx_idx in range(5):
        for nt_idx in range(5):
            start_time = time.time()
            hw1.run(nx_vec[nx_idx], nt_vec[nt_idx])
            time_matrix[nx_idx, nt_idx] += (time.time() - start_time) \
                                           / num_of_runs
for nx_idx in [0,4]:
    plt.loglog(nt_vec, time_matrix[nx_idx][:])
plt.legend([100, 500], title="Degrees of Freedom")
plt.xlabel("Number of Timesteps")
plt.ylabel("Time (seconds)")
plt.title("Timing Results")