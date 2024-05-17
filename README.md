### Техническое задание (Вариант 1)

1.	Написать на языке С++ класс выполняющий функциональность модулятора QAM (QPSK, QAM16, QAM64)
2.	Написать на языке С++ класс выполняющий функциональность добавления гауссовского шума к созвездию QAM
3.	Написать на языке С++ класс выполняющий функциональность демодулятора QAM (QPSK, QAM16, QAM64)
4.	Написать последовательный вызов 1-3 для случайной последовательности бит для разных значений дисперсия шума
5.	Построить график зависимости вероятности ошибки на бит от  дисперсии шума

### Описание проекта

Реализация классов написана в файле *QAM_Modem.hpp* (пункты 1-3), который подключается к файлу *main.cpp*, моделирующему пропускание набора битов через модулятор, добавление шума и демодулятор (пункт 4). Также есть файл *helpful_functions.hpp*, в котором прописаны некоторые используемые функции, не связанные с работой модулятора/демодулятора.

Пункт 5 сделан в файле *plot_biterr_probability.ipynb*, построен график такой, какой потребован в задании.

### Описание модулятора

Созвездие взято такое же, как в библиотеке *commpy* (Python). Расположение значений соответствует коду Грея.

Созвездие модулятора можно посмотреть в файле *plot_biterr_probability.ipynb*, в разделе `прочие графики`.

Нормировка сделана так, чтоб средняя мощность сигнала модулятора была равна 1. Нормировочные множители:

- QAM-4 : $1/\sqrt{2}$
- QAM-16: $1/\sqrt{10}$
- QAM-64: $1/\sqrt{42}$
