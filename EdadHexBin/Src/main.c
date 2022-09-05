/*
 * main.c
 *
 *  Created on: Aug 20, 2022
 *      Author: JUAN ESTEBAN RODRIGUEZ OCHOA
 *      correo: jrodriguezoc@unal.edu.co
 *
 */

#include <stdint.h>

// Mi fecha de nacimiento es el 9 de marzo del 2001

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
unsigned int bin1 = 0;
unsigned int bin2 = 0;
unsigned int mask_hex = 0;


int main(void)
{

	/* Asignacion del numero de años eficientes antes del dia 17 de agosto.
	 * supuse que naci al medio dia el dia 9 e marzo del 2001, asi
	 * lograre que el numero de dias me quede mas sencillo
	 */
	number_of_years = 21;

	/**
	 * Punto 2--> calculo del numero de dias transcurridos
	 */
	/* Posterior al numero de dias efectivo hasta el 9 de marzo de este año,
	 * empezamos a sumar el numero de dias trancurridos hasta el 17 de agosto al medio dia
	 * la convencion que hicimos nos permite contar, ya estando en el mes de agosto, siete
	 * dias exactos.
	 */
	number_of_days = number_of_years*365 + 22+ 30 + 31 + 30 + 31 + 16;
	/**
	 * Punto 3--> calculo del numero de horas transcurridos
	 */
	// usamos el numero de dias transcurridos y simplemente multimplicamos por 245 para obtener el numero de horas
	number_of_hours = number_of_days*24 + 12;
	/**
	 * Punto 4 --> calculo del numero de segundos transcurridos
	 */
	/*
	 * Por ultimo simplemente hacemos la multiplicacion de el numero de segundos que tiene un dia
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
	number_of_days_bin = 0b1111010010001;
	number_of_hours_bin = 0b101101110110100100;
	//Por ultimo asignamos en el sistema hexagesimal el valor propio de numero de segundos
	number_of_seconds_hex= 0x284cd240;
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
	 * int, es decir, tenemos para este valor 32 bits. Antes de usar la operacion tenemos  0b1111010010001
	 */
	number_of_days_bin = number_of_days_bin << 1;
	/*
	 *Luego de aplicar el shift una vez, vemos que obtenemos el siguiente resultado ---> 0b11110100100010 el cual corresponde al valor decimal
	 *15650 que corresponde al doble del valor inicial.
	 */

	number_of_days_bin = number_of_days_bin << 1;
	/*
	 * Al aplicar de nuevo la operacion shift sobre el resultado, veremos que 0b11110100100010 se convierte a
	 * ---> 0b111101001000100 que corresponde en valor decimal a 31300, de nuevo, vemos que el numero obtenido es el doble
	 * del valor anterior.
	 */
	number_of_days_bin = number_of_days_bin << 2;

	/*
	 * Por ultimo, ejecutamos la funcion Left-shift dos veces seguidas mas. A partir del ultimo resultado
	 * 0b11110100100010 tendremos el siguiente resultado ---> 0b11110100100010000 que corresponde al valor decimal  positivo 125200,
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
	 *Luego de aplicar el shift una vez, vemos que obtenemos el siguiente resultado ---> 0b10110111011010010 el cual corresponde al valor decimal
	 *93906, especificamente la mitad del valor inicial.
	 */

	number_of_hours_bin = number_of_hours_bin >> 1;
	/*
	 * Al aplicar de nuevo la operacion shift sobre el resultado, veremos que 0b10110111011010010 se convierte a
	 * ---> 0b1011011101101001 que corresponde en valor decimal a 46953, especificamente la mitad del valor anterior.
	 */
	number_of_hours_bin = number_of_hours_bin >> 2;

	/*
	 * Por ultimo, ejecutamos la funcion Right-shift 2 veces seguidas mas. A partir del ultimo resultado
	 * 0b1011011101101001 tendremos el siguiente resultado ---> 0b10110111011010 que corresponde al valor decimal 11738, especificamente un cuarto del valor encontrado.
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
	 * 0b1111010010001, el cual su negacion cambia a 0b11111111111111111110000101101110.
	 */

	number_of_days_bin = ~number_of_days_bin;

	/*
	 * Luego agregamos 0b1 binario a el valor obtenido.
	 */
	number_of_days_bin = number_of_days_bin + 0b1;
	number_of_days_bin = number_of_days_bin + number_of_days;
	/*
	 *Finalmente sumamos el valor obtenido, posterior a sumar un uno binario, con el valor original del número
	 * de dias y obtenemos como resultado 0.
	 */
	/*
	 * La explicacion de porque ocurre esto es a siguiente, entedermos el proceso mas facilmente si usamos la
	 * propiedad conmutativa de la suma, independientemente de sistema numerico en el que nos encontremos.
	 * Si sumamos primero el valor negado del numero de dias con su no negado, obtendremos 32 bits en verdadero, es decir,
	 * unos binarios. posteriormente si sumamos 1 a el valor maximo que puede alcanzarse, veremos que el conteo se reinicia,
	 * ya que el formato no le es posible mostrar el valor siguiente al maximo.
	 *
	 * Veremos que para cualquier valor binario esto se cumple
	 */

	bin1 = 0b10111101110101011100000110101010;
	bin2= ~bin1 + bin1;
	bin2= bin2 + 0b1;
	//Al ejecutar vemos que efectivamente el numero creado de forma aleatoria (bin1) se transfoma en 0 binario
	//al aplicar el procedimiento.

	/**
	 * Punto 9--> Aplicación de mascaras sobre números hexagesimales
	 */

	/*
	 * Como el numero en la posicion 4 corresponde a c osea el valor 12, entonces aplicaremos una mascara sobre el numero
	 * para obtener el valor de las posiciones 1 y 5.
	 */
	// Teniendo el valor binario del numero de segundos como referencia 0b00101000010011010111101100000000, construiremos la mascara en binario
	// primero y luego visualizamos tal mascara en hexagesimal.
	mask_hex= 0b00000000111100000000000011110000;
	/*
	 * Vemos entonces que la mascara que contruimos cuenta con los valores tales que se cancelen las posiciones 0,2,3,4,6 y 7 y se mantengan intactas las posiciones
	 * 1 y 5 luego de que a continuacion realizemos la operacion AND (&); asi lograremos salvaguardar los valores hexagesimales propios de esas posiciones.
	 */

	number_of_seconds_hex = mask_hex & number_of_seconds_hex ;

	//Efectivamente logramos hacerle mascara a los valores pedidos, asi la mascara que usamos en forma hexagesimal sera la siguiente

	mask_hex = 0xf000f0;

	/**
	 * Punto 10 ---> Aplicacion de la funcion AND y el uso de mascaras, segunda parte.
	 */
	// comenzamos dandole el valor del numero de segundos a la variable modificable.
	number_of_seconds_hex= number_of_seconds;

	/*
	 * Luego, usando la mascara 0x040 que corresponde en binario al valor 0b01000000 la aplicamops sobre el numero de segundos con la operacion
	 * AND.
	 */
	mask_hex = 0x040;

	number_of_seconds_hex = number_of_seconds_hex & mask_hex;

	// El resultado es 1, por lo que aplicaremos una mascara a el numero de segundos para que sea borrada solo la posicion 6.

	number_of_seconds_hex= number_of_seconds;
	mask_hex= 0b11110000111111111111111111111111;

	number_of_seconds_hex = number_of_seconds_hex & mask_hex;

	//Efectivamente eliminamos la sexta posicion que correspondia al valor de 8 en el numero de segundos, obtenemos 0x204cd240

	/**
	 * Punto 11--->Operacion (!)
	 */

	//Procederemos a averiguar que funcion cumple el operador signo de admiracion sobre la variable del numero de segundos.
	number_of_seconds_hex= number_of_seconds;
	number_of_seconds_hex= !number_of_seconds_hex;
	number_of_seconds_hex= !number_of_seconds_hex;
	number_of_seconds_hex= !number_of_seconds_hex;
	number_of_hours_bin = !number_of_hours_bin;
	bin1 =  !bin1 ;
	mask_hex =  !mask_hex;

	/*
	 * Despues de muchas pruebas, vemos que la operacion lo que hace es anular a la variable sobre la que se aplica,
	 * una segunda ejecucion hace que la variable ya anulada se convierta en un 1.
	 */

}
