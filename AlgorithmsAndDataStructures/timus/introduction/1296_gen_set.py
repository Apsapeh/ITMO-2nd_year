import random

COUNT = 60_000

with open("1296_big_set.txt", "w") as f:
    f.write(f"{COUNT}\n")
    for i in range(COUNT):
        rand_num = random.randint(-30000, 30000)
        f.write(f"{rand_num}\n")
