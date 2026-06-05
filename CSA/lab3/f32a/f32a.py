def sum_even_n(n):
    """Calculate the sum of even numbers from 1 to n"""
    if n <= 0:
        return -1
    total = 0
    for i in range(1, n + 1):
        if i % 2 == 0:
            total += i
    return total

print(sum_even_n(100000))
print(sum_even_n(110000))
print((2 ** 31 - 1) - sum_even_n(92680))
print((2 ** 31 - 1) - sum_even_n(92681))
print((2 ** 31 - 1) - sum_even_n(92682))

assert sum_even_n(5) == 6
assert sum_even_n(10) == 30
assert sum_even_n(90000) == 2025045000
