////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                SERVIDOR WEB CON PIC 18F4550 Y ENC28J60                     //
//                                                                            //
//                      ccs PIC C compiler v4.104                             //
//                                                                            //
//                         (c) RobotyPic 2015                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
// Conmuta encendido-apagado de 3 leds controlados desde la aplicaci�n web    //
// Muestra variables le�das por el PIC: Canal anal�gico An0 y estado bit RE0  //
// Representa en lcd el texto introducido desde la aplicaci�n web             //
////////////////////////////////////////////////////////////////////////////////

#include <18F4550.h>
#device ADC=8
#use delay(clock=20000000)
#fuses HS,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,VREGEN,NOPBADEN,WRTB

#byte porte = 0xF84

/** Configuraci�n para el uso del stack tcip **/
#define STACK_USE_ICMP        1  //M�dulo de respuesta ICMP (ping)
#define STACK_USE_ARP         1  //Para solucionar direccionamiento MAC de las IP
#define STACK_USE_TCP         1  //Para enviar paquetes TCP 
#define STACK_USE_HTTP        1  //Uso de las funciones http del stack.
#define STACK_USE_CCS_PICENS  1  //CCS PICENS (Embedded Ethernet) 18F4620 + ENC28J60
#define STACK_USE_MCPENC      1  //Uso del enc28j60 por el stack (stacktsk.c)
#define STACK_USE_MAC         1  //Uso de la tarjeta de red

#define HTTP_SOCKET 80         //N� de puerto asociado al socket.

/********** Definici�n del patillaje de conexi�n al enc28j60 ******************/
/* Existen varias posibilidades:

Opci�n 1. No definir nada, en cuyo caso se implementar� una comunicaci�n SPI por 
software y se tomar�n la definici�n de patillas establecida en enc28j60.c

SO  PIN_D7 ---- ENC >>>> PIC
SI  PIN_D6 ---- PIC >>>> ENC
CLK PIN_D5
CS  PIN_D4
RST PIN_D3
INT PIN_D2
WOL PIN_D1

Opci�n 2. Definir todas las patillas de la comunicaci�n SPI, en cuyo caso se 
implementar� una comunicaci�n SPI por software con la definici�n de patillas 
elegida. Por ejemplo...

#define PIN_ENC_MAC_SO  PIN_C4   //Entrada serie de datos
#define PIN_ENC_MAC_SI  PIN_C5   //Salida serie de datos
#define PIN_ENC_MAC_CLK PIN_B4   //Se�al de reloj
#define PIN_ENC_MAC_CS  PIN_B5   //Chip select
#define PIN_ENC_MAC_RST PIN_B6   //Reset
#define PIN_ENC_MAC_INT PIN_B7   //Interrupci�n

Opci�n 3. El que aqu� se ha utilizado, que consiste en habilitar el uso de SPI 
por hardware del PIC y definir las patillas ajenas al hardware del m�dulo SPI 
(CS, INT y RST). En este caso es imprescindible definir tambi�n la patilla SO 
para que el stack (dentro de enc28j60.c) no habilite SPI por software. Da igual 
que patilla SO se defina, la que se deber� cablear ser� la SO real del PIC    */

#define ENC_MAC_USE_SPI 1           //Uso del SPI por hardware

#define PIN_ENC_MAC_SO  PIN_B0      //Entrada serie de datos
//#define PIN_ENC_MAC_SI  PIN_C7    //Salida serie de datos (no necesario definir)
//#define PIN_ENC_MAC_CLK PIN_B1    //Se�al de reloj  (no necesario definir)
#define PIN_ENC_MAC_CS  PIN_B2      //Chip select
#define PIN_ENC_MAC_RST PIN_B3      //Reset
#define PIN_ENC_MAC_INT PIN_B4      //Interrupci�n

/******************************************************************************/

#define use_portd_lcd TRUE       //Uso del puerto d para control del lcd
#include <LCD420PIC18F_RyP.c>    //Carga librer�a del lcd de 4x20 para familia 18F

#include "tcpip/stacktsk.c"      //Carga el stack TCP/IP de Microchip 

/*********************  PAGINA WEB A MOSTRAR **********************************/
/* P�gina principal INDEX (/) */
const char  HTML_INDEX_PAGE[]="
<HTML><BODY BGCOLOR=#FFFFFF TEXT=#000000>

<center><H1>Servidor WEB con PIC 18f4550 </H1></center>
<BR><center><H2>ROBOTYPIC</H2></CENTER>


<FORM METHOD=GET>
<P>LCD: <INPUT TYPE=\"text\" NAME=\"lcd\" size=20 maxlength=16>
<input type=\"submit\" value=\"Enviar texto\">
</FORM>

<FORM METHOD=GET>
<input type=\"submit\" name=\"boton1\" value=\"Bot�n 1\">
</FORM>

<FORM METHOD=GET>
<input type=\"submit\" name=\"boton2\" value=\"Bot�n 2\">
</FORM>

<FORM METHOD=GET>
<input type=\"submit\" name=\"boton3\" value=\"Bot�n 3\">
</FORM>

<P><A HREF=\"/lecturas\">Lecturas del PIC</A>
</BODY></HTML>
";

/* P�gina secundaria (\lecturas). Accesible desde la p�gina principal */
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

/* Elecci�n de MAC. No puede haber 2 dispositivos con misma MAC en una misma red
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
   //Elecci�n de la direcci�n IP. 
   MY_IP_BYTE1=192;
   MY_IP_BYTE2=168;
   MY_IP_BYTE3=1;
   MY_IP_BYTE4=15;

   //Elecci�n de la direcci�n de puerta de enlace. 
   MY_GATE_BYTE1=192;
   MY_GATE_BYTE2=168;
   MY_GATE_BYTE3=1;
   MY_GATE_BYTE4=1;

   //Elecci�n de la m�scara de red.Si no se indica nada se tomar� 255.255.255.0
   MY_MASK_BYTE1=255;
   MY_MASK_BYTE2=255;
   MY_MASK_BYTE3=255;
   MY_MASK_BYTE4=0;
}


/***************************  FUNCI�N GET PAGE   ******************************/
/* Esta funci�n devuelve la posici�n de memoria donde se encuentra la p�gina web 
a mostrar. En este caso se trata de una web con 2 p�ginas. Una principal index(/) 
y una secundaria(/lecturas)                                                   */

int32 http_get_page(char *file_str) {
   int32 file_loc=0;
   static char index[]="/";
   static char lecturas[]="/lecturas";
   
   printf(lcd_putc,"\fRequest %s ",file_str);      //Muestra en lcd solicitud

   /* Busca la posici�n de memoria donde se encuentra la p�gina solicitada */
   if (stricmp(file_str,index)==0)                 //Si es la principal...
      file_loc=label_address(HTML_INDEX_PAGE);     //...toma su posici�n en la memoria
   else if (stricmp(file_str,lecturas)==0)            //O si es la secundaria...
      file_loc=label_address(HTML_LECTURAS_PAGE);    //...toma su posici�n en la memoria

   /* Muestra en lcd la p�gina solicitada */
   if (file_loc){                                  //Si existe... 
      printf(lcd_putc,"\n(FILE=%LU)",file_loc);    //...muestra en lcd pos. de memoria
   }
   else{                                           //Si no existe...
      printf(lcd_putc,"\n(File Not Found)");       //...muestra en lcd mensaje
   }
   /* Devuelve la posici�n en la memoria donde se encuentra la p�gina a mostrar */
   return(file_loc);                               
}

/**************************  FUNCI�N FORMAT CHAR  *****************************/
/* Con  la funci�n http_format_char  interconectamos las variables virtuales de 
la p�gina web con las variables del programa del PIC. Se encarga de enviar los 
cambios producidos en la aplicaci�n del PIC y reflejarlos en la aplicaci�n web. 
Muestra,por tanto, las lectura obtenidas por el PIC y las representa en la 
aplicaci�n de la p�gina web      

%0 es la variable virtual para representar el valor de la lectura del canal 
anal�gico
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

/***************************  FUNCI�N EXEC CGI   ******************************/
/* Con la funci�n http_exec_cgi interconectamos las variables virtuales de la 
p�gina web con las variables del programa del PIC. Se encarga de recibir 
los cambios producidos en la aplicaci�n web y reflejarlos en el hardware del PIC. 
Ejecuta, por tanto, la acci�n elegida seg�n el valor de la variable virtual recibida 
de la p�gina web

key es la variable virtual que viene de la pagina web
val es el valor de una variable virtual de la p�gina web
file es la direcci�n de la p�gina web devuelta por http_get_page ()

*/

void http_exec_cgi(int32 file, char *key, char *val) {
   static char boton1_key[]="boton1";
   static char boton2_key[]="boton2";
   static char boton3_key[]="boton3";
   static char lcd_key[]="lcd";

   printf(lcd_putc,"\fCGI FILE=%LD", file);
   printf(lcd_putc,"\nKEY=%S", key);
   printf(lcd_putc,"\nVAL=%S", val);

   /* Se ejecutar� al pulsar el bot�n "Bot�n 1" en la aplicaci�n web */
   if (stricmp(key,boton1_key)==0) {
      output_toggle(PIN_C0);
   }
   /* Se ejecutar� al pulsar el bot�n "Bot�n 2" en la aplicaci�n web */
   if (stricmp(key,boton2_key)==0) {
      output_toggle(PIN_C1);
   }
   /* Se ejecutar� al pulsar el bot�n "Bot�n 3" en la aplicaci�n web */
   if (stricmp(key,boton3_key)==0) {
      output_toggle(PIN_C2);
   }
   /* Se ejecutar� al pulsar el bot�n "Enviar texto" en la aplicaci�n web */
   if (stricmp(key,lcd_key)==0) {
      printf(lcd_putc,"\f%s",val);  //Muestra en el lcd el texto recibido
   }
}

/************************** FUNCI�N PRINCIPAL *********************************/
void main(void) {

   /* Habilitaci�n y configuraci�n del canal anal�gico 0 */
   setup_adc(ADC_CLOCK_INTERNAL);
   setup_adc_ports(AN0);
   set_adc_channel(0);
   delay_ms(1);
   
   /*Reset de las salidas */
   output_low(PIN_C0);
   output_low(PIN_C1);
   output_low(PIN_C2);
    
   /* Inicializaci�n del lcd */
   lcd_init();
   printf(lcd_putc,"\fROBOTYPIC WEB SERVER");   //Mensaje de inicio en lcd 
   delay_ms(1000);
  
   /* Inicializaci�n del Stack */
   MACAddrInit(); //Se asigna la direcci�n MAC elegida 
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
