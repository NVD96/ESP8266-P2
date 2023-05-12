////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                SERVIDOR WEB CON PIC 18F4550 Y ENC28J60                     //
//                                                                            //
//                      ccs PIC C compiler v4.104                             //
//                                                                            //
//                         (c) RobotyPic 2015                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
// Conmuta encendido-apagado de 3 leds controlados desde la aplicación web    //
// Muestra variables leídas por el PIC: Canal analógico An0 y estado bit RE0  //
// Representa en lcd el texto introducido desde la aplicación web             //
////////////////////////////////////////////////////////////////////////////////

#include <18F4550.h>
#device ADC=8
#use delay(clock=20000000)
#fuses HS,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,VREGEN,NOPBADEN,WRTB

#byte porte = 0xF84

/** Configuración para el uso del stack tcip **/
#define STACK_USE_ICMP        1  //Módulo de respuesta ICMP (ping)
#define STACK_USE_ARP         1  //Para solucionar direccionamiento MAC de las IP
#define STACK_USE_TCP         1  //Para enviar paquetes TCP 
#define STACK_USE_HTTP        1  //Uso de las funciones http del stack.
#define STACK_USE_CCS_PICENS  1  //CCS PICENS (Embedded Ethernet) 18F4620 + ENC28J60
#define STACK_USE_MCPENC      1  //Uso del enc28j60 por el stack (stacktsk.c)
#define STACK_USE_MAC         1  //Uso de la tarjeta de red

#define HTTP_SOCKET 80         //Nº de puerto asociado al socket.

/********** Definición del patillaje de conexión al enc28j60 ******************/
/* Existen varias posibilidades:

Opción 1. No definir nada, en cuyo caso se implementará una comunicación SPI por 
software y se tomarán la definición de patillas establecida en enc28j60.c

SO  PIN_D7 ---- ENC >>>> PIC
SI  PIN_D6 ---- PIC >>>> ENC
CLK PIN_D5
CS  PIN_D4
RST PIN_D3
INT PIN_D2
WOL PIN_D1

Opción 2. Definir todas las patillas de la comunicación SPI, en cuyo caso se 
implementará una comunicación SPI por software con la definición de patillas 
elegida. Por ejemplo...

#define PIN_ENC_MAC_SO  PIN_C4   //Entrada serie de datos
#define PIN_ENC_MAC_SI  PIN_C5   //Salida serie de datos
#define PIN_ENC_MAC_CLK PIN_B4   //Señal de reloj
#define PIN_ENC_MAC_CS  PIN_B5   //Chip select
#define PIN_ENC_MAC_RST PIN_B6   //Reset
#define PIN_ENC_MAC_INT PIN_B7   //Interrupción

Opción 3. El que aquí se ha utilizado, que consiste en habilitar el uso de SPI 
por hardware del PIC y definir las patillas ajenas al hardware del módulo SPI 
(CS, INT y RST). En este caso es imprescindible definir también la patilla SO 
para que el stack (dentro de enc28j60.c) no habilite SPI por software. Da igual 
que patilla SO se defina, la que se deberá cablear será la SO real del PIC    */

#define ENC_MAC_USE_SPI 1           //Uso del SPI por hardware

#define PIN_ENC_MAC_SO  PIN_B0      //Entrada serie de datos
//#define PIN_ENC_MAC_SI  PIN_C7    //Salida serie de datos (no necesario definir)
//#define PIN_ENC_MAC_CLK PIN_B1    //Señal de reloj  (no necesario definir)
#define PIN_ENC_MAC_CS  PIN_B2      //Chip select
#define PIN_ENC_MAC_RST PIN_B3      //Reset
#define PIN_ENC_MAC_INT PIN_B4      //Interrupción

/******************************************************************************/

#define use_portd_lcd TRUE       //Uso del puerto d para control del lcd
#include <LCD420PIC18F_RyP.c>    //Carga librería del lcd de 4x20 para familia 18F

#include "tcpip/stacktsk.c"      //Carga el stack TCP/IP de Microchip 

/*********************  PAGINA WEB A MOSTRAR **********************************/
/* Página principal INDEX (/) */
const char  HTML_INDEX_PAGE[]="
<HTML><BODY BGCOLOR=#FFFFFF TEXT=#000000>

<center><H1>Servidor WEB con PIC 18f4550 </H1></center>
<BR><center><H2>ROBOTYPIC</H2></CENTER>


<FORM METHOD=GET>
<P>LCD: <INPUT TYPE=\"text\" NAME=\"lcd\" size=20 maxlength=16>
<input type=\"submit\" value=\"Enviar texto\">
</FORM>

<FORM METHOD=GET>
<input type=\"submit\" name=\"boton1\" value=\"Botón 1\">
</FORM>

<FORM METHOD=GET>
<input type=\"submit\" name=\"boton2\" value=\"Botón 2\">
</FORM>

<FORM METHOD=GET>
<input type=\"submit\" name=\"boton3\" value=\"Botón 3\">
</FORM>

<P><A HREF=\"/lecturas\">Lecturas del PIC</A>
</BODY></HTML>
";

/* Página secundaria (\lecturas). Accesible desde la página principal */
const char  HTML_LECTURAS_PAGE[]="
<meta http-equiv=\"refresh\"content=\"3;url=lecturas\">

<HTML><BODY BGCOLOR=#FFFFFF TEXT=#000000>
<P>
<center><H1>Servidor WEB con PIC 18f4550 </H1></center>
<BR><center><H2>ROBOTYPIC</H2></CENTER>
<br><center>LECTURAS</center>

<P><B>AN: </B>%0 </P>
<BR/>
<P><B>RE0: </B>%1 </P>
<BR/>


<P><A HREF=\"/\">LCD/LEDs</A>
</BODY></HTML>
";

/* Elección de MAC. No puede haber 2 dispositivos con misma MAC en una misma red
   Microchip Vendor ID  MAC: 00.04.A3.xx.xx.xx.  */
void MACAddrInit(void) {
   MY_MAC_BYTE1=0;
   MY_MAC_BYTE2=0x04;
   MY_MAC_BYTE3=0xA3;
   MY_MAC_BYTE4=0x06;
   MY_MAC_BYTE5=0x07;
   MY_MAC_BYTE6=0x08;
}

void IPAddrInit(void) {
   //Elección de la dirección IP. 
   MY_IP_BYTE1=192;
   MY_IP_BYTE2=168;
   MY_IP_BYTE3=1;
   MY_IP_BYTE4=15;

   //Elección de la dirección de puerta de enlace. 
   MY_GATE_BYTE1=192;
   MY_GATE_BYTE2=168;
   MY_GATE_BYTE3=1;
   MY_GATE_BYTE4=1;

   //Elección de la máscara de red.Si no se indica nada se tomará 255.255.255.0
   MY_MASK_BYTE1=255;
   MY_MASK_BYTE2=255;
   MY_MASK_BYTE3=255;
   MY_MASK_BYTE4=0;
}


/***************************  FUNCIÓN GET PAGE   ******************************/
/* Esta función devuelve la posición de memoria donde se encuentra la página web 
a mostrar. En este caso se trata de una web con 2 páginas. Una principal index(/) 
y una secundaria(/lecturas)                                                   */

int32 http_get_page(char *file_str) {
   int32 file_loc=0;
   static char index[]="/";
   static char lecturas[]="/lecturas";
   
   printf(lcd_putc,"\fRequest %s ",file_str);      //Muestra en lcd solicitud

   /* Busca la posición de memoria donde se encuentra la página solicitada */
   if (stricmp(file_str,index)==0)                 //Si es la principal...
      file_loc=label_address(HTML_INDEX_PAGE);     //...toma su posición en la memoria
   else if (stricmp(file_str,lecturas)==0)            //O si es la secundaria...
      file_loc=label_address(HTML_LECTURAS_PAGE);    //...toma su posición en la memoria

   /* Muestra en lcd la página solicitada */
   if (file_loc){                                  //Si existe... 
      printf(lcd_putc,"\n(FILE=%LU)",file_loc);    //...muestra en lcd pos. de memoria
   }
   else{                                           //Si no existe...
      printf(lcd_putc,"\n(File Not Found)");       //...muestra en lcd mensaje
   }
   /* Devuelve la posición en la memoria donde se encuentra la página a mostrar */
   return(file_loc);                               
}

/**************************  FUNCIÓN FORMAT CHAR  *****************************/
/* Con  la función http_format_char  interconectamos las variables virtuales de 
la página web con las variables del programa del PIC. Se encarga de enviar los 
cambios producidos en la aplicación del PIC y reflejarlos en la aplicación web. 
Muestra,por tanto, las lectura obtenidas por el PIC y las representa en la 
aplicación de la página web      

%0 es la variable virtual para representar el valor de la lectura del canal 
analógico
%1 es la variable virtual para representar el valor de la lectura del bit 0 del 
puerto E.
*/
int8 http_format_char(int32 file, char id, char *str, int8 max_ret) {
   char new_str[20];
   int8 len=0;
   int8 AD0;
   int8 RE0;

   *str=0;

   switch(id) {
      case '0':
         set_adc_channel(0);
         delay_us(100);
         AD0=read_adc();
         sprintf(new_str,"0x%X",AD0);
         len=strlen(new_str);
      break;
      case '1':
         RE0=bit_test(porte,0);
         sprintf(new_str,"%d",RE0);
         len=strlen(new_str);
      break;  
      default:
      len=0;
   }

   if (len)strncpy(str, new_str, max_ret);
   else  *str=0;
   
   return(len);
}

/***************************  FUNCIÓN EXEC CGI   ******************************/
/* Con la función http_exec_cgi interconectamos las variables virtuales de la 
página web con las variables del programa del PIC. Se encarga de recibir 
los cambios producidos en la aplicación web y reflejarlos en el hardware del PIC. 
Ejecuta, por tanto, la acción elegida según el valor de la variable virtual recibida 
de la página web

key es la variable virtual que viene de la pagina web
val es el valor de una variable virtual de la página web
file es la dirección de la página web devuelta por http_get_page ()

*/

void http_exec_cgi(int32 file, char *key, char *val) {
   static char boton1_key[]="boton1";
   static char boton2_key[]="boton2";
   static char boton3_key[]="boton3";
   static char lcd_key[]="lcd";

   printf(lcd_putc,"\fCGI FILE=%LD", file);
   printf(lcd_putc,"\nKEY=%S", key);
   printf(lcd_putc,"\nVAL=%S", val);

   /* Se ejecutará al pulsar el botón "Botón 1" en la aplicación web */
   if (stricmp(key,boton1_key)==0) {
      output_toggle(PIN_C0);
   }
   /* Se ejecutará al pulsar el botón "Botón 2" en la aplicación web */
   if (stricmp(key,boton2_key)==0) {
      output_toggle(PIN_C1);
   }
   /* Se ejecutará al pulsar el botón "Botón 3" en la aplicación web */
   if (stricmp(key,boton3_key)==0) {
      output_toggle(PIN_C2);
   }
   /* Se ejecutará al pulsar el botón "Enviar texto" en la aplicación web */
   if (stricmp(key,lcd_key)==0) {
      printf(lcd_putc,"\f%s",val);  //Muestra en el lcd el texto recibido
   }
}

/************************** FUNCIÓN PRINCIPAL *********************************/
void main(void) {

   /* Habilitación y configuración del canal analógico 0 */
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(AN0);
   set_adc_channel(0);
   delay_ms(1);
   
   /*Reset de las salidas */
   output_low(PIN_C0);
   output_low(PIN_C1);
   output_low(PIN_C2);
    
   /* Inicialización del lcd */
   lcd_init();
   printf(lcd_putc,"\fROBOTYPIC WEB SERVER");   //Mensaje de inicio en lcd 
   delay_ms(1000);
  
   /* Inicialización del Stack */
   MACAddrInit(); //Se asigna la dirección MAC elegida 
   IPAddrInit();  //Se asigna IP, mascara de red y puerta de enlace elegidos
   StackInit();   //Inicializa el stack
   
   /* Muestra la IP elegida en lcd */
   printf(lcd_putc,"\n\n IP: %u.%u.%u.%u", MY_IP_BYTE1, MY_IP_BYTE2, MY_IP_BYTE3, MY_IP_BYTE4);
   printf(lcd_putc,"\n Puerto: %u", HTTP_SOCKET);
   delay_ms(10);

   while(TRUE) {
      StackTask();
    }
}
