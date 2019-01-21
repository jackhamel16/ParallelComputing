import numpy as np
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
  
def run(nx, nt):  
    gamma = 1
    
    xmin = 0
    xmax = 10
#    nx = 500
    
    dx = (xmax - xmin)/nx
    
    tmin = 0
    tmax = 10
#    nt = 1000000
    
    dt = (tmax - tmin)/nt
    
    # animation information
    animate = 0 # true will run the animation only if nt >= num_of_frames
    if animate == True:
        num_of_frames = 100
        step_size = nt // num_of_frames
        frame_mat = np.zeros((num_of_frames, nx))
    
    #initializing y positions
    y_pos = np.exp(-(np.linspace(xmin, xmax, nx) - 5)**2)
    y_vel = np.zeros(nx)
    
    if animate == True:
        frame_mat[0][:] = y_pos
    
    for t_idx in range(1, nt):
        y_vel[1:nx - 1] = y_vel[1:nx - 1] + gamma / dx**2 * dt *(y_pos[2:nx] + \
                     y_pos[0:nx - 2] - 2 * y_pos[1:nx - 1])
        y_pos = y_pos + y_vel * dt
        if animate == True:
            if t_idx % step_size == 0:
                frame_mat[t_idx // step_size][:] = y_pos   
        
    if animate == True: 
        fig = plt.figure()
        ax = plt.axes(xlim=(0,10), ylim=(-1,1))
        line, = ax.plot([], [], lw=2)
        
        def init():
            line.set_data([], [])
            return(line,)
        
        
        def animate(i):
            line.set_data(np.linspace(xmin, xmax, nx), frame_mat[i][:])
            return(line,)
        
        
        ani = animation.FuncAnimation(
            fig, animate, init_func=init, interval=30, blit=True)
        
        plt.show()

num_of_runs = 6

nx_vec = np.linspace(100, 500, 5, dtype=int)
nt_vec = np.logspace(2, 6, 5, dtype=int)

time_matrix = np.zeros((5,5))

for run_idx in range(num_of_runs):
    for nx_idx in range(5):
        for nt_idx in range(5):
            start_time = time.time()
            run(nx_vec[nx_idx], nt_vec[nt_idx])
            time_matrix[nx_idx, nt_idx] += (time.time() - start_time) \
                                           / num_of_runs
for nx_idx in [0,4]:
    plt.loglog(nt_vec, time_matrix[nx_idx][:])
plt.legend([100, 500], title="Degrees of Freedom")
plt.xlabel("Number of Timesteps")
plt.ylabel("Time (seconds)")
plt.title("Timing Results")