String inputString;
String aux;
String aux2;
String retString;

// i0212 => inputs
// o0212 => outputs
// dr05060712 => digital read
// dw011131 => digital write
// ar0001 => analog read
// aw001024 => PPVVV (pin pin valor valor valor valor) [0, 255]

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    inputString = Serial.readStringUntil('\n');
    retString = "";

    if (inputString[0] == 'i') {
      for (int i = 1; i < inputString.length()-1; i+=2) {
        aux = inputString[i];
        aux += inputString[i+1];
        pinMode(aux.toInt(), INPUT);
      }
    }
    else {
      if (inputString[0] == 'o') {
        for (int i = 1; i < inputString.length()-1; i+=2) {
          aux = inputString[i];
          aux += inputString[i+1];
          pinMode(aux.toInt(), OUTPUT);
        }
      }
      else {
        if (inputString.substring(0, 2) == "dr") {
          for (int i = 2; i < inputString.length()-1; i+=2) {
            aux = inputString[i];
            aux += inputString[i+1];
            retString += digitalRead(aux.toInt());
          }

          Serial.println(retString);
        }
        else {
          if (inputString.substring(0, 2) == "dw") {
            for (int i = 2; i < inputString.length()-2; i+=3) {
              aux = inputString[i];
              aux += inputString[i+1];
              digitalWrite(aux.toInt(), inputString[i+2]-48);
            }  
          }
          else {
            retString = "";
            if (inputString.substring(0, 2) == "ar") { // comienza desde el 54
              for (int i = 2; i < inputString.length()-1; i+=2) {
                aux = inputString[i];
                aux += inputString[i+1];
                retString.concat(analogRead(aux.toInt()+54));
                retString += ",";
              }
    
              Serial.println(retString.substring(0, retString.length()-1));
            }
            else {
              if (inputString.substring(0, 2) == "aw") {
                for (int i = 2; i < inputString.length()-4; i+=5) {
                  aux = inputString[i];
                  aux += inputString[i+1];

                  aux2 = inputString[i+2];
                  aux2 += inputString[i+3];
                  aux2 += inputString[i+4];
                  
                  analogWrite(aux.toInt(), aux2.toInt());
                }                
              }
            }
          }
        }
      }
    }
  }
}
