#include <Servo.h> //needed for servo

Servo mouth;//initalizes servo

const int mic = 7;//mononic inicator for mic input

void setup() {
  // put your setup code here, to run once:
  pinMode(mic , INPUT);//initalizes mic
  mouth.attach(11);//sets up servo

  mouth.write(110);//closes Mouth
  
  Serial.begin(9600);//begins input
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(mic) == HIGH){//if there is sound
    Serial.print("sound\n");//prints taht sound is heard
    moveMouth("open");//opens mouth
  }
  else if(digitalRead(mic) == LOW){//if there is not sound
    Serial.print("no\n");//print no sound
    moveMouth("closed");//closes mouth
  }
}

void moveMouth(String state){//moves mouth, accepts monomic input to accomadate cahnges in the mechanism
    if(state == "open"){//if mouth is open
      mouth.write(85);//move servo
      delay(150);//sounds can be brief so a delay is added so the mouth can open enough
    }
    else if(state == "closed"){//if mouth is closed
      mouth.write(110);//move servo
    }
}
