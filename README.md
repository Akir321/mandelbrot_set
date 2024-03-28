# Использование SIMD инструкций для оптимизации вычислений

## Постановка задачи

Цель проекта - изучение влияния SIMD инструкций на быстродействие программы. В языке Си, выбранном в качестве основного в данном проекте, SIMD инструкции представлены intrinsic функциями, вызов которых заменяется компилятором на соответствующую ассемблерную инструкцию. 

Для работы с SIMD инструкциями было выбрано вычисление [множества Мандельброта](https://ru.wikipedia.org/wiki/Множество_Мандельброта). Это довольно трудоемкая задача, так как необходимо для каждой точки определить, принадлежит ли она множеству, причем определяется это путем вычислений в цикле. Формулы, задающие множество Мандельброта:

$x_n := x_{n-1}^2 - y_{n-1}^2 + x_0$,         \
$y_n := 2 \cdot x_{n-1} \cdot y_{n-1} + y_0$.

Значением N для каждой точки считается первое такое натуральное n, что $r_n^2$, равное $x_n^2 + y_n^2$, больше некой наперед заданной величины (довольно часто выбирается $10^2 = 100$). Свойства множества Мандельброта таковы, что, если N точно не меньше некого большого числа (хотя бы 256), то N бесконечно, значит, можно прекратить вычисления при прохождении границы в $n = 256$.`

Кроме того, результаты вычислений (точки с разными цветами) выводятся на экран с помощью графической библиотеки SDL2.

Полученная картинка:

![Множество Мандельброта](/readme_pics/Mandelbrot.png)

В проекте написаны две программы:
1. Наивная реализация
2. SIMD-оптимизированная программа

Мой процессор поддерживает инструкции стандартов AVX и AVX2, которые работают с ymm регистрами размером 256 бит. В одном таком регистре может храниться $\frac{256} {8 \cdot 4} = 8$ переменных типа float, соответственно ожидается ускорение работы программы в 8 раз.

Для измерения времени используется функция `getProcessorTime()`, которая выполняет ассемблерную инструкцию `rdtsc` и возвращает процессорное время. Для сравнения производительности двух программ достаточно поделить один результат на другой, переводить значения в другие единцы измерения не нужно.

Обе программы компилировались с флагом -O3.

При подсчете времени работы все точки пересчитывались в цикле 250 раз, при этом каждая программа запускалась по 3 раза, и результаты усреднялись.

Кроме того, в работе учитываются возможные накладные расходы вычислений - работа с графической библиотекой, инициализация переменных и констант. Так как нам важна разница именно в скорости вычислений, также измерялось время работы у этих программ при условии, что функция вычисления всегда возвращает одно и то же значение и почти не влияет на время работы. То есть измерялось время, затрачиваемое на накладные расходы. Далее из всего времени работы можно вычесть накладные расходы и получить реальное время вычислений.

## Наивная реализация

Программа `mandelbrot_naive` вычисляет N для всех точек последовательно в соответствии с описанными выше формулами. Характерное время работы программы 1 минуты при вычислениях и около 10-15 секунд без вычислений. Такое время работы позволяет достаточно точно проводить замеры.

Усредненное время работы по результатам 3 запусков -- 595965075 тактов процессора, а накладные расходы составили 108852935 тактов.

Таким образом, на вычисления в среднем затрачивалось 487112140 тактов процессора.

## SIMD-оптимизированная версия

Программа `mandelbrot_simd` вычисляет N параллельно для 8 точек. Для этого используются SIMD инструкции стандарта AVX2. Характерное время работы программы - 20 секунд. На версию программы без вычислений затрачивалось около 10 секунд.

Усредненное время работы по результатам 3 запусков -- 198266657 тактов процессора, а накладные расходы составили 109903514 такта.

Таким образом, вычисления производились 88363143 такта.

## Результаты эксперимента

Имеем прирост производительности в $\frac{487112140}{88363143} = 5,51$ раз, что значительно меньше, чем ожидаемые $8$ раз. Возможные причины такого результата:

1. В SIMD-оптимизированной версии нам приходится выполнять дополнительные команды, например, чтобы понять, что для всех данных 8 точек вычисления уже окончены, и можно переходить к следующему этапу.
2. Точки вычисляются по 8, причем некоторые точки вычисляются сильно дольше остальных (строго говоря, бесконечно, но в нашей программе есть ограничение -- максимальное количество итераций). Соответственно, приходится ждать, когда вычислится N для всех 8 точек из набора.
3. Накладные расходы, связанные с SIMD инструкциями, которые обязаны присутствовать в вычислениях (например,   инициализация переменных).