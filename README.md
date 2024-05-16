# Cuatro-en-linea

Cuatro en Lınea es un juego de mesa para dos jugadores distribuido por Hasbro, en el que se introducen fichas en un tablero vertical con el objetivo de alinear cuatro fichas consecutivas de un mismo color sobre un tablero formado por seis filas y siete columnas. <br>
Cada jugador dispone de 21 fichas de un color (por lo general, rojas o amarillas). <br>
Por turnos, los jugadores deben introducir una ficha en la columna que prefieran (siempre que no este completa) y esta caera a la posicion mas baja. <br>
Gana la partida el primero que consiga alinear cuatro fichas consecutivas de un mismo color en horizontal, vertical o diagonal. <br>
Si todas las columnas estan llenas pero nadie ha hecho una fila valida, hay empate.<br>

## Instrucciones:

* clonar repositorio de github.

```
git clone https://github.com/maxborquez/Cuatro-en-linea.git
```

* Una vez en la carpeta debemos aplicar el comando para compilar el programa automaticamente.

```
make
```

* Ahora podemos iniciar el servidor en un puerto
  - puerto de ejemplo: 5656

```
./server 5656
```

* Ahora podemos iniciar el cliente usando la ip y puerto donde este iniciado el server
  - ip de ejemplo: 192.168.1.15
  - puerto de ejemplo: 5656

```
./client 192.168.1.15 5656 

```

## Controles para juagr

Para jugar debes seleccionar una columna usando los comandos c1,c2,c3,c4,c5,c6 y c7 <br/>
Si quieres salir puedes escribir la letra Q en mayuscula

## Ejemplo de juego

* Si el jugador comienza se vera asi
```
Bienvenido al juego de 4 en linea
Escribe c1,c2,c3,c4,c5,c6 o c7 para poner una ficha en una columna
Escribe Q para salir del juego 

Jugador comienza


Escriba su movimiento: 

```

* Si el servidor comienza se vera asi
```
Bienvenido al juego de 4 en linea
Escribe c1,c2,c3,c4,c5,c6 o c7 para poner una ficha en una columna
Escribe Q para salir del juego 

Servidor comienza
Movimiento del servidor: c2

TABLERO
              
              
              
              
              
  S           
-------------
1 2 3 4 5 6 7


Escriba su movimiento: 

```

