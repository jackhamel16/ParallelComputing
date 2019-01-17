import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

gamma = 1

xmin = 0
xmax = 10
nx = 10

dx = (xmax - xmin)/nx
x_vec = np.linspace(xmin, xmax, nx)

tmin = 0
tmax = 10
nt = 1000

dt = (tmax - tmin)/nt
t_vec = np.linspace(tmin, tmax, nt)

#initializing y positions
y_pos = np.exp(-(x_vec - 5)**2)
y_vel = np.zeros(nx)
y_acc = np.zeros(nx)

y_pos_mat = np.zeros((nt, nx))

for t_idx in range(0,np.size(t_vec)):
    y_acc[0], y_acc[nx - 1] = 0, 0
    for x_idx in range(1, nx - 1):
        y_acc[x_idx] = gamma / dx**2 * (y_pos[x_idx + 1] + \
                       y_pos[x_idx - 1] - 2 * y_pos[x_idx])
    y_pos_mat[t_idx][:] = y_pos + y_vel * dt
    y_vel = y_vel + y_acc * dt

fig, ax = plt.subplots()
line, = ax.plot(x_vec, y_pos_mat[0][:])

def init():  # only required for blitting to give a clean slate.
    line.set_ydata([np.nan] * nx)
    return(line,)


def animate(i):
    line.set_ydata(y_pos_mat[i][:])  # update the data.
    return(line,)


ani = animation.FuncAnimation(
    fig, animate, init_func=init, interval=1, blit=True, save_count=50)

plt.show()