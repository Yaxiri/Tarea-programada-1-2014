
// Programa de envio y recepción de mensajes utilizando TCP/IP



/********************Librerías utilizadas********************/

#include <stdio.h>                    // Contiene declaraciones utilizadas para entrada y salida de datos
#include <stdlib.h>                   // Libreria estándar en c, es utilizada en conversión de hileras
#include <unistd.h>                   // Libreria que permite realizar llamadas al sistema en este caso el fork ()
#include <string.h>                   // Contiene definiciones utilizadas para la manipulacion de cadenas de caracteres
#include <sys/types.h>                // Contiene definiciones de estructuras que son utilizadas por la funcion socket
#include <sys/socket.h>               // Contiene definiciones de estructuras que son utilizadas por la funcion socket
#include <netinet/in.h>               // Contiene definiciones de constantes y estructuras que son necesarias para el uso de direcciones de dominion en internet
#include <netdb.h>                    // Contiene definiciones de constantes y estructuras que son necesarias para el uso de direcciones de dominion en internet
#include <arpa/inet.h>


/********************Declaración de variable globales********************/

char ip[16];
char puerto[30];
char IP [16];
int existe;

/*********************Declaración de las funciones a utilizar******************/

int Menu();
int Comparar(char[],char[]);
int Escribir();
int ValidaContacto(char[]);
void copiar(char[], char[]);
int lee(char[]);
int muestra();
int sockets();
int enviar();

// Inicialización del main
int main () {
	printf("\033[35m Facebook Messenger                \n");
	Menu ();
	return 0;  }

//Funcion que invoca si ocurre un error
void error(const char *msg) 
{
	perror(msg);
	exit(0);
}

// Función que despliega el menú en pantalla, se encuentra en un ciclo que termina solamente cuando se desea salir de programa

int Menu(){
	printf("\033[0m                Menú Messenger                  \n 1)Envio de Mesajes\n 2)Lista de Contactos \n 3)Agregar Contacto \n 4)Enviar un archivo \nDigite 0 para salir del programa\n");
	int salir=0;
	int opcion;
	while(salir==0){
		opcion=-1;
		fflush(stdin);
		scanf("%d",&opcion);
			
		if (opcion==1){
			system("clear");  // Se llama a la función sockets para crear la comunicación entre los nodos
			sockets();
			Menu();}
		else if (opcion==2){
			system("clear");
			muestra();        // Se llama a la función mostrar para desplegar la lista de contactos que se posee
			Menu();}
		else if (opcion==3){
			system("clear");
			Escribir();      // En esta opción permite al usuario agregar amigos
			Menu();}
		else if (opcion==4){
			system("clear");
			enviar();   // En esta pción se le permite a los usuarios el envio de archivos 
			Menu();}
		else exit(0);
		}
	return 0;
}

// Función donde se da la comunicación entre nodos

int sockets()
{	
	char Usuario [30];	
	printf("Digite el usuario \n");          // Se le solicita al usuario el username del contacto con el que desea cominicarse
	scanf("%s",Usuario);			
		
	lee(Usuario);                            // Se llama a la función leer para validar que el usuario existe y a su vez nos devuelva la ip
											 // y el puerto asociado
	
	int socket1, nuevoSocket, numPuerto, mensaje,pid;   // Se declaran las variables necesarias para el socket
	socklen_t clilen;
	char buffer[1024];                                  // Tamaño del mensaje
	
	numPuerto = atoi(puerto);                           // Se convierte el numero de puerto a entero
	
	pid = fork();                                       // Se guarda el id de proceso
	
	if (pid < 0){										// Error si el fork devuelve un -1 
		error("El fork no se realizo de forma correcta\n");
	}
	
	//Si el id es igual a 0 se cierra el socket y se abre una nueva comunicacion
	if (pid == 0)                                       // La parte del sockect que envía
	{
				
		struct sockaddr_in dir_servidor;                // Se declaran las estructuras donde se 
		struct hostent *servidor;                       // guarda la ip del servidor
		
		while (1){                                      // Se crea un while infinito
			
			sleep (1);
			//Se crea el socket
			socket1 = socket(AF_INET, SOCK_STREAM, 0);
			
			//Muestra error en caso de no poder crear el socket
			if (socket1 < 0) error("El socket no se ha podido crear\n");
			
			//Obtiene la direccion ip del servidor
			if (existe == 1) {     // Si el usuario se encontraba en el archivo se llama a copiar para que elimine el \0
				copiar (ip, IP);
				servidor = gethostbyname(IP);
				}
			else servidor = gethostbyname(ip);
			
			//Si el servidor es nulo ,muestra el error 
			if (servidor == NULL)
			{
				fprintf(stderr,"Error, no se encuentra con quien conectar \n");
				exit(0);
			}
			
			//Rellena con 0
			bzero((char *) &dir_servidor, sizeof(dir_servidor));
			
			dir_servidor.sin_family = AF_INET;                  //Protocolo TCP
			
			//Copia la direccion
			bcopy((char *)servidor->h_addr,(char*)&dir_servidor.sin_addr.s_addr,servidor->h_length);
			
			dir_servidor.sin_port = htons(numPuerto);           //Representa el numero de puerto
			
			//Verifica que pueda establecer la conexion
			if (connect(socket1,(struct sockaddr *)&dir_servidor,sizeof(dir_servidor)) < 0) 
				error("Error, en crear la conexión");
			
			printf("\033[34m \n Yo: ");  // Es el printf que va indicar que le corresponde escribir
			
			//Relleno con ceros
			bzero(buffer,1024);
			
			//Lee el mensaje desde la entrada estandar
			fgets(buffer,1023,stdin);
			
						
			//Solicita escribir un mensaje
			mensaje = write(socket1,buffer,strlen(buffer));
			//Si se produce un error al escribir el msj, se muestra el error
			if (mensaje < 0)
				error("Error, no se pudo escribir el mensaje");
			
			if (strcmp(buffer,"salir\n")==0){    // Si el usuario digita "salir" la conversación se da por finalizada
				printf ("\033[31m Conversacion finalizada\n");
				sleep(2);
				system("clear");  // Limpia la pantalla
				break;
			}
			
			//Relleno con ceros
			bzero(buffer,1024);
			//Lee el mensaje
			mensaje = read(socket1,buffer,1023);
			
			//Si se produce un error al leer el msj, se muestra el error
			if (mensaje < 0)
				error("Error, no se puede leer el mensaje");
				
			//printf("%s", buffer);
			
			close(socket1);
	
		}
		return 0;
	}
		
	else {  //Servidor
	
		//printf("Entre al servidor");
		
		struct sockaddr_in dir_servidor, dir_cliente;
		socket1 = socket(AF_INET, SOCK_STREAM, 0);
		
		//validacion en caso de no se pueda establecer el socket
		if (socket1 < 0)
			error("Error, no se puede crear el socket");

		//Establece los valores del bufer a 0
		bzero((char *) &dir_servidor, sizeof(dir_servidor));
		//numPuerto obtine el numero de puerto ingresado por el usuario
		
		dir_servidor.sin_family = AF_INET;                  //Protocolo TCP
		dir_servidor.sin_addr.s_addr = INADDR_ANY;          //Representa la direccion ip
		dir_servidor.sin_port = htons(numPuerto);           //Representa el puerto

		//Verifica la conexion
		if (bind(socket1, (struct sockaddr *) &dir_servidor,sizeof(dir_servidor)) < 0)
			error("Error, no se puede crear la conexión");
		
		while(1){
			
			listen(socket1,5);
	
			//Tamaño de bytes que ocupa la direccion cliente
			clilen = sizeof(dir_cliente);
			
			//Se cra una nueva comunicación
			nuevoSocket = accept(socket1,(struct sockaddr *) &dir_cliente, &clilen);
			if (nuevoSocket < 0)
			error("Error, no se puede aceptar la conexión");
						
			bzero(buffer,256);
			sleep (2);
			//Lee el mensaje
			mensaje = read(nuevoSocket,buffer,255);
			//Muestra el error en caso de que se produzca
			if (mensaje < 0)
				error("Error, al leer el mensaje enviado");
			
			if (strcmp(buffer,"salir\n")==0){
				printf ("\033[33m Conversacion finalizada\n");
				sleep(2);
				Menu();
				sleep(2);
			}
			
			else{
				//Muestra el mensaje recibido
				printf("\033[32m \n %s:  %s\n",Usuario, buffer); //Muestra el mensaje enviado
					}
				
				close(nuevoSocket);
				}	
	}
	return 0;
}

//Funcion para mostrar todos los contactos
int muestra( ){
	FILE *archivo;//Es un buffer que almacena la dirección del archivo
    char caracteres[30];
    archivo = fopen("Contactos.txt","r");//Almacena la dirección del archivo
    
    if (archivo == NULL)  {
		 printf("\033[36m No existen amigos agregados\n");
		 Menu();
	 }
     
    while (feof(archivo) == 0) {     //Mientras que no sea el final del documento
        fgets(caracteres,30,archivo);//Toma caracter por caracter del archivo y lo almacena en caracteres
        
        if(feof(archivo) == 0)
		printf("\033[33m %s",caracteres);       
        }
        fclose(archivo);//Cierra el documento
        
        return 0;
	 
	 }
 
// Función para devolver la ip y el puerto asociado a un usuario
	int lee(char usuario[]){
		FILE *archivo;
		char caracteres[30];
              
		int i=0;
        archivo = fopen("Contactos.txt","r");  // Se trata de abrir un archivo
        			
			if (archivo != NULL) {  // Si el archivo existe:
			existe = 0;
			while (feof(archivo) == 0)  // Recorre el archivo buscando el usuario solicitado
			{
					fgets(caracteres,30,archivo);
					if(((Comparar(usuario,caracteres))==1)||(i>0)){
						if(i==1)strcpy(ip,caracteres);           // Si se encuentra se copia la información correspondiente
						if(i==2){strcpy(puerto,caracteres);      // el las variables globales para poder ser usadas por el socket
						i=0;
						existe = 1;   // Uso de manejo de IP y de usuarios no agregados
						break;}
						
						i++;
						}
						
			}
		
	}
        if (existe != 1) {  // Si no existe permite al usuario comunicarse con el contacto solo si conoce su ip
        printf("\033[33m El contacto no se encuentra, \n si conoce su IP y puerto digite 1, \n si no lo conoce digite 0\n");
        int opcion;
        scanf("%i", &opcion);
        if (opcion == 1){ // Si el usuario conoce el puerto y la ip entonces puede conectarse
 			printf("Escriba el ip:");
			scanf(" %s",ip);
			printf("Escriba el puerto:");
			scanf(" %s",puerto);
        }
        else { // Si no lo envia al menu principal
			system("clear");
			Menu ();
		}}
        
        if (archivo != NULL) 
        fclose(archivo);
        return 0;
	 
	 }

// Función que agrega los amigos al txt.
	
	int Escribir( ){
		FILE * archivo;
		char* users = "Contactos.txt";//Apunta al archivo en memoria
		
		// Se declaran variables
		char user[30];
		char ip[30];
		char puerto[30];
		
		archivo = fopen(users,"a");//agrega al final del archivo
		if(archivo==NULL) return -1;
			
		
		printf("Escriba el Nombre de usuario:");
		scanf(" %[^\n]",user); //Lee del teclado cosas con espacios hasta presionar enter
		if(!ValidaContacto(user))Menu();
		printf("Escriba el ip:");
		scanf(" %[^\n]",ip);
		printf("Escriba el puerto:");
		scanf(" %[^\n]",puerto);
		
		fprintf(archivo,"%s\n",user);
		fprintf(archivo,"%s\n",ip);
		fprintf(archivo,"%s\n",puerto);
		fclose(archivo);
		
	return 0;
		
		}
// Fumción para enviar archivos

int enviar()

{
printf("\nEL usuario desea hacer un envio.");


int sockfd;	//FD del socket client
struct sockaddr_in serv_addr;	//struct del socket del server
FILE * file_to_send;	//(Puntero) Archivo a enviar
int ch;
char toSEND[1];
int x;
const char *lfile[200]; //Nombre del archivo a enviar "/home/yaxiri/Documentos/sockets/images.jpeg";
printf("Digite el nombre del archivo:\n");
scanf("%s",&lfile);

file_to_send = fopen (lfile,"r");	//Abrir el archivo a enviar, ahora el file_To_send es el FD de lfile
// y lo leo
if(!file_to_send){
printf("Error opening file\n");
close(sockfd);
return 1;
}

else{	//SI NO HUBO ERROR A LA HORA DE ABRIR EL DOCUMENTO EMPEZAMOS EL PROCESO DE ENVIO
printf("\nSe ha abierto el archivo a enviar");	
scanf("%d",&x);

//INICIO EL CILO DE ENVIO: ENVIAREMOS DATOS UNO POR UNO
while((ch=getc(file_to_send))!=EOF){	//obtengo el dato
toSEND[0] = ch;	//lo almaceno en un array toSEND = {0}
send(sockfd, toSEND, 1, 0);	//lo envio
}
}

}
		
// Función auxiliar de la función lee en la cual se compara si el usuario ya existe

int Comparar(char arr1[],char arr2[]){
		int x=strlen(arr1);
		int y=strlen(arr2);
		int cont=0; 
		if(x!=y-1){
			return 0; // No se encuentra
		}
		else{
			while(cont+1!=x){
				if (arr1[cont]!=arr2[cont]){
					//No se encuentra
				return 0;
								
				}cont++;
			} 
			return 1; // El contacto se encuentra
					
		}
	}
	
	
	//Método para validar si el contacto que se desea agregar ya esta registrado 
	int ValidaContacto(char usuario[]){
		
		FILE *archivo;
		
        char caracteres[30];
        archivo = fopen("Contactos.txt","r");
 
        if (archivo == NULL)
                exit(1);
                
        while (feof(archivo) == 0) // Se recorre el archivo apra encontrar el usuario
        {
                fgets(caracteres,30,archivo);
                if((Comparar(usuario,caracteres))==1){ // Si en la lista de contavtos del txt se encuentra, muestra un mensaje en pantalla
					printf("\nEl contacto ya se encuentra registrado. \n \n");
					return 0;
					}
					}
        
        fclose(archivo);
        return 1; // Si no se encuentra retorna 1 para indicar que se puede digitar el username
		}
	
		
	// Copia la ip para eliminar el \0 
	void copiar(char ar1[], char ar2[]){ // Se reciben dos arreglos, el primer parametro se copia en el segundo
	int x=strlen(ar1); // Se calcula el largo del arreglo para usarlo en el termino de parada del usuario
	int i=0;
	while(i<x-1){
		 ar2[i] = ar1[i]; //Se hace la copia de los valores
		 i++;
		 }
	}
	
