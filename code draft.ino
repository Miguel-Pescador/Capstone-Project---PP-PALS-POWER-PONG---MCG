#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>

// ================= LCD SETUP =================
// I2C LCD address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= MAX7219 SETUP =================
// DIN = 51
// CLK = 52
// CS  = 53
// 4 matrix displays connected
LedControl lc = LedControl(51, 52, 53, 4);

// ================= MOTORS =================
Servo motorLR;      // left/right aiming
Servo motorUD;      // up/down aiming
Servo motorFire;    // firing motor
Servo motorReload;  // reload motor

// ================= BUTTONS =================
const int btnLeft  = 2;
const int btnRight = 3;
const int btnUp    = 4;
const int btnDown  = 5;
const int btnFire  = 6;

// ================= IR TARGET SENSORS =================
const int ir1 = 23;
const int ir2 = 25;
const int ir3 = 27;
const int ir4 = 29;
const int ir5 = 26;

// ================= RELOAD SENSOR =================
const int irReload = 7;

// ================= SCORE VARIABLES =================
int score = 0;        // current score
int highScore = 0;    // best score saved

// points for each target
int points1 = 10;
int points2 = 20;
int points3 = 30;
int points4 = 40;
int points5 = 50;

// prevents scoring same target multiple times
bool triggered1 = false;
bool triggered2 = false;
bool triggered3 = false;
bool triggered4 = false;
bool triggered5 = false;

// ================= MOTOR SIGNALS =================
// 90 = stop
// 120 = forward
// 60 = reverse
const int stopSignal = 90;
const int forward = 120;
const int reverse = 60;

// ================= GAME TIMER =================

// stores start time
unsigned long gameStartTime;

// total game time = 2 minutes 30 sec
const unsigned long gameDuration = 150000;

bool gameOver = false;
bool waitingRestart = false;

// ================= COUNTDOWN SYSTEM =================

// controls restart countdown
bool countdownActive = false;

// saves countdown start time
unsigned long countdownStart = 0;

// ================= FIRING STATES =================

// saves firing start time
unsigned long fireStartTime = 0;

// saves reverse time
unsigned long reverseStartTime = 0;

// state trackers
bool isFiring = false;
bool isReversing = false;
bool isReloading = false;

// ================= SCORE ANIMATION =================

// timer for +points animation
unsigned long scoreAnimTime = 0;

// checks if animation is active
bool showingPoints = false;

// ================= NUMBER PATTERNS =================
// 8x8 patterns for numbers 0-9
byte numbers[10][8] = {

  {0x3C,0x66,0x6E,0x76,0x66,0x66,0x3C,0x00}, // 0
  {0x18,0x38,0x18,0x18,0x18,0x18,0x3C,0x00}, // 1
  {0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00}, // 2
  {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00}, // 3
  {0x0C,0x1C,0x2C,0x4C,0x7E,0x0C,0x0C,0x00}, // 4
  {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00}, // 5
  {0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00}, // 6
  {0x7E,0x06,0x0C,0x18,0x30,0x30,0x30,0x00}, // 7
  {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00}, // 8
  {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00}  // 9
};

// ================= BYTE FLIP FUNCTION =================
// flips matrix horizontally
byte reverseByte(byte b) {

  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;

  return b;
}

// ================= SETUP =================
void setup() {

  // attach motors to pins
  motorLR.attach(9);
  motorUD.attach(10);
  motorFire.attach(11);
  motorReload.attach(12);

  // button inputs
  pinMode(btnLeft, INPUT_PULLUP);
  pinMode(btnRight, INPUT_PULLUP);
  pinMode(btnUp, INPUT_PULLUP);
  pinMode(btnDown, INPUT_PULLUP);
  pinMode(btnFire, INPUT_PULLUP);

  // target sensors
  pinMode(ir1, INPUT_PULLUP);
  pinMode(ir2, INPUT_PULLUP);
  pinMode(ir3, INPUT_PULLUP);
  pinMode(ir4, INPUT_PULLUP);
  pinMode(ir5, INPUT_PULLUP);

  // reload sensor
  pinMode(irReload, INPUT_PULLUP);

  // stop all motors
  motorLR.write(stopSignal);
  motorUD.write(stopSignal);
  motorFire.write(stopSignal);
  motorReload.write(stopSignal);

  Serial.begin(9600);

  // initialize LCD
  lcd.init();
  lcd.backlight();

  // startup screen
  lcd.setCursor(0,0);
  lcd.print("Arcade Ready!");

  lcd.setCursor(0,1);
  lcd.print("Good Luck!");

  delay(2000);

  lcd.clear();

  // initialize all 4 matrix displays
  for (int i = 0; i < 4; i++) {

    lc.shutdown(i, false);

    lc.setIntensity(i, 2);

    lc.clearDisplay(i);
  }

  // display score at start
  displayScore(score);

  // start game timer
  gameStartTime = millis();
}

// ================= MAIN LOOP =================
void loop() {

  // ================= COUNTDOWN =================
  if (countdownActive) {

    // converts milliseconds to seconds
    unsigned long countElapsed =
      (millis() - countdownStart) / 1000;

    lcd.setCursor(0,0);

    // show countdown
    if (countElapsed == 0) {
      lcd.print("Starting in 3 ");
    }
    else if (countElapsed == 1) {
      lcd.print("Starting in 2 ");
    }
    else if (countElapsed == 2) {
      lcd.print("Starting in 1 ");
    }

    // start game after countdown
    else if (countElapsed >= 3) {

      lcd.clear();
      lcd.print("GO GO GO!!!");

      delay(500);

      lcd.clear();

      // reset score
      score = 0;

      // reset all targets
      triggered1 = false;
      triggered2 = false;
      triggered3 = false;
      triggered4 = false;
      triggered5 = false;

      // show 0000
      displayScore(score);

      // restart timer
      gameStartTime = millis();

      countdownActive = false;
    }

    return;
  }

  // ================= GAME TIMER =================

  unsigned long elapsed = millis() - gameStartTime;

  // game ends when time runs out
  if (elapsed >= gameDuration) {
    gameOver = true;
  }

  // ================= GAME OVER SCREEN =================
  if (gameOver) {

    // stop all motors
    motorLR.write(stopSignal);
    motorUD.write(stopSignal);
    motorFire.write(stopSignal);
    motorReload.write(stopSignal);

    // update high score
    if (score > highScore) {
      highScore = score;
    }

    lcd.setCursor(0,0);
    lcd.print("TIME UP!      ");

    lcd.setCursor(0,1);

    lcd.print("S:");
    lcd.print(score);

    lcd.print(" HS:");
    lcd.print(highScore);

    waitingRestart = true;

    // restart game when fire button pressed
    if (waitingRestart && digitalRead(btnFire) == LOW) {

      countdownActive = true;

      countdownStart = millis();

      waitingRestart = false;

      gameOver = false;

      lcd.clear();
    }

    return;
  }

  // ================= TIMER DISPLAY =================

  int timeLeft = (gameDuration - elapsed) / 1000;

  int minutes = timeLeft / 60;

  int seconds = timeLeft % 60;

  lcd.setCursor(10,0);

  lcd.print(minutes);

  lcd.print(":");

  // adds leading zero
  if (seconds < 10) lcd.print("0");

  lcd.print(seconds);

  // warning when 30 sec left
  if (timeLeft <= 30) {

    lcd.setCursor(0,0);

    lcd.print("HURRY UP!!! ");
  }

  // check for score hits
  checkScore();

  // return from +points animation
  if (showingPoints &&
      millis() - scoreAnimTime > 1000) {

    displayScore(score);

    showingPoints = false;
  }

  // ================= AIM CONTROL =================

  // left/right
  if (digitalRead(btnLeft) == LOW) {
    motorLR.write(reverse);
  }

  else if (digitalRead(btnRight) == LOW) {
    motorLR.write(forward);
  }

  else {
    motorLR.write(stopSignal);
  }

  // up/down
  if (digitalRead(btnUp) == LOW) {
    motorUD.write(forward);
  }

  else if (digitalRead(btnDown) == LOW) {
    motorUD.write(reverse);
  }

  else {
    motorUD.write(stopSignal);
  }

  // ================= FIRE BUTTON =================

  if (digitalRead(btnFire) == LOW &&
      !isFiring &&
      !isReversing &&
      !isReloading) {

    lcd.clear();

    lcd.print("FIRE!");

    // start firing motor
    motorFire.write(forward);

    fireStartTime = millis();

    isFiring = true;
  }

  // ================= FIRE COMPLETE =================

  if (isFiring &&
      millis() - fireStartTime >= 1500) {

    // reverse motor back
    motorFire.write(reverse);

    reverseStartTime = millis();

    isFiring = false;

    isReversing = true;

    lcd.clear();

    lcd.print("Resetting...");
  }

  // ================= STOP REVERSE =================

  if (isReversing &&
      millis() - reverseStartTime >= 400) {

    // stop firing motor
    motorFire.write(stopSignal);

    lcd.clear();

    lcd.print("Reloading...");

    // start reload motor
    motorReload.write(forward);

    isReversing = false;

    isReloading = true;
  }

  // ================= RELOAD SENSOR =================

  if (isReloading &&
      digitalRead(irReload) == LOW) {

    // stop reload motor
    motorReload.write(stopSignal);

    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("LOCK IN");

    lcd.setCursor(0,1);
    lcd.print("Score:");
    lcd.print(score);

    // reset targets
    triggered1 = false;
    triggered2 = false;
    triggered3 = false;
    triggered4 = false;
    triggered5 = false;

    isReloading = false;
  }
}

// ================= SCORE SYSTEM =================
void checkScore() {

  // target 1
  if (digitalRead(ir1) == LOW && !triggered1) {

    score += points1;

    triggered1 = true;

    showPoints(points1);
  }

  // target 2
  if (digitalRead(ir2) == LOW && !triggered2) {

    score += points2;

    triggered2 = true;

    showPoints(points2);
  }

  // target 3
  if (digitalRead(ir3) == LOW && !triggered3) {

    score += points3;

    triggered3 = true;

    showPoints(points3);
  }

  // target 4
  if (digitalRead(ir4) == LOW && !triggered4) {

    score += points4;

    triggered4 = true;

    showPoints(points4);
  }

  // target 5
  if (digitalRead(ir5) == LOW && !triggered5) {

    score += points5;

    triggered5 = true;

    showPoints(points5);
  }
}

// ================= +POINTS DISPLAY =================
void showPoints(int pts) {

  showingPoints = true;

  scoreAnimTime = millis();

  // clear all displays
  for (int i = 0; i < 4; i++) {
    lc.clearDisplay(i);
  }

  // split points into digits
  int d3 = (pts / 10) % 10;

  int d4 = pts % 10;

  // plus symbol
  byte plus[8] = {
    0x00,
    0x18,
    0x18,
    0x7E,
    0x18,
    0x18,
    0x00,
    0x00
  };

  // display +points
  for (int row = 0; row < 8; row++) {

    lc.setRow(0, 7-row, plus[row]);

    lc.setRow(
      2,
      7-row,
      reverseByte(numbers[d3][row])
    );

    lc.setRow(
      3,
      7-row,
      reverseByte(numbers[d4][row])
    );
  }
}

// ================= TOTAL SCORE DISPLAY =================
void displayScore(int num) {

  // clear displays
  for (int i = 0; i < 4; i++) {
    lc.clearDisplay(i);
  }

  // split score into digits
  int d1 = (num / 1000) % 10;

  int d2 = (num / 100) % 10;

  int d3 = (num / 10) % 10;

  int d4 = num % 10;

  int digits[4] = {d1, d2, d3, d4};

  // display digits
  for (int device = 0; device < 4; device++) {

    for (int row = 0; row < 8; row++) {

      lc.setRow(
        device,
        7-row,
        reverseByte(numbers[digits[device]][row])
      );
    }
  }
}
