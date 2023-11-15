#include <SPI.h>
#include <mcp2515.h>
int sensorPin = A0;    // select the input pin for the potentiometer
int ledPin = 9;
int decisionPin = 7;
int Pin = 4;
int LDR_Read, value, ledV, LDR_Read2, decision, value1, value2;
int person = 1, person1 = 1;
int bits_led;
float tempo_decorrido, tempo_outro;
// custo = 0.1598 kWh / 3600 segundos -> €/kWs , valor retirado do site da EDP
float custo = (0.1598 / 3600), custo1, custo_outro; 
float power = 0.001 /*kW*/, power1, power_outro, total_power;
int BITS0, BITS1;
int c, decisao, h, decisao1;
float LUX11, LUX22, LUX33, flicker, fli, LUX1, LUX2, LUX3, flicker1, fli1, timee_segundos, timee_segundos1;
int timeini, timefin, timee, timeini1, timefin1, timee1;
float FLICKER_TOTAL, FLICKER_ERROR, FLICKER_TOTAL1, FLICKER_ERROR1, FLICKER_CONJUNTO, N, N1;
float EXT0, EXT1, EXT, EXTE;
float tempo_decorrido1, tempo_outro1, duty1, duty_outro1;
unsigned long counter = 0, LUC2, BITSLED, current, OCU;
float ldrV, ldrR, ldrLux, ledLux, ldrLux2, Des_lux, LUC1, LUC, ldrV1, ldrR1, LUC_OUTRO, infl;
float GAIN1, GAIN2, GAIN21, GAIN12, dif;
float duty_outro, duty, LUXES;
float i, j, Bellow, Total, Visibility, Bellow1, Total1, Visibility1, Visibility_sum0, Visibility_sum1, TotalVisibility;
int Des_lux00 = 10, Des_lux01 = 20, Des_lux10 = 10, Des_lux11 = 20;
String leitura = "";
float tempo, tempo_inicial, tempo_final, energy = 0, energy_outro = 0, total_energy;
MCP2515 mcp2515(10); //SS pin 10
union my_can_msg {
  uint32_t value;
  uint8_t bytes[4];
};

//###########################################################

MCP2515::ERROR write(uint32_t id, uint32_t val) {
  can_frame frame;
  //decision = digitalRead(decisionPin);
  frame.can_id = decision;
  frame.can_dlc = 4;
  my_can_msg msg;
  msg.value = val;
  for (int i = 0; i < 4; i++)
    frame.data[i] = msg.bytes[i];
  return mcp2515.sendMessage(&frame);

}
MCP2515::ERROR read(unsigned long &c) {
  can_frame frame;
  my_can_msg msg;
  MCP2515::ERROR err = mcp2515.readMessage(&frame);
  decision = digitalRead(decisionPin);
  if (err == MCP2515::ERROR_OK) {
    for (int i = 0; i < 4; i++)
      msg.bytes[i] = frame.data[i];
    c = msg.value;

  }
  return err;
}



void pedido() { //ESTA FUNCAO DÁ RESPOSTA AS MENSAGEM ENVIADAS PARA O SERIAL DO ARDUINO. SE A MENSAGEM LIDA FOR DIFERENTE DAS AQUI PRESENTES, RETORNA "err".
  if (Serial.available() > 0) {
    leitura = Serial.readStringUntil('\n');
    
    if (leitura == "g x <1>") {
        EXT = LUXES - (BITS0 * GAIN1) - (BITS1 * GAIN21);
        if (EXT<0) {
          EXT = 0;
          }
    }

    if (leitura == "g x <2>") {
        EXTE = LUC_OUTRO - BITS1 * GAIN2 - BITS0 * GAIN1;
        if (EXTE<0){
          EXTE = 0;
          }
        Serial.print("x <2> <"); Serial.print(EXTE); Serial.println(">");
    }

    else if (leitura == "g l <1>") {
        Serial.print("g <1> <"); Serial.print(LUXES); Serial.println(">");
    }

    else if (leitura == "g l <2>") {
        Serial.print("g <2> <"); Serial.print(LUC_OUTRO); Serial.println(">");
    }

    else if (leitura == "g o <1>") {
        Serial.print("g <1> <"); Serial.print(person); Serial.println(">");
    }

    else if (leitura == "g o <2>") {
        Serial.print("g <2> <"); Serial.print(person1); Serial.println(">");
    }


    else if (leitura == "o <1> <1>") {
      if (decision == 0) {
        person = 1;
        Des_lux01 = 20;
        Serial.println("ack ");
      }
    }
    else if (leitura == "o <1> <0>") {
      if (decision == 0) {
        person = 0;
        Des_lux00 = 10;
        Serial.println("ack ");
      }
    }

    /*else if (leitura == "o <2> <1>") {
      if (decision == 1) {
        person1 = 1;
        Des_lux11 = 20;
        Serial.println("ack ");
      }
    }
    else if (leitura == "o <2> <0>") {
      if (decision == 1) {
        person1 = 0;
        Des_lux10 = 10;
        Serial.println("ack ");
      }
    }*/

    else if (leitura == "O <1> <30>") {
      if (decision == 0) {
        Des_lux01 = 30;
        Serial.println("ack ");
      }
      //Set lower bound on illuminance for Occupied state at desk <i> with <VAL>
    }

    else if (leitura == "O <1> <20>") {
      if (decision == 0) {
        Des_lux01 = 20;
        Serial.println("ack ");
      }
      //Set lower bound on illuminance for Occupied state at desk <i> with <VAL>
    }

    else if (leitura == "O <1> <40>") {
      if (decision == 0) {
        Des_lux01 = 40;
        Serial.println("ack ");
      }
      //Set lower bound on illuminance for Occupied state at desk <i> with <VAL>
    }

    /*else if (leitura == "O <2> <30>") {
      if (decision == 1) {
        Des_lux11 = 30;
        Serial.println("ack ");
      }
      //Set lower bound on illuminance for Occupied state at desk <i> with <VAL>
    }*/

    else if (leitura == "U <1> <7>") {
      if (decision == 0) {
        Des_lux00 = 7;
        Serial.println("ack ");
      }
      //Set lower bound on illuminance for unoccupied state at desk <i> with <VAL>
    }

    else if (leitura == "U <1> <10>") {
      if (decision == 0) {
        Des_lux00 = 10;
        Serial.println("ack ");
      }
      //Set lower bound on illuminance for unoccupied state at desk <i> with <VAL>
    }

    else if (leitura == "U <1> <15>") {
      if (decision == 0) {
        Des_lux00 = 15;
        Serial.println("ack ");
      }
      //Set lower bound on illuminance for unoccupied state at desk <i> with <VAL>
    }

    /*else if (leitura == "U <2> <7>") {
      if (decision == 1) {
        Des_lux01 = 7;
        Serial.println("ack ");
      }
      //Set lower bound on illuminance for Unoccupied state at desk <i> with <VAL>
    }*/


    else if (leitura == "g L <1>") {
        Serial.print("L <1> <"); Serial.print(Des_lux00); Serial.println(">");
    }

    else if (leitura == "g L <2>") {
        Serial.print("L <2> <"); Serial.print(Des_lux10); Serial.println(">");
    }


    else if (leitura == "g U <1>") {
        Serial.print("U <1> <"); Serial.print(Des_lux00); Serial.println(">");
    }


    else if (leitura == "g U <2>") {
        Serial.print("U <2> <"); Serial.print(Des_lux10); Serial.println(">");
    }

    else if (leitura == "g O <1>") {
        Serial.print("O <1> <"); Serial.print(Des_lux01); Serial.println(">");
    }


    else if (leitura == "g O <2>") {
        Serial.print("O <2> <"); Serial.print(Des_lux11); Serial.println(">");
    }

    else if (leitura == "g r <1>") {
      if (person == 0) {
        Serial.print("r <1> <"); Serial.print(Des_lux00); Serial.println(">");
      }
      else if (person == 1) {
        Serial.print("r <1> <"); Serial.print(Des_lux01); Serial.println(">");
      }
    }


    else if (leitura == "g r <2>") {
      if (person1 == 0) {
        Serial.print("r <2> <"); Serial.print(Des_lux10); Serial.println(">");
      }
      else if (person1 == 1) {
        Serial.print("r <2> <"); Serial.print(Des_lux11); Serial.println(">");
      }
    }


    else if (leitura == "ganhos") {
      Serial.print("Ganho_Primeiro:  ");Serial.println(GAIN1);
      Serial.print("Ganho_Segundo:   ");Serial.println(GAIN2);
      Serial.print("Ganho_segundo_no_primeiro:  ");Serial.println(GAIN21);
      Serial.print("Ganho_primeiro_no_segundo:   ");Serial.println(GAIN12);
      Serial.print("lux_exterior_primeiro:  ");Serial.println(EXT0);
      Serial.print("lux_exterior_segundo:   ");Serial.println(EXT1);
    }


    else if (leitura == "g e <1>") {
        Serial.print("e <1> <"); Serial.print(energy); Serial.println(">");
    }


    else if (leitura == "g e <2>") {
        Serial.print("e <2> <"); Serial.print(energy_outro); Serial.println(">");
    }


    else if (leitura == "g e T") {
      Serial.print("e T <"); Serial.print(total_energy); Serial.println(">");
    }





    else if (leitura == "g t <1>") {

        Serial.print("t <1> <"); Serial.print(tempo_decorrido); Serial.println(">");
    }

    else if (leitura == "g t <2>") {
        Serial.print("t <1> <"); Serial.print(tempo_outro); Serial.println(">");
    }



    else if (leitura == "g d <1>") {
        Serial.print("d <1> <"); Serial.print(duty); Serial.println(">");
    }

    else if (leitura == "g d <2>") {
        Serial.print("d <2> <"); Serial.print(duty_outro); Serial.println(">");
    }

    else if (leitura == "g f <1>") {
      Serial.print("f <1> <"); Serial.print(FLICKER_ERROR); Serial.println(">");
    }

    else if (leitura == "g f <2>") {
      Serial.print("f <2> <"); Serial.print(FLICKER_ERROR1); Serial.println(">");
    }

    else if (leitura == "g f T") {
      Serial.print("f T <"); Serial.print(FLICKER_CONJUNTO); Serial.println(">");
    }

    else if (leitura == "g v <1>") {
      Serial.print("v <1> <"); Serial.print(Visibility); Serial.println(">");
    }

    else if (leitura == "g v <2>") {
      Serial.print("v <2> <"); Serial.print(Visibility1); Serial.println(">");
    }

    else if (leitura == "g v T") {
      Serial.print("v T <"); Serial.print(TotalVisibility); Serial.println(">");
    }

    else if (leitura == "g c <1>") {
      Serial.print("c <1> <"); Serial.print(custo1); Serial.println(">");
    }

    else if (leitura == "g c <2>") {
      Serial.print("c <2> <"); Serial.print(custo_outro); Serial.println(">");
    }
    

    else if (leitura == "g p <1>") {
      Serial.print("p <1> <"); Serial.print(power1); Serial.println(">");
    }

    else if (leitura == "g p <2>") {
      Serial.print("p <2> <"); Serial.print(power_outro); Serial.println(">");
    }

    else if (leitura == "g p T") {
      Serial.print("p T <"); Serial.print(total_power); Serial.println(">");
    }

    else {
      Serial.println("err ");
    }

  }
}


//##################################**********#########################
void setup() {
  
  Serial.begin(115200);
  SPI.begin();
  pinMode(ledPin, OUTPUT);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_1000KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();
  person = 1;
  person1 = 1;
  Serial.println("Inicio da calibracao, espere um momento");
  //####################

  unsigned long ldrLux2;
  float LUXE00, LUXE01, LUXE02, LUXE10, LUXE11, LUXE12;
  MCP2515::ERROR err;
  analogWrite(Pin, 255);
  decision = digitalRead(decisionPin); //variavel usada para controlo do arduino. Um foi ligado a HIGH e outro a LOW.
  Serial.print("\n\ndecision: ");
  Serial.println(decision);
  //Aqui é feita a calibracao inicial para podermos ter acesso aos ganhos. depois dos valores lidos com os dois led's desligados, calculando a luz exterior, 
  //é ligado um led, mantendo o outro desligado e lidos os valores dos LDR. Depois, o led acesso apaga-se, acendendo-se o outro, calculando os ganhos de ambos
  //e os ganhos da influencia de um no outro.
  if (decision == 1) {
    analogWrite(ledPin, 0);
  }
  else if (decision == 0 ) {
    analogWrite(ledPin, 0);
  }
  delay(1000); //time to breath
  LDR_Read = analogRead(sensorPin);
  
  ldrV = LDR_Read * (5.00 / 1024);
  ldrR = ((10000.00 * 5.00) / ldrV) - 10000.00;
  LUXE00 = pow(10, ((log10(ldrR) - 4.8603) / (-0.7797)));
  write(counter, LDR_Read);
  delay(1000); //time to breath
  if (read(ldrLux2) == MCP2515::ERROR_OK) {
    ldrV = ldrLux2 * (5.00 / 1024);
    ldrR = ((10000.00 * 5.00) / ldrV) - 10000.00;
    LUXE10 = pow(10, ((log10(ldrR) - 4.8603) / (-0.7797)));
  }

  
  delay(1000); //time to breath
  if (decision == 1) {
    analogWrite(ledPin, 100);
  }
  else if (decision == 0 ) {
    analogWrite(ledPin, 0);
  }
  delay(1000); //time to breath
  LDR_Read = analogRead(sensorPin);
  
  ldrV = LDR_Read * (5.00 / 1024);
  ldrR = ((10000.00 * 5.00) / ldrV) - 10000.00;
  LUXE01 = pow(10, ((log10(ldrR) - 4.8603) / (-0.7797)));
  write(counter, LDR_Read);
  delay(1000); //time to breath
  if (read(ldrLux2) == MCP2515::ERROR_OK) {
    ldrV = ldrLux2 * (5.00 / 1024);
    ldrR = ((10000.00 * 5.00) / ldrV) - 10000.00;
    LUXE11 = pow(10, ((log10(ldrR) - 4.8603) / (-0.7797)));
  }

  
  delay(1000); //time to breath
  if (decision == 1) {
    analogWrite(ledPin, 0);
  }
  else if (decision == 0 ) {
    analogWrite(ledPin, 100);
  }
  delay(1000); //time to breath
  LDR_Read = analogRead(sensorPin);
  
  ldrV = LDR_Read * (5.00 / 1024);
  ldrR = ((10000.00 * 5.00) / ldrV) - 10000.00;
  LUXE02 = pow(10, ((log10(ldrR) - 4.8603) / (-0.7797)));
  write(counter, LDR_Read);
  delay(1000); //time to breath
  if (read(ldrLux2) == MCP2515::ERROR_OK) {
    ldrV = ldrLux2 * (5.00 / 1024);
    ldrR = ((10000.00 * 5.00) / ldrV) - 10000.00;
    LUXE12 = pow(10, ((log10(ldrR) - 4.8603) / (-0.7797)));
  }
  
  delay(1000); //time to breath
  GAIN1 = ((LUXE02 - LUXE00) / (100.00 - 0.00));
  GAIN2 = ((LUXE11 - LUXE10) / (100.00 - 0.00));
  GAIN21 = ((LUXE01 - LUXE00) / (100.00 - 0.00));
  GAIN12 = ((LUXE12 - LUXE10) / (100.00 - 0.00));
  EXT0 = (LUXE00);
  EXT1 = (LUXE10);


  // Decisao dos Lux desejados de acordo com a ocupacao ou nao ocupacao da secretaria 
  if (decision == 0) {
    if (person == 0) {
      Des_lux00 = 10;
    }
    else if (person == 1) {
      Des_lux01 = 20;

    }
    if (person1 == 0) {
      Des_lux10 = 10;
    }
    else if (person1 == 1) {
      Des_lux11 = 20;

    }
  }
  else if (decision == 1) {
    if (person1 == 0) {
      Des_lux10 = 10;
    }
    else if (person1 == 1) {
      Des_lux11 = 20;
    }
    if (person == 0) {
      Des_lux00 = 10;
    }
    else if (person == 1) {
      Des_lux01 = 10;
    }
  }


  if (decision == 0) {
    if (person == 1) {
      bits_led = (Des_lux01 / GAIN1);
      analogWrite(ledPin, bits_led);
      //Serial.print(Des_lux01); Serial.println(" Des_lux01");
    }
    else if (person == 0) {
      bits_led = (Des_lux00 / GAIN1);
      analogWrite(ledPin, bits_led);
      //Serial.print(Des_lux00); Serial.println(" Des_lux00");
    }
  }
  if (decision == 1) {
    if (person1 == 1) {
      bits_led = (Des_lux11 / GAIN2);
      analogWrite(ledPin, bits_led);
      //Serial.print(Des_lux11); Serial.println("Des_lux11");
    }
    else if (person1 == 0) {
      bits_led = (Des_lux10 / GAIN2);
      analogWrite(ledPin, bits_led);
      //Serial.print(Des_lux10); Serial.println(" Des_lux10");
    }
  }
Serial.println("Fim da calibraçāo");
}




//#########################################################

void loop() {
  tempo_inicial = millis(); // INICIALIZACAO DO TEMPO DO LOOP
  unsigned long LUC2, BITSLED, TIME;
  MCP2515::ERROR err;

  float LUC, potencia, potencia_outro, ledV, ledV1, ledR, ledR1, ldrV1, ldrR1, usar;
  
  //é feita a leitura dos LDR's e a informacao enviada para o outro arduino;
  LUC1 = analogRead(sensorPin);
  write(counter, LUC1);
  ldrV = LUC1 * (5.00 / 1024);
  ldrR = ((10000.00 * 5.00) / ldrV) - 10000.00;
  LUC = pow(10, ((log10(ldrR) - 4.8603) / (-0.7797)));
  LUXES = LUC;
  delay (100);
  if (read(LUC2) == MCP2515::ERROR_OK) {
    //wait
    ldrV1 = LUC2 * (5.00 / 1024);
    ldrR1 = ((10000.00 * 5.00) / ldrV1) - 10000.00;
    LUC_OUTRO = pow(10, ((log10(ldrR1) - 4.8603) / (-0.7797)));
    usar = LUC2;
    //com a informacao dos bits presentes no outro ldr é entao possivel calcular o duty cycle e a energia deste
    duty_outro1 = usar / 1024;
    duty_outro = (duty_outro1 * 100.00);
    energy_outro = energy_outro + (power * (duty_outro1 * tempo));
  }


  delay(20);
  
  pedido();// verifica se ha informacao a ser lida no Serial do arduino

  /*Ajuste da luminusidade do led para obter os valores esperados. é feita uma comparacao entre os lux desejados e os Lux presentes e,
  no caso de os presentes serem mais do que os desejados, diminui 1 bit no numero de bits a enviar ao LED, mandando 0 se este valor for menor que 3.
   No caso de haver mais Lux do que o desejado, ha tamvbem o ajuste de menos um bit , sendo ajustado ao valor 255 se o valor necessario for maior do que 253.*/
  if (decision == 0) {
    if (person == 1) {
      if (LUC < Des_lux01) {
        if (bits_led > 253) {
          bits_led = 255;
          analogWrite(ledPin, bits_led);

        }
        else {
          bits_led = bits_led + 1;
          analogWrite(ledPin, bits_led);


        }
      }
      else if (LUC > Des_lux01) {
        if (bits_led < 3) {
          bits_led = 0;
          analogWrite(ledPin, bits_led);

        }
        else {
          bits_led = bits_led - 1;
          analogWrite(ledPin, bits_led);


        }
      }
    }
    else if (person == 0) {
      if (LUC < Des_lux00) {
        if (bits_led > 253) {
          bits_led = 255;
          analogWrite(ledPin, bits_led);

        }
        else {
          bits_led = bits_led + 1;
          analogWrite(ledPin, bits_led);


        }
      }
      else if (LUC > Des_lux00) {
        if (bits_led < 3) {
          bits_led = 0;
          analogWrite(ledPin, bits_led);

        }
        else {
          bits_led = bits_led - 1;
          analogWrite(ledPin, bits_led);


        }
      }
    }
  }
  else if (decision == 1) {
    if (person1 == 1) {
      if (LUC < Des_lux11) {
        if (bits_led > 253) {
          bits_led = 255;
          analogWrite(ledPin, bits_led);

        }
        else {
          bits_led = bits_led + 1;
          analogWrite(ledPin, bits_led);


        }
      }
      else if (LUC > Des_lux11) {
        if (bits_led < 3) {
          bits_led = 0;
          analogWrite(ledPin, bits_led);

        }
        else {
          bits_led = bits_led - 1;
          analogWrite(ledPin, bits_led);


        }
      }
    }
    else if (person1 == 0) {
      if (LUC < Des_lux10) {
        if (bits_led > 253) {
          bits_led = 255;
          analogWrite(ledPin, bits_led);

        }
        else {
          bits_led = bits_led + 1;
          analogWrite(ledPin, bits_led);


        }
      }
      else if (LUC > Des_lux10) {
        if (bits_led < 3) {
          bits_led = 0;
          analogWrite(ledPin, bits_led);

        }
        else {
          bits_led = bits_led - 1;
          analogWrite(ledPin, bits_led);


        }
      }
    }
  }
  
  write(counter, bits_led);
  BITS0 = bits_led;
  while (read(LUC2) != MCP2515::ERROR_OK) {
    //wait
  }
  BITS1 = LUC2;


  //-----------------------------------CALCULO DO TEMPO DECORRIDO -------------------------
  tempo_final = millis();
  tempo = (tempo_final - tempo_inicial);
  tempo_decorrido1 = tempo_decorrido1 + tempo;
  tempo_decorrido = (tempo_decorrido1 * 0.001);//calculo do tempo decorrido em segundos
  

  write(counter, tempo_decorrido1);
  while (read(TIME) != MCP2515::ERROR_OK) {
    //wait
  }
  tempo_outro1 = TIME;
  tempo_outro = (tempo_outro1 * 0.001);
  duty1 = LUC1 / 1024;
  duty = (duty1 * 100.00); //duty cycle em percentagem

  //----------------------CALCULO DA ENERGIA E POTENCIA DE AMBOS E DA ENERGIA E POTENCIA TOTAL----------------------------------------
  
  energy = energy + (power * (duty1 * tempo));
  power1 = (energy / tempo_decorrido);
  custo1 = (energy * custo);
  energy_outro = energy_outro + (power * (duty_outro1 * tempo));
  power_outro = (energy_outro / tempo_outro);
  custo_outro = (energy_outro * custo);
  total_energy = energy + energy_outro;
  total_power = (power1 + power_outro);

  //---------------------------------------------- CALCULO DO VISIBILITY ERROR PARA AMBAS AS SECRETARIAS---------------------------------------------
  
    if (person == 0){
      if (LUC < Des_lux00){
          i = i+1;
          Bellow = Des_lux10 - LUC;
          Total = Total + Bellow;
          Visibility = ( 1 / i ) * Total;
      }
    }
    if(person == 1){
      if (LUC < Des_lux01){
          i = i+1;
          Bellow = Des_lux01 - LUC;
          Total = Total + Bellow;
          Visibility = ( 1 / i ) * Total;
      }
   }

   if (person1 == 0){
      if (LUC_OUTRO < Des_lux10){
          j = j+1;
          Bellow1 = Des_lux10 - LUC_OUTRO;
          Total1 = Total1 + Bellow1;
          Visibility1 = ( 1 / j ) * Total1;
      }
    }
    if(person1 == 1){
      if (LUC_OUTRO < Des_lux11){
          j = j+1;
          Bellow1 = Des_lux11 - LUC_OUTRO;
          Total1 = Total1 + Bellow1;
          Visibility1 = ( 1 / j ) * Total1;
      }
   }

   TotalVisibility = Visibility1 + Visibility;


  //---------------------------------------------- CALCULO DO FLICKER ERROR PARA AMBOS OS LEDS---------------------------------------------
 
  c = c + 1;
  if (c == 1) {
    timeini = millis();
    LUX33 = LUC;
  }
  else if (c == 2) {
    LUX22 = LUC;
  }
  else if (c == 3) {
    LUX11 = LUC;
    timefin = millis();
    timee = timefin - timeini;
    timee_segundos = (timee * 0.001);
    decisao = 1;
    c = 0;
    N = N + 1;
  }

  if (decisao == 1) {
    flicker = (LUX11 - LUX22) * (LUX22 - LUX33);
    if (flicker < 0 ) {
      fli = ((abs(LUX11 - LUX22)) + (abs(LUX22 - LUX33))) / (2 * timee_segundos1);
    }
    else {
      fli = 0;
    }
    FLICKER_TOTAL = FLICKER_TOTAL + fli;
    FLICKER_ERROR = (1 / N) * FLICKER_TOTAL;
    decisao = 0;
  }


  h = h + 1;

  if (h == 1) {
    timeini1 = millis();
    LUX3 = LUC_OUTRO;
  }
  else if (h == 2) {
    LUX2 = LUC_OUTRO;
  }
  else if (h == 3) {
    LUX1 = LUC_OUTRO;
    timefin1 = millis();
    timee1 = timefin1 - timeini1;
    timee_segundos1 = (timee1 * 0.001);
    decisao1 = 1;
    h = 0;
    N1 = N1 + 1;
  }

  if (decisao1 == 1) {
    flicker1 = (LUX1 - LUX2) * (LUX2 - LUX3);
    if (flicker1 < 0 ) {
      fli1 = ((abs(LUX1 - LUX2)) + (abs(LUX2 - LUX3))) / (2 * timee_segundos1);
    }
    else {
      fli1 = 0;
    }
    FLICKER_TOTAL1 = FLICKER_TOTAL1 + fli1;
    FLICKER_ERROR1 = ((1 / N1) * FLICKER_TOTAL1);
    decisao1 = 0;
  }

  FLICKER_CONJUNTO = FLICKER_ERROR + FLICKER_ERROR1;

  delay(10); //time to breath  
}
