#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){

	if(argc != 4){
		printf("\n Uso: %s <ip of server> <local port> <remote port> \n",argv[0]);
		return 1;
		}

	int eleccion;
	printf("\n\n\nEste sistema realiza el envío y recepción de cualquier tipo de archivo digital\npor medio de la capa de transporte del modelo OSI, conforme al protocolo TCP.\n");
	printf("\nDigite 1 para envío de archivos, u otro número para recepción de archivos:");
	scanf("%d", &eleccion);

	if(eleccion==1){
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

		if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){	//Creacion del socket
			fprintf(stderr,"\n Error : Could not create socket \n");
			return 1;
			}
		printf("\nSe ha creado el socket");

		serv_addr.sin_family = AF_INET;	//Protocolo TCP, Tipo de datos STREAM
		serv_addr.sin_port = htons(atoi(argv[3])); //Puerto remoto del servidor dentro del struct

		if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){ //Paso el IPv4 del servidor al struct
			printf("\n inet_pton error occured\n");
			return 1;
			}
			
		if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ //Conectar el cliente con el servidor
			printf("\n Error : Connect Failed \n");
			return 1;
			}

		printf("\nSe ha realizado la conexion con el servidor");

		file_to_send = fopen (lfile,"r");	//Abrir el archivo a enviar, ahora el file_To_send es el FD de lfile
		// y lo leo
		if(!file_to_send){
			printf("Error opening file\n");
			close(sockfd);
			return 1;
			}

		else{	//SI NO HUBO ERROR A LA HORA DE ABRIR EL DOCUMENTO EMPEZAMOS EL PROCESO DE ENVIO
			printf("\nSe ha abierto el archivo a enviar");	

			//INICIO EL CILO DE ENVIO: ENVIAREMOS DATOS UNO POR UNO
			while((ch=getc(file_to_send))!=EOF){	//obtengo el dato
				toSEND[0] = ch;	//lo almaceno en un array toSEND = {0}
				send(sockfd, toSEND, 1, 0);	//lo envio
				}
			}

		fclose(file_to_send);	
		close(sockfd);
		printf("\nSe ha cerrado el socket\n");
		return 0;

		}//fin del -if-

	else{
		printf("\nEL usuario desea hacer una recepción.");

		  
		char BUFFER2[20];	//Buffer que se utilizara para enviar cada uno de los datos del lfile.
		int socketfd, clientfd;	//socket del server y del cliente
		struct sockaddr_in serv_addr, client_addr; //struct para almacenar la info del socket del server y del cliente
		socklen_t client_addr_len;	//longitud del struct del socket del cliente
		FILE * recv_file;
		char *filename = "recibido";
		int x;

		if((socketfd = socket(AF_INET, SOCK_STREAM, 0))<0){	//CREACION DEL SOCKET DEL SERVIDOR
			fprintf(stderr,"Error: Couldn't create the socket");
			close(socketfd);
			return 1;
			}

		printf("\nSe ha creado el socket.");
			
		serv_addr.sin_family = AF_INET; //LLENMOS LA ESTRUCTURA DEL SOCKET DEL SERVIDOR
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //CON INFORMACION, como el IPv4, el PRotocolo y el puerto
		serv_addr.sin_port = htons(atoi(argv[2]));

		if( (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)))<0){	//Enlazamos el FD con el struct
			fprintf(stderr, "Error binding");
			close(socketfd);
			return 1;
			}

		printf("\nSe ha enlazado el socket");

		listen(socketfd, 5); //ponemos el socket del servidor a escuchar
				
		client_addr_len = sizeof(client_addr); //obtenemos el tamaño del struct del cliente

		if((clientfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len))<0){//aceptamos las conexiones entrantes
			printf("Cannot accept connection\n");
			close(socketfd);
			return 1;
			}

		printf("\nSe ha aceptado la conexion");

		recv_file = fopen ( filename,"w" );	//abrimos el archivo en el que el servidor va a escribir los datos que le vaya a mandar el cliente
		float tama = 0;
		//CICLO en el que el servidor va a recibir los datos del cliente uno por uno y los va a escribir en recv_file
		while(1){

			if( recv(clientfd, BUFFER2, 1, 0) != 0 ) {	//Recibe un byte del cliente
				fwrite (BUFFER2 , sizeof(BUFFER2[0]) , 1 , recv_file );	//lo escribe en recv_file
				BUFFER2[0] = 0;
				tama++;
				//printf("\nRecibido");
				}// if interno

			else {
				close(socketfd);
				fclose(recv_file);
				printf("Se recibio un archivo con tamaño de %f Kilobytes", (tama/1000));
				printf("\nSe ha cerrado el socket\n");
				return 1;	
				}

			}//while interno
		close(socketfd);
		return 0;
		}//fin del -else-


	return 0;

}//---------------------------Corchete final---------------------------
