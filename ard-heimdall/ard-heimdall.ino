#include <MPU6050_tockn.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial esp (A2, A3); //rx tx

MPU6050 mpu6050(Wire);

// Arduino UNO
//ordEsp();         Função que verifica as mensagens recebidas do Esp8266
//lineFlw();        Função que segue as linhas do trajeto
//rAngle();         Função que controla o ângulo do servo motor
//Gyro();           Função que atualiza e retorna o estado do sensor giroscópio
//motorControl();   Função que controla os motores
//driveStr();       Função que utiliza o giróscopio para andar em linha reta
//driveTurn();      Função que utiliza o giróscopio para realizar a rotação do robô
//smothS();         Função que define define um trajeto, amortecido para o servo motor
//knowColor();      Função que retorna a incidência de uma cor captada pelo sensor de cor
//SetNewZero();     Função que define um novo ponto 0, para que driveStr(), siga esse ponto



#define servo 2 // servo motor

#define sOb_1 4 // sensores de obstáculo
#define sOb_2 7

#define sLn_1 A0  // sensores de linha
#define sLn_2 A1

#define m1_ah 3  // motores
#define m1_h 5
#define m2_ah 6
#define m2_h 9

#define Out 8 // Sensor de Cor
#define S0 10
#define S1 11
#define S2 12
#define S3 13

unsigned long I; // integral
unsigned long Time; //Tempo
unsigned long pastTime; // Tempo passado

float gyro = 0; // Angulo atual do Giroscópio

bool es_sOb_1, es_sOb_2; // Sensores de Obstáculo


int es_sLn_1, es_sLn_2; // Sensores de Linha

int redC = 0, x = 0, y = 0;
bool enable = false;


void setup() {
  delay(2000);
  Wire.begin();
  mpu6050.begin();

  Serial.begin(9600); //Inicial comunicação serial entre esp e arduino, e entre arduino e pc
  esp.begin(9600);

  mpu6050.calcGyroOffsets(true);

  pinMode(sOb_1, INPUT); //Sensores de Obstáculos
  pinMode(sOb_2, INPUT);

  pinMode(sLn_1, INPUT); //Sensores de Linha
  pinMode(sLn_2, INPUT);

  pinMode(Out, INPUT); //Sensor de Cor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(m1_ah, OUTPUT); //Motor 1, sentido anti-horario e horário
  pinMode(m1_h, OUTPUT);

  pinMode(m2_ah, OUTPUT); //Motor 2, sentido anti-horario e horário
  pinMode(m2_h, OUTPUT);

  Serial.println("");
  Serial.println("iniciando");
  esp.println("p");
}
void loop() {
  Time = millis(); //Contando tempo atual
  Gyro("Z");//Atualizando o giróscopio

  //Atribuindo o estado dos sensores a uma variável

  es_sOb_1 = !digitalRead(sOb_1);//Sensores de Obstáculo
  es_sOb_2 = !digitalRead(sOb_2);

  es_sLn_1 = !digitalRead(sLn_1);//Sensores de Linha
  es_sLn_2 = !digitalRead(sLn_2);


  while (enable == true) {
    Gyro("Z");

    Serial.println("etapa-1");

    while (knowColor("red") != HIGH) {

      lineFlw(-50);
      Gyro("Z");
    }
    Serial.println("etapa-2");

    motorControl(60, -60);
    delay(500);
    es_sLn_2 = LOW;
    while ( es_sLn_2 == LOW) {
      es_sLn_2 = !digitalRead(sLn_2);
      Gyro("Z");
      motorControl(29, -29);
    }
    while (knowColor("red") != HIGH) {
      lineFlw(40);
      Gyro("Z");
    }
    Serial.println("etapa-3");
    setNewZero();
    driveStr(40);
    delay(300);
    while ( redC < x) {
      Gyro("Z");
      driveStr(27);
      if (knowColor("red") == HIGH ) {
        redC++;
        delay(300);
      }
    }
    Serial.println("etapa-4");
    motorControl(-35, 35);
    delay(1150);
    es_sLn_2 = LOW;
    while ( es_sLn_2 == LOW) {
      es_sLn_2 = !digitalRead(sLn_2);
      Gyro("Z");
      motorControl(-30, 30);
    }
    Serial.println("etapa-5");
    while (knowColor("red") != HIGH) {

      lineFlw(45);
      Gyro("Z");
    }
    motorControl(0, 0);
    enable = false;
    redC = 0;
    break;

  }

  String espCm = ordEsp();
  if (espCm.indexOf("add") == 0) {
    String Cdd = espCm.substring(4, 7);
    if (Cdd == "1x1") {
      esp.println("e");
      enable = true;
      y = 3;
      x = 1;
      Serial.println("Adicionando");
    } else if (Cdd == "1x2") {
      esp.println("e");
      enable = true;
      y = 3;
      x = 2;
    } else if (Cdd == "1x3") {
      esp.println("e");
      enable = true;
      y = 3;
      x = 3;
    } else if (Cdd == "2x1") {
      esp.println("e");
      enable = true;
      y = 2;
      x = 1;
    } else if (Cdd == "2x2") {
      esp.println("e");
      enable = true;
      y = 2;
      x = 2;
    } else if (Cdd == "2x3") {
      esp.println("e");
      enable = true;
      y = 2;
      x = 3;
    } else if (Cdd == "3x1") {
      esp.println("e");
      enable = true;
      y = 1;
      x = 1;
    } else if (Cdd == "3x2") {
      esp.println("e");
      enable = true;
      y = 1;
      x = 2;
    } else if (Cdd == "3x3") {
      esp.println("e");
      enable = true;
      y = 1;
      x = 3;
    }
  }

  //
  //
  //  Serial.print("S.linha_1:");
  //  Serial.print(es_sLn_1);
  //  Serial.print(',');
  //
  //  Serial.print("S.linha_2:");
  //  Serial.print(es_sLn_2);
  //  Serial.print(',');
  //
  //    Serial.print("Giroscópio:");
  //    Serial.print(Gyro("Z"));
  //    Serial.print(',');

  //
  //  Serial.print("S.Ob_1:");
  //  Serial.print(es_sOb_1);
  //  Serial.print(',');
  //
  //  Serial.print("S.Ob_2:");
  //  Serial.print(es_sOb_2);
  //  Serial.print(',');
  //
  //  Serial.println();


}
void lineFlw(int sp) {
  es_sLn_1 = !digitalRead(sLn_1);
  es_sLn_2 = !digitalRead(sLn_2);

  int snd_sp = sp / 4;

  if (sp < 0) {
    if (es_sLn_1 == HIGH) {
      motorControl(snd_sp, sp);
    } else if (es_sLn_1 == LOW && es_sLn_2 == LOW) {
      motorControl(sp, sp);
    }
    if (es_sLn_2 == HIGH) {
      motorControl(sp, snd_sp);
    } else if (es_sLn_1 == LOW && es_sLn_2 == LOW) {
      motorControl(sp, sp);
    }
  } else {
    if (es_sLn_1 == HIGH) {
      motorControl(sp, snd_sp);
    } else if (es_sLn_1 == LOW && es_sLn_2 == LOW) {
      motorControl(sp, sp);
    }
    if (es_sLn_2 == HIGH) {
      motorControl(snd_sp, sp);
    } else if (es_sLn_1 == LOW && es_sLn_2 == LOW) {
      motorControl(sp, sp);
    }
  }
}

void driveTurn(float angle) {
  gyro = angle + Gyro("Z");
  if (gyro <= 0)
  {
    while (gyro < Gyro("Z")) {
      float Motor, error = Gyro("Z") - gyro;
      int mSp = 30 ;
      Motor = abs(error) / gyro * mSp - 45;
      motorControl(-Motor, Motor);
      Serial.println(Gyro("Z"));
    }
  } else {

    while ( Gyro("Z") < gyro) {
      float Motor, error = gyro - Gyro("Z");
      int mSp = 30 ;
      Motor = abs(error) / gyro * mSp + 45;
      motorControl(-Motor, Motor);
      Serial.println(Gyro("Z"));
    }
  }

  motorControl(0, 0);
}
void smothS(int Min, int Max) {
  int stepTime = 5;
  if (Max > Min) {
    for (float i = Min; i <= Max; i += 1) {
      rAngle(servo, i, HIGH);
      delay(stepTime);
    }
  } else {
    for (float i = Min; i >= Max; i -= 1) {
      rAngle(servo, i, HIGH);
      delay(stepTime);
    }
  }
}
bool knowColor (String color) {
  int red, blue, green;
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW); digitalRead(Out) == HIGH ? LOW :
  red = pulseIn(Out, digitalRead(Out) == HIGH ? LOW : HIGH);

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  blue = pulseIn(Out, digitalRead(Out) == HIGH ? LOW : HIGH);

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  green = pulseIn(Out, digitalRead(Out) == HIGH ? LOW : HIGH);

  if ((red > 900 && blue > 800 && green > 1000 ) && color == "black") {
    return true;
  } else if ((red > 480 && blue > 350 && (green > 390 && green < 500)) && color == "green") {
    return true;
  } else if ((red > 480 && (blue > 300 && blue < 400) && green > 560) && color == "blue") {
    return true;
  } else if (((red > 350 && red < 480) && blue > 400 && green > 600) && color == "red") {
    return true;
  } else if ((red < 420 ) && (blue < 330) && (green < 450) && color == "white") {
    return true;
  } else {
    return false;
  }


}
void setNewZero() {
  gyro = Gyro("Z");
}
void driveStr (int Speed) {
  float m1, m2, kp, error = gyro - Gyro("Z");
  int mSp = Speed;
  kp = 3;
  m1 = mSp - error * kp;
  m2 = mSp + error * kp;
  (m1 > 100 && m1 > 0) ? m1 = 100 : m1 = m1;
  (m2 > 100 && m2 > 0) ? m2 = 100 : m2 = m2;

  (m1 < -100 && m1 < 0) ? m1 = -100 : m1 = m1;
  (m2 < -100 && m2 < 0) ? m2 = -100 : m2 = m2;

  motorControl(m1, m2);
}

void motorControl ( int m1, int m2) {

  float m1_conv = m1 / 100.0 * 255.0;
  float m2_conv = m2 / 100.0 * 255.0;

  if (m1_conv < 0) {
    analogWrite(m1_ah, m1_conv * -1);
    analogWrite(m1_h, 0);
  } else {
    analogWrite(m1_ah, 0);
    analogWrite(m1_h, m1_conv);
  }
  if (m2_conv < 0) {
    analogWrite(m2_ah, m2_conv * -1);
    analogWrite(m2_h, 0);
  } else {
    analogWrite(m2_ah, 0);
    analogWrite(m2_h, m2_conv);
  }

}

float Gyro(String Ax) {
  mpu6050.update();
  float value;
  if (Ax == "X") {
    return mpu6050.getAngleX();
  }
  else if (Ax == "Y") {
    return mpu6050.getAngleY();
  }
  else if (Ax == "Z") {
    return mpu6050.getAngleZ();
  }
  else {
    return 0;
  }
}

void rAngle(int pino, float sPos, bool esSer) {
  if (esSer == HIGH) {
    float sDel = sPos / 220.0 * 2200.0 + 350.0;

    digitalWrite(pino, HIGH);

    delayMicroseconds((int)sDel);
    digitalWrite(pino, LOW);

    delayMicroseconds(10000);
    delayMicroseconds(10000 - (int)sDel);
  }
}

String ordEsp () {
  char c;
  String msg;
  if (esp.available() > 0) {
    while (esp.available() > 0) {
      c = esp.read();
      msg.concat(c);
    }
    Serial.println(msg);
    return msg;
    msg = "";
    esp.flush();
  }

}
