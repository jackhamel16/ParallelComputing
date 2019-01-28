import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
 
gamma = 1

xmin = 0
xmax = 10
nx = 500

dx = (xmax - xmin)/nx

tmin = 0
tmax = 10
nt = 1000000

dt = (tmax - tmin)/nt

# animation information
animate = 1 # true will run the animation only if nt >= num_of_frames
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