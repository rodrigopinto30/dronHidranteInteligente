// Utilizamos la libreria LiquidCrystal y Servo, para manejar
// el LCD 16 x 2 y los servomotores respectivamente.
#include <LiquidCrystal.h>
#include <Servo.h>
//Servomotores
Servo p1, p2;
//LED LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//Sensor Ultrasonico
int distancia = 0, duracion = 0, ping = 7;
//Sensor PIR
int pir = 13;
int valor = 0;
//Sensor Inclinacion
int valor_incli = 0;
//Sensor Temperatura
int vt = 0;
float valor_temp = 0;
// Interruptor
int gris = 8;
//contador
int cont = 0, cont1 = 0, contLoop = 0;
// bandera
bool bandera = true;
//Delay igual para todos los actuadores, asi mantengan uniformidad.
int esperar = 400;

// PINES
// Direccion de desplazamiento de los bits,
// MSBFIRST (Primero el Bit Mas Significativo.
int latchPin = 0;
// El pin por el que sale cada bit
int dataPin = 1;
// El pin que cambia cada vez que dataPin ha sido establecido
// a su valor correecto
int clockPin = 9;

void setup()
{
  pinMode(A4, INPUT);
  pinMode(gris, INPUT_PULLUP);
  pinMode(A5, INPUT);
  pinMode(pir, INPUT);

  lcd.begin(16, 2);
  lcd.print("Iniciando...");

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  cero();
}

void loop()
{
  // Si se activa el interruptor comienzan la ejecucion de los
  //  sensores y actuadores
  contLoop = 1;
  if (digitalRead(gris) == HIGH)
  {
    // Se activan los sensores y actuadores,
    // encendiendose las luces verdes.
    sensor_ping();
    sensor_pir();
    sensor_inclinacion();
    sensor_temperatura();
    contLoop = 0;
    cero();
  }
  if (!digitalRead(gris) && contLoop != 0)
  {
    delay(esperar);
    lcd.clear();
    lcd.print("Dron inmovil");
  }
}

/*Sensor Temperatura*/
void sensor_temperatura()
{
  lcd.clear();
  lcd.print("Temperatura: ");
  vt = analogRead(4);
  valor_temp = (vt - 102) / 2;
  lcd.setCursor(0, 1);
  lcd.print(valor_temp);
  delay(esperar);
  bandera = true;

  // Comienza a trabajar solo si la temp es mayor a 85°.
  while (valor_temp > 85)
  {
    if (valor_temp > 100 && digitalRead(gris) == HIGH)
    {
      lcd.clear();
      lcd.print("  Ayuda");
      lcd.setCursor(0, 1);
      lcd.print("  en proceso");
      if (digitalRead(gris) == HIGH)
      {
        cont = 0;
        cero();

        while (cont < 9 && digitalRead(gris) == HIGH)
        {
          // Comienza la cuenta ascendente para solicitar ayuda.
          cont++;
          switch (cont)
          {
          case 0:
            cero();
            lcd.clear();
            lcd.print("  Ayuda");
            lcd.setCursor(0, 1);
            lcd.print("  en proceso");
            break;
          case 1:
            uno();
            break;
          case 2:
            dos();
            break;
          case 3:
            tres();
            break;
          case 4:
            cuatro();
            break;
          case 5:
            cinco();
            break;
          case 6:
            seis();
            break;
          case 7:
            siete();
            break;
          case 8:
            ocho();
            break;
          case 9:
            nueve();
            //Se solicita ayuda a otros drones
            lcd.clear();
            lcd.print("Solicita ayuda");
            lcd.setCursor(0, 1);
            lcd.print("a un Dron");
            break;
          }
        }
      }
      // Volvemos a leer valores, esto es asi porque la temp
      // pudo descender.
      vt = analogRead(A4);
      valor_temp = (vt - 102) / 2;
      delay(esperar);
    }
    else if (valor_temp > 85 && valor_temp <= 100 && digitalRead(gris) == HIGH)
    {
      // Si la temperatura esta entre 85° y 100° apagara el incendio
      lcd.clear();
      lcd.print("  Apagando");
      lcd.setCursor(0, 1);
      lcd.print("  incendio");
      vt = analogRead(A4);
      valor_temp = (vt - 102) / 2;
      delay(esperar);
    }
    else if (valor_temp < 85 && digitalRead(gris) == HIGH)
    {
      // Cuando la temperatura es menor a 85° se concidera
      // que se termino con el foco de incendio.
      lcd.clear();
      lcd.print("  Incendio");
      lcd.setCursor(0, 1);
      lcd.print("  apagado");
      delay(esperar);
      vt = analogRead(A4);
      valor_temp = (vt - 102) / 2;
      delay(esperar);
    }
    else
    {
      // Informa que se desactivo el interruptor de forma manual o
      // por cualquier otro problema interno del dron.
      lcd.clear();
      lcd.print(" Se corto");
      lcd.setCursor(0, 1);
      lcd.print(" la corriente");
      delay(esperar);
      break;
    }
  }
}

/*Sensor PIR*/
void sensor_pir()
{
  valor = digitalRead(pir);
  lcd.clear();
  lcd.print("Movimiento: ");
  lcd.setCursor(0, 1);
  if (valor == 1)
  {
    lcd.print("detectado");
    delay(esperar);
    while (valor == 1)
    {
      // Este While siempre se ejecutara hasta que se aleje, es decir
      // que la entrada sea 0 (cero), por eso se vuelva a leer la entrada
      // en la linea 192.
      lcd.clear();
      lcd.print("Alejarse");
      valor = 0;
      valor = digitalRead(pir);
      delay(100);
    }
  }
  else
  {
    lcd.print("no detectado");
    delay(esperar);
  }
}

/*MicroServomotor*/
void servomotor(int suelo)
{
  //p1=azul y p2 = negro
  delay(esperar);
  if (suelo > 150 && suelo <= 300)
  {
    p1.write(180);
    p2.write(150);
    lcd.clear();
    lcd.print("Angulos: ");
    lcd.setCursor(0, 1);
    lcd.println("180 y 150");
  }
  else if (suelo > 100 && suelo <= 150)
  {
    // Se configura los servomotores con difrenetes angulos.
    p1.write(120);
    p2.write(100);
    lcd.clear();
    lcd.print("Angulos: ");
    lcd.setCursor(0, 1);
    lcd.print("120 y 100");
  }
  else
  {
    p1.write(90);
    p2.write(90);
    lcd.clear();
    lcd.print("Angulos: ");
    lcd.setCursor(0, 1);
    lcd.println("90 y 90");
  }
}

/*Sensor PING*/
void sensor_ping()
{
  pinMode(ping, OUTPUT);
  digitalWrite(ping, LOW);
  digitalWrite(ping, HIGH);
  digitalWrite(ping, LOW);
  pinMode(ping, INPUT);
  duracion = pulseIn(ping, HIGH);
  distancia = duracion / 29 / 2;
  lcd.clear();
  lcd.print("Distancia: ");
  lcd.setCursor(0, 1);
  lcd.print(distancia);

  if (distancia <= 300 && digitalRead(gris) == HIGH)
  {
    while (distancia <= 300 && digitalRead(gris) == HIGH)
    {
      p1.attach(6);
      p2.attach(10);
      //Llamamos a servomotor() con la distancia detectada
      servomotor(distancia);
      pinMode(ping, OUTPUT);
      digitalWrite(ping, LOW);
      digitalWrite(ping, HIGH);
      digitalWrite(ping, LOW);
      pinMode(ping, INPUT);
      duracion = pulseIn(ping, HIGH);
      distancia = duracion / 29 / 2;
    }
  }
  delay(esperar);
}

/*Sensor de inclinacion*/
void sensor_inclinacion()
{
  lcd.clear();
  lcd.print("Inclinacion: ");
  lcd.setCursor(0, 1);

  valor_incli = analogRead(A5);
  switch (valor_incli)
  {
  case 1013:
    lcd.print("Estable");
    delay(esperar);
    break;
  case 1023:
    lcd.print("No estable");
    delay(esperar);
    bandera = true;
    while (bandera)
    {
      lcd.setCursor(0, 1);
      lcd.print("Estabilizando");
      if (analogRead(A5) < 1023)
      {
        lcd.setCursor(0, 1);
        lcd.print("Estable");
        bandera = false;
        delay(esperar);
      }
    }
    break;
  }
}

/* Se definen 10 metodos que configuraran el led 7 segmentos,
   que digita de 0 a 9.
*/

void cero()
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  //0
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B10000001);
  digitalWrite(latchPin, HIGH);
  delay(100);
}

void uno()
{
  //1
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B10110111);
  digitalWrite(latchPin, HIGH);
  delay(500);
}

void dos()
{
  //2
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 24);
  digitalWrite(latchPin, HIGH);
  delay(500);
}

void tres()
{
  //3
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 18);
  digitalWrite(latchPin, HIGH);
  delay(500);
}

void cuatro()
{
  //4
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B10100110);
  digitalWrite(latchPin, HIGH);
  delay(500);
}

void cinco()
{
  //5
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B1000010);
  digitalWrite(latchPin, HIGH);
  delay(500);
}

void seis()
{
  //6
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B11000000);
  digitalWrite(latchPin, HIGH);
  delay(500);
}

void siete()
{
  //7
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 7);
  digitalWrite(latchPin, HIGH);
  delay(500);
}

void ocho()
{
  //8
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, B10000000);
  digitalWrite(latchPin, HIGH);
  delay(500);
}

void nueve()
{
  //9
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 2);
  digitalWrite(latchPin, HIGH);
  delay(500);
}
