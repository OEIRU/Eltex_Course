#!/bin/bash

# Имя файла лога
LOG_FILE="observer.log"

# Список скриптов для мониторинга
SCRIPTS=(
    "/home/user/scripts/script1.sh"
    "/home/user/scripts/script2.sh"
    "/home/user/scripts/script3.sh"
)

# Функция для записи в лог
log_message() {
    echo "[$(date +'%Y-%m-%d %H:%M:%S')] $1" >> "$LOG_FILE"
}

# Проверка каждого скрипта из списка
for script_path in "${SCRIPTS[@]}"; do
    # Пропускаем пустые строки (на случай ошибок в массиве)
    if [[ -z "$script_path" ]]; then
        continue
    fi

    # Проверка существования скрипта
    if [[ ! -f "$script_path" ]]; then
        log_message "Ошибка: Скрипт $script_path не существует."
        continue
    fi

    # Получаем имя скрипта без пути
    script_name=$(basename "$script_path")

    # Проверяем, запущен ли процесс
    if pgrep -fl "$script_path" > /dev/null; then
        log_message "Процесс $script_name уже запущен."
    else
        # Запускаем скрипт в фоновом режиме с nohup
        nohup "$script_path" > "/dev/null" 2>&1 &
        log_message "Перезапущен процесс $script_name ($script_path)."
    fi
done