/*Codigo de la calculadora

Nota a tener en cuenta: Mientras se esta mostrando el resultado de una operacion o una serie de operaciones (boton =),
no pulsar ningun otro boton, para su correcto funcionamiento, ya que le he introducido un retardo que tiene que procesar*/


#include "18F4520.h"
#fuses HS, NOPROTECT, NOWDT, BROWNOUT, PUT, NOLVP, NOXINST
#use delay(clock=8000000)
#include "lcdeasy.c" //Funciones de CCS, adaptadas a la EasyPIC.
#define LCD_DATO 1
#define LCD_COMANDO 0
#define LCD_CLEAR 0x01
#define LCD_HOME 0x02
#use fast_io(B)
#include "KBD2.c" //Control del teclado matricial

char tecla;

/*Metodo utilizado para introducir un numero, de tal manera que lo devuelve.
Tambien guarda en un puntero la operacion introducida al termimar de introducir el numero*/
float solicitarNumero(char *operacion){

   lcd_send_byte(0,1);

   float numero=0;
   int continuar=1;

   while (continuar==1){                //Controlamos que se introduzcan numeros
      
      tecla=0;
      while (tecla==0) tecla=kbd_getc();
      lcd_send_byte(0, 0);              //Posición inicial
      lcd_putc(tecla);
      
      if(tecla>=48 && tecla<=57){       //Si se ha introducido un numero guardamos y seguimos
         numero=(numero*10)+(tecla-48);
      }else{                            //En caso contrario, finalizamos
         continuar=0;
         *operacion=tecla;              //Guardamos el operador
      }
   }
   
   return numero;
}


void main(){

   float numero=0;
   float resultado=0;
   char operacion='+';
   char operacion2='+';
   
   lcd_init();                           // Comandos de inicialización del LCD.

   while(TRUE){
   
       //Las operaciones = y # queremos que sean inmediatas
   
      if (operacion=='='){                 //Si el operador pulsado es =
         lcd_send_byte(0, 1);
         printf(lcd_putc,"%f",resultado);   //Mostramos el resultado
         delay_ms(1000);
         numero=0;
      
      }
      
      if (operacion=='#'){                 //Si el operador pulsado es ON
         numero=0;
         resultado=0;
         operacion='+';
         operacion2='+';
         lcd_send_byte(0,1);
      
      }
   
      operacion2=operacion;                 //Guardamos el operador anterior en una variable para mantenerlo
      numero=solicitarNumero(&operacion);   //Introducimos numero y obtenemos nuevo operador
      
      //Las operaciones algebraicas han de hacerse sobre el numero anterior y no sobre lo que venga despues
      
      if (operacion2=='+'){                 //Si el operador pulsado es +
         resultado=resultado+numero;
      }
      
      if (operacion2=='-'){                 //Si el operador pulsado es -
         resultado=resultado-numero;
      }
      
      if (operacion2=='X'){                 //Si el operador pulsado es *
         resultado=resultado*numero;
      }
      
      if (operacion2=='/'){                 //Si el operador pulsado es /
         if(numero==0){
            printf(lcd_putc,"ERROR");
         }else{
            resultado=resultado/numero;
         }
      }
      
   }
   
}
