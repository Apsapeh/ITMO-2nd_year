import math

R = 0.15

M1 = 0.5
M2 = 1.5

P1 = [-1.0, 0.0]
P2 = [0.0,  0.5]

V1 = [1.0, 0.3]
V2 = [-0.5, -0.5]


M1 = 1.0
M2 = 100.0
P1 = [-1.0, 0.0]
P2 = [0.0,  0.0]

V1 = [0.6, 0.0]
V2 = [0.0, 0.0]

dT = 1.0 / 1000000

def main():
    pos1 = P1
    pos2 = P2
    vel1 = V1
    vel2 = V2

    prev_dist = dist(pos1, pos2)

    while True:
        cur_dist = dist(pos1, pos2)

        if prev_dist < cur_dist :
            print("Нет столкновения")
            return

        if cur_dist <= R * 2:
            vel1, vel2 = collide(vel1, vel2, pos1, pos2)
            print(f"Столконвение!\n\tP: [{pos1[0]:>10.4f}, {pos1[1]:>10.4f}] - [{pos2[0]:>10.4f}, {pos2[1]:>10.4f}]\n\tV: [{vel1[0]:>10.4f}, {vel1[1]:>10.4f}] - [{vel2[0]:>10.4f}, {vel2[1]:>10.4f}]")
            
            return

        pos1[0] += vel1[0] * dT
        pos1[1] += vel1[1] * dT
        pos2[0] += vel2[0] * dT
        pos2[1] += vel2[1] * dT
        prev_dist = cur_dist


def collide(vel1, vel2, pos1, pos2):
    # нормаль
    # Это вектор от шара 1 к шару 2
    # Направление удара
    dx = pos2[0] - pos1[0]
    dy = pos2[1] - pos1[1]
    d = math.sqrt(dx*dx + dy*dy)
    nx, ny = dx/d, dy/d

    # проекции скоростей на нормаль
    # Сколько скорости от шара к шару идёт вдоль нормали,
    # т.е. линии удара
    v1n = vel1[0]*nx + vel1[1]*ny
    v2n = vel2[0]*nx + vel2[1]*ny

    # новые проекции
    # После удара ни сумма импульса, ни энергии не меняются
    # Ek1 + Ek2 = Ek1' + Ek2'
    # p1 + p2 = p1' + p2'
    # выводим и находим новую скалярную скорость вдоль нормали
    v1n_new = (v1n*(M1-M2) + 2*M2*v2n) / (M1+M2)
    v2n_new = (v2n*(M2-M1) + 2*M1*v1n) / (M1+M2)

    # Считаем разницу между старой и новой скоростью по мормали
    # Т.к. нам важно именно изменение вдоль нормали,
    # мы не можем просто исползьать скаляр
    dv1n = v1n_new - v1n
    dv2n = v2n_new - v2n

    # Финальная векторная скорость
    # Берём изначальную векторную скорость и изменяем
    # с новой скоростью по нормали
    return (
        [vel1[0] + dv1n*nx, vel1[1] + dv1n*ny],
        [vel2[0] + dv2n*nx, vel2[1] + dv2n*ny],
    )

def dist(p1, p2):
    return math.sqrt(
        (p2[0] - p1[0]) ** 2 + (p2[1] - p1[1]) ** 2
    )

def speed(v):
    return math.sqrt(v[0] ** 2 + v[1] ** 2)
    

if __name__ == "__main__":
    main()
