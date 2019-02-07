# Computer Graphics - Práctica 2
## Óscar Subirats - 183850
## Sergi Olives - 193196

Los documentos siguientes contienen:
* Código fuente de la aplicación
* Imágenes usadas como demostraciones

Esta práctica consiste en la implementación de 4 algoritmos:
* Dibujar una línea mediante el algoritmo DDA.
* Dibujar una línea mediante el algoritmo de Bresenham.
* Dibujar un círculo mediante el algoritmo de Bresenham, donde este puede estar relleno o no.
* Dibujar un triángulo, donde se puede escoger entre tener: 
		* El triángulo con las líneas de solo un color.
		* El triángulo con cada línea de distinto color.
		* El triángulo relleno con un solo color.
		* El triángulo relleno con tres colores.
		* El tríangulo relleno mediante el baricentro.

## Línea DDA (se accede pulsando la tecla del número 1):
Para dibujar una línea simplemente se tiene que hacer click izquierdo en dos lados diferentes y la línea aparecerá.

## Línea Bersenham (se accede pulsando la tecla del número 2):
Como en la línea DDA, simplemente se tiene que hacer click izquierdo en dos lados diferentes y la línea aparecerá.

## Círculo de Bresenham (se accede pulsando la tecla del número 3):
Para dibujar el círculo se tiene que hacer click izquierdo dos veces (la primera vez correspondería a la posición del centro, y la segunda a la circumferencia).
	* Si se pulsa la tecla TAB al hacer el círculo este estaría relleno.

## Triángulo:
* Si se pulsa la tecla del número 4 podemos dibujar el triángulo haciendo click izquierdo en tres puntos.
	* Si se pulsa la tecla TAB al hacer el triángulo este estaría relleno de un solo color.
* Si se pulsa la tecla del número 5 podemos dibujar el triángulo coloreándolo mediante el baricentro.
	* Los colores por defecto son rojo, verde y azul, pero se puede cambiar el color de cada vértice antes de hacer click, es decir, escogemos un primer color y clickamos por primera vez, escogemos el segundo color y clickamos por segunda vez y escogemos el tercer color y clickamos por última vez.
* Si se pulsa la tecla del número 6 podemos dibujar el triángulo con tres colores distintos.
	* El procedimiento seria el mismo con el del baricentro, solo que con este estamos escogiendo el color de las líneas.
	* Si se pulsa la tecla TAB se dibuja un triangulo relleno con los tres colores elegidos.

## Controles:
* Colores:
El color por defecto es el blanco ya que el fondo es negro.
	* Al pulsar la tecla "A" el color se cambia a blanco.
	* Al pulsar la tecla "S" el color se cambia a rojo.
	* Al pulsar la tecla "D" el color se cambia a verde.
	* Al pulsar la tecla "F" el color se cambia a azul.
	* Al pulsar la tecla "G" el color se cambia a negro.

* Relleno:
El relleno esta desactivado por defecto.
	* Al pulsar la tecla "TAB" se activa o desactiva el relleno.

* Figuras:
	* Al pulsar la tecla "1" se activa el modo de dibujar la línea DDA.
	* Al pulsar la tecla "2" se activa el modo de dibujar la línea de Bresenham.
	* Al pulsar la tecla "3" se activa el modo de dibujar el círculo.
	* Al pulsar la tecla "4" se activa el modo de dibujar el triángulo.
	* Al pulsar la tecla "5" se activa el modo de dibujar el triángulo relleno mediante el baricentro.
	* Al pulsar la tecla "6" se activa el modo de dibujar el triángulo tri-color.
	* Al pulsar la tecla "7" se reinicia el framebuffer.