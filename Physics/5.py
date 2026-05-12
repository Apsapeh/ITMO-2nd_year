import numpy as np

# --- Физические константы ---
G = 6.67430e-11
dt = 86400.0  # 1 день
steps = 36500   # 1 год

# --- Данные планет (Масса, Положение [x, y], Скорость [vx, vy]) ---
# Добавляй новые планеты в этот список
planets_data = [
    {"name": "Sun",   "m": 1.989e30, "pos": [0.0, 0.0],       "vel": [0.0, 0.0]},
    {"name": "Earth", "m": 5.972e24, "pos": [1.521e11, 0.0], "vel": [0.0, 29290.0]},
    {"name": "Mars",  "m": 6.39e23,  "pos": [2.492e11, 0.0], "vel": [0.0, 21970.0]},
]

# Превращаем в удобные массивы NumPy
masses = np.array([p["m"] for p in planets_data])
positions = np.array([p["pos"] for p in planets_data])
velocities = np.array([p["vel"] for p in planets_data])
names = [p["name"] for p in planets_data]

def compute_all_accelerations(pos, m):
    """Твоя логика, расширенная на N тел через векторные вычисления"""
    n = len(m)
    acc = np.zeros_like(pos)
    for i in range(n):
        for j in range(n):
            if i == j: continue
            r_vec = pos[j] - pos[i]
            dist = np.linalg.norm(r_vec)
            # Закон всемирного тяготения
            acc[i] += (G * m[j] / dist**3) * r_vec
    return acc

def get_eccentricity(p_pos, p_vel, s_pos, s_vel, m_central, m_planet):
    """Расчет эксцентриситета относительно центрального тела (Солнца)"""
    r_rel = p_pos - s_pos
    v_rel = p_vel - s_vel
    mu = G * (m_central + m_planet)
    
    r = np.linalg.norm(r_rel)
    v2 = np.dot(v_rel, v_rel)
    h = r_rel[0] * v_rel[1] - r_rel[1] * v_rel[0] # Удельный угловой момент
    energy = 0.5 * v2 - mu / r                   # Удельная энергия
    
    return np.sqrt(max(0, 1 + (2 * energy * h**2) / (mu**2)))

# --- Основной цикл (Метод Верле) ---
a = compute_all_accelerations(positions, masses)

for _ in range(steps):
    # 1. Обновление координат
    positions += velocities * dt + 0.5 * a * dt**2
    
    # 2. Новое ускорение
    new_a = compute_all_accelerations(positions, masses)
    
    # 3. Обновление скоростей
    velocities += 0.5 * (a + new_a) * dt
    a = new_a

# --- Вывод результатов ---
print(f"{'Планета':<10} | {'Эксцентриситет':<15}")
print("-" * 30)
for i in range(1, len(names)):
    e = get_eccentricity(positions[i], velocities[i], positions[0], velocities[0], masses[0], masses[i])
    print(f"{names[i]:<10} | {e:.5f}")
