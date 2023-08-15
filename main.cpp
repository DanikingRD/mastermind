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
// Nota: Todos los arreglos con los que trabajamos son de tamaño KEY_LENGTH.
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
    // - `rand` genera un entero aleatorio ej 12346567
    // - `min` y `max` se utilizan para 'convertir' el numero que devuelva al rango [min ,max]
    // - ej: [76312317, 56312310, 35678513, 17352184] -> [6, 5, 3, 1]
    return min + (rand() % max);
}

/*
 * Verifica si un valor está presente en un arreglo dado.
 * @param arr El arreglo en el que se va a buscar.
 * @param target El valor que se desea buscar.
 * @return true si el valor existe en el arreglo, false en caso contrario.
 */
bool arrayContains(int *arr, int target)
{
    // 1) Iterar sobre cada posicion del arreglo
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        // 2) Verificar cada elemento
        if (arr[i] == target)
        {
            return true; // El valor fue encontrado en el arreglo.
        }
    }
    return false; // El valor no se encontró en el arreglo.
}

/*
 * Convierte un arreglo en una cadena formateada.
 * @tparam T El tipo de dato del arreglo, puede ser int o char.
 * @param array El arreglo que se va a formatear.
 * @return Una cadena de texto con el formato [valor1, valor2, ..., valorN].
 * Ej: [1, 2, 3, 4], [X, X, F, C]
 */
template <typename T>
string formatArray(T *array)
{
    // 1) Iniciar el string con un corchete abierto.
    string output = "[";
    // 2) Iterar sobre cada posicion
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        // 3) Si el tipo de dato es (char), simplemente añadelo al string
        if (typeid(T) == typeid(char))
        {
            output += array[i]; // Agrega el elemento (char) al arreglo formateado.
        }
        else
        {
            // 4) Para cualquier otro tipo de dato (en este caso `int`), conviertelo a un string
            output += to_string(array[i]);
        }
        // 5) Añade una "," por cada elemento del arreglo, a excepción del último. 
        if (i < KEY_LENGTH - 1)
        {
            output += ", ";
        }
    }
    // 5) Añade un corchete cerrado al final del string.
    output += "]";
    return output;
}

/*
 * Genera una clave con digitos entre [KEY_RANGE_MIN] y [KEY_RANGE_MAX].
 * Cada dígito de la clave es único, es decir, no se repite.
 * @param table Referencia a la tabla donde se guardará la clave, en la primera fila.
 */
void genSecretKey(table_t table)
{
    // 1) Referenciar al arreglo que va a almacenar la clave
    int *key = table[0]; // clave vacía.
    // 2) Iterar sobre cada posición.
    for (int col = 0; col < KEY_LENGTH; col++)
    {
        // 3) Generar un dígito aleatorio entre 1 y 6
        int randomDigit = genRange(KEY_RANGE_MIN, KEY_RANGE_MAX);
        // 4) Si el número ya existe en la clave, se vuelve a generar otro número.
        while (arrayContains(key, randomDigit))
        {
            randomDigit = genRange(KEY_RANGE_MIN, KEY_RANGE_MAX);
        }
        // 5) Colocar el dígito en su posición correspondiente.
        key[col] = randomDigit;
    }
}

/*
 * Genera los resultados del intento [attempt] y los almacena en el arreglo [result].
 * @param table Referencia a la tabla donde se almacenan los intentos del usuario.
 * @param result Referencia al arreglo donde se almacenarán las pistas, ej: [C, F, X, C]
 * @param attempt El número de intento actual. (entre 1 y 10)
 * @return true si el usuario adivinó la clave, false en caso contrario.
 */
bool getResults(table_t table, char *result, int attempt)
{
    // 1) Referenciar el arreglo que contiene la clave secreta.
    int *key = table[0];
    int hits = 0; // Contiene el numero de digito que el usuario ha adivinado.

    // 2) Iterar sobre c/u de las posiciones
    for (int index = 0; index < KEY_LENGTH; index++)
    {
        // 3) Obtener la entrada del usuario correspondiente
        int input = table[attempt][index];
        // 4) Verificar si dicha entrada se encuentra en la clave secreta.
        bool isPresent = arrayContains(key, input);
        // 5) Verificar si se encuentra en la posicion correcta
        // 6) Añadir el resultado al arreglo
        if (isPresent)
        {
            bool f2f = key[index] == input; // face-to-face
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
    // 7) Regresar si el usuario ha adivinado todos los digitos.
    // True indica que el usuario ha adivinado la clave.
    return hits == KEY_LENGTH;
}

/*
 * Verifica si una cadena contiene dígitos duplicados.
 * @param toCheck La cadena que se va a verificar.
 * @return true si la cadena contiene dígitos duplicados, false en caso contrario.
 */
bool stringContainsDuplicates(string *toCheck)
{
    // 1) Obtener la longitud de la cadena
    int len = toCheck->length();

    // complejidad: O(n)
    // 2) Declarar un arreglo de booleanos de tamaño [KEY_RANGE_MAX + 1] y llenarlo de `false`.
    bool digitsFound[KEY_RANGE_MAX + 1] = {false};
    // [false, false, false, false, false, false, false]

    // El algoritmo consiste en iterar sobre cada dígito de la cadena
    // e ir marcando en el arreglo de booleanos si dicho dígito ya se encuentra en la cadena.

    // Si el dígito ya se encuentra en el arreglo, entonces hay duplicados.
    // Por ejemplo:
    /*
     *
     * Digits found: [false, false, false, false, false, false, false]
     * Posiciones:   [ 0      1      2      3      4      5      6  ]
     *
     * Iteración 1:
     * - Dígito encontrado: `1`
     *   Digits found: [false, true, false, false, false, false, false]
     *   note que digitsFound[1] ahora es true.
     *
     * Iteración 2:
     * - Dígito: 4
     * Digits found: [false, true, false, false, true, false, false]
     *
     * Iteración 3:
     * - Dígito: 1
     *   Como digitsFound[1] ya era true, entonces el dígito 1 ya se encuentra en la cadena.
     */
    for (int i = 0; i < len; i++)
    {
        int digit = int((*toCheck)[i] - '0');
        // Si el digito ya se encuentra en el arreglo, entonces hay duplicados.
        if (digitsFound[digit])
        {
            return true;
        }
        digitsFound[digit] = true;
    }
    // Método alternativo
    // Complejidad: O(n^2)
    // for (int i = 0; i < len; i++)
    // {
    //     for (int j = i + 1; j < len; j++)
    //     {
    //         if (toCheck[i] == toCheck[j])
    //         {
    //             return true;
    //         }
    //     }
    // }

    return false;
}

/*
 * Transforma el texto ingresado por el usuario a un arreglo de enteros
 * (string -> int[]) almacenado en el parámetro [output].
 * @param input La entrada del usuario.
 * @param output El arreglo donde se almacena el nuevo arreglo de enteros.
 * @return Si todos los dígitos son válidos, retorna true, false en caso contrario.
 */
bool stringToArray(string input, int *output)
{
    if (input.length() != KEY_LENGTH)
    {
        return false;
    }
    // Pasamos una referencia al string 'input'
    // para no duplicar la cadena.
    if (stringContainsDuplicates(&input))
    {
        return false;
    }

    // 1) Iterar sobre todas las posiciones
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        // 2) Obtener el caracter.
        // Nota: - '0' es una forma de convertir el 'ASCII' de un caracter a su valor entero.
        char c = input[i] - '0';
        // 3) Verificar que cada dígito se encuentre entre 1 y 6.
        if (c < KEY_RANGE_MIN || c > KEY_RANGE_MAX)
        {
            return false; // No se pudo convertir string a -> int[]
        }
        output[i] = c;
    }

    // Si llegaste hasta aqui significa que no hubo ningún problema con la conversión
    // returna true.
    return true;
}

/*
 * Lee los digitos ingresado por el usuario
 * @param input El arreglo que va a almacenar la entrada del usuario.
 * @param attempt El número de intento actual.
 */
void readUserDigits(int *input, int attempt)
{
    // String temporal que contiene los digitos del usuario, ej: 1234
    // 1) Declarar el string que almacena la entrada del usuario
    string temp;
    // 2) Validar que cin pueda leer el string y que el string se pueda convertir a un arreglo de enteros.
    while (!(cin >> temp) || !stringToArray(temp, input))
    {
        // 3) De lo contrario, limpiar el buffer de entrada y volver a intentar.
        cout << "Valor inválido. Ingrese cuatro dígitos entre " << KEY_RANGE_MIN << " y " << KEY_RANGE_MAX << "." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Intento " << attempt << "/" << MAX_ATTEMPTS << ". Ingrese su clave: ";
    }
}

/*
 * Lee la entrada del usuario y la almacena en la tabla.
 * @param table Referencia a la tabla donde se almacenará la entrada.
 * @param attempt El número de intento actual.
 */
void readInput(table_t table, int attempt)
{
    cout << endl;
    /// 1) Imprimir el intento actual
    cout << "Intento " << attempt << "/" << MAX_ATTEMPTS << ". Ingrese su clave: ";
    // 2) Declarar arreglo temporal que almacena la entrada del usuario.
    int input[KEY_LENGTH];
    // 3) Leer la entrada del usuario y almacenarla en el arreglo temporal.
    readUserDigits(input, attempt);
    // 4) Iterar sobre cada posición del arreglo temporal y almacenarla
    // en su lugar correspondiente dentro de la tabla.
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        table[attempt][i] = input[i];
    }
}

/*
 * Muestra las tablas de resultados al usuario.
 * @param table Referencia a la tabla donde se almacenan los intentos del usuario.
 * @param attempt El número de intento actual.
 * @return true si el usuario adivinó la clave, false en caso contrario.
 */
bool displayResults(table_t table, int attempt)
{
    // 1) Indicar que significa cada letra.
    cout << "C = caliente, F = frío, X = no existe" << endl;
    cout << "   Intento   "
         << "\tEntrada    "
         << "\t Salida"
         << endl;

    // 2) Iterar sobre cada fila de la tabla.
    for (int row = 0; row < attempt; row++)
    {

        // 3) Obtener el resultado de la fila actual.
        char result[KEY_LENGTH]; // ej: [C, C, F, F]
        // Aqui usamos row + 1 porque la primera fila es la clave secreta.
        // y lo que queremos es obtener el resultado de la fila actual.
        int position = row + 1;
        bool youWon = getResults(table, result, position);
        string margin = "     "; // margen para alinear los resultados.

        // [intentos]
        cout << margin << "[" << position << "]"
             // [1, 2, 3, 4]
             << margin << formatArray<int>(table[position])
             // [C, C, F, F]
             << margin << formatArray<char>(result) << endl;

        if (youWon)
        {
            cout << "Ganaste!" << endl;
            return true;
        }
    }
    // Si llegaste hasta aqui significa que el usuario no ha adivinado la clave.
    return false;
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
    // Este es el loop principal del programa.
    // el orden de ejecución es:
    //   1 - Leer entrada del usuario.
    //   2 - Mostrar resultados.
    //   3 - Si el usuario adivinó la clave, terminar el programa.
    //   4 - De lo contrario, incrementar el número de intentos.
    //   5 - Repetir hasta que el usuario adivine la clave o se quede sin intentos.
    int attempt = 1;
    while (attempt <= MAX_ATTEMPTS)
    {
        // 1)
        readInput(table, attempt);
        // 2)
        bool youWon = displayResults(table, attempt);
        // 3)
        if (youWon)
        {
            break;
        }
        // 4)
        attempt++;
    }
    return (MAX_ATTEMPTS - attempt) + 1;
}

int main(int argc, char const *argv[])
{
    cout << "Bienvenido/a al Master Mind!" << endl;
    cout << "Adivina la clave de 4 digitos. Tienes " << MAX_ATTEMPTS << " intentos." << endl;

    srand(time(NULL)); // Inicializar el generador de números aleatorios.
    table_t table_t;
    // 1) Generar clave secreta
    genSecretKey(table_t);
    // 2) Ejecutar el main loop
    int score = run(table_t);
    // 3) Mostrar puntaje final
    cout << "Tu puntaje es: " << score << "/" << MAX_ATTEMPTS << endl;
    cout << "La clave secreta era: " << formatArray<int>(table_t[0]) << endl;
    return 0;
}
