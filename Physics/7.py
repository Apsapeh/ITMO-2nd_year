# Var 3


import numpy as np

m1 = 10
m2 = 10
V = 7
E = 0.30

rhs = E * (m1 + m2) * V**2
coeff = m2**2 / m1 + m2

V2_0 = np.sqrt(rhs / coeff)
V1_0 = (m2 / m1) * V2_0

print(f"V1_0 (ЦС) = {V1_0:.4f}")
print(f"V2_0 (ЦС) = {V2_0:.4f}")
print()
print(f"{'θ':>6} {'V1x_ЦС':>10} {'V1y_ЦС':>10} {'V2x_ЦС':>10} {'V2y_ЦС':>10} {'V1_ЛС':>10} {'θ1':>10} {'V2_ЛС':>10} {'θ2':>10}")
print("-" * 94)

for theta_deg in range(0, 361, 10):
    theta = np.radians(theta_deg)

    # Векторы в ЦС
    V1x_cs = V1_0 * np.cos(theta)
    V1y_cs = V1_0 * np.sin(theta)
    V2x_cs = V2_0 * np.cos(theta + np.pi)
    V2y_cs = V2_0 * np.sin(theta + np.pi)

    # Векторы в ЛС
    V1_lab = np.sqrt((V1x_cs + V)**2 + V1y_cs**2)
    theta1  = np.degrees(np.arctan2(V1y_cs, V1x_cs + V))

    V2_lab = np.sqrt((V2x_cs + V)**2 + V2y_cs**2)
    theta2  = np.degrees(np.arctan2(V2y_cs, V2x_cs + V))

    print(f"{theta_deg:>6} {V1x_cs:>10.4f} {V1y_cs:>10.4f} {V2x_cs:>10.4f} {V2y_cs:>10.4f} {V1_lab:>10.4f} {theta1:>10.4f} {V2_lab:>10.4f} {theta2:>10.4f}")
