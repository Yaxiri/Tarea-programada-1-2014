#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>




int main(int argc, char *argv[]){
    
    if(argc != 2){
        printf("\n Uso: %s <port> \n",argv[0]);
        return 1;
		}
    
/*~~~~~~~~~~Declaracion de variables~~~~~~~~~~*/
  
    char BUFFER2[4096];	//Buffer que se utilizara para enviar cada uno de los datos del lfile.		
    int socketfd, clientfd;	//socket del server y del cliente
    struct sockaddr_in serv_addr, client_addr;  		//struct para almacenar la info del socket del server y del cliente
	socklen_t client_addr_len;				//longitud del struct del socket del cliente
    FILE * recv_file;
    char *filename = "recibido.jpeg";
	int x; 
	    
    if((socketfd = socket(AF_INET, SOCK_STREAM, 0))<0){				//CREACION DEL SOCKET DEL SERVIDOR	
		fprintf(stderr,"Error: Couldn't create the socket");
		close(socketfd);
		return 1;
		}
		
	printf("\nSe ha creado el socket.");
    scanf("%d",&x);
    
    serv_addr.sin_family = AF_INET;                          //LLENMOS LA ESTRUCTURA DEL SOCKET DEL SERVIDOR
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);       //CON INFORMACION, como el IPv4, el PRotocolo y el puerto
    serv_addr.sin_port = htons(atoi(argv[1])); 

    if( (bind(socketfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in)))<0){			//Enlazamos el FD con el struct
		fprintf(stderr, "Error binding");
		close(socketfd);
		return 1;
		} 
	printf("\nSe ha enlazado el socket");
	scanf("%d",&x);
	
    listen(socketfd, 5); 			//ponemos el socket del servidor a escuchar
        
    client_addr_len = sizeof(client_addr);     //obtenemos el tamaño del struct del cliente 
	
	if((clientfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len))<0){//aceptamos las conexiones entrantes
		printf("Cannot accept connection\n");
	    close(socketfd);
	    return 1;
		}
	
	printf("\nSe ha aceptado la conexion");
	scanf("%d",&x);
		
		recv_file = fopen ( filename,"w" );	//abrimos el archivo en el que el servidor va a escribir los datos que le vaya a mandar el cliente
		
		//CICLO en el que el servidor va a recibir los datos del cliente uno por uno y los va a escribir en recv_file
		while(1){
			
			if( recv(clientfd, BUFFER2, 1, 0) != 0 ) {		//Recibe un byte del cliente
				fwrite (BUFFER2 , sizeof(BUFFER2[0]) , 1 , recv_file );	//lo escribe en recv_file
				BUFFER2[0] = 0;
				printf("\nRecibido");
			}// if interno 
			
			else {
				close(socketfd);
				fclose(recv_file);
				printf("\nSe ha cerrado el socket\n");
				return 1;		
				}
				
			}//while interno
		
		close(socketfd);
	    return 0;
	
	}//:~~~~
