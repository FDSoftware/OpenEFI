/***
 *     _____                       _____ ______  _____         _   _    __      _____ __   __
 *    |  _  |                     |  ___||  ___||_   _|       | | | |  /  |    |  ___|\ \ / /
 *    | | | | _ __    ___  _ __   | |__  | |_     | |         | | | |  `| |    |___ \  \ V / 
 *    | | | || '_ \  / _ \| '_ \  |  __| |  _|    | |         | | | |   | |        \ \ /   \ 
 *    \ \_/ /| |_) ||  __/| | | | | |___ | |     _| |_        \ \_/ /  _| |_ _ /\__/ // /^\ \
 *     \___/ | .__/  \___||_| |_| \____/ \_|     \___/         \___/   \___/(_)\____/ \/   \/
 *           | |                                                                             
 *           |_|                                                                             
 */
/*
╔════════════════════════════════════════════════════════════════════════╗
║                    Open EFI V 1.5X Team:                               ║
║  Main Programer   FDSoftware | dfrenoux@gmail.com | codereactor.xyz    ║
║  Ex Co-Programer's                                                     ║
║                  FrancisJPK |                                          ║
║                  SFADalmiro | dalmiro.farinello99@gmail.com            ║
║ Program features:                                                      ║
║    ∆ Control of diesel / petrol engines up to 8 cylinders              ║ 
║    ∆ Single point and multipoint injection                             ║
║    ∆ Independent ignition                                              ║
║    ∆ Error report                                                      ║
║    ∆ Fixed injection / ignition mode                                   ║ 
╚════════════════════════════════════════════════════════════════════════╝
*/
/*-----( Importar Librerias )-----*/
#include <Arduino.h>
#include <malloc.h>
/*-----( Declarar variables :D )-----*/

/*-----( Define's )-----*/

#define mtr 1   //definir tipo de motor 0 = diesel ; 1 = nafta
#define dev 1   //habilita modo desarollo
#define alpha 1 //Habilita el modo de prubas alpha
#define cil 4   //cantidad de cilindros del motor

/*-----( Variables Globales )-----*/
//estas variables las puede usar cualquieeer funcion

int _RPM       = 0,       //las rpm :V
    _TEMP      = 0,       //temperatura
    _POS       = 0,       //posicion del cigueñal (en dientes)
    _AE        = 0,       //avance de encendido
    dnt        = 150,     //numero de dientes del cigueñal
    _MAP       = 0,       //valor en Kpa de presion del multiple de admision
    _MAR       = 0,       //valor mariposa de admision
    _RLT       = 750;     //rpm minimas para Ralenti
bool DET       = false,   //variable para indicar si el motor esta detenido 
     ACL       = false;   //indica si se esta 
/*-----( Variables C_PWM )-----*/

int AVC        = 15,      //Avance de encendido
    PMSI       = 30,      //Cantidad de dientes entre PMS
    AVCI       = 10,      //Avance de inyeccion
    PWM_FLAG_1 = 0,
    PWM_FLAG_1A= 0,
    PWM_FLAG_2 = 0,
    PWM_FLAG_3 = 0,
    INYT1      = 250,     //tiempo de inyeccion combustible
    INYT2      = 1800;    //tiempo de encendido bobina
  
/*-----( Variables RPM )-----*/

int  RPM_per   = 350;     //periodo en ms en el que se actualizan las rpm
long T_RPM_AC  = 0;       //para saber tiempo actual
long T_RPM_A   = 0;       //para saber tiempo anterior
int  _PR       = 0;       //numero de diente / pulso

/*-----( Variables INYECCION )-----*/
int INY[]     = {23,27,29,25};    //Pines de arduino que esta conectados los inyectores **CAMBIAR PINES**
int ECN[]     = {22,28,26,24};    //Pines del arduino que estan conectados a las bobinas(Encendido secuencial con 1 bobina por cilindro) **CAMBIAR PINES**
int mar       = A1,               //pin de mariposa de acelerador
    marv      = 0;                //valor actual de mariposa de acelerador
  int TINY[18][11]={//0 ;18;27;36;45;55;64;73;82;91;100 */
    /*800*/    {2 ,3 ,3 ,3 ,4 ,6 ,6 ,7 ,8 ,8 ,8 },
    /*1000*/   {2 ,3 ,3 ,4 ,5 ,8 ,8 ,8 ,10,10,10},
    /*1200*/   {2 ,5 ,5 ,5 ,6 ,9 ,9 ,10,11,12,12},
    /*1500*/   {7 ,8 ,8 ,8 ,8 ,10,10,11,13,14,14},
    /*1700*/   {9 ,10,10,10,10,11,11,12,14,15,15},
    /*2000*/   {11,12,12,12,12,13,13,14,15,16,16},
    /*2200*/   {12,14,14,14,14,15,15,16,17,18,18},
    /*2500*/   {14,16,16,16,16,17,17,17,18,19,19},
    /*2700*/   {16,18,18,18,18,19,19,19,20,21,21},
    /*3000*/   {18,20,20,20,20,21,21,21,22,23,23},
    /*3200*/   {20,22,22,22,22,23,23,23,24,25,25},
    /*3500*/   {22,24,24,24,24,25,25,25,26,27,27},
    /*3700*/   {24,26,26,26,26,27,27,27,28,29,29},
    /*4000*/   {25,27,27,27,27,28,28,28,29,30,30},
    /*4500*/   {27,28,28,29,29,30,30,30,31,32,32},
    /*5000*/   {29,31,32,32,32,33,33,33,34,35,35},
    /*5500*/   {31,33,33,33,33,33,33,34,36,37,37},
    /*6000*/   {33,34,34,34,34,34,34,35,37,38,38}
    };         //matriz tabla de inyeccion

 int VE[18][11]={//0 ;18;27;36;45;55;64;73;82;91;100 */
    /*800*/    {2 ,3 ,3 ,3 ,4 ,6 ,6 ,7 ,8 ,8 ,8 },
    /*1000*/   {2 ,3 ,3 ,4 ,5 ,8 ,8 ,8 ,10,10,10},
    /*1200*/   {2 ,5 ,5 ,5 ,6 ,9 ,9 ,10,11,12,12},
    /*1500*/   {7 ,8 ,8 ,8 ,8 ,10,10,11,13,14,14},
    /*1700*/   {9 ,10,10,10,10,11,11,12,14,15,15},
    /*2000*/   {11,12,12,12,12,13,13,14,15,16,16},
    /*2200*/   {12,14,14,14,14,15,15,16,17,18,18},
    /*2500*/   {14,16,16,16,16,17,17,17,18,19,19},
    /*2700*/   {16,18,18,18,18,19,19,19,20,21,21},
    /*3000*/   {18,20,20,20,20,21,21,21,22,23,23},
    /*3200*/   {20,22,22,22,22,23,23,23,24,25,25},
    /*3500*/   {22,24,24,24,24,25,25,25,26,27,27},
    /*3700*/   {24,26,26,26,26,27,27,27,28,29,29},
    /*4000*/   {25,27,27,27,27,28,28,28,29,30,30},
    /*4500*/   {27,28,28,29,29,30,30,30,31,32,32},
    /*5000*/   {29,31,32,32,32,33,33,33,34,35,35},
    /*5500*/   {31,33,33,33,33,33,33,34,36,37,37},
    /*6000*/   {33,34,34,34,34,34,34,35,37,38,38}
    };         //matriz tabla de Eficiencia Volumentrica

/*-----( Variables C_INY )-----*/
int INY_L = 150,   //tiempo de apertura del inyecctor en microsegundos
    INY_P = 500,   //tiempo en uS adicional para acelerar el motor
    INY_C = 25000; //Es el valor constante , que determina el tiempo de apertura para que se crea la mezcla estequiométrica (lambda=1 ) , para cilindrada del motor , presión a 100kPa , temperatura del aire a 21ºC y VE 100% .
/*-----( Variables _LMB )-----*/
bool LMBM = false; //en true si se utilizan las dos sondas lambda
bool LMBM2= false; //en true si esta en ciclo cerrado, sino ciclo abierto
int
    LMBA  = A4,
    LMBB  = A5,
    CTA   = 250, //Correcion de tiempo A, mezcla rica, se le sacan X uS
    CTB   = 300; //Correcion de tiempo B, mezcla pobre, se le agregan X uS
    P_LMB = 250; //periodo en mS en el que se corrije por sonda lamba
    T_LMB = 45;  //temperatura a partir de la cual se intenta correjir el tiempo de inyeccion

float FLMBA = 1.5,  //factor maximo de lambda
      FLMBB = 0.85; //factor minimo de lambda
int  RAM_per   = 1200;     //periodo en ms en el que se actualiza la ram libre

long T_LMB_AC  = 0.        //para saber tiempo actual
     T_LMB_A   = 0;       //para saber tiempo anterior

/*-----( Variables Comunicacion )-----*/
bool _msg     = false;           //si esta en true hay un mensaje nuevo del puerto serie
String msg    = "";              //Mensaje que llego por serie

/*-----( Variables Ciclos )-----*/
int i    = 0,     //se usa para for
    i2   = 0;     //se usa para for anidado 1° nivel [for(i){for(i2){}}]

/*-----( Variables Free RAM )-----*/
int  RAM_per   = 1200;     //periodo en ms en el que se actualiza la ram libre
long T_RAM_AC  = 0;        //para saber tiempo actual
long T_RAM_A   = 0;       //para saber tiempo anterior
extern char _end;
extern "C" char *sbrk(int i);
char *ramstart=(char *)0x20070000;
char *ramend=(char *)0x20088000;

void setup(){
     Serial.begin(115200);
     //Iniciamos "hilos"
     Serial.println("DBG ARANCAMO WASHOOOO");
     for(i = 0; i < 4; i++){
       pinMode(INY[i], OUTPUT);
       pinMode(ECN[i], OUTPUT);
    }
    pinMode(A4, INPUT);
    pinMode(A8, INPUT);
    pinMode(6, OUTPUT);
    Serial.println("LMB 1.2");
    Serial.println("00V 12.45");
    Serial.println("INY 3.82");
    Serial.println("AVC 12.3");
    attachInterrupt(digitalPinToInterrupt(2), I_RPM, CHANGE);
    Serial.print("DBG Tarde: ");
    Serial.print(millis());
    Serial.println("mS en arrancar esta mierda :V");
}

void loop(){
    //este hilo se encarga de la comunicacion con pc y programacion

    if(_msg){   //Nos llego un mensaje desde la pc y nos ponemo re felices :3 XDD
       Serial.print("DBG Mensaje:");
       Serial.println(msg);
     //  msg  = ""; //vaciamo la variable para que no rompa las bola :3
     //  _msg = false;
    }
    C_INY();
    HILO_1();
    C_PWM();
    vent();
    #if dev == 1
      FXM();
    #endif
}

//funcion para manejo de enventos por serie
void serialEvent(){
    while(Serial.available()){
        char _T = (char)Serial.read();
        msg += _T;
        if (_T == '\n') {
            _msg = true;
        }
    }
}

void HILO_1(){
  //RPM / Temp / Sensores
  T_RPM_AC = millis();
  if (T_RPM_AC - T_RPM_A >= RPM_per) {
      T_RPM_A = T_RPM_AC;
      int _X_RPM = (_PR / dnt)*90; //calculo para obtener las rpm
      Serial.print("RPM ");
      Serial.println(_X_RPM);
      _PR = 0;
      Serial.print("AVC ");
      Serial.println(AVC_E());
    }

    T_RAM_AC = millis();
    if (T_RAM_AC - T_RAM_A >= RAM_per) {
      //Actualizamos la memoria libre y algunas pavadas mas :V
      T_RAM_A = T_RAM_AC;
      char *heapend=sbrk(0);
      register char * stack_ptr asm ("sp");
      struct mallinfo mi=mallinfo();
      Serial.print("RAM  ");
      float RAM = (stack_ptr - heapend + mi.fordblks) * 0.001 ;
      Serial.println(RAM);
      delay(5);
      Serial.print("TMP ");
      Serial.println(temp());
      delay(5);
      Serial.print("LAD ");
      Serial.println(_vmar());
    }
      
}

int temp(){
  //esta funcion mide la temperatura y devuelve float en celsius
  //**solo para sensor LM35**
  #if alpha == 0
    int value = analogRead(sensorT);
    float millivolts = (value / 1023.0) * 5000;
    float celsius = millivolts / 10; 
    return celsius;
  #endif

  #if alpha == 1
     int val = analogRead(A4);
     return map(val, 0, 1023, 0 , 120);
  #endif
}


void vent(){
  //funcion que controla el ventilador :V
  analogWrite(6, map(temp(),0,120,0,254));
}

int _vmar(){
  //funcion para obtener la posicion de la mariposa de aceleracion
  #if alpha == 1
    int val = analogRead(A8);
    return map(val, 0, 1023, 0 , 99);
  #endif
}
void I_RPM(){ //interrupcion para rpm
    _PR++;
    _POS++;
    if(_POS >= dnt){
      _POS = 0;
    }
    PWM_FLAG_1++;
    PWM_FLAG_1A++;
}

void C_INY(){
    //funcion que maneja el control de la inyeccion del motor
    if(_TEMP >= 46 && !ACL){
        INYT1 = Tiny(_RPM, _MAR,1);
    }
    if(_RPM < _RLT && !ACL){
        INYT1 = Tiny(_RPM, _MAR,3);
    }
}

int Tiny(int rpm2, int marv2, int OP){
    //esto hace toda la magia :V
    //con esta funcion se obtiene el tiempo base de inyeccion y luego se puede...
    //recalcular dependiendo de la necesidad
	int rpm3  = map(rpm2,0,7000,0,17); //aproximamos las rpm
	int marv3 = map(marv2,0,255,0,10); //aproximamos el valor de la mariposa de aceleracion
	switch(OP){ 
        case 0://Tiempo de inyeccion base
            return TINY[marv3,rpm3];
         break;

        case 1://Tiempo de inyeccion base + correccion por lambda
            return _LMB(TINY[marv3,rpm3]);
         break;
         case 2: //ALPHA-N con MAP
            return _APHPLUS();
         break;
         case 3: //Tiempo de inyeccion para arrancar
            return INY_L + (INY_P *2);
         break;
    }
}

int _APHPLUS(){ //ALPHA-N con MAP
/*
 PW = INJ_CONST * VE(tps,rpm) * MAP * AirDensity + AccEnrich +InjOpeningTime

 PW (pulse width) ---el tiempo final de apertura del inyector .

 INJ_CONST ----Es el valor constante , que determina el tiempo de apertura para que se crea la mezcla estequiométrica (lambda=1 ) , para cilindrada del motor , presión a 100kPa , temperatura del aire a 21ºC y VE 100% .

 VE para Speed Density (map, rpm)----Valor de eficiencia volumétrica tomada de la mapa VE .

 VE para ALPHA-N (tps, rpm)----Valor de eficiencia volumétrica tomada de la mapa VE .

 MAP(manifold absolute pressure)--- Presión tomada en el colector de admisión .

 AirDensity ---- Porcentual diferencia de densidad de aire comparada con la densidad de aire a temperatura 21ºC

 AccEnrich (acceleration enrichment) ---- Enriquecimiento de la mezcla en fase de aceleración .

 InjOpeningTime ---- Tiempo de apertura de inyector hasta el momento de inicio de inyección de combustible ( valor de retardo tomado de la mapa de calibración , INJECTORS CAL. )
*/
 return INY_C * VE[_MAP, _rpm] * _MAP * 1.20 + INY_P + INY_L ;
}
int _LMB(int T){

    //Esta funcion mide la/s sonda lambda y corrige la mezcla / 
    //tiempo de inyeccion
    float _LMB_XA = analogRead(LMBA) * (3.3 / 1023); //medimos la sonda A
    float _LMB_XB = 0;
    //V menor a 0.45 = mezcla pobre
    if(_LMB_XA < 0.45){ //aca comparo por Volt pero tendriaaaa que hacerlo con los el factor lamba :S
        T = T + CTB;
    } else if(_LMB_XA > 0.45){ //V mayor a 0.45 = mezcla rica
        T = T - CTA;
    }
    if(LMBM){ 
        _LMB_XB = analogRead(LMBB) * (3.3 / 1023); //medimos la sonda B (Si corresponde)
        //V menor a 0.45 = mezcla pobre
        if(_LMB_XB < 0.45){ //aca comparo por Volt pero tendriaaaa que hacerlo con los el factor lamba :S
             T = T + CTB;
        } else if(_LMB_XB > 0.45){ //V mayor a 0.45 = mezcla rica
            T = T - CTA;
        }
    }
    return T;
}

void C_PWM(){
 bool C1 = (PWM_FLAG_1 >= (PMSI - AVCI)); //Condicional para inyeccion
 #if mtr == 1
 bool C2 = (PWM_FLAG_1A >= (PMSI - AVC));  //Condicional para encendido
 #endif
//*_*_*_*_*_*CONTROL INYECCION*_*_*_*_*_*_*
  if(C1){
     digitalWrite(INY[PWM_FLAG_2],!digitalRead(INY[PWM_FLAG_2]));
     delayMicroseconds(INYT1);
     digitalWrite(INY[PWM_FLAG_2],!digitalRead(INY[PWM_FLAG_2]));
     PWM_FLAG_2++;
     PWM_FLAG_1 = 0;
  }
  if(PWM_FLAG_2 > (cil - 1)){
     PWM_FLAG_2 = 0;
  }
//*_*_*_*CONTROL DE ENCENDIDO *_*_*_*_*_*_*_*_*_*_*
#if mtr == 1
  if(C2){
    digitalWrite(ECN[PWM_FLAG_3],!digitalRead(ECN[PWM_FLAG_3]));
    delayMicroseconds(INYT2);
    digitalWrite(ECN[PWM_FLAG_3],!digitalRead(ECN[PWM_FLAG_3]));
    PWM_FL AG_3++;
    PWM_FLAG_1A = 0;
  }
  if(PWM_FLAG_3 > (cil - 1)){
    PWM_FLAG_3 = 0;
  }
#endif
}

#if (mtr == 1)

int tablaAvance[18][11]={//0 ;18;27;36;45;55;64;73;82;91;100 */
                /*800*/    {2 ,3 ,3 ,3 ,4 ,6 ,6 ,7 ,8 ,8 ,8 },
                /*1000*/   {2 ,3 ,3 ,4 ,5 ,8 ,8 ,8 ,10,10,10},
                /*1200*/   {2 ,5 ,5 ,5 ,6 ,9 ,9 ,10,11,12,12},
                /*1500*/   {7 ,8 ,8 ,8 ,8 ,10,10,11,13,14,14},
                /*1700*/   {9 ,10,10,10,10,11,11,12,14,15,15},
                /*2000*/   {11,12,12,12,12,13,13,14,15,16,16},
                /*2200*/   {12,14,14,14,14,15,15,16,17,18,18},
                /*2500*/   {14,16,16,16,16,17,17,17,18,19,19},
                /*2700*/   {16,18,18,18,18,19,19,19,20,21,21},
                /*3000*/   {18,20,20,20,20,21,21,21,22,23,23},
                /*3200*/   {20,22,22,22,22,23,23,23,24,25,25},
                /*3500*/   {22,24,24,24,24,25,25,25,26,27,27},
                /*3700*/   {24,26,26,26,26,27,27,27,28,29,29},
                /*4000*/   {25,27,27,27,27,28,28,28,29,30,30},
                /*4500*/   {27,28,28,29,29,30,30,30,31,32,32},
                /*5000*/   {29,31,32,32,32,33,33,33,34,35,35},
                /*5500*/   {31,33,33,33,33,33,33,34,36,37,37},
                /*6000*/   {33,34,34,34,34,34,34,35,37,38,38}
                };         //matriz tabla de avance

int AVC_E(){
    //----CONTROL DE AVANCE ANTES DE MANDAR CHISPA----
    if(_RPM < 600){
    //controlador de avance en frio
        return avanceArranque();
    }else if(_RPM >=600){
    return avanceNormal();
    }
    //---------------FIN DE CONTROL DE AVANCE PARA PROCEDER A MANDAR CHISPA-----------
  }
//--------------------BLOQUE DE CONTROLADORES DE AVANCE--------------------------

int avanceArranque(){
  int avc2 = dientes(3);
  return avc2;
}

int avanceNormal(){
  int temperatura = temp();
  int indiceTemp = 0;//indice que indica a que columna de la matriz acceder(temperatura)
  int indiceRPM = 0;//indice que indica a que fila de la matriz acceder(RPM)
	
  //obtener el avance por tabla segun temperatura y rpm
  indiceTemp = map(temperatura,0,100,0,10);//obtener la columna correspondiente en tabla
  indiceRPM = map(_RPM,0,8000,0,17);//obtener la fila correspondiente en tabla
	
  //finalmente accedemos al valor en tabla correspondiente al estado actual del motor
  int avc2 = dientes(tablaAvance[indiceRPM][indiceTemp]);
  
  return dientes(avc2);//retornamos el avance en cantidad de dientes
  
}
//--------------------FIN DE BLOQUE DE CONTROLADORES DE AVANCE----------------

#endif

//convierte grados en dientes del sensor hall
int dientes(float grados){
  float grad = 360 / dnt; //dividimos 360 grados por la cantidad de dientes
  //dividimos grados por grad, luego multiplicamos por 100 para transformar el float en int
  int x2     = (grados / grad) * 100; 
  //dividimos por 100, al hacer esto se eliminan los decimales, en prox ver redondear
  int dnt2    = x2 / 100;
  return dnt2; 
}

/*###############################################
########### Fixed Mode Loop & Control############  
#################################################*/

void FXM(){
    //este seria seudo loop que tendria el modo fijo,

    bool _FM = true;
    String _t = "";
    if(_msg && msg.startsWith("FXD.E")){
        do{
            C_PWM();
            HILO_1();
            vent();

            if(_msg){
                if(msg.startsWith("FXD.D")){ // "FX.D" Termina el modo fijo de inyeccion
                    _FM = false;
                }else if(msg.startsWith("INY")){ // "INY" cambia el tiempo de inyeccion
                    _t = msg.erase(0,3);
                    INYT1 = _t.toInt();
                }else if(msg.startsWith("AVC")){ // "AVC" cambia el tiempo de avance
                    _t = msg.erase(0,3);
                    AVC = _t.toInt();
                }
                //falta la parte de arranque
                _msg = false;
            }
        }while(_FM);
    }
}
