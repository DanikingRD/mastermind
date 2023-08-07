#include <iostream>

using namespace std;

/*
 * ENUNCIADO: Juego Mastermind
 * INTEGRANTES: 1113684 - Clay Manuel Gomera Arias
 *              1114116 - Jean Carlos Araujo Molina
 *              1116906 - Gabriel Mejía Acosta
 *              1114441 - Rafael Eduardo Peralta Pimentel
 *              1114814 - Hadelin Maria García Cedano
 *              1114950 - Elian Gabriel Villegas Báez
 *              1116614 - Luis Daniel de la Cruz García
 *              1116623 - Aquilenyi Suero de los Santos
 * FECHA: 05/08/2023 <== Fecha de realización
*/

/// @brief La cantidad de dígitos que contiene la clave.
const uint KEY_LENGTH = 4;
/// @brief El número mas pequeño del rango de números de la clave.
const uint KEY_RANGE_MIN = 1;
/// @brief El número más grande del rango de números de la clave.
const uint KEY_RANGE_MAX = 6;
/// @brief La cantidad máxima de intentos posibles para adivinar la clave.
const uint MAX_ATTEMPTS = 10;

/*
 *  Representa una tabla con los intentos del usuario y la clave secreta,
 *  haciendo uso de un arreglo bidimensional (2D) de enteros.
 *
 *  La primera fila contiene la clave secreta. Las siguientes filas son los intentos del usuario.
 *  Debido a esto, la tabla tiene un tamaño de [MAX_ATTEMPTS] + 1 filas * KEY_LENGTH columnas.
 *
 *  +---+---+---+---+
 *  | 5 | 6 | 1 | 1 | <- Clave secreta
 *  +---+---+---+---+
 *  | 3 | 4 | 5 | 6 | <- Intento 1
 *  +---+---+---+---+
 *  | 1 | 5 | 2 | 2 | <- Intento 2
 *  +---+---+---+---+
 *  | 4 | 2 | 3 | 1 | <- ..Intento MAX_ATTEMPTS
 *  +---+---+---+---+
 *
*/

typedef int table_t[MAX_ATTEMPTS + 1][KEY_LENGTH];

/*
 * Genera un número aleatorio entre [min] y [max].
 *
 * @param min El valor mínimo del rango.
 * @param max El valor máximo del rango.
 *
 * @return Un número entre entre [min] y [max].
*/
int genRange(int min, int max)
{
    return min + (rand() % max);
}

/*
 * Verifica si un arreglo contiene un valor, de manera lineal.
 * @param buf El arreglo a verificar.
 * @param val El valor a buscar.
 * @return true si el valor existe en el arreglo, false en caso contrario.
*/
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

/// @brief  Formatea un arreglo de enteros.
/// @tparam T El tipo de dato del arreglo, puede ser int o char.
/// @param buf El arreglo a formatear.
/// @return Una cadena de texto con el formato [1, 2, 3, n]
template <typename T>
string formatArray(T *buf)
{
    string out = "[";
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        typeid(T) == typeid(char) ? out += buf[i] : out += to_string(buf[i]);
        if (i < KEY_LENGTH - 1)
        {
            out += ", ";
        }
    }
    out += "]";
    return out;
}

/*
 * Genera una clave con digitos entre [KEY_RANGE_MIN] y [KEY_RANGE_MAX].
 * Cada dígito de la clave es único, es decir, no se repite.
 * @param table Referencia a la tabla donde se guardará la clave, en la primera fila.
*/
void genSecretKey(table_t table)
{
    int *row = table[0];
    for (int col = 0; col < KEY_LENGTH; col++)
    {
        int dice = genRange(KEY_RANGE_MIN, KEY_RANGE_MAX);
        // Si el número ya existe en la clave, se vuelve a generar otro número.
        while (arrayContains(row, dice))
        {
            dice = genRange(KEY_RANGE_MIN, KEY_RANGE_MAX);
        }
        row[col] = dice;
    }
    cout << "[SECRET_KEY]: " << formatArray<int>(table[0]) << endl;
}

/*
 * Actualiza la tabla con los intentos del usuario y genera las pistas.
 *
 * @param table Referencia a la tabla donde se almacenan los intentos del usuario.
 * @param buf Referencia al arreglo donde se almacenarán las pistas.
 * @param attempt El número de intento actual.
 * @return true si el usuario adivinó la clave, false en caso contrario.
*/
bool updateTable(table_t table, char *buf, int attempt)
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

/*
 * Verifica si el dígito ingresado por el usuario es válido,
 * es decir, si está dentro del rango de números de la clave.
 * @param input El dígito ingresado por el usuario.
 * @return Si el dígito es válido o no.
*/
bool verifyInput(int input)
{
    return input >= KEY_RANGE_MIN && input <= KEY_RANGE_MAX;
}

/*
 * Lee un dígito ingresado por el usuario, y lo valida.
 * @param input Referencia al entero donde se almacenará el dígito ingresado.
 * @param attempt El número de intento actual.
 * @param index El índice del dígito actual de [KEY_LENGTH].
*/
void readDigit(int *input, int attempt, int index)
{
    while (true)
    {
        cout << "Ingrese el valor " << (index + 1) << "/" << KEY_LENGTH << ": ";
        if (!(cin >> *input) || !verifyInput(*input))
        {
            cout << "Valor inválido. Ingrese un número entre " << KEY_RANGE_MIN << " y " << KEY_RANGE_MAX << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        break;
    }
}

/*
 * Lee la entrada del usuario y la almacena en la tabla.
 * @param table Referencia a la tabla donde se almacenará la entrada.
 * @param attempt El número de intento actual.
*/
void readInput(table_t table, int attempt)
{
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        int input;
        readDigit(&input, attempt, i);
        table[attempt][i] = input;
    }
}

/*
 * Muestra los intentos del usuario y las pistas.
 * Además, verifica si el usuario adivinó la clave.
 * @param table Referencia a la tabla donde se almacenan los intentos del usuario.
 * @param attempt El número de intento actual.
*/
bool showAttempts(table_t table, int attempt)
{
    for (int row = 0; row < attempt; row++)
    {
        char clue[KEY_LENGTH];
        bool hitAll = updateTable(table, clue, row + 1);
        cout << "Intento " << row + 1 << "/" << MAX_ATTEMPTS << " - "
             << "Entrada: " << formatArray<int>(table[row + 1])
             << " - Salida: " << formatArray<char>(clue) << endl;
        if (hitAll)
        {
            cout << "Ganaste!" << endl;
            return false;
        }
    }

    return true;
}

/*
 * Ejecuta el `main loop` del programa.
 *
 * Los intentos incrementan a medida que el usuario ingresa sus valores.
 * Las condiciones contempladas para terminar el programa son:
 *  - El usuario adivina la clave.
 *  - El usuario se queda sin intentos.
*/
int run(table_t table)
{
    int attempt = 1;
    while (attempt <= MAX_ATTEMPTS)
    {
        cout << " --- Intento " << attempt << "/" << MAX_ATTEMPTS << " --- " << endl;
        readInput(table, attempt);

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
    // 1) Generar clave secreta
    genSecretKey(table_t);
    // 2) Ejecutar el main loop
    int score = run(table_t);
    // 3) Mostrar puntaje final
    std::cout << "Tu puntaje es: " << score << "/" << MAX_ATTEMPTS << std::endl;
    return 0;
}
