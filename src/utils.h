/** @file utils.h
 *  @brief Prototipos de funciones utilidades
 *
 *  Este archivo contiene los prototipos correspondientes
 *  a las funciones de utilidades varias.
 *
 *  @author Jorge Courett (JC)
 *  @bug Sin bugs conocidos.
 */

#ifndef __UTIL_H__
#define __UTIL_H__

#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Serial Port
 */
void calc_serialport(const char *charaux, char *msp);
char calc_parity(const char *charaux);

int getSHMSize(const char *msfilename);

/**
 * @brief Carga en los registros Modbus el valor entero dado
 *
 *  Orden de los elementos en los registros [HNUM][LNUM]
 *  addrs apunta a HNUM
 *
 * @param tab_registers Registros Modbus uint16_t
 * @param addrs Indice de los registros
 * @param valor Valor ingresado
 */
void intToMBTabReg(uint16_t *tab_registers,int addrs, unsigned int valor);

/**
 * @brief Carga en los registros HMI Modbus el valor entero dado
 *
 *  Orden de los elementos en los registros [LNUM][HNUM]
 *  addrs apunta a HNUM
 *
 * @param tab_registers Registros Modbus uint16_t
 * @param addrs Indice de los registros
 * @param valor Valor ingresado
 */
void intToMBTabRegHMI(uint16_t *tab_registers,int addrs, unsigned int valor);

/**
 * @brief Carga en los registros Modbus el valor entero dado segun bits
 *
 *  Orden de los elementos en los registros [LNUM][HNUM]
 *  addrs apunta a LNUM
 *
 * @param tab_registers Registros Modbus uint16_t
 * @param addrs Indice de los registros
 * @param valor Valor ingresado
 * @param bits  Cantidad de bits
 */
void valueToMB(uint16_t *tab_registers,int addrs, unsigned int valor, int bits );

/**
 * @brief Obtiene el valor entero desde  los registros Modbus segun bits
 *
 *  Orden de los elementos en los registros [LNUM][HNUM]
 *  addrs apunta a HNUM
 *
 * @param tab_registers Registros Modbus uint16_t
 * @param addrs Indice de los registros
 * @return Valor entero desde el registro MB
 */
int MBToValue(uint16_t *tab_registers,int addrs, int bits);

/**
 * @brief Obtiene el id del Puerto
 *
 * Traduce un string que contiene el nombre del puerto a un número que
 * lo identifica.
 *
 * @param puerto Nombre del puerto
 * @return Id del puerto
 */
int puerto_formato_eMB ( const char *puerto );

/**
 * @brief Convierte de string a long long int
 *
 * @param s string
 * @return El valor en long long int
 */
long long int atolld(const char* s);

/**
 * Files
 */
#define MAX_LEN_FILE_NAME  256
char * get_filename(const char *fmt, ...);
bool remove_file(const char *fmt, ...);
bool file_exists(const char *fmt, ...);
bool file_exists_read_int( int *value, const char *fmt, ...);
bool file_exists_read_uint( uint32_t *value, const char *fmt, ...);
bool file_exists_read_ints(const char *fmtv,int cv, int value[],const char *fmt,...);
bool file_exists_read_uint16(uint16_t *value, const char *fmt, ...);
bool file_exists_read_uint16_t1(uint16_t *value,const char *value_fmt, const char *fmt, ...);
bool file_exists_read_uint16_t2(uint16_t *value,const char *value_fmt, const char *fmt, ...);
bool file_exists_test_str(const char *fmtv,const char *test_value,const char *fmt,...);
bool file_write(char *filename, const char *fmt, ...);

time_t get_file_cTime(char *filename);
bool del_file_at(char *filename, unsigned int sec_diff_time );

/**
 * Time
 */
char* get_fulltime();
char* get_fulltime_mlls();
int minutoActual();
int minutoSiguiente();
int segundoActual();
int64_t timespecDiffms(struct timeval *timeA_p, struct timeval *timeB_p);
double get_diff_time_now(time_t since);
time_t get_time_now();
int64_t get_diff_time_now_ms(struct timeval since, struct timeval *now);
struct timeval get_time_now_ms();
struct tm *get_now();
char *set_time_str(const char* fmt, const char mlls_sep, struct timeval time);
void add_time_mlls(struct timeval *time, int64_t mlls);
struct timeval get_time_str(const char* fmt, const char *str);

/**
 * @brief Reemplazo de strings
 *
 * @param original string original
 * @param pattern  patron a reemplazar
 * @param replacement string de reemplazo
 * @return String con el patron reemplazado
 */
char * replaceString(
   const char * const original,
   const char * const pattern,
   const char * const replacement
   );

/**
 * @brief Reemplazo ide formato HTML a strings
 *
 * @param str string con formato HTML
 * @return String de salida
 */
char * htmlToString(const char *str);

/**
 * @brief Calcula el desplazamiento en un buffer circular
 *
 * @param int orig indice origen
 * @param int increment incremento de indice
 * @param int size tamaÃo del buffer
 * @return int indice del buffer
 */
int get_circular_index(int orig, int increment, int size);

/**
 * @brief Split de una cadena
 * @param tk token
 * @param instr String de entrada
 * @param split_str[] arreglo con las cadenas separadas
 * @return Retorna el numero de cadenas separadas
 */
int split(char *tk, const char *instr,char *split_str[]);

/**
 * @brief Free de la memoria reservada en un split
 * @param spn cantidad de cadenas separadas
 * @param split_str[] arreglo con las cadenas separadas
 */
void split_free(int spn, char *split_str[]);

/**
 * @brief Obtiene el elemento num del la linea csv separada por sep
 * @param line Linea csv
 * @param num Elemento a obtener 
 * @param sep Seperador 
 */
const char* get_csv_field(char* line, int num,const char *sep);

/**
 * Redondeo a multiplos de la alineacion
 */
#define P2ROUNDUP(x, align)     (-(-(x) & -(align)))

#endif
