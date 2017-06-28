#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMPALABRAS			30		// maximo tamaño de palabras
#define TAMANOLINEA	2048	//tamaño maximo de linea
#define SEPARADOR	" \t,.;\n"		//separador de palabras
#define ARCHIVO	"textobusq.txt"
#define SALIDA	"salida.txt"	//nombre del archivo de salida

#define	CRECIMIENTO		1000	//crecimiento si el array se llena
//prototipos
void shell_sort (char* array, int cantidad, char* ordenacion); // ordenacion shell 

int main (void)
{
	FILE*			fp;						// puntero al documento
	char			linea[TAMANOLINEA];		//array para almacenar las lineas que se leen
	int				i;						// variable temporal
	char*			palabras;				//array dinamico con las palabras
	int				numero_palabras;		// numero de palabras que se guardan
	int				numero_palabras_segmento; // si esta llega a crecimiento debemos agrandar el array
	
	int				numero_segmento;		// indica en cuál segemento de memoria estamos almacenando las palabras 
	char*			p;						// puntero de uso general 
	char			*pal, *pal2;			//puntero a las palabras
	char			ordenacion[5];			//array para guardar la ordenacion

	fp = fopen (ARCHIVO, "r");	//abrimos el documento
	
	if (fp == NULL) {
		fprintf (stderr, "%s: no se puede abrir el archivo\n", ARCHIVO);
		return (1);	//error
	}

	// creamos un array para guardar las palabras este crece dinamicamente

	palabras = (char*)malloc(CRECIMIENTO * TAMPALABRAS);	// array para guardar palabras
	pal = palabras;	// puntero palabras
	if (palabras == NULL) {
		fprintf(stderr, "No se pudo obtener la memoria para el array\n", ARCHIVO);
		return (1);
	}
	numero_palabras = 0;	// numero de palabras array
	numero_palabras_segmento = 0;	// numero de palabras segmento memoria
	numero_segmento = 1;		//se comienza en el primer segmento
	//se leen las palabras y se guardan en el segmento 
	
	while (fgets(linea, TAMANOLINEA, fp) != NULL) {
		linea[TAMANOLINEA-1] = '\0';	// nos aseguramos de que tiene null al final
		if (strlen (linea) < 2) {	// saltamos lineas en blanco 
			continue;
		}
		//extraemos palabra por palabra
		p = strtok (linea, SEPARADOR);	// extraemos la primera palabra  de la linea
		if (p == NULL) {
			continue;// linea con palabra no valida vamos a la siguiente 
		}
		while (p != NULL) {
			strcpy (pal, p);
			numero_palabras++;
			numero_palabras_segmento++;
			if (numero_palabras_segmento >= CRECIMIENTO) { // limite extendemos palabras
				numero_segmento++;	// agregamos segmento 
				
				pal2 = malloc(CRECIMIENTO * TAMPALABRAS * numero_segmento); // obtener espacio mas grande
				memcpy(pal2, palabras, CRECIMIENTO * TAMPALABRAS * (numero_segmento - 1)); //copia viejo en nuevo
				free(palabras);	// libarar viejo 
				palabras = pal2;	//ajuste puntero 
				pal = palabras + TAMPALABRAS * (numero_palabras - 1); // reacomodacion puntero
				numero_palabras_segmento = 0; // reiniciamos a cero el numero de palabras de este segmento 
			}
			p = strtok(NULL, SEPARADOR);	//extraemos la siguiente palabra de la linea
			pal += TAMPALABRAS;				// movemos el puntero a la siguiente palabra 
		}
	}
	fclose (fp); //cerramos el puntero que apunta al archivo 
	//pedimos el tipo de ordenacion
	while (1) {
		printf("Indique el tipo de ordenación ascendente(A - Z) o descendente(Z - A)\n");
		printf("Indique ascendente(a) o descendente(d): ");
		gets(ordenacion);
		if (ordenacion[0] != 'a' && ordenacion[0] != 'A' && ordenacion[0] != 'd' && ordenacion[0] != 'D') {
			printf("Opción incorrecta inserte 'a' para ascendente, 'd' para descendente.\n");
			continue;
		}
		else {
			break;
		}
	}
	shell_sort (palabras, numero_palabras, ordenacion);
	fp = fopen(SALIDA, "w");		//abrimos el archivo de salida 

	if (fp == NULL) {
		fprintf(stderr, "%s: no se puede abrir el archivo de salida\n", SALIDA);
		return (1);	// error
	}
	//guardamos la palabras ordenadas en el archivo de salida 
	pal = palabras;	//apuntamos de nuevo al puntero al inicio de la memoria de palabras 
	for (i = 0; i < numero_palabras; i++) {
		fprintf(fp, "%s\n", pal);
		pal += TAMPALABRAS;				//movemos el puntero a la siguiente palabra
	}
	fclose(fp); // cerramos el fichero 
	printf("Las palabras han sido almacenadas exitosamente en el archivo %s\n", SALIDA);
	free(palabras); //liberamos espacio
	return 0;	
}


// Esta rutina ordena utilizando el método de ordenación Shell


void shell_sort (char* palabras, int cantidad, char* ordenacion)
{
	char temp[TAMPALABRAS];	//array temporal para intercambiar palabras 
	int i, j, h;
	int ord;	// indica como se debe orenar 1 asc 0 desc
	char	*s1, *s2;

	for (h = cantidad; h /= 2;) {
		for (i = h; i < cantidad; i++) {
			s1 = palabras + TAMPALABRAS * i;
			strcpy(temp, s1);
			for (j = i; j >= h; j -= h) {
				s2 = palabras + TAMPALABRAS * (j - h);
				switch (ordenacion[0]) {
				case 'a':	//asc
					if (stricmp(temp, s2) > 0)
						ord = 0;
					else
						ord = 1;
					break;
				case 'd':	//desc
					if (stricmp(temp, s2) > 0)
						ord = 1;
					else
						ord = 0;
					break;
				}
				if (!ord) {
					break;
				}
				s1 = palabras + TAMPALABRAS * j;
				s2 = palabras + TAMPALABRAS * (j - h);
				strcpy(s1, s2);  // a[j] = a[j - h];
			}
			s1 = palabras + TAMPALABRAS * j;
			strcpy(s1, temp);  // a[j] = a[j - h];
		}
	}

}
