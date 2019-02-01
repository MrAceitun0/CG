# Computer Graphics - Practice 1
## Óscar Subirats - 183850
## Sergi Olives - 193196

Los documentos siguientes contienen:
* Código fuente de la aplicación
* Imágenes usadas como demostraciones

Esta práctica consiste en la implementación de 4 algoritmos:
* Dibujar figuras geométricas (línias, rectangulos y círculos) con interacción del usuario
* Programa del estilo "Paint"
* Procesamiento de imagen: animar imagen y conversión de imagen de color a grayscale
* Animación de dibujo de espirales de color con interacción del usuario

## Figuras Geométricas (se accede pulsando la tecla del número 1)
En este modo, se permite al usuario dibujar línias, rectangulos y círculos interactuando con la pantalla.

* Click izquierdo: si se pulsa en dos puntos diferentes, se va a dibujar una línia entre estos dos puntos.
* Click derecho: si se pulsa en dos puntos diferentes, se va a dibujar un rectangulo donde los dos puntos son puntos diagonales del rectangulo.
* Click central: si se pulsa en dos puntos diferentes se va a dibujar un círculo con centro en el primer click y el radio con la distancia entre el primer click y el segundo.

Si se pulsa el espacio, se va a mostrar una demostración de las figuras geométricas anteriores dibujadas con las funciones que también utilizan los usuarios con su interacción. 

Observaciones: las línias con inclinación elevada contienen errores al ser dibujadas por el algoritmo usado para su dibujado.

## Paint (se accede pulsando la tecla del número 2)
En este modo, se permite al usuario dibujar libremente.

* Click izquierdo: para pintar o si se pulsa en la zona debida, para cambiar de color o para aumentar o disminuir el tamaño del pincel. Si se mantiene pulsado el click izquierdo y se arrastra pintara por donde se mueva el cursor.
* Tab: para poner el fondo de color blanco.
* Letra Q: para poner el fondo de color negro.
* Letra W: para poner el fondo de color azul.
* Letra E: para poner el fondo de color rojo.
* Letra R: para poner el fondo de color verde.

## Editor Imagen (se accede pulsando la tecla del número 3)
En este modo, se carga una imagen y si el usuario pulsa la tecla espacio la imagen se mueve, y si se hace click izquierdo una parte de la imagen se vuelve en gris.

## Animación Espirales (se accede pulsando la tecla del número 4)
En este modo, el usuario puede pulsar con click izquierdo en cualquier sitio de la pantalla y empezará una animación de una espiral con centro donde el usuario ha hecho click. Si clica en otro punto, la primera espiral parará y empezará una segundo con centro en la posición del último click. Además, todos los píxeles de las espirales van a tener colores distintos con una función random.
