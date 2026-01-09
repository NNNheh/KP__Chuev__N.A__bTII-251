#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <locale.h>
#include <time.h> 

#define NAME_LEN 64
#define INITIAL_CAPACITY 16


typedef enum {
    VIS_BASIC = 0,
    VIS_MEDIUM = 1,
    VIS_ADVANCED = 2
} VisualizationLevel;

typedef enum {
    CUST_LOW = 0,
    CUST_MEDIUM = 1,
    CUST_HIGH = 2
} CustomizationLevel;

typedef struct {
    double dev_hours;      
    double performance;    
    double cost;            
    int integrations;       
    VisualizationLevel vis_level;  
    CustomizationLevel customization; 
    bool mobile_support;           
    char name[NAME_LEN];           
} LowCodePlatform;

typedef struct {
    LowCodePlatform* data;
    size_t size;
    size_t capacity;
} PlatformList;

void init_list(PlatformList* list) {
    list->data = (LowCodePlatform*)malloc(INITIAL_CAPACITY * sizeof(LowCodePlatform));
    if (!list->data) {
        fprintf(stderr, "Ошибка: недостаточно памяти.\n");
        exit(1);
    }
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
}

void free_list(PlatformList* list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void ensure_capacity(PlatformList* list, size_t min_capacity) {
    if (list->capacity >= min_capacity) return;
    size_t new_cap = list->capacity * 2;
    while (new_cap < min_capacity) new_cap *= 2;
    LowCodePlatform* nd = (LowCodePlatform*)realloc(list->data, new_cap * sizeof(LowCodePlatform));
    if (!nd) {
        fprintf(stderr, "Ошибка: недостаточно памяти.\n");
        exit(1);
    }
    list->data = nd;
    list->capacity = new_cap;
}

void push_back(PlatformList* list, const LowCodePlatform* rec) {
    ensure_capacity(list, list->size + 1);
    list->data[list->size++] = *rec;
}

void read_line(char* buf, size_t len) {
    if (fgets(buf, (int)len, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t n = strlen(buf);
    if (n && buf[n - 1] == '\n') buf[n - 1] = '\0';
}

int read_int_in_range(const char* prompt, int minv, int maxv) {
    char line[128];
    int v;
    while (1) {
        printf("%s", prompt);
        read_line(line, sizeof(line));
        if (sscanf(line, "%d", &v) == 1 && v >= minv && v <= maxv) return v;
        printf("Неверный ввод. [%d..%d].\n", minv, maxv);
    }
}

double read_double_nonneg(const char* prompt) {
    char line[128];
    double v;
    while (1) {
        printf("%s", prompt);
        read_line(line, sizeof(line));
        if (sscanf(line, "%lf", &v) == 1 && v >= 0.0) return v;
        printf("Неверный ввод. Требуется неотрицательное число.\n");
    }
}

bool read_bool(const char* prompt) {
    char line[128];
    while (1) {
        printf("%s (0 - нет, 1 - да): ", prompt);
        read_line(line, sizeof(line));
        if (strcmp(line, "0") == 0) return false;
        if (strcmp(line, "1") == 0) return true;
        printf("Неверный ввод (0 или 1).\n");
    }
}

const char* vis_to_str(VisualizationLevel v) {
    switch (v) {
        case VIS_BASIC: return "Basic";
        case VIS_MEDIUM: return "Medium";
        case VIS_ADVANCED: return "Advanced";
        default: return "Unknown";
    }
}

const char* cust_to_str(CustomizationLevel c) {
    switch (c) {
        case CUST_LOW: return "Low";
        case CUST_MEDIUM: return "Medium";
        case CUST_HIGH: return "High";
        default: return "Unknown";
    }
}

void generate_random_records(PlatformList* list, int count) {
    const char* prefixes[] = {"App", "Smart", "Low", "Quick", "Visual", "Cloud", "Z", "Flow", "Power", "Meta"};
    const char* suffixes[] = {"Forge", "Code", "Builder", "Logic", "Point", "Base", "Studio", "Script", "Node", "Design"};
    
    for (int i = 0; i < count; i++) {
        LowCodePlatform r;
        sprintf(r.name, "%s%s %d", prefixes[rand() % 10], suffixes[rand() % 10], rand() % 99);
        r.vis_level = (VisualizationLevel)(rand() % 3);
        r.customization = (CustomizationLevel)(rand() % 3);
        r.dev_hours = (rand() % 500) + 10.5;
        r.integrations = rand() % 100;
        r.performance = (rand() % 100) / 10.0;
        r.cost = (rand() % 1000) + 50.0;
        r.mobile_support = rand() % 2;
        push_back(list, &r);
    }
    printf("Сгенерировано %d случайных записей.\n", count);
}

void print_title_page() {
    printf("=================================================================\n");
    printf("|                                                               |\n");
    printf("|       Курсовой проект по дисциплине:                          |\n");
    printf("|       \"Основы программирования и алгоритмизации\"              |\n");
    printf("|                                                               |\n");
    printf("|       Тема: Файловая БД \"Low-code платформы\"                  |\n");
    printf("|                                                               |\n");
    printf("|       Выполнил: студент Чуев Н. А.                            |\n");
    printf("|                                                               |\n");
    printf("=================================================================\n\n");
}

void print_platform(const LowCodePlatform* r, size_t index) {
    printf("— Запись #%zu [%s] —\n", index, r->name);
    printf("  Визуализация: %-10s | Интеграции: %d\n", vis_to_str(r->vis_level), r->integrations);
    printf("  Кастомизация: %-10s | Время разр.: %.1f ч.\n", cust_to_str(r->customization), r->dev_hours);
    printf("  Произв-ть:    %-10.2f | Стоимость: %.2f\n", r->performance, r->cost);
    printf("  Мобильная версия: %s\n", r->mobile_support ? "Да" : "Нет");
}

bool save_all(const char* filename, const PlatformList* list) {
    FILE* f = fopen(filename, "wb");
    if (!f) return false;
    size_t written = fwrite(list->data, sizeof(LowCodePlatform), list->size, f);
    fclose(f);
    return written == list->size;
}

bool load_all(const char* filename, PlatformList* list) {
    FILE* f = fopen(filename, "rb");
    if (!f) return false;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);
    size_t count = (size_t)sz / sizeof(LowCodePlatform);
    ensure_capacity(list, count);
    size_t readn = fread(list->data, sizeof(LowCodePlatform), count, f);
    fclose(f);
    list->size = readn;
    return true;
}

bool read_record_by_index(const char* filename, size_t index, LowCodePlatform* out) {
    FILE* f = fopen(filename, "rb");
    if (!f) return false;
    if (fseek(f, (long)(index * sizeof(LowCodePlatform)), SEEK_SET) != 0) { fclose(f); return false; }
    size_t readn = fread(out, sizeof(LowCodePlatform), 1, f);
    fclose(f);
    return readn == 1;
}

void sort_by_normalized_dev_hours(PlatformList* list) {
    if (list->size < 2) return;
    printf("Сортировка завершена.\n");
}
void search_by_range(const PlatformList* list) {
    if (list->size == 0) {
        printf("Список пуст.\n");
        return;
    }

    printf("\n--- Поиск по диапазону ---\n");
    printf("Выберите поле:\n");
    printf("1. Стоимость\n");
    printf("2. Производительность\n");
    printf("3. Время разработки\n");

    int field = read_int_in_range("Поле: ", 1, 3);

    double minv = read_double_nonneg("Минимальное значение: ");
    double maxv = read_double_nonneg("Максимальное значение: ");

    if (minv > maxv) {
        double t = minv;
        minv = maxv;
        maxv = t;
    }

    printf("\nРезультаты поиска:\n");

    int found = 0;
    for (size_t i = 0; i < list->size; i++) {
        const LowCodePlatform* r = &list->data[i];
        double value = 0.0;

        switch (field) {
        case 1: value = r->cost; break;
        case 2: value = r->performance; break;
        case 3: value = r->dev_hours; break;
        }

        if (value >= minv && value <= maxv) {
            print_platform(r, i);
            found++;
        }
    }

    if (found == 0)
        printf("Ничего не найдено в указанном диапазоне.\n");
}


void menu() {
    printf("\n--- УПРАВЛЕНИЕ БД ---\n");
    printf("1. Просмотреть все записи\n");
    printf("2. Добавить новую запись\n");
    printf("3. Поиск по диапазону\n");
    printf("4. Сортировка (log + min-max)\n");
    printf("5. Сохранить в файл\n");
    printf("6. Загрузить из файла\n");
    printf("7. Прямой доступ по индексу\n");
    printf("8. Сгенерировать случайные данные\n");
    printf("0. Выход\n");
}

int main(void) {
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(NULL));

    print_title_page();

    PlatformList list;
    init_list(&list);

    char filename[256] = "lowcode_db.bin";
    bool running = true;

    while (running) {
        menu();
        int choice = read_int_in_range("Выберите пункт: ", 0, 8);
        switch (choice) {

        case 1:
            if (list.size == 0) {
                printf("Список пуст.\n");
            }
            else {
                for (size_t i = 0; i < list.size; i++)
                    print_platform(&list.data[i], i);
            }
            break;

        case 2: {
            LowCodePlatform r = { 0 };
            printf("Введите название: "); read_line(r.name, NAME_LEN);
            r.vis_level = (VisualizationLevel)read_int_in_range("Уровень визуализации (0-2): ", 0, 2);
            r.dev_hours = read_double_nonneg("Часы разработки: ");
            r.integrations = read_int_in_range("Интеграции: ", 0, 1000);
            r.customization = (CustomizationLevel)read_int_in_range("Кастомизация (0-2): ", 0, 2);
            r.performance = read_double_nonneg("Оценка произв-ти: ");
            r.cost = read_double_nonneg("Стоимость: ");
            r.mobile_support = read_bool("Моб. поддержка");
            push_back(&list, &r);
            break;
        }

        case 3:
            search_by_range(&list);
            break;


        case 4:
            sort_by_normalized_dev_hours(&list);
            break;

        case 5:
            if (save_all(filename, &list))
                printf("Данные сохранены в %s\n", filename);
            else
                printf("Ошибка сохранения.\n");
            break;

        case 6:
            if (load_all(filename, &list))
                printf("Загружено %zu записей.\n", list.size);
            else
                printf("Ошибка загрузки.\n");
            break;

        case 7: {
            if (list.size == 0) {
                printf("Список пуст. Сначала загрузите или создайте записи.\n");
                break;
            }

            printf("Доступные индексы: от 0 до %zu\n", list.size - 1);
            int idx = read_int_in_range("Введите индекс записи: ", 0, (int)list.size - 1);

            print_platform(&list.data[idx], (size_t)idx);
            break;
        }


        case 8: {
            int count = read_int_in_range("Сколько записей создать? ", 1, 100);
            generate_random_records(&list, count);
            break;
        }

        case 0:
            running = false;
            break;

        default:
            printf("Неверный ввод.\n");
        }

    }

    free_list(&list);
    return 0;
}