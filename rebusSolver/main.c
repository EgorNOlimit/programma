#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LENGTH 256
#define MAX_WORDS 8 // 7 слогаемых + результат

// Исходный ребус
char expression[MAX_LENGTH];

// Слова из ребуса
char words[MAX_WORDS][MAX_LENGTH];

// Количество слов (включая результат)
int word_count = 0;

// Уникальные буквы
char letters[10];
int letter_count = 0;

// Массив соответствий буквам
int digits[26];

// Использованные цифры
int used_digits[10];

// Проверка, является ли буква первой в слове
int leading[26];

// Найдено ли решение
int solution_found = 0;



void initialize(void)
{
    for (int i = 0; i < 26; i++)
    {
        digits[i] = -1;
        leading[i] = 0;
    }

    for (int i = 0; i < 10; i++)
    {
        used_digits[i] = 0;
    }

    word_count = 0;
    letter_count = 0;
    solution_found = 0;
}



int parser(void)
{
    int pos = 0;

    for (int i = 0; expression[i] != '\0'; i++)
    {
        char c = expression[i];

        if (c >= 'A' && c <= 'Z')
        {
            if (word_count >= MAX_WORDS)
                return 0;

            words[word_count][pos++] = c;

            if (pos >= MAX_LENGTH)
                return 0;

            int index = c - 'A';

            if (digits[index] == -1)
            {
                int exists = 0;

                for (int j = 0; j < letter_count; j++)
                {
                    if (letters[j] == c)
                    {
                        exists = 1;
                        break;
                    }
                }

                if (!exists)
                {
                    if (letter_count >= 10)
                        return 0;

                    letters[letter_count++] = c;
                }
            }
        }
        else if (c == '+' || c == '=')
        {
            if (pos == 0)
                return 0;

            words[word_count][pos] = '\0';

            if (pos > 1)
                leading[words[word_count][0] - 'A'] = 1;

            word_count++;
            pos = 0;
        }
        else if (c != ' ')
        {
            return 0;
        }
    }

    if (pos == 0 || word_count >= MAX_WORDS)
        return 0;

    words[word_count][pos] = '\0';

    if (pos > 1)
        leading[words[word_count][0] - 'A'] = 1;

    word_count++;

    return 1;
}


long long calculate(const char* word)
{
    long long number = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'A';

        number = number * 10 + digits[index];
    }

    return number;
}


int check_solution(void)
{
    long long sum = 0;

    for (int i = 0; i < word_count - 1; i++)
    {
        sum += calculate(words[i]);
    }

    long long result = calculate(words[word_count - 1]);

    return sum == result;
}


int search(int pos)
{
    if (pos == letter_count)
    {
        return check_solution();
    }

    int index = letters[pos] - 'A';

    for (int digit = 0; digit <= 9; digit++)
    {
        if (used_digits[digit])
            continue;

        if (digit == 0 && leading[index])
            continue;

        digits[index] = digit;
        used_digits[digit] = 1;

        if (search(pos + 1))
            return 1;

        digits[index] = -1;
        used_digits[digit] = 0;
    }

    return 0;
}


void print_solution(void)
{
    printf("\nSolution:\n");

    for (int i = 0; i < word_count; i++)
    {
        printf("%lld", calculate(words[i]));

        if (i < word_count - 2)
            printf(" + ");
        else if (i == word_count - 2)
            printf(" = ");
    }

    printf("\n");
}


int main(void)
{
    initialize();

    printf("Enter rebus: ");

    if (fgets(expression, sizeof(expression), stdin) == NULL)
        return 1;

    expression[strcspn(expression, "\n")] = '\0';

    if (!parser())
    {
        printf("Parsing error!\n");
        return 1;
    }

    clock_t start = clock();

    int found = search(0);

    clock_t end = clock();

    if (found)
    {
        print_solution();
    }
    else
    {
        printf("No solution found.\n");
    }

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Execution time: %.6f seconds\n", elapsed);

    return 0;
}

