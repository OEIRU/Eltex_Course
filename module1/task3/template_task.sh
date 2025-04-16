#!/bin/bash

# Получаем имя скрипта без пути
SCRIPT_NAME="${0##*/}"
LOG_FILE="report_${SCRIPT_NAME%.*}.log"  # Удаляем расширение .sh если есть

# Запись о запуске
echo "[$$] $(date +'%Y-%m-%d %H:%M:%S') Скрипт запущен" >> "$LOG_FILE"

# Генерация случайного времени от 30 до 1800 секунд
RANDOM_SECONDS=$(( (RANDOM % 1771) + 30 ))  # 1800 - 30 + 1 = 1771
sleep $RANDOM_SECONDS

# Вычисление времени работы в минутах
MINUTES=$(( RANDOM_SECONDS / 60 ))

# Запись о завершении
echo "[$$] $(date +'%Y-%m-%d %H:%M:%S') Скрипт завершился, работал $MINUTES минут" >> "$LOG_FILE"
