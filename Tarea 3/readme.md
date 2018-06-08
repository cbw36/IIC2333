Tarea 3

Connor Wolfe
Ignacio Jara  14619245

Consideraciones

La tarea esta funcional con la condicion de que el disco estew fromateado (no funcionan las funciones read and write si no esta reseteado).

Funciones:
      open: funciona bien si escribo archivo nuevo con cz write, con cz intit igual se puede leer y escribir en el. Pero si ya existe un archivo en el disco y se saca ese archivo con cz open, se puede ver que con el cz open que no se estan actualizando todos los atributos de ese archivo(tales como read positon, write positon) y estos son muy importantes para el cz read y cz write.
      read: funciona perfectamente.
      write: falta tamano, falta direccionamiento indirecto y no esta actualizando el tiempo (timestamp).
      exist: funciona perfectamente.
      close: funciona perfefctamente.
      mv: funciona perfectamente.
      cp:funciona perfectamente.
      rm: no borra el bitmat los espacios de datos.
      void cz_ls(): funciona completamente.
