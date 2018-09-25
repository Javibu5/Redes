//PRACTICA 1 Cliente
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

/* --------------------------------------------------------------------------------------
 
 Envía un número aleatorio al servidor, quien el devuelve el número incremetnado

---------------------------------------------------------------------------------------- */
 
int main ( )
{
  		int contador = 0;
		/*---------------------------------------------------- 
			Descriptor del socket y buffer para datos 
		-----------------------------------------------------*/
		int Socket_Cliente;
	
		char Datos[80];

		struct timeval timeout;
		fd_set lectura;
		int salida;
	
	//Inicializar la estructua timeval
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	
	//Inicializar los conjuntos fd_set
	FD_ZERO(&lectura);
	FD_SET(0,&lectura);
	
   	/* -----------------------------------------------------
   		Informaci\ufffdn del Servidor
   	-----------------------------------------------------*/
   	struct sockaddr_in Servidor;  
   	socklen_t Longitud_Servidor;
  
  
   	/* --------------------------------------------------
			Se abre el socket cliente
		---------------------------------------------------*/
		Socket_Cliente = socket (AF_INET, SOCK_DGRAM, 0);
		if (Socket_Cliente == -1)
		{
			printf ("No se puede abrir el socket cliente\n");
    		exit (-1);	
		}

 		/*---------------------------------------------------------------------
			Necesitamos una estructura con la informaci\ufffdn del Servidor
			para poder solicitarle un servicio.
   		----------------------------------------------------------------------*/
		Servidor.sin_family = AF_INET;
		Servidor.sin_port = htons(2000);
		Servidor.sin_addr.s_addr =  inet_addr("127.0.0.1");
  	 	Longitud_Servidor = sizeof(Servidor);
	
		printf("Indica que enviar al servidor\n");
		fgets(Datos , 80 , stdin);


   	
		/*-----------------------------------------------------------------------
			Se env\ufffda mensaje al Servidor
		-----------------------------------------------------------------------*/
		int enviado = sendto (Socket_Cliente, (char *) Datos, sizeof(Datos), 0,
			(struct sockaddr *) &Servidor, Longitud_Servidor);

   		if (enviado < 0)
    	{
			printf("Error al solicitar el servicio\n");
		}
    	else
		{
   
		/*----------------------------------------------------------------------
			Esperamos la respuesta del Servidor
			----------------------------------------------------------------------- */ 
			while (contador < 3){
			salida = select(Socket_Cliente + 1,&lectura,NULL,NULL,&timeout);

   			if(salida == -1)
			{
					printf("Se ha producido un error en select\n");
			}

			else if(salida == 0)
							{
				printf("Se ha agotado el tiempo\n");
				contador++;
				}
				else
				{
					int recibido = recvfrom (Socket_Cliente, (char *)Datos, sizeof(Datos), 0,(struct sockaddr *) &Servidor, &Longitud_Servidor);
						if (recibido > 0)
						{
							int enviado = sendto (Socket_Cliente, (char *) Datos, sizeof(Datos), 0,
							(struct sockaddr *) &Servidor, Longitud_Servidor);
								if (enviado < 0)
    						{
								printf("Error al solicitar el servicio\n");
							}
					}

				}
		}
		
	}
   		
		
		close(Socket_Cliente);

	return 0;
}
