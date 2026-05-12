
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


l = 0.8          
rho = 0.02       
N_force = 0.04   

a = np.sqrt(N_force / rho)   # скорость волны

c = 1            
alpha = 0.38
beta = 0.42

def f(x):
    # начальное отклонение
    return np.sqrt(x * (l - x))

def g(x):
    # начальная скорость
    return np.where((x >= alpha) & (x <= beta), c, 0)

# Фурье
M = 100      
Nx = 500     

x = np.linspace(0, l, Nx)

A = np.zeros(M)
B = np.zeros(M)

# численное интегрирование
x_int = np.linspace(0, l, 5000)

for n in range(1, M + 1):
    phi = np.sin(np.pi * n * x_int / l)

    integrand_A = f(x_int) * phi
    A[n - 1] = (2 / l) * np.trapz(integrand_A, x_int)

    integrand_B = g(x_int) * phi
    B[n - 1] = (2 / (np.pi * n * a)) * np.trapz(integrand_B, x_int)


def u(x, t):
    result = np.zeros_like(x)

    for n in range(1, M + 1):
        omega_n = np.pi * n * a / l
        result += (
            A[n - 1] * np.cos(omega_n * t)
            + B[n - 1] * np.sin(omega_n * t)
        ) * np.sin(np.pi * n * x / l)

    return result







fig, ax = plt.subplots(figsize=(10, 5))

line, = ax.plot([], [], lw=2)

ax.set_xlim(0, l)
ax.set_ylim(-0.8, 0.8)

ax.set_xlabel("x")
ax.set_ylabel("u(x,t)")
ax.set_title("Колебания струны")

ax.grid(True)

# время
Tmax = 3
frames = 300

times = np.linspace(0, Tmax, frames)

def init():
    line.set_data([], [])
    return line,

def animate(i):

    t = times[i]

    y = u(x, t)

    line.set_data(x, y)

    ax.set_title(f"Колебания струны   t = {t:.2f}")

    return line,

anim = FuncAnimation(
    fig,
    animate,
    init_func=init,
    frames=frames,
    interval=30,
    blit=True
)

plt.show()
