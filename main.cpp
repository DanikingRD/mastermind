#include <iostream>
// Not recommended, but this is a very small program.
using namespace std;

const uint KEY_LENGTH = 4;
const uint KEY_RANGE_MIN = 1;
const uint KEY_RANGE_MAX = 6;
const uint MAX_ATTEMPTS = 10;

// Properly document visual representation of the table.

/*
    Representa una tabla con los intentos del usuario y la clave secreta.
    - La primera fila es la clave secreta.
    - Cada fila representa un intento del usuario.

        Representacion visual:

        -   +---+---+---+---+
        -   | 5 | 2 | 6 | 1 |
        -   +---+---+---+---+
        -   | 1 | 2 | 3 | 4 |
        -   +---+---+---+---+
        -   | 1 | 2 | 3 | 4 |
        -   +---+---+---+---+
        -   | 1 | 2 | 3 | 4 |
        -   +---+---+---+---+
*/
typedef int table_t[MAX_ATTEMPTS + 1][KEY_LENGTH];

int genRange(int min, int max)
{
    return min + (rand() % max);
}

// TODO: try to make a generic formatter that works for both int* and char*
// without casting pointers or using templates >_<.
string formatIntArray(int *row)
{
    string out = "[";
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        out += to_string(row[i]);
        if (i < KEY_LENGTH - 1)
        {
            out += ", ";
        }
    }
    out += "]";
    return out;
}

string formatClue(char *buf)
{
    string out = "[";
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        out += buf[i];
        if (i < KEY_LENGTH - 1)
        {
            out += ", ";
        }
    }
    out += "]";
    return out;
}

void genSecretKey(table_t table)
{
    for (int col = 0; col < KEY_LENGTH; col++)
    {
        table[0][col] = genRange(KEY_RANGE_MIN, KEY_RANGE_MAX);
    }

    cout << "key: " << formatIntArray(table[0]) << endl;
}

bool arrayContains(int *buf, int val)
{
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        if (buf[i] == val)
        {
            return true;
        }
    }
    return false;
}

bool getClue(table_t table, char *buf, int attempt)
{
    int *key = table[0];

    // first thing is to check whether the input exists in the key
    // For each entry in the current table row, we check if it's present on the key.
    int hits = 0;
    for (int index = 0; index < KEY_LENGTH; index++)
    {
        int input = table[attempt][index];
        bool isPresent = arrayContains(key, input);
        bool f2f = key[index] == table[attempt][index];
        isPresent ? f2f ? buf[index] = 'C' : buf[index] = 'F' : buf[index] = 'X';
        buf[index] == 'C' ? hits++ : hits;
    }
    return hits == KEY_LENGTH;
}

bool checkInput(int input)
{
    return input >= KEY_RANGE_MIN && input <= KEY_RANGE_MAX;
}

void readDigit(int *input, int attempt, int index)
{
    while (true)
    {
        cout << "Ingrese el valor " << (index + 1) << "/" << KEY_LENGTH << ": ";
        if (!(cin >> *input) || !checkInput(*input))
        {
            cout << "Valor inválido. Ingrese un número entre " << KEY_RANGE_MIN << " y " << KEY_RANGE_MAX << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        break;
    }
}
void updateTable(table_t table, int attempt)
{
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        int input;
        readDigit(&input, attempt, i);
        table[attempt][i] = input;
    }
}

bool showAttempts(table_t table, int attempt)
{
    for (int row = 0; row < attempt; row++)
    {
        char clue[KEY_LENGTH];
        bool hitAll = getClue(table, clue, row + 1);
        cout << "Intento " << row + 1 << "/" << MAX_ATTEMPTS << " - "
             << "Entrada: " << formatIntArray(table[row + 1])
             << " - Salida: " << formatClue(clue) << endl;

        if (hitAll)
        {
            cout << "Ganaste!" << endl;
            return false;
        }
    }

    return true;
}

int run(table_t table)
{
    int attempt = 1;
    while (attempt <= MAX_ATTEMPTS)
    {
        cout << " --- Intento " << attempt << "/" << MAX_ATTEMPTS << " --- " << endl;
        updateTable(table, attempt);

        bool shouldContinue = showAttempts(table, attempt);

        if (!shouldContinue)
        {
            break;
        }

        attempt++;
    }
    return (MAX_ATTEMPTS - attempt) + 1;
}

int main(int argc, char const *argv[])
{
    cout << "Bienvenido/a al Master Mind!" << endl;
    cout << "Adivina la clave de 4 digitos. Tienes " << MAX_ATTEMPTS << " intentos." << endl;
    srand(time(NULL));

    table_t table_t;
    genSecretKey(table_t);
    int score = run(table_t);
    std::cout << "Tu puntaje es: " << score << "/" << MAX_ATTEMPTS << std::endl;
    return 0;
}