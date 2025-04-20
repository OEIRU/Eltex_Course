# Компилятор и инструменты
CC      := gcc
AR      := ar

# Флаги
CFLAGS  := -Wall -Wextra -Iinclude
ARFLAGS := rcs

# Цели
LIB      := libdllist.a
EXEC     := lab6_1
OBJS_LIB := dllist.o
OBJS_APP := main.o

.PHONY: all clean

# Цель по умолчанию
all: $(LIB) $(EXEC)
	@echo "Сборка завершена."

# Сборка статической библиотеки из объектников
$(LIB): $(OBJS_LIB)
	$(AR) $(ARFLAGS) $@ $^

# Общие правила компиляции .c → .o
%.o: %.c dllist.h
	$(CC) $(CFLAGS) -c $< -o $@

# Линковка приложения с библиотекой
$(EXEC): $(OBJS_APP) $(LIB)
	$(CC) $(CFLAGS) $< -L. -ldllist -o $@

# Удаление результатов сборки
clean:
	rm -f $(OBJS_LIB) $(OBJS_APP) $(LIB) $(EXEC)
