
import sys
import matplotlib.pyplot as plt

def main():
    x_coords = []
    y_coords = []

    print("Вставьте данные и нажмите Ctrl+D (в Linux/Mac) или Ctrl+Z (в Windows), чтобы завершить ввод:")

    # Читаем данные из stdin построчно
    for line in list(sys.stdin)[1:]:
        # Пропускаем пустые строки
        if not line.strip():
            continue
        
        try:
            # Разделяем строку на части и конвертируем в float
            parts = line.split()
            if len(parts) >= 2:
                x_coords.append(float(parts[0]))
                y_coords.append(float(parts[1]))
        except ValueError:
            print(f"Ошибка при обработке строки: {line.strip()}")

    if not x_coords:
        print("Данные не были введены.")
        return

    # Создание графика
    plt.figure(figsize=(10, 6))
    plt.plot(x_coords, y_coords, marker='o', linestyle='-', color='b', label='Траектория')
    
    # Оформление
    plt.title("Визуализация данных из stdin")
    plt.xlabel("Координата X")
    plt.ylabel("Координата Y")
    plt.grid(True)
    plt.legend()
    
    print("Генерация графика...")
    plt.show()

if __name__ == "__main__":
    main()
