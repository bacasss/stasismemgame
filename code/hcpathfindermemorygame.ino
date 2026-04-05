/*
to start, click all 3 buttons at once. when 3 buttons are pressed, the sequence starts. 1st sequence shows 1 led and you have to click the corresponding button. 2nd sequence shows
1st led and a random 2nd led and the user clicks the corresponding button for each led. the win goal will be 10 sequences in a row for simplicity. for the rotary encoder, it controls
the speed the led shows up in (basically more u turn the less delay between leds u get). button click on encoder gets you to a mode that lets you adjust said speed and clicking it
again would reset the game to start with 1 led. 3 speed settings, level 1 delay would be 800ms, level 2 delay would be 500ms, and level 3 delay would be 200ms which might be a little
too fast
*/


const int b1 = 1;
const int b2 = 2;
const int b3 = 3;
const int led1 = 4;
const int led2 = 5;
const int led3 = 6;
const int encb = 7;
const int enca = 8;
const int b4 = 9;
const int buzz = 10;
int leds[3] = {led1, led2, led3};

int State;
int old_State;
int seq[10];
int roundc = 1;
int speed = 1;
int delayset = 500;

bool seqdone = false;
bool start = false;



void setup() {
  // put your setup code here, to run once:
  pinMode(b1, INPUT);
  pinMode(b2, INPUT);
  pinMode(b3, INPUT);
  pinMode(b4, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(enca, INPUT);
  pinMode(encb, INPUT);

  Serial.begin (9600);
  //Read First Position of Channel A
  old_State = digitalRead(enca);
  //bool start = false;

}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(b1) == HIGH && digitalRead(b2) == HIGH && digitalRead(b3) == HIGH){
    start = true;
  }

  if (start == true){
    for (int count = 0; count<10; count++){
      seq[count] = random(0, 3);
    }
    start = false;
    seqdone = true;
  }

  if (seqdone == true){
    for (int rcount = 0; rcount < roundc; rcount++){
      digitalWrite(leds[seq[rcount]], HIGH);
      delay(delayset);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      delay(300);
    }
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(200);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(200);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    delay(200);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    delay(500);
  }
  
  if (roundc < 10){
    roundc++;
  }
  else if (roundc >= 10){
    roundc = 1;
    start = false;
    seqdone = false;
  }

  if (digitalRead(b4) == HIGH){
    start = false;
    State = digitalRead(enca);
    if (State != old_State){
      if (digitalRead(encb) != State){
        speed ++;
        }
      else {  
        speed --;
      }
      if (speed > 2){
        speed = 2;
      }
      if (speed < 0){
        speed = 0;
      }
      if (speed = 0){
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
      }
      if (speed = 1){
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, LOW);
      }
      if (speed = 2){
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
      }
      
    }
    old_State = State; // the first position was changed

    if (digitalRead(b4) == HIGH){
      if (speed == 0){
        delayset = 800;
      }
      if (speed == 1){
        delayset = 500;
      }
      if (speed == 2){
        delayset = 200;
      }
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      start = false;
      roundc = 1;
      seqdone = false;
    }
  }

}
