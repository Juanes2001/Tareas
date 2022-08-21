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
unsigned short number_of_days_bin = 0;
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






	while(1){

	}
}
