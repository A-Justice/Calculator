/*-------------------------------------------*\
// Funciones para la gestión del teclado

// char kbd_getc()   Devuelve el código ASCII de la tecla pulsada.
//         Si no se pulsó ninguna, devuelve 0.


// Conexiones del teclado.
/* 
 
            RD0 RD1 RD2 RD3
             ^   ^   ^   ^
             |   |   |   |
           |---|---|---|---|
  RD4 ---> | 7 | 8 | 8 | - |
           |---|---|---|---|
  RD5 ---> | 4 | 5 | 6 | x |
           |---|---|---|---|
  RD6 ---> | 1 | 2 | 3 | - |
           |---|---|---|---|
  RD7 ---> | on | 0 | =| + |
           |---|---|---|---|
*/


// Caracteres ASCII asociados a cada tecla:
char const KEYS[4][4] = {{'7','8','9','/'},
                         {'4','5','6','X'},
                         {'1','2','3','-'},
                         {'#','0','=','+'}};

#byte kbd_port_d = 0xF83 


char kbd_getc()
{
  char tecla=0;
  int f,c,t,i,j;


  set_tris_d(0b00001111); // Rd7-Rd4 salidas, Rd3-Rd0 entradas

  for(f=0x10, i=0; i<4; f<<=1, i++)
  {
    for(c=0x01, j=0; j<4; c<<=1, j++)
    {
      kbd_port_d = ~f;
      delay_cycles(1);
      t = kbd_port_d & 0x0F;
      t = ~(t | 0xF0);
      if(t == c)
      {
        delay_ms(20);
        tecla=KEYS[i][j];
        while(t==c)
        {
          restart_wdt();
          t = kbd_port_d & 0x0F;
          t = ~(t | 0xF0);
        }
        break;
      }
    }
    if(tecla)
      break;
  }


  return tecla;
}
