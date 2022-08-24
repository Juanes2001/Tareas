/*
 * main.c
 *
 *  Created on: Aug 20, 2022
 *      Author: JUAN ESTEBAN
 */

#include <stdint.h>

/**
 * Punto 1--> creacion de las variables
 */

// asignamos 8 bits a la variable de años ya que solo almacenara hasta 21 años que es mi edad
unsigned char number_of_years = 0;
// asignamos 16 bits a la variable de dias ya que solo almacenara del orden de 8000 dias
unsigned short number_of_days = 0;
// asignamos 32 bits a la variable de segundos ya que solo almacenara del orden de 700 millones de segundos
unsigned int number_of_seconds = 0;
// asignamos 32 bits a la variable de horas ya que solo almacenara del orden de 700 millones de segundos
unsigned int number_of_hours = 0;
//Las variables creadas a partir de aca seran de uso a modo de replica para distintas representaciones
//de numeros decimales en binarios y hexagesimales; tambien para usarlas en operaciones logicas.
unsigned int number_of_days_bin = 0;
unsigned int number_of_hours_bin = 0;
unsigned int number_of_seconds_hex = 0;

int main(void)
{

	/* asignacion del numero de años eficientes antes del dia 17 de agosto.
	 * supuse que naci al medio dia el dia 9 e marzo del 2001, asi
	 * lograre que el numero de dias me quede mas sencillo
	 */
	number_of_years = 21;

	/**
	 * Punto 2--> calculo del numero de dias transcurridos
	 */
	/* posterior al numero de dias efectivo hasta el 9 de marzo de este año,
	 * empezamos a sumar el numero de dias trancurridos hasta el 17 de agosto al medio dia
	 * la convencion que hicimos nos permite contar, ya estando en el mes de agosto, siete
	 * dias exactos.
	 */
	number_of_days = number_of_years*365 + 22+ 30 + 31 + 30 + 31 + 17;
	/**
	 * Punto 3--> calculo del numero de horas transcurridos
	 */
	// usamos el numero de dias transcurridos y simplemente multimplicamos por 245 para obtener el numero de horas
	number_of_hours = number_of_days*24;
	/**
	 * Punto 4 --> calculo del numero de segundos transcurridos
	 */
	/*
	 * por ultimo simplemente hacemos la multiplicacion de el numero de segundos que tiene un dia
	 * por el resultado del numero de dias y asi obtenemos el numero de segundos que han pasado.
	 */
	number_of_seconds = number_of_hours*3600;


	/**
	 * Punto 5--> Asignacion de el valor binario a numero de dias y numero de horas, y
	 * representacion hexagesimal a la variable numero de segundos
	 */

	// Lo que hago es definir de la variable nueva creada al inicio, del codigo, y simplemente asignarle ya dentro del main
	// el valor correspondiente al numero de dias en binario. Esta conversion se puede ver directamente en la tabla de variables
	// con la opcion que entrega el entorno de cambiar el tipo de representacion numerica.
	number_of_days_bin = 0b1111010010010;
	number_of_hours_bin = 0b101101110110110000;
	//Por ultimo asignamos en el sistema hexagesimal el valor propio de numero de segundos
	number_of_seconds_hex= 0x284d7b00;
	/*
	 * El tipo de variable que adquiere cada una de las binarias y la hexagesimal es la misma definida para el caso decimal
	 Ya que para representar el mismo numero en otra representacion debe de estar dentro del rango posible de valores que tiene
	permitido mostrar
	 */
	/**
	 * Punto 6--> Aplicacion de la operacion bitwise left-shift
	 */

	/*
	 * Aplicamos una primera vez sobre el valor númerico binario del numero de dias transcurridos en formato
	 * int, es decir, tenemos para este valor 32 bits. Antes de usar la operacion tenemos  0b1111010010010
	 */
	number_of_days_bin = number_of_days_bin << 1;
	/*
	 *Luego de aplicar el shift una vez, vemos que obtenemos el siguiente resultado ---> 0b11110100100100 el cual corresponde al valor decimal
	 *15652 que corresponde al doble del valor inicial. (Ejecutar el codigo hasta este paso y ver efectivamente que si es verdad)
	 */

	number_of_days_bin = number_of_days_bin << 1;
	/*
	 * Al aplicar de nuevo la operacion shift sobre el resultado, veremos que 0b11110100100100 se convierte a
	 * ---> 0b111101001001000 que corresponde en valor decimal a 31304, de nuevo, vemos que el numero obtenido es el doble
	 * del valor anterior. (Ejecutar el codigo hasta este paso y ver efectivamente que si es verdad)
	 */
	number_of_days_bin = number_of_days_bin << 4;

	/*
	 * Por ultimo, ejecutamos la funcion Left-shift cuatro veces seguidas. A partir del ultimo resultado
	 * 0b111101001001000 tendremos el siguiente resultado ---> 0b1111010010010000000 que corresponde al valor decimal  positivo 500864,
	 * 4 veces el resultado anterior en decimal.
	 */

	/**
	 * Punto 7 --> Aplicacion de la operacion bitwise right-shift
	 */

	/*
	 * Aplicamos una primera vez sobre el valor numerico binario del numero de horas transcurridos en formato
	 * int, es decir, tenemos para este valor 32 bits. Antes de usar la operacion tenemos  0b101101110110110000
	 */
	number_of_hours_bin = number_of_hours_bin >> 1;
	/*
	 *Luego de aplicar el shift una vez, vemos que obtenemos el siguiente resultado ---> 0b10110111011011000 el cual corresponde al valor decimal
	 *93912, especificamente la mitad del valor inicial. (Ejecutar el codigo hasta este paso y ver efectivamente que si es verdad)
	 */

	number_of_hours_bin = number_of_hours_bin >> 1;
	/*
	 * Al aplicar de nuevo la operacion shift sobre el resultado, veremos que 0b10110111011011000 se convierte a
	 * ---> 0b1011011101101100 que corresponde en valor decimal a 46956, especificamente la mitad del valor anterior. (Ejecutar el codigo hasta este paso y ver efectivamente que si es verdad)
	 */
	number_of_hours_bin = number_of_hours_bin >> 4;

	/*
	 * Por ultimo, ejecutamos la funcion Right-shift cuatro veces seguidas. A partir del ultimo resultado
	 * 0b1011011101101100 tendremos el siguiente resultado ---> 0b101101110110 que corresponde al valor decimal 2934, especificamente un cuarto del valor encontrado.
	 * (Ejecutar el codigo hasta este paso y ver efectivamente que si es verdad)
	 */

	/**
	 * Punto 8 --> Operaciones logicas de adicion y NOT.
	 */

	/*
	 * Le daremos de nuevo el valor inicial del numero de dias
	 */
	number_of_days_bin = number_of_days;
	/*
	 * Luego aplicaremos la funcion not (~) a el numero de dias, inicialmente tendremos el valor binario
	 * 0b1111010010010, el cual su negacion cambia a 0b1110000101101101
	 */

	number_of_days_bin = ~number_of_days_bin;

	/*
	 * Luego agregamos 0b1 binario a el valor obtenido.
	 */
	number_of_days_bin = number_of_days_bin + 0b1;
	//number_of_days_bin = number_of_days_bin + number_of_days;
	/*
	 *
	 */



}
