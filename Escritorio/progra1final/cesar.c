#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 128

//prototipo de funciones
void encriptar(char frase[MAX], int numero);
void desencriptar(char frase[MAX], int numero);

//procedimiento principal
void main1()
{
	//variables necesarias
	int x;
	char cadena[MAX];
	
	//presentacion
	printf("Encriptacion/Desencriptacion:\n");
	printf("Metodo Cesar:\n");
	printf("------------------\n");
	printf("Introduce la cadena:      \n");
	
	//un especificador que admite texto:
	scanf("%128[^\n]",cadena);
	//vacia el buffer del teclado
	while (getchar()!='\n');
	//pedimos el numero
	printf("Introduce el numero: ");
	scanf("%d",&x);
	//vacia el buffer del teclado
	while (getchar()!='\n');
	//llamamos a la funcion encriptar
	encriptar(cadena, x);
	getchar();
	//llamamos la funcion desencriptar
	desencriptar(cadena, x);
	getchar();
	//salimos
	//return 0;
}

//funcion para desencriptar
void desencriptar(char frase[MAX], int numero)
{
	int i=0;
	while(frase[i]!='\0'){
		frase[i]=frase[i]-numero;
		i++;
	}
	printf("\nLa frase desencriptada es:\n%s\n",frase);
}

//funcion para encriptar
void encriptar(char frase[MAX], int numero)
{
	int i=0;
	char letra;
	while (frase[i]!='\0'){
		frase[i]=frase[i]+numero;
		i++;
	}
	printf("\nLa frase encriptada es:\n%s\n", frase);
}
