
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
extern fing_st cdb_rfing[Nc][POS_N];   /* firmas: por cada loc. tenemos un listado de AP y nivel */


/* listado de estaciones de radio (access points) */
extern char ERs[Nc][MAC_LEN];

/* datos del movil */
extern char movil_gps[Nc][LAT_LONG_LEN]; /* listado de localizaciones (del movil) */
extern fing_st movil_rfing[Nc][POS_N];     /* firmas: por cada loc. tenemos un listado de AP y nivel */

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
distancia_t loc[Nc]; /* localizacion segun SRC de cada posicion del movil */

int compare_d (const void * a, const void * b)
{
  distancia_t *data_1 = (distancia_t *)a;
  distancia_t *data_2 = (distancia_t *)b;
  return ( data_1->dis > data_2->dis );
}

void d_init(distancia_t d[Nc]) 
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
	x*=-1;
	
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
	y*=-1;
	
	return y;
}

int K;
//#define K 3 
#define L 3 

void maf()
{
	int i,j;
	float p;

	/* MAF */
	float x = 0;
	float y = 0;
	p = 0;
	for (i=1; i<(Nc-1); i++) {
		if (loc[i+1].dis == 3)
			break;

		x=0; y=0;
		for (j=-1; j<=1; j++) {
			x = x + get_x( cdb_gps[ loc[i+j].n ] );
			y = y + get_y( cdb_gps[ loc[i+j].n ] );
		}
		x=x/K; y=y/K;

		printf("movil:%i - cdb gps x,y: %f,%f - maf[%i] x,y: %f,%f\n", 
			i,
			get_x( cdb_gps[ loc[i].n ] ),
			get_y( cdb_gps[ loc[i].n ] ),
			K, x, y);
		printf("web movil%i: {lat: %f , lng: %f, name: \"maf\",  address: \"maf\"}, \n", i, x, y);
		printf("web movil%i: {lat: %f , lng: %f, name: \"cdb\",  address: \"cdb\"}, \n", i,
			get_x( cdb_gps[ loc[i].n ] ),
			get_y( cdb_gps[ loc[i].n ] ) );
		printf("web movil%i: {lat: %f , lng: %f, name: \"cdb\",  address: \"cdb\"}, \n", i,
			get_x( movil_gps[i]),
			get_y( movil_gps[i]) );
	}

}

/* k-vecinos mas cercanos (KNN) */
void knn(int movil_n)
{
	int i;
	float p;

	/* KNN */
	float x = 0;
	float y = 0;
	p = 0;
	for (i=0; i<K; i++) {
		x = x + get_x( cdb_gps[ d[i].n ] );
		y = y + get_y( cdb_gps[ d[i].n ] );
	}
	x=x/K; y=y/K;

	printf("movil:%i - cdb gps x,y: %f,%f - knn[%i] x,y: %f,%f\n", 
		movil_n,
		get_x( cdb_gps[ d[0].n ] ),
		get_y( cdb_gps[ d[0].n ] ),
		K, x, y);
	printf("web movil%i: {lat: %f , lng: %f, name: \"knn\",  address: \"knn\"}, \n", movil_n, x, y);

}

/* moving average filter (MAF) */
void main(int argc, char *argv[])
{
	int i,j;
	float p;

	if (argc < 2) {
		printf("Error. Ejecutar con:\n\t %s K  # donde K es un entero para el calculo de knn\n\n", argv[0]);
		exit(1);
	}
	K = atoi(argv[1]);
	
	data_load(f_cdb, cdb);
	data_load(f_movil, movil);
	ERs_load(f_er);
	print_datos();
	
	v_ordenar(cdb_rfing[2], vr);
	v_ordenar(tfing, vt);


	d_init(loc);
	for (j=0; j<Nc; j++) {
		if (movil_rfing[j][0].rss == -1)
			break;

		d_init(d);

		for (i=0; i<Nc; i++) {
			if (cdb_rfing[i][0].rss == -1)
				break;
			v_ordenar(cdb_rfing[i], vr);
			v_ordenar(movil_rfing[j], vt);
			p = p_calc(vt, vr);
			d[i].dis = (float) 1-p;
			d[i].n = i;
		}
		qsort (d, Nc, sizeof(distancia_t), compare_d);
		loc[j].dis = d[0].dis;
		loc[j].n = d[0].n;
		knn(j);
	};
	
	printf("\n");

	maf();

}
