Tarea 0

Connor Wolfe
Ignacio Jara  14619245

Consideraciones

La tarea corre dentro de un while el cual finaliza cuando todos los procesos son atendidos. Cada vez que entramos al while se decide cual proceso vamos a atender o si atenderemos multiples procesos y luego de eso lo atendemos. Se tienen opciones al atender el procesos, la primera es cuando el burst es igual al quantum por lo que se cambia de cola y se revisa si el proceso termino. La segunda es cuando el quantum es menor al batch, por lo que se cambia de cola al terminar el proceso. La tercera es cuando el burst es menor al quantum por lo que se mantiene en la cola y actualiza los valores de su quantum y los burst completados. Se crearon todas las estructuras necesarias para desarrollar la tarea (allQueue, Queue, List y Procces) y el resto se desarrollo en el main. La tarea esta totalmente funcional en las 3 versiones. Cabe mencionar que se realizaron una serie de print para entender cada evento que sucedía al interior de la simulación. La simulación fue realizada en tiempo discreto, en base a eventos. En el cual los eventos que actualizaban el reloj era cuando un proceso dejaba la CPU. No logramos desarrolla el makefile por lo que subimos un ejecutable, a pesar de esto el programa se corre con el mismo comando. 

Comparación de versiones

Podemos notar que las versiones implementadas eran bastante similares, las cuales diferian de una reglas. En el caso de la versión 1 V/S version 2 la diferencia era la inclusión de que en un periodo de tiempo S todos los procesos volvian a la cola de mayor prioridad. Esto lo implementamos de forma que comparabamos el S con el tiempo al cabo de un proceso e si esteera mayor pasabamos todos los procesos a la cola de mayor prioridad y actualizabamos el S sumandolos consigo mismo para seguir comparando que volviera a suceder esta lapso de tiempo. En el caso de la version 3 esta tenia una leve diferencia con las otras dos versiones. Esta era igual a la version 2 pero incluía una regla la cuan decía que el quantum cambiaba segun la cola que estaba. Esto fue implementado a travez de al instanciar las colas, las cuales poseian un atributo quantum, le actualizabamos este segun su prioridad. 


Tiempo de simulación

Como se desarrollo una simulacion para eventos discretos, para obtener el tiempo de simulacion, se realizo un reloj el cual se actualizaba cada ves que un proceso terminaba o cedía la cpu ya sea por que completo el quantum o el burst. 


Cantidad de bloqueos

En relacion a la cantidad de bloqueos que es sometido un proceso, se considero las veces que el proceso agoto su quantum por lo que paso a estado waiting

Análisis Estadístico
A continuación se presentan los resultado de los test entregados:

Version I
	Clock: 45
 	Change state of process 0 to finished, CPU TIME =  6 BLOCKED = 3 TURNAROUND TIME = 36 RESPONSE TIME = 0  
 	Change state of process 1 to finished, CPU TIME =  6 BLOCKED = 3 TURNAROUND TIME = 27 RESPONSE TIME = 0  
 	Change state of process 2 to finished, CPU TIME =  7 BLOCKED = 5 TURNAROUND TIME = 31 RESPONSE TIME = 0  

Version II
	Clock: 45
	Change state of process 0 to finished, CPU TIME =  6 BLOCKED = 3 TURNAROUND TIME = 29 RESPONSE TIME = 0  
	Change state of process 1 to finished, CPU TIME =  6 BLOCKED = 3 TURNAROUND TIME = 30 RESPONSE TIME = 0  
	Change state of process 2 to finished, CPU TIME =  7 BLOCKED = 5 TURNAROUND TIME = 31 RESPONSE TIME = 0  

Version III
	Clock: 45
	Change state of process 0 to finished, CPU TIME =  4 BLOCKED = 1 TURNAROUND TIME = 30 RESPONSE TIME = 0  
	Change state of process 1 to finished, CPU TIME =  4 BLOCKED = 1 TURNAROUND TIME = 34 RESPONSE TIME = 3  
	Change state of process 2 to finished, CPU TIME =  3 BLOCKED = 1 TURNAROUND TIME = 28 RESPONSE TIME = 5

Podemos notar que el trunaround time promedio en la versión 2 y 3 mejora pero respecto al tiempo total de simulacion se mantiene constante, lo que es consistente. En la versión 3 tambien notamos que pasa menos veces por la cpu y cada proceso esta menos veces bloqueado dado que posee un quantum mayor.



