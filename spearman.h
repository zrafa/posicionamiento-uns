
/* spearman.h: API del manejo y preparacion los datos de gps y ers.
 *	       para el Calculo de la distancia Spearman.
 * 	       (Spearman Rank Correlation Coefficient - SRC )
 *
 * Curso de posicionamiento y seguimiento.
 * Prof. Dr. Favio Mason
 * Alumno: Rafael Zurita
 */

#ifndef SPEARMAN_H
#define SPEARMAN_H

# define ERs_FILE "ERs.txt"
# define GPSs_ERs_FILE "gps-er.txt"
# define MAC_LEN        17      /* 17 letras tiene una MAC de ER */
# define LAT_LONG_LEN   80      /* len de la cadena latitud,longitud */
# define LINE_LEN       80      /* 17 letras tiene una MAC de ER */
# define Nc     9       /* 30 ERs maximo en la matriz */
# define POS_N  40      /* maxima cantidad de puntos. Ej: 5 puntos por cuadra, 4 cuadras */

typedef struct v_st {
	int id;
	int pos;
} v_st;

typedef struct fing_st {
	char mac[MAC_LEN];
	int rss;
} fing_st;

void v_init(v_st *v);
void f_init(fing_st *f);
void bd_init(fing_st *f[]);
void v_ordenar(fing_st *f, v_st *v);
int ERs_load(const char *fname, char ers[][MAC_LEN]);
int compare (const void * a, const void * b);
void print_datos(void);
int GPSs_ERs_load(const char *fname, fing_st rfings[][Nc], char gps[][LAT_LONG_LEN]);
//int GPSs_ERs_load(void); 
float pt_pr_calc(v_st *v);
/* 
 * p_calc: calcula el Spearman Rank Correlation Coefficient (SRC)
 */
float p_calc(v_st *vt, v_st *vr);
#endif /* SPEARMAN_H */
