# Contagio del COVID-19 en el sistema de autobuses  de tránsito rápido de Bogotá (TransMilenio): Un estudio de reacción-difusión en redes de transporte público



En este proyecto estudiamos un punto crítico en el contagio de toda enfermedad contagiosa por contacto, el contagio en sistemas de transporte público. Este estudio presenta el modelo utilizado para la simulación de este sistema de transporte y la dinámica de contagio en la población estudiada.

## Inicio 

Los archivos que en  la carpeta "SIMULACIONES" contienen las simulaciones con archivos .cpp, .h y CMAKE , en la carpeta "RESULTADOS" encontrará algunos códigos .py para generar histogramas y hacer un análisis básico de los datos de salida de los códigos en C++. 


### Prerequisitos
####Para las simulaciones
cmake superior a 3.0
g++ superior a 4.8 
 Recomendamos el uso de sanitizer solo en caso de contactarse previamente con el desarrollador (dfortizc@unal.edu.co)
 ####Para el análisis de los datos
Python 2.7 o superior 


## Corriendo los tests
En la carpeta  de "SIMULACION/" que haya escogido utilice cmake para compilar el proyecto:
```
cmake cmakelist 
make 
easy_route.x
```
Los archivos generados son en formato csv, recomendamos el uso de python para el análisis pues  las rutinas de los resultados preliminares fueron desarrolladas en este lenguaje.



## Modelo implementado
###Algoritmo  estocástico de contagio en poblaciones con dinámica rápida. 
Se implementó un modelo de saltos discretos SE+I en el que se estudian todas las *reacciones* posibles en el sistema de transporte en el contagio de una epidemia.   Entendemos por reacción la transformación de un conjunto de agentes de un tipo a otro, los tipos de agentes que utilizamos son:

Tipo de agente     | Nombre
-------- | ---
Susceptible | S
Expuesto    | E
Infeccioso | I

Y las posibles reacciones que suceden en el sistema son:

Evento     | Reacción
-------- | ---
 Exposición| $S+I\rightarrow E+I$
Entrada de Susceptible   | $\phi\rightarrow S$
Salida de Susceptible   | $S\rightarrow \phi$
Salida de Expuesto| $E\rightarrow \phi$

Así, la primera reacción modela el contacto entre una persona susceptible y un infeccioso, exponiendo a la persona susceptible y convirtiéndola en expuesta, esta persona no presenta síntomas ni puede infectar a demás personas durante la simulación debido a la escala temporal del escenario (máximo 2 horas). 
Las demás reacciones simulan entrada y salida de usuarios al sistema y sólo pueden suceder en las estaciones.
El algoritmo de simulación utilizado fue el método directo de Gillespie para dinámica de procesos de salto (Para información del algoritmo recomendamos el artículo original en reacciones químicas http://www.math.kit.edu/ianm3/lehre/semmathmodelle2010w/media/gillespie_general_method.pdf). Cabe aclarar que este tipo de algoritmos son análogos a resolver ecuaciones diferenciales del sistema (e.g. sistema SIR y sus reacciones)cuando se simula las suficientes veces pero simulan caso por caso(discretamente) la población y sus transformaciones individuales. 
Cada población (Estación o bus) se mantiene en esta simulación durante el algoritmo de transporte utilizado. 

Con el fin de evitar contagios excesivos debido a la asunción de "sistema bien revuelto" (como lo asume el modelo estocástico simulado) se limitó el contacto del infeccioso a una cantidad fija de personas en la estación en cada paso de tiempo(estas personas pueden ser distintas en cada paso) por medio de una Red-Ego centrada en el infeccioso, construida con la topología de una red aleatoria de Erdös- Renyi de densidad $p$
##Algoritmo de transporte en redes
Se utilizó una red bipartita siendo las clases *estación* y *bus*, en las *estaciones* las personas esperan algún *bus* y se suben con cierta probabilidad. La única reacción posible en el *bus* es el contagio durante el trayecto, el tiempo entre reacciones sale de una distribución exponencial como es demostrado por el algoritmo de reacción utilizado(	Método Directo de Gillespie).
Cada cierto tiempo(predeterminado) un *bus* para en una *estacion*, en un periodo de tiempo se activa el intercambio de pasajeros entre ambos nodos. Una vez el *bus* haya parado, espera un tiempo (tiempo de viaje) para realizar la conexión en la otra estación y repetir el ciclo. 
Utilizamos una linea de 10 estaciones con 10 buses ruta fácil moviéndose en cada paso de tiempo de conexión.  

##Un resumen del código
En una red bipartita, donde cada nodo es una población (*bus* o *estación*) se implementó un algoritmo de reacción de Gillespie para un contagio SE con un infectado. La dinámica de transporte corresponde a la de un conjunto de rutas fáciles moviéndose sobre un conjunto de 10 estaciones alineadas entre sí. 

##Un muy breve resumen de los resultados
Por medio de este algoritmo, simulamos el contagio en transmilenio con una sola persona infecciosa en el sistema. Variamos los parámetros del modelo y obtuvimos que la cantidad de personas expuestas crece exponencialmente con la cantidad de vecinos que lo rodea
## Autores

* **Daniel Felipe Ortiz** - *BSc Physics  Student* - [UNAL]
* **José Alejandro Rojas** - *MSc-Physics Student* - [UNAL]
* **Camilo Naizaque** - *MSc-Physics Student* - [UNAL]
* **Rafael Rentería** - *PhD, Professor* - [UNAD,UNAL]
* **Rafael Hurtado** - *PhD, Professor* - [UNAL]
 



## Agradecimientos 
Extendemos nuestro agradecimiento a las personas que hicieron posible estas simulaciones: 
EQUIPO UN
* Juan David Garcia
* Liliana Lopez Kleine
*  Andres Pinzon
*  Santiago Ramirez
*  Francisco Rodriguez, 
*  Carolina Saavedra
*  Andres Torres Meriño,
* Piedad Urdinola
