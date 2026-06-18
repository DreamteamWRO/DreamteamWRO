#include <Servo.h>


// ======================================================
//                    CONFIGURACIÓ
// ======================================================


// ---------- MOTORS ----------
const int IN3 = 10;
const int IN4 = 11;

const int ENB = 5;


// ---------- SERVO DIRECCIÓ ----------
Servo direccion;
const int servoPin = 7;


// Posicions del servo
const int CENTRE =80 ;
const int GIR_ESQUERRA = 50;
const int GIR_DRETA = 105;


// ---------- SENSOR ULTRASÒNIC ESQUERRA ----------
const int trigIzq = 8;
const int echoIzq = 9;


// ---------- SENSOR ULTRASÒNIC DRETA ----------
const int trigDer = 2;
const int echoDer = 3;

// ---------- SENSOR ULTRASÒNIC Frontal ----------
const int trigFro = 4;
const int echoFro = 12;

// ======================================================
//                  CONFIGURACIÓ
// ======================================================


// Distància per començar a evitar obstacle
const int distanciaSegura = 35;


// Distància crítica
const int distanciaCritica = 25;


// Velocitats
const int velocidadAlta = 225;
const int velocidadMedia = 200;
const int velocidadBaja = 140;


// ======================================================
//                        SETUP
// ======================================================


void setup() {

Serial.begin(9600);
 // Motors

 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);

 pinMode(ENB, OUTPUT);


 // Sensors
 pinMode(trigIzq, OUTPUT);
 pinMode(echoIzq, INPUT);


 pinMode(trigDer, OUTPUT);
 pinMode(echoDer, INPUT);

pinMode(trigFro, OUTPUT);
 pinMode(echoFro, INPUT);

 // Servo
 direccion.attach(servoPin);
 direccion.write(CENTRE);


 // Arrancar motors endavant
 avanzar();


 // Velocitat inicial
 moverMotores(velocidadAlta);


 delay(550);
}


// ======================================================
//                  DISTANCIA A MONITOR
// ======================================================


void loop() {


 // Llegir sensors
 int distanciaIzq = medirDistancia(trigIzq, echoIzq);
 delay(5);


 int distanciaDer = medirDistancia(trigDer, echoDer);
 delay(5);

 int distanciaFro = medirDistancia(trigFro, echoFro);
 delay(5);


 
 Serial.print("Izq: ");
 Serial.print(distanciaIzq);


 Serial.print("  Der: ");
 Serial.println(distanciaDer);

 Serial.print("  Fro: ");
 Serial.println(distanciaFro);
 

 // ==================================================
 //              DETECTAR OBSTACLES
 // ==================================================


 // Si obstacle a l'esquerra -> gira dreta
 if (distanciaIzq < distanciaSegura &&
     distanciaDer >= distanciaSegura) {


   direccion.write(GIR_DRETA);


   moverMotores(velocidadMedia);
 }


 // Si obstacle a la dreta -> gira esquerra
 else if (distanciaDer < distanciaSegura &&
          distanciaIzq >= distanciaSegura) {


   direccion.write(GIR_ESQUERRA);


   moverMotores(velocidadMedia);
 }


 // ==================================================
 //       SI HI HA OBSTACLE ALS DOS COSTATS
 // ==================================================


 else if (distanciaIzq < distanciaSegura &&
          distanciaDer < distanciaSegura) {


   // Escollir costat amb més espai


   if (distanciaIzq > distanciaDer) {


     // Més espai esquerra
     direccion.write(GIR_ESQUERRA);


   } else {


     // Més espai dreta
     direccion.write(GIR_DRETA);
   }


   moverMotores(velocidadBaja);
 }


 // ==================================================
 //                  NO OBSTACLES
 // ==================================================


 else {


   direccion.write(CENTRE);


   moverMotores(velocidadAlta);
 }


 // ==================================================
 //              DISTÀNCIA CRÍTICA
 // ==================================================


 if (distanciaFro < 15) {


   // Frenar una mica
   parar();


   delay(1000);


   // Tornar a avançar
   avanzar();


   // Girar cap al costat amb més espai
   if (distanciaIzq > distanciaDer) {


     direccion.write(GIR_ESQUERRA);


   } else {


     direccion.write(GIR_DRETA);
   }


   moverMotores(velocidadBaja);


   delay(300);
 }


 delay(40);
}


// ======================================================
//                DISTÀNCIA
// ======================================================


int medirDistancia(int trig, int echo) {


 digitalWrite(trig, LOW);
 delayMicroseconds(2);


 digitalWrite(trig, HIGH);
 delayMicroseconds(10);


 digitalWrite(trig, LOW);


 long duracion = pulseIn(echo, HIGH, 25000);


 int distancia = duracion * 0.034 / 2;


 // Si no detecta res
 if (distancia == 0) {
   distancia = 300;
 }


 return distancia;
}


// ======================================================
//                    AVANÇAR
// ======================================================


void avanzar() {

 digitalWrite(IN3, HIGH);
 digitalWrite(IN4, LOW);
}


// ======================================================
//                      PARAR
// ======================================================


void parar() {

 digitalWrite(IN3, LOW);
 digitalWrite(IN4, HIGH);
 direccion.write(GIR_ESQUERRA);
}


// ======================================================
//                 VELOCITAT MOTORS
// ======================================================


void moverMotores(int velocidad) {

 analogWrite(ENB, velocidad);
}