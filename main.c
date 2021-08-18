
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

/* formato de los datos :
 *        Latitud,Longitud. Ej: 3849.84448,06804.07505
 *        MAC (ID) y nivel. Ej: 38:6B:1C:27:7C:DA 60
 */

/* base de datos */
extern char cdb_gps[Nc][LAT_LONG_LEN]; /* listado de localizaciones (puntos gps) */
extern fing_st cdb_rfing[POS_N][Nc];   /* firmas: por cada loc. tenemos un listado de AP y nivel */


/* listado de estaciones de radio (access points) */
extern char ERs[Nc][MAC_LEN];

/* datos del movil */
extern char mov_l[Nc][LAT_LONG_LEN]; /* listado de localizaciones (del movil) */
extern fing_st mov_f[POS_N][Nc];     /* firmas: por cada loc. tenemos un listado de AP y nivel */

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
const char *f_movil = "movil.txt";
const char *f_er = "ERs.txt";

typedef struct distancia_t {
		float dis;	/* distancia */
		int n;		/* pos en la cdb */
	} distancia_t;

distancia_t d[Nc];

int compare_d (const void * a, const void * b)
{
  distancia_t *data_1 = (distancia_t *)a;
  distancia_t *data_2 = (distancia_t *)b;
  return ( data_1->dis > data_2->dis );
}

void d_init(void) 
{
	int i;

	for (i=0; i<Nc; i++) {
		d[i].dis = 3;
	}

}

float get_x(const char *s)
{
	float x;
	char temp[20];
	strncpy(temp, s, 2);
	temp[2] = '\0';
	x = atof(temp);
	
	strncpy(temp, s+2, 8);
	temp[8] = '\0';
	x = x + (atof(temp) / 60);
	
	return x;
}

float get_y(const char *s)
{
	float y;
	char temp[20];
	strncpy(temp, s+11, 3);
	temp[3] = '\0';
	y = atof(temp);
	
	strncpy(temp, s+14, 8);
	temp[8] = '\0';
	y = y + (atof(temp) / 60);
	
	return y;
}

#define K 3 
#define L 3 

void main(void)
{
	int i;

	data_load(f_cdb, cdb);
	//data_load(f_movil, movil);
	ERs_load(f_er);
	print_datos();
	
	v_ordenar(cdb_rfing[2], vr);
	v_ordenar(tfing, vt);

	for (i=0; i<Nc; i++) {
		printf("vt %i id:%i pos:%i\n", i, vt[i].id, vt[i].pos); 
	}

	for (i=0; i<Nc; i++) {
		printf("vr %i id:%i pos:%i\n", i, vr[i].id, vr[i].pos); 
	}

	float p;

	d_init();

	for (i=0; i<Nc; i++) {
		if (cdb_rfing[i][0].rss == -1)
			break;
		v_ordenar(cdb_rfing[i], vr);
		v_ordenar(tfing, vt);
		p = p_calc(vt, vr);
		d[i].dis = (float) 1-p;
		d[i].n = i;
	}
	qsort (d, Nc, sizeof(distancia_t), compare_d);

	for (i=0; i<Nc; i++) {
		printf("i:%i - dis:%f\n", d[i].n, d[i].dis);
	}

	/* KNN */
	float x = 0;
	float y = 0;
	p = 0;
	for (i=0; i<K; i++) {
		x = x + get_x( cdb_gps[ d[i].n ] );
		y = y + get_y( cdb_gps[ d[i].n ] );
	}
	x=x/K; y=y/K;

	printf("gps x,y: %f,%f - knn[%i] x,y: %f,%f\n", 
		get_x( cdb_gps[ d[0].n ] ),
		get_y( cdb_gps[ d[0].n ] ),
		K, x, y);

	p = p_calc(vt, vr);
	printf("c: %f\n", p);
	printf("d=1-p = %f\n", (float)1-p);

	
}
