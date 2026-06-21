import webview
import subprocess
import os

class Api:
    def start_scan(self):
        print("MAX: Запуск сканирования...")
        # Вызываем наш скомпилированный MAX.exe
        try:
            # Сканируем текущую директорию как пример
            result = subprocess.run(['./MAX.exe', '.'], capture_output=True, text=True)
            return result.stdout
        except Exception as e:
            return f"Ошибка: {str(e)}"

def main():
    api = Api()
    # Создаем окно с нашим современным дизайном
    window = webview.create_window(
        'MAX Antivirus',
        'ui_mockup.html',
        js_api=api,
        width=1200,
        height=800,
        resizable=True
    )
    webview.start()

if __name__ == '__main__':
    main()
