#include <EEPROM.h>  //LIBRERIA EEPROM
#include <Wire.h>    //LIBRERIA RTC
#include <RTClib.h>  //LIBRERIA RTC
#include <SPI.h>     //LIBRERIA SD
#include <SD.h>      //LIBRERIA SD

RTC_DS3231 sensor_rtc;  //MÓDULO DE TIEMPO RTC DS3231 PIN A4 Y A5
//File sd_txt;
//const int mudulo_sd = 2;  //LEECTOR DE SD
const int led = 13;
const int sensor_laser = 8;  //SENSOR LASER 5CM
int valor_sensor_laser;
const int sensor_movimiento = 9;  //SENSOR MOVIMIENTO 3M
int valor_sensor_movimiento;
const int fin_carrera_planta_baja = 4;  //ESTATUS FIN DE CARRERA PLANTA BAJA
int valor_fin_carrera_planta_baja;
const int fin_carrera_piso_uno = 5;  //ESTATUS FIN DE CARRERA PISO UNO
int valor_fin_carrera_piso_uno;
const int polipasto_boton_subir = 6;  //POLIPASTO BOTÓN SUBIR
int valor_polipasto_boton_subir;
const int polipasto_boton_bajar = 7;  //POLIPASTO BOTÓN BAJAR
int valor_polipasto_boton_bajar;
const int relay_polipasto_boton_subir = A0;  //RELAY POLIPASTO BOTÓN SUBIR
int valor_relay_polipasto_boton_subir;
const int relay_polipasto_boton_bajar = A1;  //RELAY POLIPASTO BOTÓN BAJAR
int valor_relay_polipasto_boton_bajar;
int estatus_elevador;
int estatus_elevador_actual;
const int control_rt = 10;  //BOTÓN CONTROL
int valor_control_rt;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(sensor_laser, INPUT);                     //SENSOR LASER
  pinMode(sensor_movimiento, INPUT);                //SENSOR MOVIMIENTO
  pinMode(fin_carrera_planta_baja, INPUT_PULLUP);   //FIN DE CARRERA PLANTA BAJA
  pinMode(fin_carrera_piso_uno, INPUT_PULLUP);      //FIN DE CARRERA PISO UNO
  pinMode(polipasto_boton_subir, INPUT_PULLUP);     //POLIPASTO BOTÓN SUBIR
  pinMode(polipasto_boton_bajar, INPUT_PULLUP);     //POLIPASTO BOTÓN BAJAR
  pinMode(control_rt, INPUT_PULLUP);                //POLIPASTO BOTÓN BAJAR
  pinMode(relay_polipasto_boton_subir, OUTPUT);     //RELAY POLIPASTO BOTÓN SUBIR
  pinMode(relay_polipasto_boton_bajar, OUTPUT);     //RELAY POLIPASTO BOTÓN BAJAR
  digitalWrite(relay_polipasto_boton_subir, HIGH);  //BOTÓN SUBIR
  digitalWrite(relay_polipasto_boton_bajar, HIGH);  //BOTÓN BAJAR
  EEPROM.put(estatus_elevador, 0);                  //VARIABLE EEPROM ESTATUS ELEVADOR
  sensor_rtc.begin();
  //SD.begin(mudulo_sd);
  //   if (!SD.begin(mudulo_sd)) {
  //   Serial.println("modulo no encontrado");
  //   //while(1);
  // }

  // //DateTime sensor_fecha = sensor_rtc.now();         //VALOR SENSOR RTC
  // if (!sensor_rtc.begin()) {
  //   Serial.println("modulo no encontrado");
  //   while(1);
  // }
  //sensor_rtc.adjust(DateTime(__DATE__, __TIME__));  //AJUSTE DE TIEMPO MÓDULO RTC

  // sd_txt = SD.open("log.txt", FILE_WRITE);
  // // if the file opened okay, write to it:
  // if (sd_txt) {
  //   Serial.print("escribiendo log.txt");
  //   // close the file:
  //   sd_txt.close();
  //   Serial.println("done.");
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.print("no se encuentra log.txt");
  // }

  delay(10000);  //ESTABILIZACIÓN SENSOR MOVIMIENTO 90 SEGUNDOS
  //delay(5000);  //ESTABILIZACIÓN SENSOR MOVIMIENTO
}

void loop() {
  valor_sensor_laser = digitalRead(sensor_laser);                        //VALOR SENSOR LASER
  valor_sensor_movimiento = digitalRead(sensor_movimiento);              //VALOR SENSOR MOVIMIENTO
  valor_fin_carrera_planta_baja = digitalRead(fin_carrera_planta_baja);  //VALOR FIN DE CARRERA PLANTA BAJA
  valor_fin_carrera_piso_uno = digitalRead(fin_carrera_piso_uno);        //VALOR FIN DE CARRERA PISO
  valor_polipasto_boton_subir = digitalRead(polipasto_boton_subir);      //VALOR POLIPASTO BOTÓN SUBIR
  valor_polipasto_boton_bajar = digitalRead(polipasto_boton_bajar);      //VALOR POLIPASTO BOTÓN BAJAR
  valor_control_rt = digitalRead(control_rt);                            //VALOR CONTROL RT
  DateTime sensor_fecha = sensor_rtc.now();                              //VALOR SENSOR RTC
  digitalWrite(relay_polipasto_boton_subir, HIGH);                       //POLIPASTO BOTÓN SUBIR DESACTIVADO
  digitalWrite(relay_polipasto_boton_bajar, HIGH);                       //POLIPASTO BOTÓN BAJAR DESACTIVADO
  EEPROM.get(estatus_elevador, estatus_elevador_actual);                 //VALOR ESTATUS ELEVADOR

  if (estatus_elevador_actual == 0 && valor_fin_carrera_planta_baja == 0) {  //ELEVADOR EN LA PLANTA BAJA
    digitalWrite(led, LOW);                                                  //APAGAR LED
    Serial.println("ELEVADOR: EN PLANTA BAJA LISTO PARA SUBIR");             //ESTATUS ELEVADOR
    EEPROM.get(estatus_elevador, estatus_elevador_actual);
    //Serial.print(estatus_elevador_actual);

    if (valor_polipasto_boton_subir == 0 || valor_polipasto_boton_bajar == 0 || valor_control_rt == 1) {
      //Serial.println("ACTIVADO");  //ESTATUS POLIPASTO BOTÓN SUBIR
      EEPROM.update(estatus_elevador, 1);
      EEPROM.get(estatus_elevador, estatus_elevador_actual);
      //Serial.print(estatus_elevador_actual);
      //digitalWrite(relay_polipasto_boton_subir, LOW);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
    } else if (valor_polipasto_boton_subir == 1 || valor_polipasto_boton_bajar == 1) {
      //Serial.println("NO ACTIVADO");  //ESTATUS POLIPASTO BOTÓN SUBIR
      EEPROM.get(estatus_elevador, estatus_elevador_actual);
      //Serial.print(estatus_elevador_actual);
    }

  } else if (estatus_elevador_actual == 1 && valor_fin_carrera_planta_baja == 0) {  //ELEVADOR EN EL PRIMER PISO
    digitalWrite(relay_polipasto_boton_subir, LOW);                                 //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
    digitalWrite(relay_polipasto_boton_bajar, HIGH);                                //POLIPASTO BOTÓN BAJAR DESACTIVADO
    Serial.println("ELEVADOR: SUBIENDO");                                           //ESTATUS ELEVADOR
  } else if (estatus_elevador_actual == 1 && valor_fin_carrera_planta_baja == 1) {  //ELEVADOR SUBIENDO
    digitalWrite(relay_polipasto_boton_subir, LOW);                                 //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
    digitalWrite(relay_polipasto_boton_bajar, HIGH);                                //POLIPASTO BOTÓN BAJAR DESACTIVADO
    Serial.println("ELEVADOR: SUBIENDO");                                           //ESTATUS ELEVADOR

    if (valor_sensor_laser == 0) {
      digitalWrite(led, HIGH);  //ENCENDER LED
      //Serial.println("OBSTÁCULO: DETECTADO");  //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: DETENIDO");             //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);  //POLIPASTO BOTÓN BAJAR DESACTIVADO
      delay(3000);
    } else if (valor_sensor_movimiento == 1) {
      digitalWrite(led, HIGH);  //ENCENDER LED
      //Serial.println("OBSTÁCULO: DETECTADO");  //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: DETENIDO");             //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);  //POLIPASTO BOTÓN BAJAR DESACTIVADO
      delay(3000);
    } else {
      digitalWrite(led, LOW);  //APAGAR LED
      //Serial.println("OBSTÁCULO: LIBRE");         //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: EN MOVIMIENTO");        //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, LOW);   //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);  //POLIPASTO BOTÓN BAJAR DESACTIVADO
    }
    if (estatus_elevador_actual == 1 && valor_fin_carrera_piso_uno == 0) {  //ELEVADOR EN EL PRIMER PISO
      digitalWrite(relay_polipasto_boton_subir, HIGH);                      //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);                      //POLIPASTO BOTÓN BAJAR DESACTIVADO
      Serial.println("ELEVADOR: EN PRIMER PISO LISTO PARA BAJAR");          //ESTATUS ELEVADOR
      EEPROM.update(estatus_elevador, 2);
      EEPROM.get(estatus_elevador, estatus_elevador_actual);
      //Serial.print(estatus_elevador_actual);
    }
  } else if (estatus_elevador_actual == 2 && valor_fin_carrera_piso_uno == 0) {

    if (valor_polipasto_boton_bajar == 0 || valor_polipasto_boton_subir == 0 || valor_control_rt == 1) {
      //Serial.println("ACTIVADO");  //ESTATUS POLIPASTO BOTÓN SUBIR
      EEPROM.update(estatus_elevador, 3);
      EEPROM.get(estatus_elevador, estatus_elevador_actual);
      //Serial.print(estatus_elevador_actual);
      //digitalWrite(relay_polipasto_boton_subir, LOW);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
    } else if (valor_polipasto_boton_bajar == 1 || valor_polipasto_boton_subir == 1) {
      //Serial.println("NO ACTIVADO");  //ESTATUS POLIPASTO BOTÓN SUBIR
      EEPROM.get(estatus_elevador, estatus_elevador_actual);
      //Serial.print(estatus_elevador_actual);
    }

  } else if (estatus_elevador_actual == 3 && valor_fin_carrera_piso_uno == 0) {
    digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: DESACTIVADO
    digitalWrite(relay_polipasto_boton_bajar, LOW);   //POLIPASTO BOTÓN BAJAR ACTIVADO: BAJAND
    Serial.println("ELEVADOR: BAJANDO");              //ESTATUS ELEVADOR
  } else if (estatus_elevador_actual == 3 && valor_fin_carrera_piso_uno == 1) {
    digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
    digitalWrite(relay_polipasto_boton_bajar, LOW);   //POLIPASTO BOTÓN BAJAR DESACTIVADO
    Serial.println("ELEVADOR: BAJANDO");              //ESTATUS ELEVADOR

    if (valor_sensor_laser == 0) {
      digitalWrite(led, HIGH);  //ENCENDER LED
      //Serial.println("OBSTÁCULO: DETECTADO");  //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: DETENIDO");             //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);  //POLIPASTO BOTÓN BAJAR DESACTIVADO
      delay(3000);
    } else if (valor_sensor_movimiento == 1) {
      digitalWrite(led, HIGH);  //ENCENDER LED
      //Serial.println("OBSTÁCULO: DETECTADO");  //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: DETENIDO");             //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);  //POLIPASTO BOTÓN BAJAR DESACTIVADO
      delay(3000);
    } else {
      digitalWrite(led, LOW);  //APAGAR LED
      //Serial.println("OBSTÁCULO: LIBRE");         //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: EN MOVIMIENTO");        //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, LOW);   //POLIPASTO BOTÓN BAJAR DESACTIVADO
    }
    if (estatus_elevador_actual == 3 && valor_fin_carrera_planta_baja == 0) {
      digitalWrite(relay_polipasto_boton_subir, HIGH);              //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);              //POLIPASTO BOTÓN BAJAR DESACTIVADO
      Serial.println("ELEVADOR: EN PLANTA BAJA LISTO PARA SUBIR");  //ESTATUS ELEVADOR
      EEPROM.update(estatus_elevador, 0);
      EEPROM.get(estatus_elevador, estatus_elevador_actual);
      //Serial.print(estatus_elevador_actual);
    }
  } else {
    digitalWrite(led, LOW);  //APAGAR LED
    //Serial.println("ELEVADOR: PUNTO DE INICIO PLANTA BAJA");
    digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
    digitalWrite(relay_polipasto_boton_bajar, LOW);   //POLIPASTO BOTÓN BAJAR DESACTIVADO

    if (valor_sensor_laser == 0) {
      digitalWrite(led, HIGH);  //ENCENDER LED
      //Serial.println("OBSTÁCULO: DETECTADO");  //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: DETENIDO");             //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);  //POLIPASTO BOTÓN BAJAR DESACTIVADO
      delay(3000);
    } else if (valor_sensor_movimiento == 1) {
      digitalWrite(led, HIGH);  //ENCENDER LED
      //Serial.println("OBSTÁCULO: DETECTADO");  //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: DETENIDO");             //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, HIGH);  //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, HIGH);  //POLIPASTO BOTÓN BAJAR DESACTIVADO
      delay(3000);
    } else {
      digitalWrite(led, LOW);  //APAGAR LED
      //Serial.println("OBSTÁCULO: LIBRE");         //SENSOR DE MOVIMIENTO
      Serial.println("ELEVADOR: PUNTO DE INICIO PLANTA BAJA");  //ESTATUS ELEVADOR
      digitalWrite(relay_polipasto_boton_subir, HIGH);          //POLIPASTO BOTÓN SUBIR ACTIVADO: SUBIENDO
      digitalWrite(relay_polipasto_boton_bajar, LOW);           //POLIPASTO BOTÓN BAJAR DESACTIVADO
    }
  }
  //Serial.print("LASER: ");
  //Serial.println(valor_sensor_laser);
  //Serial.print("MOVIMIENTO: ");
  //Serial.println(valor_sensor_movimiento);
  // Serial.print(sensor_fecha.year());
  // Serial.print("-");
  // Serial.print(sensor_fecha.month());
  // Serial.print("-");
  // Serial.print(sensor_fecha.day());
  // Serial.print(" ");
  // Serial.print(sensor_fecha.hour());
  // Serial.print(":");
  // Serial.print(sensor_fecha.minute());
  // Serial.print(":");
  // Serial.println(sensor_fecha.second());
  //Serial.print(valor_control_rt);
  delay(100);
}