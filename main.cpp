#pragma warning(disable : 4996)
#include <iostream>
#include <vector>
#include <list>

#define DEBUG false

struct cell {
    int len; //длина последовательности нулей нач. в этой ячейке
    int vlen; //длина верт. последовательности нулей, в которую входит ячейка 


    cell() {
        len = 0;
        vlen = 0;
    }

    cell(int l, int v) {
        len = l;
        vlen = v;
    }
};

int adcSteps = 0;

int main() {
    int m, n;
    std::cin >> n >> m;
    std::vector<std::vector<char>> matrix(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char a = 0;
            std::cin >> a;
            if (a == '0') {
                a = 0;
            }
            else {
                a = 1;
            }
            matrix[i].push_back(a);
        }
    }

    //заполнили
    //разбираем матрицу по рядам

    //заполняем нулевой ряд
    std::vector<cell> prev(m, cell());

    prev[m - 1] = cell();
    for (int i = m - 2; i >= 0; --i) {
        prev[i] = cell();
    }
    
    int Sm = 0;
    //обработка рядов
    for (int k = 0; k < n; ++k) {
        std::vector<cell> next(m, cell());
        next[m - 1] = ((matrix[k][m - 1] == 1) ? cell() : cell(1, 1));
        for (int i = m - 2; i >= 0; --i) { //заполнение форм.ряда

            if (matrix[k][i] == 0) {
                if (next[i + 1].len == 0) {
                    next[i] = cell(1, 1);
                }
                else {
                    next[i] = next[i + 1];
                    next[i].len++;
                }

            }
            else {
                next[i] = cell();
            }

        }

        /*
            есть карта высот, идем по ней, записываем в массив число последовательно идущих высот, покуда некая из них не прервет свое действие
            коль высота прервала свое действие, вычислим площадь прямоугольника, образованного ею и ежели она больше, чем площадь наибольшего из ранее образованных
            сей высотой прямоугольников, запишем ее.
            Продолжаем, покуда все высоты не пркратят свое действие.
            Взглянем на список площадей и вычислим наибольшую из них.

            Оптимизации:
                1. Будем записывать площади не для каждой высоты по отдельности, а для всех сразу.
                2. Чую я, можно сделать за О(m) но не вразумию как
        */
        //проход по ряду
        
        std::vector<int> heighths(n + 1);
        std::list<int> hm;
        if(DEBUG) printf("next row! (Sn = %d)\n", Sm);
        for (int i = m - 1; i >= 0; --i) {//обработка с учетом предыдущего ряда
            bool aits = false;

            if (next[i].len > 0 && prev[i].len > 0) {
                next[i].vlen = prev[i].vlen + 1;
            }

            if (i == m - 1) {
                if (heighths[next[i].vlen] == 0) {
                    hm.push_back(next[i].vlen);
                    aits = true;
                }
                heighths[next[i].vlen]++;
                continue;
            }

            //считаем площади мы только на конце поля действия высоты
            if (next[i].vlen >= next[i + 1].vlen) {
                if (heighths[next[i].vlen] == 0) {
                    hm.push_back(next[i].vlen);
                    aits = true;
                }
                heighths[next[i].vlen]++;
            }
            else {
                if (DEBUG) printf("\t |fall of height from %d to %d\n", next[i + 1].vlen, next[i].vlen);
                if (heighths[next[i + 1].vlen] * next[i + 1].vlen > Sm) {
                    Sm = heighths[next[i + 1].vlen] * next[i + 1].vlen;
                }
                //текущую высоту мы проверили, нашли площадь, теперь добавляем ее кол-во к кол-ву высоты пред. (оно в ней не учтено)
                //текущую высоту (число ее столбцов) зануляем

                hm.pop_back();
                if(!hm.empty()) heighths[hm.back()] += heighths[next[i + 1].vlen];
                heighths[next[i + 1].vlen] = 0;
                //проверка всех высот, срезанных вместе с первой
                while (!hm.empty() && hm.back() > next[i].vlen) {

                    adcSteps++;
                    if (heighths[hm.back()] * hm.back() > Sm) {
                        Sm = heighths[hm.back()] * hm.back();
                    }

                    int j = hm.back();
                    //зануляем проверенную высоту, идем дальше
                    hm.pop_back();
                    if (!hm.empty()) heighths[hm.back()] += heighths[j];
                    heighths[j] = 0;
                }

                //добпаляем текущую высоту
                if (heighths[next[i].vlen] == 0) {
                    hm.push_back(next[i].vlen);
                    aits = true;
                }
                heighths[next[i].vlen]++;
            }

        }

        if (DEBUG) {
            printf("|");
            for (int j = 0; j < n + 1; ++j) {
                printf("%d ", heighths[j]);
            }
            printf("|\n");
            if (!hm.empty()) printf("| %d <hmm|\n", hm.back());
            printf("row fin (Sn = %d)\n", Sm);
        }

        //закончили ряд, если остались высоты, значит, они все срезаются границей матрицы
        //учтем их всех
        while (!hm.empty()) {

            adcSteps++;
            if (heighths[hm.back()] * hm.back() > Sm) {
                Sm = heighths[hm.back()] * hm.back();
            }

            int j = hm.back();
            //зануляем проверенную высоту, идем дальше
            hm.pop_back();
            if(!hm.empty()) heighths[hm.back()] += heighths[j];
            heighths[j] = 0;
        }
        
        prev = next;
    }

    printf("%d\n",Sm);
    if(DEBUG)printf("ad: %d\n", adcSteps);

}


/*
тестики

4 4
1 0 0 1
1 1 0 1
1 1 1 1
1 1 1 1

4 4
1 0 0 1
0 1 0 1
1 0 0 0
1 1 1 1


4 4
1 0 0 1
0 0 0 1
1 0 0 0
1 1 1 1


4 4
1 0 1 1
0 0 0 1
1 0 0 0
1 0 1 1

4 4
1 0 1 1
1 0 0 0
1 0 1 0
1 0 1 1

4 4
1 1 1 1
1 0 0 0
1 0 0 0
1 0 0 1

4 4
1 1 1 1
0 0 0 1
0 0 0 0
0 0 0 0

4 5
1 1 1 1 1
0 0 0 1 1
0 0 0 0 0
0 0 0 0 0


3 5
0 1 1 1 1 
0 1 1 1 1
0 0 0 0 0

5 5
0 0 0 1 1
0 0 0 1 1
1 1 0 0 0
1 1 0 0 0
1 1 0 1 0


4 7
0 0 1 1 1 1 1
0 0 0 1 1 1 1 
0 0 0 0 0 1 1 
0 0 0 0 0 0 1

10 10
1 1 1 0 1 1 1 1 1 1
1 0 1 1 1 0 0 1 0 1
1 0 0 1 1 1 0 0 0 1
1 0 0 1 0 1 0 1 1 1
0 0 1 1 1 1 0 0 0 1
0 0 0 1 1 1 1 1 1 1
0 0 0 0 0 1 1 0 0 0
0 0 0 0 0 0 1 0 0 1
1 1 1 0 0 1 1 1 1 1
1 1 1 1 0 0 1 1 1 1

10 10
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
1 0 1 0 1 0 1 0 1 0
*/