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
nt = 10000

dt = (tmax - tmin)/nt

#initializing y positions
y_pos = np.exp(-(np.linspace(xmin, xmax, nx) - 5)**2)
y_vel = np.zeros(nx)
y_vel2 = np.zeros(nx)
y_acc = np.zeros(nx)
y_acc2 = y_acc

y_pos_mat = np.zeros((nt, nx))
y_pos_mat[0][:] = y_pos

for t_idx in range(0,nt - 1):
#    y_acc2[0], y_acc2[nx - 1] = 0, 0
    for x_idx in range(1, nx - 1):
#        y_acc[x_idx] = gamma / dx**2 * (y_pos[x_idx + 1] + \
#                       y_pos[x_idx - 1] - 2 * y_pos[x_idx])
        y_acc2[x_idx] = gamma / dx**2 * (y_pos_mat[t_idx][x_idx + 1] + \
                           y_pos_mat[t_idx][x_idx - 1] - 2 * y_pos_mat[t_idx][x_idx])
#    y_pos = y_pos + y_vel * dt
    y_pos_mat[t_idx + 1][:] = y_pos_mat[t_idx][:] + y_vel2 * dt
#    y_vel = y_vel + y_acc * dt
    y_vel2 = y_vel2 + y_acc2 * dt
#    y_pos_mat[t_idx][:] = y_pos
#    if t_idx == 0:
#        if y_acc[x_idx] != gamma / dx**2 * (y_pos_mat[t_idx][x_idx + 1] + \
#                           y_pos_mat[t_idx][x_idx - 1] - 2 * y_pos_mat[t_idx][x_idx]):
#            print(
#        y_acc[x_idx] = gamma / dx**2 * (y_pos_mat[t_idx][x_idx + 1] + \
#                           y_pos_mat[t_idx][x_idx - 1] - 2 * y_pos_mat[t_idx][x_idx])
#    y_pos_mat[t_idx][:] = y_pos_mat[t_idx][:] + y_vel * dt
#    y_vel = y_vel + y_acc * dt
#    y_pos_mat[t_idx][:] = y_pos
    
#fig, ax = plt.subplots()
fig = plt.figure()
ax = plt.axes(xlim=(0,10), ylim=(-1,1))
#line, = ax.plot(x_vec, y_pos_mat[0][:])
line, = ax.plot([], [], lw=2)
def init():  # only required for blitting to give a clean slate.
    line.set_data([], [])
    return(line,)


def animate(i):
    line.set_data(np.linspace(xmin, xmax, nx), y_pos_mat[10*i][:])  # update the data.
    return(line,)


ani = animation.FuncAnimation(
    fig, animate, init_func=init, interval=30, blit=True)

plt.show()