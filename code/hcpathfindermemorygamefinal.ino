const int btn1 = D0;
const int btn2 = D1;
const int btn3 = D2;

const int ledGreen = D3;
const int ledYellow = D4;
const int ledRed = D5;

const int buzzer = D9;
const int encBtn = D8;
const int encA = D7;
const int encB = D6;

bool gameStarted = false;
bool playerTurn = false;
bool settingsMode = false;

int level = 1;
int difficulty = 1;

int seq[15];

int playerIndex = 0;

int prev1, prev2, prev3;
int prevEnc;
int lastclk;

unsigned long lastPressTime = 0;
const int debounceDelay = 300;
unsigned long encptime = 0;

void beep(int t) {
  digitalWrite(buzzer, HIGH);
  delay(t);
  digitalWrite(buzzer, LOW);
}

void settingsLEDs(){
  digitalWrite(ledGreen, difficulty >= 1);
  digitalWrite(ledYellow, difficulty >=2);
  digitalWrite(ledRed, difficulty >= 3);
}

void resetGame() {
  gameStarted = false;
  playerTurn = false;
  settingsMode = false;
  level = 1;
  playerIndex = 0;

  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);

  Serial.println("RESET");
}

void generateSeq() {
  for (int i = 0; i < 15; i++) {
    seq[i] = random(0, 3);
  }
}

void showSequence() {
  playerTurn = false;
  playerIndex = 0;

  int ontime = 350 - (difficulty * 70);

  for (int i = 0; i < level; i++) {

    if (seq[i] == 0) digitalWrite(ledGreen, HIGH);
    if (seq[i] == 1) digitalWrite(ledYellow, HIGH);
    if (seq[i] == 2) digitalWrite(ledRed, HIGH);

    beep(120);
    delay(ontime);

    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);

    delay(200);
  }

  prev1 = digitalRead(btn1);
  prev2 = digitalRead(btn2);
  prev3 = digitalRead(btn3);

  playerTurn = true;
}

int readPress() {

  if (millis() - lastPressTime < debounceDelay) return -1;

  int c1 = digitalRead(btn1);
  int c2 = digitalRead(btn2);
  int c3 = digitalRead(btn3);

  int press = -1;

  if (prev1 == HIGH && c1 == LOW) press = 0;
  if (prev2 == HIGH && c2 == LOW) press = 1;
  if (prev3 == HIGH && c3 == LOW) press = 2;

  if (press != -1) lastPressTime = millis();

  prev1 = c1;
  prev2 = c2;
  prev3 = c3;

  return press;
}

void loseEffect() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, HIGH);
    beep(200);
    delay(300);

    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    delay(300);
  }
}

void winEffect() {
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledYellow, HIGH);
  digitalWrite(ledRed, HIGH);

  beep(800);
  delay(3000);

  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);
}

void setup() {

  Serial.begin(9600);
  randomSeed(analogRead(A0));

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);

  pinMode(encBtn, INPUT_PULLUP);
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);

  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);

  pinMode(buzzer, OUTPUT);

  prev1 = prev2 = prev3 = HIGH;
  prevEnc = HIGH;
  lastclk = digitalRead(encA);

  Serial.println("READY");
}

void loop() {
  int enc = digitalRead(encBtn);

  if (prevEnc == HIGH && enc == LOW) {
    encptime = millis();
    // if (!gameStarted) {

    //   gameStarted = true;
    //   level = 1;

    //   Serial.println("GAME STARTED");
    //   beep(300);

    //   generateSeq();
    //   showSequence();
    // }
  }

  if (prevEnc == LOW && enc == HIGH) {
    long duration = millis() - encptime;

    if (duration > 1000) {
      settingsMode = !settingsMode;
      gameStarted = false;
      if (settingsMode) {
        Serial.println("settings mode");
        beep(100); delay(100); beep(100);
      } else {
        Serial.println("exitted settigs mode");
        resetGame();
      }
    } 
    else if (!settingsMode && !gameStarted) {
      gameStarted = true;
      level = 1; 
      Serial.println("GAME STARTED");
      beep(300);
      generateSeq();
      showSequence();
    }
  }

  prevEnc = enc;

  if (settingsMode) {
    settingsLEDs();
    int currclk = digitalRead(encA);
    if (currclk != lastclk && currclk == LOW) {
      if (digitalRead(encB) == currclk) {
        difficulty = min(difficulty + 1, 3);
      } else {
        difficulty = max(difficulty - 1, 1);
      }
      Serial.print("Difficulty: "); Serial.println(difficulty);
      beep(50);
    }
    lastclk = currclk;
    return;
  }

  if (playerTurn) {

    int p = readPress();

    if (p != -1) {

      beep(80);

      if (seq[playerIndex] == p) {
        playerIndex++;
      } else {
        Serial.println("LOSE");
        loseEffect();
        resetGame();
        return;
      }

      if (playerIndex >= level) {

        Serial.println("LEVEL CLEAR");
        beep(500);

        level++;

        delay(1000);

        if (level > 15) {
          Serial.println("WIN");
          winEffect();
          resetGame();
        } else {
          showSequence();
        }
      }
    }
  }
}