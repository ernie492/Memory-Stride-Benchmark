#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

// Функция для запуска теста пропускной способности
double run_benchmark(size_t buffer_size_bytes, size_t stride_bytes) {
    // Выделяем массив байт
    size_t num_elements = buffer_size_bytes / sizeof(uint64_t);
    std::vector<uint64_t> buffer(num_elements, 1);

    size_t stride_elements = stride_bytes / sizeof(uint64_t);
    if (stride_elements == 0) stride_elements = 1;

    // Переменная volatile нужна, чтобы умный компилятор Visual Studio 
    // не догадался удалить наш цикл оптимизацией
    volatile uint64_t sum = 0;

    // Количество повторов для точности замера
    const int iterations = 50;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int iter = 0; iter < iterations; ++iter) {
        // Главный цикл с шагом (stride)
        for (size_t i = 0; i < num_elements; i += stride_elements) {
            sum += buffer[i];
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end_time - start_time;

    // Считаем объем прочитанных данных в гигабайтах
    double total_bytes_read = static_cast<double>(num_elements / stride_elements) * sizeof(uint64_t) * iterations;
    double seconds = elapsed.count() / 1000.0;

    // Переводим в ГБ/с (Гигабайты в секунду)
    double bandwidth_gb_s = (total_bytes_read / (1024.0 * 1024.0 * 1024.0)) / seconds;

    return bandwidth_gb_s;
}

int main() {
    std::ofstream csv_file("memory_benchmark_results.csv");
    csv_file << "BufferSizeKB,StrideBytes,BandwidthGBs\n";

    // Тестируем разные размеры массивов: от 32 КБ до 128 МБ
    size_t min_size = 32 * 1024;
    size_t max_size = 128 * 1024 * 1024;

    // Тестируем разные шаги (stride): от 8 байт (последовательно) до 4096 байт
    std::vector<size_t> strides = { 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };

    std::cout << "Running benchmark...\n";

    for (size_t size = min_size; size <= max_size; size *= 4) {
        for (size_t stride : strides) {
            // Если шаг больше текущего размера буфера, пропускаем
            if (stride > size) continue;

            double bw = run_benchmark(size, stride);

            // Сохраняем в файл
            csv_file << (size / 1024) << "," << stride << "," << bw << "\n";

            std::cout << "Size: " << (size / 1024) << " KB | Stride: " << stride
                << " bytes | Speed: " << bw << " GB/s\n";
        }
    }

    csv_file.close();
    std::cout << "Done! Results saved to memory_benchmark_results.csv\n";
    return 0;
}