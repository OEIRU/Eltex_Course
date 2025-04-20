# Компилятор и флаги
CC        := gcc
CFLAGS    := -Wall -Wextra -fPIC -Iinclude
LDFLAGS   := -shared

# Файлы
LIB_NAME  := libdllist.so
LIB_OBJS  := dllist.o

EXEC_NAME := lab6_2
MAIN_OBJ  := main.o

.PHONY: all clean

all: $(LIB_NAME) $(EXEC_NAME)

# Сборка динамической библиотеки
$(LIB_NAME): $(LIB_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Компиляция объекта библиотеки
src/%.o: src/%.c dllist.h
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка объекта главного приложения
$(MAIN_OBJ): main.c dllist.h
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка исполняемого файла с вшитым rpath ($ORIGIN = папка бинарника)
$(EXEC_NAME): $(MAIN_OBJ) $(LIB_NAME)
	$(CC) -Wall -Wextra -Iinclude $< -L. -ldllist \
	      -Wl,-rpath,'$$ORIGIN' \
	      -o $@

# Удаление сгенерированных файлов
clean:
	rm -f src/*.o $(MAIN_OBJ) $(LIB_NAME) $(EXEC_NAME)
