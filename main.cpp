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

/*
 * Formatea un arreglo de enteros.
 * T El tipo de dato del arreglo, puede ser int o char.
 * Una cadena de texto con el formato [1, 2, 3, n]
 */
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
 * Genera los resultados del intento [attempt] y los almacena en el arreglo [result].
 *
 * @param table Referencia a la tabla donde se almacenan los intentos del usuario.
 * @param result Referencia al arreglo donde se almacenarán las pistas.
 * @param attempt El número de intento actual.
 * @return true si el usuario adivinó la clave, false en caso contrario.
 */
bool getResults(table_t table, char *result, int attempt)
{
    int *key = table[0];
    int hits = 0;
    for (int index = 0; index < KEY_LENGTH; index++)
    {
        int input = table[attempt][index]; // cada digito
        bool isPresent = arrayContains(key, input);
        if (isPresent)
        {
            bool f2f = key[index] == table[attempt][index];
            if (f2f)
            {
                result[index] = 'C';
                hits++;
            }
            else
            {
                result[index] = 'F';
            }
        }
        else
        {
            result[index] = 'X';
        }
    }
    return hits == KEY_LENGTH;
}

/*
 * Verifica si el texto ingresado por el usuario es válido,
 * es decir, si está dentro del rango de números de la clave.
 * Luego lo almacena en el arreglo [output].
 * @param input El dígito ingresado por el usuario.
 * @return Si todos los dígitos son válidos, retorna true, false en caso contrario.
 */
bool intoIntArray(string input, int *output)
{
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        char c = input[i] - '0';
        if (c < KEY_RANGE_MIN || c > KEY_RANGE_MAX)
        {
            return false;
        }
        output[i] = c;
    }
    return true;
}

/*
 * Lee un dígito ingresado por el usuario, y lo valida.
 * @param input Referencia al entero donde se almacenará el dígito ingresado.
 * @param attempt El número de intento actual.
 * @param index El índice del dígito actual de [KEY_LENGTH].
 */
void readChars(int *input, int attempt)
{
    string inputStr;
    while (!(cin >> inputStr) || !intoIntArray(inputStr, input))
    {
        cout << "Valor inválido. Ingrese cuatro dígitos entre " << KEY_RANGE_MIN << " y " << KEY_RANGE_MAX << endl;
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

/*
 * Lee la entrada del usuario y la almacena en la tabla.
 * @param table Referencia a la tabla donde se almacenará la entrada.
 * @param attempt El número de intento actual.
 */
void readInput(table_t table, int attempt)
{
    int input[KEY_LENGTH];
    readChars(input, attempt);
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        table[attempt][i] = input[i];
    }
}

/*
 * Muestra los intentos del usuario y las pistas.
 * Además, verifica si el usuario adivinó la clave.
 * @param table Referencia a la tabla donde se almacenan los intentos del usuario.
 * @param attempt El número de intento actual.
 */
bool displayResults(table_t table, int attempt)
{
    cout << "C = caliente, F = frío, X = no existe" << endl;
    cout << "   Intento   "
         << "\tEntrada    "
         << "\t Salida"
         << endl;

    for (int row = 0; row < attempt; row++)
    {
        char result[KEY_LENGTH];
        bool hitAll = getResults(table, result, row + 1);
        string margin = "     ";
        cout << margin << "[" << row + 1 << "]" << margin << formatArray<int>(table[row + 1]) << margin << formatArray<char>(result) << endl;
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

        bool shouldContinue = displayResults(table, attempt);
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
