# posicionamiento-uns
Curso de posicionamiento y seguimiento UNS - Favio Masson

Alumnos: Rafael Ignacio Zurita <rafa@fi.uncoma.edu.ar>

Este repositorio contiene todos los elementos elaborados para el trabajo final del curso.
Los cuales consisten de:

Datos crudos
------------

- datos crudos recolectados directamente conversando con el driver serial de Linux (el gps-rtk conectado por serial)
- datos de los access point alrededor recolectados directamente con iwlist en Linux


Filtrado de datos crudos
------------------------

- Obtenemos la base de datos de firmas por posicion
- y lo mismo para lo recolectado por el movil.

Procesamiento 
-------------

- Se desarrolló un en C que realiza el procesamiento: carga, ordenacion, preparación, spearman, distancia, knn y maf.

Visualización (presentación)
----------------------------

- El programa en C se validó con los datos de prueba del apunte de Favio
- Luego, con los datos reales, se vizualiza mediante una web que va mostrando el progreso del movil y todas la localizaciones (spearman-cdb, knn y maf).
