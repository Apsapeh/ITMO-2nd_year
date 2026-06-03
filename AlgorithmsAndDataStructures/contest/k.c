#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Регион памяти
// Может быть занят или свободен
struct MemoryRegion {
  uint8_t is_free;
  uint32_t ll_node; // Указатель на ноду двусвязного списка
  uint32_t start;
  uint32_t size;
};

// Компаратор региона
// Сначала сравниваем размеры, если размеры равны, сравниваем адрес
static inline int memory_region_compare(const struct MemoryRegion *a_in,
                                        const struct MemoryRegion *b_in) {
  if (a_in->size != b_in->size) {
    return (a_in->size > b_in->size) ? 1 : -1;
  }
  if (a_in->start != b_in->start) {
    return (a_in->start > b_in->start) ? 1 : -1;
  }
  return 0;
}

// Нода двусвязного списка
// Все поля хранятся как индексы из глобальных массивов с элементами.
// В данном контексте можно нзвать их указателями
struct LinkedListNode {
  uint32_t mem_region;
  uint32_t prev_node;
  uint32_t next_node;
};

// Нода ПиВо (ПИрамида + дереВО, treap)
// По горизнотали BST
// по вертикали пирамида
struct TreeNode {
  uint32_t mem_region;
  int prior;
  uint32_t left;
  uint32_t right;
};

// История запросов
uint32_t *g_reqHistory = NULL;
uint32_t g_reqHistoryCounter = 0;

// Массивы для выделения элементов
struct MemoryRegion *g_memoryRegions = NULL;
uint32_t g_memoryRegionsCounter = 1;

struct LinkedListNode *g_llNodes = NULL;
uint32_t g_llNodesCounter = 1;

struct TreeNode *g_treeNodes = NULL;
uint32_t g_treeNodesCounter = 1;
uint32_t g_treeRoot = 1;

// Функция разрезания дерева на два дерева в ПиВо
static void tree_split(uint32_t t, const struct MemoryRegion *key, uint32_t *l,
                       uint32_t *r) {
  if (!t) {
    *l = *r = 0;
    return;
  }
  struct MemoryRegion *cur_reg = &g_memoryRegions[g_treeNodes[t].mem_region];

  if (memory_region_compare(cur_reg, key) < 0) { // cur_reg < key
    tree_split(g_treeNodes[t].right, key, &g_treeNodes[t].right, r);
    *l = t;
  } else {
    tree_split(g_treeNodes[t].left, key, l, &g_treeNodes[t].left);
    *r = t;
  }
}

// Функция соединения двух дереьвев ПиВо в одно
static uint32_t tree_merge(uint32_t l, uint32_t r) {
  if (!l || !r)
    return l ? l : r;

  if (g_treeNodes[l].prior > g_treeNodes[r].prior) {
    g_treeNodes[l].right = tree_merge(g_treeNodes[l].right, r);
    return l;
  } else {
    g_treeNodes[r].left = tree_merge(l, g_treeNodes[r].left);
    return r;
  }
}

// Функция вставки региона памяти в дерево ПиВо
void tree_insert(uint32_t region_idx) {
  uint32_t l, r;
  struct MemoryRegion *reg = &g_memoryRegions[region_idx];

  // Выделяем новую ноду дерева
  uint32_t new_node = g_treeNodesCounter++;
  g_treeNodes[new_node].mem_region = region_idx;
  g_treeNodes[new_node].prior = rand();
  g_treeNodes[new_node].left = 0;
  g_treeNodes[new_node].right = 0;

  // Вставляем её в месте разреза по ключу
  tree_split(g_treeRoot, reg, &l, &r);
  g_treeRoot = tree_merge(tree_merge(l, new_node), r);
}

// Функция удаления региона памяти из дерева ПиВо по ключу
void tree_erase_by_region(uint32_t region_idx) {
  struct MemoryRegion *reg = &g_memoryRegions[region_idx];

  uint32_t l, mid, r;

  // Отделяем всё, что меньше нашего региона
  tree_split(g_treeRoot, reg, &l, &r);

  // В правой части r ищем границу
  // Создаем временный ключ - тот же размер, но адрес на 1 больше
  struct MemoryRegion next_key = *reg;
  next_key.start++;

  // Отделяем наш конкретный узел от того, что больше него
  tree_split(r, &next_key, &mid, &r);

  // Склеиваем всё обратно
  g_treeRoot = tree_merge(l, r);
}

uint32_t allocate(uint32_t size) {
  // 1. Поиск в дереве
  // 2. Разбиение региона в двусвязном списке на две области (занятую и
  // незанятую)
  // 3. Удаление старого региона из дерева, добавление нового

  // 1. Ищем лучшее совпадение в дереве свободной памяти
  uint32_t current = g_treeRoot;
  uint32_t best = 0;

  while (current) {
    struct TreeNode *node = g_treeNodes + current;
    struct MemoryRegion *region = g_memoryRegions + node->mem_region;

    // Если регион подходит, то запоминаем его, как лучший
    if (region->size >= size) {
      best = current;
      // Идём влево и ищем более оптимальный регион
      current = node->left;

      // Если регион слишком мал, то идём вправо
    } else {
      current = node->right;
    }
  }

  // Если не нашли свободного места, то аллокация провалилась
  if (best == 0) {
    return 0;
  }

  struct TreeNode *tree_node = g_treeNodes + best;
  const uint32_t memory_region_idx = tree_node->mem_region;
  struct MemoryRegion *memory_region = g_memoryRegions + memory_region_idx;

  // Удаляем найденный регион из дерева
  tree_erase_by_region(memory_region_idx);

  // Если размер найденного региона равен размеру аллокации, то
  // устанавливаем, что регион занят и возвращаем его
  // Аллокация удалась
  if (memory_region->size == size) {
    memory_region->is_free = 0;
    return memory_region_idx;
  }

  struct LinkedListNode *ll_node = g_llNodes + memory_region->ll_node;
  // Создаём новый регион и ноду в списке для свободной памяти
  struct MemoryRegion *memory_region_new_free =
      g_memoryRegions + g_memoryRegionsCounter;
  struct LinkedListNode *ll_node_new_free = g_llNodes + g_llNodesCounter;

  // Инициализируем новый регион оставщейся после аллокации памяти
  memory_region_new_free->is_free = 1;
  memory_region_new_free->start = memory_region->start + size;
  memory_region_new_free->size = memory_region->size - size;
  memory_region_new_free->ll_node = g_llNodesCounter;

  // Добавляем новый регион в список
  ll_node_new_free->next_node = ll_node->next_node;
  ll_node_new_free->prev_node = memory_region->ll_node;
  ll_node_new_free->mem_region = g_memoryRegionsCounter;

  if (ll_node->next_node)
    g_llNodes[ll_node->next_node].prev_node = g_llNodesCounter;

  ll_node->next_node = g_llNodesCounter;

  // Добавляем новый регион в дерево
  tree_insert(g_memoryRegionsCounter);

  // Устанавлием старый регион, как занятыей
  memory_region->is_free = 0;
  memory_region->size = size;

  // Инкрементируем счётчики выделенных объектов
  ++g_llNodesCounter;
  ++g_memoryRegionsCounter;

  return memory_region_idx;
}

void deallocate(uint32_t req_number) {
  // 1. Получение региона из истории запросов
  // 2. Объединение региона в двусвязном списке
  // 3. Обновление дерева (удаление регионов, которые были объединены,
  // добавление нового региона)

  // 1. Получение региона
  uint32_t reg_idx = g_reqHistory[req_number - 1];
  if (reg_idx == 0)
    return;

  struct MemoryRegion *cur = &g_memoryRegions[reg_idx];
  uint32_t cur_ll_idx = cur->ll_node;
  struct LinkedListNode *cur_ll = &g_llNodes[cur_ll_idx];

  // Слияние с правым
  if (cur_ll->next_node) {
    uint32_t next_ll_idx = cur_ll->next_node;
    struct MemoryRegion *next_reg =
        &g_memoryRegions[g_llNodes[next_ll_idx].mem_region];
    // Если правый регион пустой, то удаляем его и объединяем
    if (next_reg->is_free) {
      tree_erase_by_region(g_llNodes[next_ll_idx].mem_region);
      cur->size += next_reg->size;

      // Удаляем next_ll из списка
      cur_ll->next_node = g_llNodes[next_ll_idx].next_node;
      if (cur_ll->next_node)
        g_llNodes[cur_ll->next_node].prev_node = cur_ll_idx;
    }
  }

  // Слияние с левым
  if (cur_ll->prev_node) {
    uint32_t prev_ll_idx = cur_ll->prev_node;
    struct MemoryRegion *prev_reg =
        &g_memoryRegions[g_llNodes[prev_ll_idx].mem_region];
    if (prev_reg->is_free) {
      tree_erase_by_region(g_llNodes[prev_ll_idx].mem_region);
      prev_reg->size += cur->size;

      // Исключаем текущую ноду из списка
      prev_reg->ll_node = prev_ll_idx; // На всякий случай
      g_llNodes[prev_ll_idx].next_node = cur_ll->next_node;
      // Если у текущеий ноды есть след нода, то в ней обновить предыдущую ноду
      if (cur_ll->next_node)
        g_llNodes[cur_ll->next_node].prev_node = prev_ll_idx;

      // Теперь основной нодой стала левая нода
      reg_idx = g_llNodes[prev_ll_idx].mem_region;
    }
  }

  // Устанавливаем флаг, что регион пустой
  g_memoryRegions[reg_idx].is_free = 1;
  tree_insert(reg_idx);
}

int main(void) {
  uint32_t cells_count, req_count;
  scanf("%" SCNu32 " %" SCNu32, &cells_count, &req_count);

  // Выделяем несколько доп элементов, на всякий случай
  req_count += 6;

  // Выделяем память
  g_reqHistory = (uint32_t *)malloc(sizeof(uint32_t) * req_count);
  g_memoryRegions =
      (struct MemoryRegion *)malloc(sizeof(struct MemoryRegion) * req_count);
  g_llNodes = (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode) *
                                               req_count);
  g_treeNodes = (struct TreeNode *)malloc(sizeof(struct TreeNode) * req_count);

  // Инициализируем первый, полностью пустой регион
  struct MemoryRegion *memory_region =
      g_memoryRegions + g_memoryRegionsCounter;
  memory_region->is_free = 1;
  memory_region->start = 1;
  memory_region->size = cells_count;
  memory_region->ll_node = g_llNodesCounter;

  // Добавляем первый регион, как корень
  struct TreeNode *tree_node = g_treeNodes + g_treeNodesCounter;
  tree_node->mem_region = g_memoryRegionsCounter;
  tree_node->prior = rand();
  tree_node->left = 0;
  tree_node->right = 0;

  // Устанавливаем корень
  g_treeRoot = g_treeNodesCounter;

  // Добавляем первый регион в список
  struct LinkedListNode *ll_node = g_llNodes + g_llNodesCounter;
  ll_node->mem_region = g_memoryRegionsCounter;
  ll_node->prev_node = 0;
  ll_node->next_node = 0;

  // Увеличиваем счётчики
  // Можно MemoryRegion, TreeNode, LinkedListNode каждый раз выделять через
  // malloc, но это глупо По этому, память уже выделена под все элементы. И
  // когда надо получить новый элемент, просто инкрементируется счётчик и
  // выдаётся элемент по адресу
  g_memoryRegionsCounter++;
  g_llNodesCounter++;
  g_treeNodesCounter++;

  // Сбрасываем доп элементы
  req_count -= 6;

  // Читаем ввод
  for (int i = 0; i < (int)req_count; ++i) {
    int req;
    scanf("%d", &req);

    // Выделение
    if (req > 0) {
      // Пробуем выделить.
      // 0 - выделение не удалось
      // >0 - успешно
      uint32_t mem_region_idx = allocate(req);
      // Сохранем, что выделили
      g_reqHistory[g_reqHistoryCounter++] = mem_region_idx;
      // Печатаем начало региона, если смогли выделить
      if (mem_region_idx) {
        printf("%d\n", g_memoryRegions[mem_region_idx].start);
      } else {
        printf("-1\n");
      }

      // Удаление
    } else {
      deallocate(-req);
      g_reqHistory[g_reqHistoryCounter++] = 0;
    }
  }

  free(g_reqHistory);
  free(g_memoryRegions);
  free(g_llNodes);
  free(g_treeNodes);
  return 0;
}
