#include <LiquidCrystal.h>
#include <time.h>

#define NONE   0
#define RIGHT  2
#define LEFT   4
#define UP     8
#define DOWN   16
#define SELECT 32

#define ALL (RIGHT | LEFT | UP | DOWN | SELECT)

#define PIN_RS 8
#define PIN_EN 9
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7

LiquidCrystal lcd(PIN_RS, PIN_EN, PIN_D4, PIN_D5, PIN_D6, PIN_D7);        // select the pins used on the LCD panel

#define DOWN_ARROW ((uint8_t) 1)
byte downArrow[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};


int get_button_press() {
  int buttonVoltage = analogRead(0);

  if (buttonVoltage < 60) {
    return RIGHT;
  } else if (buttonVoltage < 200){ 
    return UP;
  } else if (buttonVoltage < 400) {
    return DOWN;
  } else if (buttonVoltage < 600) {
    return LEFT;
  } else if (buttonVoltage < 800) {
    return SELECT;
  } else {
    return NONE;
  }
}

int wait_for_button_press(int waitedOn) {
  if (waitedOn == NONE) {
    return NONE;
  }

  int buttonPressed;
  while (!((buttonPressed = get_button_press()) & waitedOn));
  while (get_button_press() != NONE); 
  return buttonPressed;
}

char *button_to_string(int button) {
  switch (button) {
    case NONE:
      return "NONE";
    case RIGHT:
      return "RIGHT";
    case LEFT:
      return "LEFT";
    case UP:
      return "UP";
    case DOWN:
      return "DOWN";
    case SELECT:
      return "SELECT";
    default:
      return 0;
  }
}

void wait_for_button_sequence(int *buttonSequence) {
  int *nextButton = buttonSequence;
  int buttonPressed = NONE;
  int cleared = 0;

  int buttonsEnteredLen = 6;
  char buttonsEntered[] = {' ', ' ', ' ', ' ', ' ', ' '};

  while (*nextButton != NONE) {
      buttonPressed = wait_for_button_press(ALL);

      if (!cleared) {
        lcd.clear();
        cleared = 1;
      }

      for (int i = 0; i < buttonsEnteredLen - 1; i++) {
        buttonsEntered[i] = buttonsEntered[i + 1];
      }
      buttonsEntered[buttonsEnteredLen - 1] = button_to_string(buttonPressed)[0];

      lcd.setCursor(0, 0);
      for (int i = 0; i < buttonsEnteredLen; i++) {
        lcd.print(buttonsEntered[i]);
      }
      
      if (buttonPressed == *nextButton) {
        nextButton++;
        continue;
      }

      nextButton = buttonSequence;

      if (buttonPressed == *nextButton) {
        nextButton++;
        continue;
      }
  }
}

int get_rating() {
  int rating = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" L            R ");
  lcd.setCursor(3 + rating, 0);
  lcd.write(DOWN_ARROW);
  lcd.setCursor(0, 1);
  lcd.print("   0123456789   ");

  wait_for_butto_press(LEFT | RIGHT | SELECT);

  return rating;
}

void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.createChar(DOWN_ARROW, downArrow);

  Serial.println("started");
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonPressed;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter passowrd:");
  lcd.setCursor(0, 1);
  lcd.print("");

  int password[] = {DOWN, UP, LEFT, LEFT, SELECT, NONE};

  wait_for_button_sequence(password);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome!");
  lcd.setCursor(0, 1);
  lcd.print("(Press any but.)");

  buttonPressed = wait_for_button_press(ALL);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Q: How are you");
  lcd.setCursor(0, 1);
  lcd.print("feeling?");

  buttonPressed = wait_for_button_press(ALL);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("U) Good :)");
  lcd.setCursor(0, 1);
  lcd.print("D) Bad :(");
  
  buttonPressed = wait_for_button_press(UP | DOWN);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I'm sorry to");
  lcd.setCursor(0, 1);
  lcd.print("hear that");

  buttonPressed = wait_for_button_press(ALL);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("From 0 to 9 rate");
  lcd.setCursor(0, 1);
  lcd.print("your lecturer");

  buttonPressed = wait_for_button_press(ALL);

  get_rating();

  buttonPressed = wait_for_button_press(ALL);







}
