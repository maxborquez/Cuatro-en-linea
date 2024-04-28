# Cuatro-en-linea

Cuatro en Lınea es un juego de mesa para dos jugadores distribuido por Hasbro, en el que se introducen fichas en un tablero vertical con el objetivo de alinear cuatro fichas consecutivas de un mismo color. Fue creado en 1974. 
El objetivo del juego es alinear cuatro fichas sobre un tablero formado por seis filas y siete columnas. 
Cada jugador dispone de 21 fichas de un color (por lo general, rojas o amarillas). 
Por turnos, los jugadores deben introducir una ficha en la columna que prefieran (siempre que no este completa) y esta caera a la posicion mas baja. Gana la partida el primero que consiga alinear cuatro fichas consecutivas de
un mismo color en horizontal, vertical o diagonal. Si todas las columnas estan llenas pero nadie ha hecho una fila valida, hay empate.

Instrucciones:
Despues de clonar el repositorio abrimos una terminal en la carpeta del proyecto
En la terminal debemos aplicar el comando make para compilar el programa automaticamente

Para iniciar el servidor usamos el comando ./server 5656

Para iniciar el cliente abrimos otra terminal en la carpeta del proyecto y usamos el comando ./client IP 5656

por IP nos referimos a la ip del servidor 
el cual se consulta con el comando "ip a" en la terminal del servidor



Para jugar introduce el comando c + numero entre 1 y 7 por ejemplo:

Jugador comienza.
Enter a message: c1
Server response: c3

TABLERO
              
              
              
              
              
C   S         
-------------
1 2 3 4 5 6 7
Enter a message: c2
Server response: c2

TABLERO
              
              
              
              
  S           
C C S         
-------------
1 2 3 4 5 6 7
Enter a message: c1
Server response: c4

TABLERO
              
              
              
              
C S           
C C S S       
-------------
1 2 3 4 5 6 7
Enter a message: c1
Server response: c3

TABLERO
              
              
              
C             
C S S         
C C S S       
-------------
1 2 3 4 5 6 7
Enter a message: c1
TABLERO
              
              
C             
C             
C S S         
C C S S       
-------------
1 2 3 4 5 6 7

¡Has ganado!
