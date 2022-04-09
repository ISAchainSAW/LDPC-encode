#include <iostream>
#include <ctime>
#include <fstream>

using namespace std;
void fillArray(int** arr, int rows, int cols, int one, int& pop_one, int second, int & inf); //Функция заполнения разряженной проверочной матрицы
void showArray(int** arr, int cols); //Функция отображения массива с 1 строчкой на экран
void showArray(int** arr, int rows, int cols); //Функция отображения массива на экран
void showArray(int** arr, int rows, int cols, int** arr2); //Функция отображения 2-х массивов на экран
int rar(int one, const int cols, int& pop_one); //Функция генерации элементов массива
void Gaus(int** arr, int rows, int cols); //Метод Гауса, обнуляет область под диагональю
void Gaus_UP(int** arr, int rows, int cols); //Метод Гауса, Обнуляем область над диагональю, складываем строчки снизу вверх
void SWAP_COLS(int** arr, int rows, int cols, int* H_swap);//Замена столбцов для формирования единичной матрицы в методе Гауса
int one_chek(int** arr, int rows); //Функция проверки единиц по диагонали
int one_chek(int** arr, int rows, int step); //Функция проверки единиц по диагонали с указанием изначального столбца
void Sort(int** arr, int rc, int cols, int* H_swap, int rows);  //Переставляет столбцы по матрице перестановок
void Cycle(int** arr, int rows, int cols);

//read_me найти НОД чисел, потом поделить оба числа на этот НОД, получим кол-во единиц в строчках и столбцах соответственно
//Восстановил неодинаковость строк 
int main()
{
    srand(time(NULL));
    fstream obj;
    obj.open("Matrix.txt", fstream::out | fstream::app);
    if (!obj.is_open())
    {
        std::cout << "Open ERROR\n";
    }
    int rows = 0;//Length message word (K)
    int cols = 0;//Length code word + parity matrix (N)
    int one = 0, pop_one = 0, second = 0;
    int repeat = 1; //Количество повторов выполнения программы
    int inf = 0;
    std::cout << "Input rows = K = length message\nAnd input cols = N = length code word\n\nrows<cols\n\n";
    std::cout << "rows= "; cin >> rows;
    std::cout << "cols= "; cin >> cols;
    std::cout << "one for rows= "; cin >> second;
    std::cout << "one for cols= "; cin >> one;
    int rc = cols - rows;//Length rows H matrix
    if (cols <= rows)
    {
        std::cout << "\n\nError input\n\n";
        return -1;
    }
    while (repeat!= 0) {
        int* H_swap = new int[cols]; //Swap matrix
        for (int i = 0; i < cols; i++)
        {
            H_swap[i] = i;
        }
        int** H = new int* [rc];//Origainal mass
        int** HH = new int* [rc];//Copy original
        int** HHH = new int* [rc];//After sort mass

        //Fill arr pointer
        for (int i = 0; i < rc; i++)
        {
            H[i] = new int[cols] {};
            HH[i] = new int[cols] {};
            HHH[i] = new int[cols] {};
        }
        fillArray(H, rc, cols, one, pop_one, second, inf);
        //std::cout << "\nH matrix\n";
        //showArray(H, rc, cols);

        if (inf == 150)
        {
            std::cout << "\nError infinity\n";
            for (int i = 0; i < rc; i++)
            {
                delete[] H[i];
                delete[] HH[i];
                delete[] HHH[i];
            }
            delete[] H;
            delete[] HH;
            delete[] HHH;
            inf = 0;
            system("cls");
            continue;
        }

        //=============================================================================<<<<< 
        //ЦИКЛЫ----------------------------------------------------------------------------------
        //Cycle(H, rc, cols);

        //Copy matrix
        for (int i = 0; i < rc; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                HH[i][j] = H[i][j];
            }
        }
        Gaus(H, rc, cols);
        if (one_chek(H, rc) < rc) // Проверка единиц по главной диагонали единичной матрицы
        {
            SWAP_COLS(H, rc, cols, H_swap);
             if (one_chek(H, rc) == rc) {
             Gaus(H, rc, cols);
                 if (one_chek(H, rc) < rc) 
                     SWAP_COLS(H, rc, cols, H_swap);   
             }
            if (one_chek(H, rc) < rc) {
                std::cout << "\nBad Generation_1\n";
                for (int i = 0; i < rc; i++)
                {
                    delete[] H[i];
                    delete[] HH[i];
                    delete[] HHH[i];
                }
                delete[] H;
                delete[] HH;
                delete[] HHH;
                continue;
            }
        }
        Gaus_UP(H, rc, cols);
        Sort(H, rc, cols, H_swap, rows);
        if (one_chek(H, rc, rows) < rc) {
            std::cout << "\nBad Generation_2\n";
            for (int i = 0; i < rc; i++)
            {
                delete[] H[i];
                delete[] HH[i];
                delete[] HHH[i];
            }
            delete[] H;
            delete[] HH;
            delete[] HHH;
            continue;
        }


        //Инициализируем массив под порождающую матрицу 
        int** Garr = new int* [rows];
        for (int i = 0; i < rows; i++)
        {
            Garr[i] = new int[cols];
        }

        //Формируем порождающую матрицу 
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                if (j == i)
                    Garr[i][j] = 1;
                else Garr[i][j] = 0;
            }
        }
        int r = 0;
        for (int i = 0; i < rows; i++)
        {
            for (int j = rows; j < cols; j++)
            {
                Garr[i][j] = H[r][i];
                r++;
            }
            r = 0;
        }

        std::cout << "G matrix\n";
        showArray(Garr, rows, cols);

        for (int k = 0; k < cols; k++)
        {
            for (int i = 0; i < rc; i++)
            {
                HHH[i][k] = HH[i][H_swap[k]];
            }
        }
         //Вывод матрицы H spare matrix в txt
        obj << "\nLD_H_matrix\n[";
        for (int i = 0; i < rc; i++)
        {
            obj << "[";
            for (int j = 0; j < cols; j++)
            {
                if (j == (cols - 1))
                {
                    obj << HHH[i][j];
                    break;
                }
                obj << HHH[i][j] << ",";
            }
            if (i == rc - 1)
            {
                obj << "]]\n";
                break;
            }
            obj << "],\n";
        }
        //Вывоод матрицы G в txt
        obj << "\nG_matrix\n[";
        for (int i = 0; i < rows; i++)
        {
            obj << "[";
            for (int j = 0; j < cols; j++)
            {
                if (j == (cols - 1))
                {
                    obj << Garr[i][j];
                    break;
                }
                obj << Garr[i][j] << ",";
            }
            if (i == rows - 1)
            {
                obj << "]]\n";
                break;
            }
            obj << "],\n";
        }

        for (int i = 0; i < rc; i++)
        {
            delete[] H[i];
            delete[] HH[i];
            delete[] HHH[i];
        }
        delete[] H;
        delete[] HH;
        delete[] HHH;
        repeat--;
    }
}
void Sort(int** arr, int rc, int cols, int* H_swap, int rows) 
{ 
    int** tmp = new int* [rc];
    int l = 0;
    int m = 0;
    int tmp_swap = 0; // переменная обмена
    int* H_swap2 = new int[cols];
    //Копируем массив перестановок
    for (int i = 0; i < cols; i++)
    {
        H_swap2[i] = H_swap[i];
    }
    //Массив копии
    for (int i = 0; i < rc; i++)
    {
        tmp[i] = new int[cols];
    }
    //Копируем массив
    for (int i = 0; i < rc; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            tmp[i][j] = arr[i][j];
        }
    }
    //Выставляем в начало rows последних столбцов 
    for (int i = rc; i < cols; i++)
    {
        H_swap[l] = H_swap2[i];
        for (int j  = 0; j < rc; j++)
        {
            arr[j][l] = tmp[j][i];
        }
        l++;
    }
    //Выставляем полсе rows столбцов единичную матрицу
    for (int i = l; i < cols; i++)
    {
        H_swap[i] = H_swap2[m];
        for (int k = 0; k < rc; k++)
        {
            arr[k][i] = tmp[k][m];
        }
        m++;
    }
    //std::cout << "\n";
    //for (int i = 0; i < cols; i++)
    //{
    //    std::cout << "idx =" << i << "\t" << H_swap[i] << "\t" << H_swap2[i] << "\n";
    //}
    for (int i = 0; i < rc; i++)
    {
        delete[] tmp[i];
    }
    delete[] tmp;
    delete[]H_swap2;
}
int one_chek(int** arr, int rows, int step)
{
    int chek = 0;
    for (int i = 0; i < rows; i++)
    {
        if (arr[i][step] == 1)
        {
            chek++;
        }
        step++;
    }
    return chek;
}
int one_chek(int** arr, int rows)
{
    int chek = 0;
    for (int i = 0; i < rows; i++)
    {
        if (arr[i][i] == 1)
        {
            chek++;
        }
    }
    return chek;
}
void SWAP_COLS(int** arr, int rows, int cols, int* H_swap)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = i; j < cols;)
        {
            //Поиск единиц по диагонали
            if (arr[i][j] != 1)
            {
                //std::cout << "NeRAVNO\t" << i << "\t" << j << "\n";
                //Поиск подходящего столбца на замену
                for (int m = cols - 1; m > i + 1; m--)
                {
                    if (arr[i][m] == 1)
                    {
                        
                        //std::cout << "Found = "<< m <<"\n";
                        int tmp_swap = 0; // переменная обмена
                        //Записываем обмен столбцов в матрицу перемещений
                        tmp_swap = H_swap[m];
                        H_swap[m] = H_swap[j];
                        H_swap[j] = tmp_swap;
                        //Обмен столбцов
                        for (int k = 0; k < rows; k++)
                        {
                            //цикл обмена
                            tmp_swap = arr[k][j];
                            arr[k][j] = arr[k][m];
                            arr[k][m] = tmp_swap;
                        }
                        break;
                    }
                    //std::cout << "m= " << m<<"\n";
                }         
            }
            break;
        }
    }
}
void Gaus(int** arr, int rows, int cols)
{
    bool chek = false;
    int idx = 0;
    for (int i = 0; i < rows; i++)
    {
        chek = false;
        for (int j = 0 + i; j < rows; j++)
        {
            if (arr[j][i] == 1)
            {
                if (chek)
                {
                    for (int m = 0; m < cols; m++)
                    {
                        arr[j][m] += arr[idx][m];
                        arr[j][m] %= 2;
                    }
                }
                if (!chek)
                {
                    chek = true;
                    idx = j;
                }
            }
        }
        if (idx != i && chek)
        {
            int* tmp = new int[cols];
            tmp = arr[idx];
            arr[idx] = arr[i];
            arr[i] = tmp;
            tmp = nullptr;
            delete[] tmp;
        }
        idx = 0;
    }
}
void Gaus_UP(int** arr, int rows, int cols)
{
    for (int i = rows - 1; i > 0; i--)
    {
        int s_cols = i; //Выбираем крайний правый столбец единичной матрицы
        for (int m = 0; m < i; m++)
        {
            if (arr[m][s_cols] == 1)
            {
                for (int j = 0; j < cols; j++)
                {
                    arr[m][j] += arr[i][j];
                    arr[m][j] %= 2;
                }
            }
        }
    }
}
void showArray(int** arr, int cols) 
{
    std::cout << "\t";
    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << "\n";
    }
}
void showArray(int** arr, int rows, int cols)//Функция отображения массива на экран
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << "\n";
    }
}
void showArray(int** arr, int rows, int cols, int** arr2)
{
    for (int i = 0; i < rows; i++)
    {
        std::cout << arr2[0][i] << "\t";
        for (int j = 0; j < cols; j++)
        {
            std::cout << arr[i][j] << "\t";
        }
        std::cout << "\n";
    }
}
void fillArray(int** arr, int rows, int cols, int one, int& pop_one, int second  , int& inf)
{
    //oneR[0] for cols //ones in cols must be  == one           //walk by cols
    //oneC[1] for rows // ones in rows must be < 2              //walk by rows
    int** oneR = new int* [1];
    int** oneC = new int* [1];
    bool sign = true; // для функции notzero переключатель режима
    int chek_one = 0; // проверка единиц на совпадение строк
    for (int i = 0; i < 1; i++)
    {
        oneR[i] = new int[cols] {};
        oneC[i] = new int [rows] {};
    }
    //**************************************************************************************
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {

            if (arr[i][j] != 1) {
                if (oneR[0][j] == one) continue;
                else arr[i][j] = rar(second, cols, pop_one);
                if (arr[i][j] == 1)
                {
                    oneR[0][j]++;
                    oneC[0][i]++;
                }
            }
            //Кол-во единиц в строчке и столбце не превышает значений
            if (oneC[0][i] > second)
            {
                arr[i][j] = 0;
                oneR[0][j]--;
                oneC[0][i]--;
                pop_one--;
            }
            if (oneR[0][j] > one)
            {
                arr[i][j] = 0;
                oneR[0][j]--;
                oneC[0][i]--;
                pop_one--;
            }
        }
        if (oneC[0][i] == second) {
            pop_one = 0;
            //Неодинаковость пары строк
           
            if (i > 0) //Проверка начинается со 2 строчки
            {
                for (int p = 0; p < i; p++)//Цикл строк
                {
                    for (int f = 0; f < cols; f++)//Цикл столбцов
                    {
                        if (arr[i][f] == 1 && arr[p][f] == 1)//Проверка пересечения единиц в строках
                        {
                            chek_one++;
                        }
                    }
                    if (chek_one > 1)//проверка на пересечение единиц в строках
                    {
                        cout << "p= " << p << "\n";
                        for (int g = 0; g < cols; g++)
                        {
                            if (arr[i][g] == 1)
                            {
                                oneR[0][g]--;
                                oneC[0][i]--;
                            }
                            arr[i][g] = 0;
                        }
                    }
                    chek_one = 0;
                }
            }
        }
        if (oneC[0][i] != second) //Условие повторной генерации единиц в строках
        {
            inf++;
            i--;
            if (inf == 150)
            {
                pop_one = 0;
                break;
            }
        }
        //std::cout << "\nH matrix\n";
        //showArray(oneR, cols);
        //showArray(arr, rows, cols, oneC);
        //std::cout << "________________________\n";
    }
}
int rar(int one, const int cols, int& pop_one) 
{
    int res = 0;
    float rand_v = rand() % 101 / static_cast<float>(100);
    float p = (static_cast<float>(one) - pop_one) / (static_cast<float>(cols) - pop_one);
    if (rand_v < p)
    {
        res = 1;
        pop_one++;
    }
    return res;
}
void Cycle (int** arr, int rows, int cols)
{
    int** mas = new int*[rows];
    int size_s = 0;
    int k = 0, tmp = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (arr[i][j] == 1) size_s++;
        }
        mas[i] = new int[size_s];
        for (int j = 0; j < cols; j++)
        {
            if (arr[i][j] == 1) {
                mas[i][k] = j;
                    k++;
            }
        }
        k = 0;
        tmp = size_s;
        size_s = 0;
    }   
    cout << "\n";
    showArray(mas, rows, tmp);
}