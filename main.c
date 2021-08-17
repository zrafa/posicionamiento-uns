
/* main.c: Utiliza spearman.c para implementar el trabajo final.
 *	       Calcula la distancia Spearman y emite KNN y MAF.
 *
 * Curso de posicionamiento y seguimiento.
 * Prof. Dr. Favio Mason
 * Alumno: Rafael Zurita
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "spearman.h"

/* base de datos */
extern char GPSs[Nc][LAT_LONG_LEN]; /* listado de localizaciones (puntos gps) */
extern fing_st RFINGs[POS_N][Nc];   /* firmas: por cada loc. tenemos un listado de AP y nivel */

/* listado de estaciones de radio (access points) */
extern char ERs[Nc][MAC_LEN];


char mov_l[Nc][LAT_LONG_LEN]; /* listado de localizaciones (del movil) */
fing_st mov_f[POS_N][Nc];   /* firmas: por cada loc. tenemos un listado de AP y nivel */

/* vectores para el calculo de spearman */
v_st vt[Nc];
v_st vr[Nc];

/* tfing: datos obtenidos por un movil (caso del apunte de Favio) */
fing_st tfing[] = { 

        {"38:6B:1C:27:7C:00", 62},
        {"38:6B:1C:27:7C:06", 60},
        {"38:6B:1C:27:7C:AA", 54},
        {"38:6B:1C:27:7C:05", 43},
        {"38:6B:1C:27:7C:AC", 40},
        {"38:6B:1C:27:7C:AC", -1}
        };

const char *f_cdb = "gps-er.txt";
const char *f_er = "ERs.txt";

void main(void)
{
	int i;

	//GPSs_ERs_load();
	GPSs_ERs_load(f_cdb, RFINGs, GPSs);
	ERs_load(f_er, ERs);
	print_datos();
	
	v_ordenar(RFINGs[2], vr);
	v_ordenar(tfing, vt);
	//v_ordenar(RFINGs[2], vt);

	for (i=0; i<Nc; i++) {
		printf("vt %i id:%i pos:%i\n", i, vt[i].id, vt[i].pos); 
	}

	for (i=0; i<Nc; i++) {
		printf("vr %i id:%i pos:%i\n", i, vr[i].id, vr[i].pos); 
	}

	float p = p_calc(vt, vr);
	printf("c: %f\n", p);
	printf("d=1-p = %f\n", (float)1-p);

	
}
