#include <iostream>
#include <locale.h>
#include "string.h"
#include <vector>

using namespace std;

void Linearity(int** a, int n);
int** Multiply(int** a, int n);
bool Equally(int** M1, int** M2, int n);
void Output_M(int** a, int n, const string s);
void Output_Md(double** a, int n, const string s);
int** MatrEq(int** a, int n);
vector <vector <int>> Make_Classes(int** a, int n);
void Output_C(vector <vector <int>> v);
vector <vector <int>> Razb_Classes(vector <vector <int>> v, double** a, int n);
void Output_C1(vector <vector <int>> v);
double** MatrDomBezr(double** a, int n, int m);

int main()
{
    setlocale(LC_ALL, "RUS");
    int n,m;
    cout << "Введите размерность матрицы:\nКоличество строк:  ";
    cin >> n;
    cout << "Количество столбцов:  ";
    cin >> m;
    cout << endl;
    bool flag;

    //Создание и заполнение матрицы бальных оценок
    double ** O = new double* [n];
    for (int i = 0; i < n; i++) O[i] = new double[m];

    for (int i = 0; i < n; i++)
    {
        cout << "Введите элементы " << i + 1 << " строки через пробел:  ";
        for (int j = 0; j < m; j++) cin >> O[i][j];
    }
    system("cls");
    O = MatrDomBezr(O, n, m);
    Output_Md(O, n, "DomBezr");

    //Создание матрицы доминирование-безразличие
    int** M = new int* [n];
    for (int i = 0; i < n; i++) M[i] = new int[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (O[i][j] != 1 && O[i][j] != 0) M[i][j] = 1;
            else M[i][j] = O[i][j];

    int** M1 = new int* [n];
    for (int i = 0; i < n; i++) M1[i] = new int[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) M1[j][i] = M[j][i];
    
    //Проверка на линейность и равенство матриц для дальнейших действий
    Linearity(M1, n);
    flag = Equally(M, M1, n);
    
    if (!flag) Output_M(M, n, "исходная");

    // Нахождение квадрата матрицы, сравнение и вывод
    int** M2 = Multiply(M1, n);
    flag = Equally(M1, M2, n);
    Output_M(M1, n, "L");
    Output_M(M2, n, "L^2");
    if (flag)    cout << "\x1b[33mМатрицы равны, значит отношение p транзитивно\x1b[0m\n\n";
    else cout << "\x1b[33mМатрицы не равны, значит отношение р не транзитивно\x1b[0m\n\n";

    // Построение булевой матрицы отношения квазипорядка
    int** ME = MatrEq(M2, n);
    Output_M(ME, n, "булевая отношения квазипорядка");

    // Построение и вывод классов
    vector <vector <int>> v = Make_Classes(ME, n);
    Output_C(v);
    
    if (flag) exit(0);

    // Разбиение внутри классов = Тонкое ранжирование
    vector <vector<int>> v1 = Razb_Classes(v, O, n);
    Output_C1(v1);
}

double** MatrDomBezr(double** a, int n, int m)
{
    double** b = new double* [n];
    for (int i = 0; i < n; i++) b[i] = new double[n];
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++)
        {
            int k1 = 0;
            int k2 = 0;
            for (int ij = 0; ij < m; ij++)
            {
                if (a[i][ij] > a[j][ij]) k1++;
                if (a[i][ij] < a[j][ij]) k2++;
            }
            if (k1 > k2)
            {
                b[i][j] = 1;
                b[j][i] = 0;
            }
            else if (k2 > k1)
            {
                b[i][j] = 0;
                b[j][i] = 1;
            }
            else b[i][j] = b[j][i] = 0.5;
        }
    return b;
}

void Linearity(int** a, int n)// Проверка матрицы на линейность
{
    vector<int> v;
    for (int i = 1; i < n; i++)
        for (int j = 0; j < i; j++)
            if (a[i][j] + a[j][i] == 0)
            {
                v.push_back(j);
                v.push_back(i);
            }
    if (v.size() == 0)
    {
        cout << "Отношение линейно\n\n";
    }
    else
    {
        cout << "\nОтношение не линейно. Дополнить до линейности?\n1. Да\n2. Нет\nОтвет: ";
        int o;
        cin >> o;
        if (o == 1)
        {
            for (int i = 0; i < v.size(); i += 2) a[v[i]][v[i + 1]] = 1;
        }
        else
        {
            cout << "Конец программы";
            exit(0);
        }
    }
}

int** Multiply(int** a, int n)// Получение квадрата матрицы
{
    int** M2 = new int* [n];
    for (int i = 0; i < n; i++) M2[i] = new int[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) M2[i][j] = 0;
    int* b = new int[n];
    for (int i = 0; i < n; i++)
    {
        for (int ij = 0; ij < n; ij++) b[ij] = a[i][ij];
        for (int j = 0; j < n; j++)
            for (int ij = 0; ij < n; ij++)
            {
                M2[i][j] += b[ij] * a[ij][j];
                if (M2[i][j] > 1) M2[i][j] = 1;
            }

    }
    return M2;
}

bool Equally(int** M1, int** M2, int n)// Проверка на равенство матриц
{
    bool flag = true;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            if (M1[i][j] != M2[i][j])
            {
                flag = false;
                break;
            }
        }
    return flag;
}

void Output_M(int** a, int n, const string s)// Вывод матрицы с названием
{

    cout << "Матрица " << s << ":\n\n";
    cout << "   |";
    for (int i = 0; i < n; i++) cout << "\x1b[33m " << char(97 + i) << "\x1b[0m |";
    cout << endl;
    for (int i = 0; i < n + 1; i++) cout << "====";
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == 0) cout << "\x1b[33m " << char(97 + i) << "\x1b[0m |";
            if (a[i][j] == 1) cout << "\x1b[31m " << a[i][j] << "\x1b[0m |";
            else cout << "\x1b[34m " << a[i][j] << "\x1b[0m |";
        }
        cout << endl;
        for (int ij = 0; ij < n + 1; ij++) cout << "====";
        cout << endl;
    }
    cout << endl;
}

void Output_Md(double** a, int n, const string s)
{
    cout << "Матрица " << s << ":\n\n";
    cout << "     |";
    for (int i = 0; i < n; i++) cout << "\x1b[33m  " << char(97 + i) << "\x1b[0m  |";
    cout << endl;
    for (int i = 0; i < n + 1; i++) cout << "======";
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (j == 0) cout << "\x1b[33m  " << char(97 + i) << "\x1b[0m  |";
            if (a[i][j] == 1) cout << "\x1b[31m  " << a[i][j] << "\x1b[0m  |";
            else if (a[i][j]==0.5) cout << "\x1b[31m " << a[i][j] << "\x1b[0m |";
            else cout << "\x1b[34m  " << a[i][j] << "\x1b[0m  |";
        }
        cout << endl;
        for (int ij = 0; ij < n + 1; ij++) cout << "======";
        cout << endl;
    }
    cout << endl;
}

int** MatrEq(int** a, int n)// Булева матрица отношения эквивалентности 
{
    int** ME = new int* [n];
    for (int i = 0; i < n; i++) ME[i] = new int[n];
    for (int i = 0; i < n; i++) ME[i][i] = 1;
    for (int i = 1; i < n; i++)
        for (int j = 0; j < i; j++)
            if (a[i][j] == a[j][i]) ME[i][j] = ME[j][i] = 1;
            else ME[i][j] = ME[j][i] = 0;
    return ME;

}

vector <vector <int>> Make_Classes(int** a, int n)// Создание матрицы с разбиением на классы
{
    vector <vector <int>> v;
    v.push_back(vector<int>());
    int j = 0;
    int k = 0;
    for (int i = 0; k < n; i++)
    {
        bool f = false;
        for (int i1 = 0; i1 < j; i1++)
            for (int j1 = 0; j1 < v[i1].size(); j1++)
                if (v[i1][j1] == i) f = true;
        if (f) continue;
        for (int i1=0; i1<n; i1++)
            if (a[i][i1] == 1)
            {
                v[j].push_back(i1);
                k++;
            }
        if (k != n)
        {
            v.push_back(vector<int>());
            j++;
        }
    }
    /*v[j].push_back(i);
    if ((a[i][i + 1] != 1) && (i != (n - 1)))
    {
        v.push_back(vector<int>());
        j++;
    }*/
    return v;
}

void Output_C(vector <vector <int>> v)
{
    cout << "Классы эквивалентности:\n\n";
    for (int i = 0; i < v.size(); i++)
    {
        cout << "\x1b[33m" << "Класс С" << i + 1 << ": " << "\x1b[0m";
        for (int j = 0; j < v[i].size(); j++)
            cout << char(v[i][j]+97) << " ";
        cout << endl;
    }
}

vector <vector <int>> Razb_Classes(vector <vector <int>> v, double** a, int n)
{
    vector <vector <int>> v1;
    int t = -1;
    for (int i = 0; i < v.size(); i++)
    {
        v1.push_back(vector <int>());
        t++;
        if (v[i].size() == 1)
        {
            v1[t].push_back(v[i][0]);
            continue;
        }
        int** b = new int* [v[i].size()];
        for (int j = 0; j < v[i].size(); j++) b[j] = new int[v[i].size()];

        for (int j = 0; j < v[i].size(); j++)
            for (int j1 = 0; j1 < v[i].size(); j1++)
            {
                if (j == j1) b[j][j1] = 0;
                else b[j][j1] = a[v[i][j]][v[i][j1]] * 2;
            }

        // Проверка на однотипность
        int k = 0;
        for (int j = 0; j < v[i].size(); j++)
            for (int j1 = 0; j1 < v[i].size(); j1++)
                if (b[j][j1] == b[0][1]) k++;
        if ((k == v[i].size() * (v[i].size() - 1)) || (k == v[i].size() * v[i].size()))
        {
            for (int j = 0; j < v[i].size(); j++) v1[t].push_back(v[i][j]);
            for (int j = 0; j < v[i].size(); j++);
            continue;
        }

        // Сортировка по убыванию
        int** c = new int* [v[i].size()];
        for (int j = 0; j < v[i].size(); j++) c[j] = new int[2];
        for (int j = 0; j < v[i].size(); j++)
        {
            int s1 = 0;
            for (int j1 = 0; j1 < v[i].size(); j1++)
                s1 += b[j][j1];
            c[j][0] = v[i][j];
            c[j][1] = s1;
        }
        for (int j = 0; j < v[i].size(); j++)
            for (int j1 = 0; j1 < v[i].size() - 1; j1++)
            {
                if ((c[j1][1] < c[j1 + 1][1]) || (c[j1][1] == c[j1 + 1][1] && c[j1][0] > c[j1 + 1][0]))
                {
                    int m1 = c[j1][0];
                    c[j1][0] = c[j1 + 1][0];
                    c[j1 + 1][0] = m1;
                    m1 = c[j1][1];
                    c[j1][1] = c[j1 + 1][1];
                    c[j1 + 1][1] = m1;
                }
            }

        for (int j = 0; j < v[i].size(); j++)
        {
            v1[t].push_back(c[j][0]);
            if (j != v[i].size() - 1)
            {
                v1.push_back(vector<int>());
                t++;
            }
        }
    }
    return v1;
}

void Output_C1(vector <vector <int>> v)
{
    cout << "После 'Тонкого' ранжирования получилось такое разбиение:\n\n";
    for (int i = 0; i < v.size(); i++)
    {
        cout << " ";
        for (int j = 0; j < v[i].size(); j++)
            cout << char(v[i][j] + 97) << " ";
        if (i != v.size() - 1) cout << "\x1b[33m" << endl << " |\n\\ /\n" << "\x1b[0m";
    }
}
