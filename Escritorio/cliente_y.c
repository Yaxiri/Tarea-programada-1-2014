#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


int main( int argc, char *argv[]){
	
	if(argc != 3){
        printf("\n Uso: %s <ip of server> <port> \n",argv[0]);
        return 1; 
    }
    
    int sockfd;									//FD del socket client
	struct sockaddr_in serv_addr;				//struct del socket del server
	FILE * file_to_send;						//(Puntero) Archivo a enviar
	int ch;
	char toSEND[1];
	int x;

	const char *lfile[200]; 					//Nombre del archivo a enviar "/home/yaxiri/Documentos/sockets/images.jpeg";
	printf("Digte el nombre del archivo:\n");
	scanf("%s",lfile);
	printf("%s",lfile);
	
	//  /home/yaxiri/Documentos/sockets/Hola.txt
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){				//Creacion del socket
       fprintf(stderr,"\n Error : Could not create socket \n");
       return 1;
		}
	 printf("\nSe ha creado el socket");
	 scanf("%d",&x);
	
	serv_addr.sin_family = AF_INET;									//Protocolo TCP, Tipo de datos STREAM
    serv_addr.sin_port = htons(atoi(argv[2])); 						//Puerto remoto del servidor dentro del struct
	
	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0){  		//Paso el IPv4 del servidor al struct
        printf("\n inet_pton error occured\n");
        return 1;
		}
    
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){   //Conectar el cliente con el servidor
       printf("\n Error : Connect Failed \n");
       return 1;
		}
	
	printf("\nSe ha realizado la conexion con el servidor");
	scanf("%d",&x);
	
	file_to_send = fopen (lfile,"r");							//Abrir el archivo a enviar, ahora el file_To_send es el FD de lfile
															   // y lo leo
	if(!file_to_send){   					
		 printf("Error opening file\n");
	     close(sockfd);
	     return 1;
			}
	
	
	else {				//SI NO HUBO ERROR A LA HORA DE ABRIR EL DOCUMENTO EMPEZAMOS EL PROCESO DE ENVIO
	printf("\nSe ha abierto el archivo a enviar");		
	scanf("%d",&x);
	
	//INICIO EL CILO DE ENVIO: ENVIAREMOS DATOS UNO POR UNO
				
			while((ch=getc(file_to_send))!=EOF){		//obtengo el dato
				toSEND[0] = ch;						//lo almaceno en un array  toSEND = {0}
				send(sockfd, toSEND, 1, 0);			//lo envio
				printf("\nEnviado");
				}//while
			}
			
	    fclose(file_to_send);	
	    close(sockfd);
		printf("\nSe ha cerrado el socket\n");
	    return 0;
	}//~~~
	
