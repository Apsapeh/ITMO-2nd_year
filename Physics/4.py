
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import Animation, FuncAnimation

# --- Параметры системы ---
G = 1
dt = 0.01
steps = 10000

# Тело 1
m1 = 50.0
pos1 = np.array([-5.0, 0.0])
vel1 = np.array([0.0, -1.5])

# Тело 2
m2 = 50.0
pos2 = np.array([5.0, 0.0])
vel2 = np.array([0.0, 1.5])

# Списки для хранения траекторий
path1, path2 = [], []

# Функция расчета ускорения 
def compute_acceleration(p1, p2, m1, m2):
    r_vec = p2 - p1
    dist = np.linalg.norm(r_vec)
    force_mag = G * m1 * m2 / (dist**2)
    force_vec = force_mag * (r_vec / dist)
    acc1 = force_vec / m1
    acc2 = -force_vec / m2
    return acc1, acc2

# Цикл симуляции (Метод Верле) 
a1, a2 = compute_acceleration(pos1, pos2, m1, m2)

for _ in range(steps):
    path1.append(pos1.copy())
    path2.append(pos2.copy())
    
    # Обновление координат
    pos1 = pos1 + vel1 * dt + 0.5 * a1 * dt**2
    pos2 = pos2 + vel2 * dt + 0.5 * a2 * dt**2
    
    # Новое ускорение
    new_a1, new_a2 = compute_acceleration(pos1, pos2, m1, m2)
    
    # Обновление скоростей по Верле
    vel1 = vel1 + 0.5 * (a1 + new_a1) * dt
    vel2 = vel2 + 0.5 * (a2 + new_a2) * dt
    
    a1, a2 = new_a1, new_a2

path1, path2 = np.array(path1), np.array(path2)




# ------------ Анимация ---------------
fig, ax = plt.subplots(figsize=(8, 8))
ax.set_xlim(-15, 15)
ax.set_ylim(-15, 15)
ax.set_aspect('equal')
ax.grid(True)

line1, = ax.plot([], [], 'yo', ms=10, label='Body 1')
line2, = ax.plot([], [], 'bo', ms=5, label='Body 2')
trace2, = ax.plot([], [], 'b-', alpha=0.3)          
trace1, = ax.plot([], [], 'y-', alpha=0.3)           

def update(frame):
    line1.set_data([path1[frame, 0]], [path1[frame, 1]])
    line2.set_data([path2[frame, 0]], [path2[frame, 1]])
    trace1.set_data(path1[:frame, 0], path1[:frame, 1])
    trace2.set_data(path2[:frame, 0], path2[:frame, 1])
    return line1, line2, trace1, trace2

ani = FuncAnimation(fig, update, frames=range(0, steps, 5), interval=20, blit=True)
plt.legend()
plt.show()
