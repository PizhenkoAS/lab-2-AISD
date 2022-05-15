#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct stats {
    size_t comparison_count = 0;
    size_t copy_count = 0;
};

stats selection_sort(std::vector<int>& data);
stats shell_sort(std::vector<int>& data);
vector<int> generate_vector(int n); //создать массив размера n случайных чисел
vector<int> generate_vector_asc(int n); //создать упорядоченный массив размера n случайных чисел
vector<int> generate_vector_desc(int n); //создать обратно упорядоченный массив размера n случайных чисел
vector<int> copy_vector(vector<int> A); //получить копию вектора


int main()
{
    setlocale(LC_ALL, "Russian");

    vector<int> a;
    vector<int> b;
    //вычисляем показатели для сортировки выбором

    cout << "Сортировка выбором\n";
    cout << "| Размер | Ср. сравнений | Ср. перемещений | Сравн. упор. | Перемещений упор. | Сравн. обр. упор. | Перемещений обр. упор. |\n";
    for (int size = 1000; size <= 10000; size += 1000) //размеры массивов
    {
        //среднее для 100 массивов
        size_t slct_cmp = 0, slct_cp = 0; //суммы сравнений и копирований для с. выбором
        for (int i = 0; i < 100; i++)
        {
            a = generate_vector(size); //получаем массив
            stats slct_stat = selection_sort(a); //выполняем сортировку
            slct_cmp += slct_stat.comparison_count; //суммируем статистику
            slct_cp += slct_stat.copy_count;
        }
        //вычисляем средние для выбором
        long double mean_slct_cmp = (long double)slct_cmp / 100.0;
        long double mean_slct_cp = (long double)slct_cp / 100.0;
        //полностью отсортированные
        a = generate_vector_asc(size); //получаем массив
        stats slct_stat_asc = selection_sort(a);
        //отсортированные в обратном порядке
        a = generate_vector_desc(size); //получаем массив
        stats slct_stat_desc = selection_sort(a);
        //вывод результатов
        cout << "| " << setw(6) << size << " | " << setprecision(8) << setw(13) << mean_slct_cmp << " | " << setprecision(8) << setw(15) << mean_slct_cp << " | "
            << setw(12) << slct_stat_asc.comparison_count << " | " << setw(17) << slct_stat_asc.copy_count << " | " << setw(17) << slct_stat_desc.comparison_count << " | " << setw(22) << slct_stat_desc.copy_count << " |\n";
    }

    //для шелла
    cout << "\nСортировка Шелла\n";
    cout << "| Размер | Ср. сравнений | Ср. перемещений | Сравн. упор. | Перемещений упор. | Сравн. обр. упор. | Перемещений обр. упор. |\n";
    for (int size = 1000; size <= 10000; size += 1000) //размеры массивов
    {
        //среднее для 100 массивов
        size_t shell_cmp = 0, shell_cp = 0; //для шелла
        for (int i = 0; i < 100; i++)
        {
            a = generate_vector(size); //получаем массив
            stats shell_stat = shell_sort(a); //сортировка шелла
            shell_cmp += shell_stat.comparison_count; //суммируем статистику
            shell_cp += shell_stat.copy_count;
        }
        //вычисляем средние шелла
        long double mean_shell_cmp = (long double)shell_cmp / 100.0;
        long double mean_shell_cp = (long double)shell_cp / 100.0;
        //полностью отсортированные
        a = generate_vector_asc(size); //получаем массив
        stats shell_stat_asc = shell_sort(a); //аналогично шелла
        //отсортированные в обратном порядке
        a = generate_vector_desc(size); //получаем массив
        stats shell_stat_desc = shell_sort(a);
        //вывод результатов
        cout << "| " << setw(6) << size << " | " << setprecision(8) << setw(13) << mean_shell_cmp << " | " << setprecision(8) << setw(15) << mean_shell_cp << " | "
            << setw(12) << shell_stat_asc.comparison_count << " | " << setw(17) << shell_stat_asc.copy_count << " | " << setw(17) << shell_stat_desc.comparison_count << " | " << setw(22) << shell_stat_desc.copy_count << " |\n";
    }
}

//создать массив размера n случайных чисел
vector<int> generate_vector(int n)
{
    vector<int> a;
    for (int i = 0; i < n; i++)
    {
        a.push_back(-1000 + rand() % 2000); //случайное -1000..1000
    }
    return a;
}

//получить копию вектора
vector<int> copy_vector(vector<int> a)
{
    vector<int> b;
    for (int i = 0; i < a.size(); i++)
        b.push_back(a[i]);
    return b;
}

//создать упорядоченный массив размера n случайных чисел
vector<int> generate_vector_asc(int n)
{
    vector<int> a;
    a.push_back(-1000 + rand() % 2000); //получаем первое число
    for (int i = 1; i < n; i++)
    {
        a.push_back(a[i - 1] + (1 + rand() % 100)); //следующее число заведомо больше предыдущего
    }
    return a;
}

//создать обратно упорядоченный массив размера n случайных чисел
vector<int> generate_vector_desc(int n)
{
    vector<int> a;
    a.push_back(rand() % 2000); //получаем первое число
    for (int i = 1; i < n; i++)
    {
        a.push_back(a[i - 1] - (1 + rand() % 100)); //следующее число заведомо меньше предыдущего
    }
    return a;
}

//сортировка выбором
stats selection_sort(std::vector<int>& data)
{
    stats result; //структура со статистикой сортировки
    result.copy_count = 0;
    result.comparison_count = 0;
    for (int i = 1; i < data.size(); i++)
    {
        int x = data[i]; //сортируемое значени
        int j = i - 1;
        result.comparison_count++; //+ сравнение
        while (j >= 0 && data[j] > x) //пока проходим по элементам больше сортируемого
        {
            data[j + 1] = data[j]; //сдвигаем
            j--;
            result.comparison_count++; //+ сравнение
            result.copy_count++; //+ 1 перемещение
        }
        data[j + 1] = x;
        result.copy_count++; //+1 перемещение
    }
    return result;
}

//сортировка шелла
stats shell_sort(std::vector<int>& data)
{
    stats result; //структура со статистикой сортировки
    result.copy_count = 0;
    result.comparison_count = 0;
    int gap = data.size() / 2; //длина промежутка, начинаем с половины длины, затем уменьшаем вдвое
    while (gap > 0)
    {
        // сортировка вставками для текущего расстояния
        // элементы data[0..gap-1] будут в нужном порядке,
        // выполняем вставки по одному элементу
        for (int i = gap; i < data.size(); i++)
        {
            int t = data[i]; // запоминаем элемент, который вставляем

            // сдвиг элементов, пока не найдем место для текущего
            int j;
            for (j = i; j >= gap && data[j - gap] > t; j -= gap)
            {
                data[j] = data[j - gap];
                //на каждом шаге + сравнение и перемещение
                result.comparison_count++;
                result.copy_count++;
            }
            data[j] = t; //вставка текущего элемента на нужное место
            result.copy_count++; //+1 перемещение
        }
        gap /= 2; //в два раза уменьшаем расстояние сортировки
    }
    return result;  
}
