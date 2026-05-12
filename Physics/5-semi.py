
import numpy as np

G = 6.67430e-11
dt = 86400.0
steps = 365 * 300   


planets_info = [
    {"name": "Солнце",     "m": 1.989e30, "r": 0.0,       "v": 0.0},
    {"name": "Меркурий", "m": 3.301e23, "r": 6.982e10,  "v": 38860.0},
    {"name": "Венера",   "m": 4.867e24, "r": 1.089e11, "v": 34790.0},
    {"name": "Земля",   "m": 5.972e24, "r": 1.521e11, "v": 29290.0},
    {"name": "Марс",    "m": 6.390e23, "r": 2.492e11, "v": 21970.0},
    {"name": "Юпитер", "m": 1.898e27, "r": 8.166e11, "v": 12440.0},
    {"name": "Сатурн",  "m": 5.683e26, "r": 1.503e12, "v": 9090.0},
    {"name": "Уран",  "m": 8.681e25, "r": 3.006e12, "v": 6490.0},
    {"name": "Нептун", "m": 1.024e26, "r": 4.546e12, "v": 5370.0}
]

names = [p["name"] for p in planets_info]
masses = np.array([p["m"] for p in planets_info])
pos = np.array([[p["r"], 0.0] for p in planets_info])
vel = np.array([[0.0, p["v"]] for p in planets_info]) 

def get_accelerations(p, m):
    n = len(m)
    a = np.zeros_like(p)
    for i in range(n):
        diff = p - p[i]
        dist = np.linalg.norm(diff, axis=1)[:, np.newaxis]
        dist[dist == 0] = 1.0 
        a[i] = np.sum((G * m[:, np.newaxis] / dist**3) * diff, axis=0)
    return a

r_min = np.full(len(masses), float('inf'))
r_max = np.full(len(masses), float('-inf'))

acc = get_accelerations(pos, masses)

for _ in range(steps):
    dists = np.linalg.norm(pos - pos[0], axis=1)
    r_min = np.minimum(r_min, dists)
    r_max = np.maximum(r_max, dists)

    pos += vel * dt + 0.5 * acc * dt**2
    new_acc = get_accelerations(pos, masses)
    vel += 0.5 * (acc + new_acc) * dt
    acc = new_acc

print(f"{'Планета':<10} | {'Эксцентриситет (e)':<20}")
print("-" * 35)
for i in range(1, len(names)):
    # e = (r_max - r_min) / (r_max + r_min)
    e = (r_max[i] - r_min[i]) / (r_max[i] + r_min[i])
    print(f"{names[i]:<10} | {e:.5f}")
