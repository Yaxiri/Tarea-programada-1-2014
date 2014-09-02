#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


int existe;
char ip[30];
char puerto[30];
int Comparar(char[],char[]);
int main(int argc, char *argv[]){
    
    if(argc != 2){
        printf("\n Uso: %s <port> \n",argv[0]);
        return 1;
		}
    
/*~~~~~~~~~~Declaracion de variables~~~~~~~~~~*/
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
    
    serv_addr.sin_family = AF_INET;                          //LLENAMOS LA ESTRUCTURA DEL SOCKET DEL SERVIDOR
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
	
	//verifica si el mensaje es un adios para cerrar la conexion
int salida(char msj[]){
	char adios[]={'a','d','i','o','s'};
	int i;					//variable para el for
	for(i=0;i<5;i++){
		if (msj[i]!=adios[i])
			return 0;
	}
	return 1;
}


int muestra( ){
	FILE *archivo;//Es un buffer que almacena la dirección del archivo
    char caracteres[30];
    archivo = fopen("listaContactos.txt","r");//Almacena la dirección del archivo
    
    if (archivo == NULL)  {
		 printf("\033[36m Usted aún no ha agregado ningun amigo\n");
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
		usuario= "juan";
		FILE *archivo;
		char caracteres[30];    
		int i=0;
        archivo = fopen("listaContactos.txt","r");  // Se trata de abrir un archivo
        			
			if (archivo != NULL) {  // Si el archivo existe:
			existe = 0;
			while (feof(archivo) == 0)  // Recorre el archivo buscando el usuario solicitado
			{
			//		fgets(caracteres,30,archivo);
			//		if(((Comparar(usuario,caracteres))==1)||(i>0)){
				//		if(i==1)strcpy(ip,caracteres);           // Si se encuentra se copia la información correspondiente
					//	if(i==2){strcpy(puerto,caracteres);      // el las variables globales para poder ser usadas por el socket
					//  i=0;
					//	existe = 1;   // Uso de manejo de IP y de usuarios no agregados
					//	break;}
						
					//	i++;
					//	}
						
			}
		
	}
	return 0;
	}
	




