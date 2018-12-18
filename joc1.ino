#include "LedControl.h"
#include <LiquidCrystal.h>

#define RS 2
#define E 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7
#define PWM 9
#define JOY_X A0
#define JOY_Y A1

LiquidCrystal lcd(RS,E,D4,D5,D6,D7);
LedControl lc = LedControl(12, 11, 10, 1); 
bool initial[8][8] = 
{
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 0, 0}
};
bool m[8][8] = 
{
  {0, 0, 0, 1, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 1, 1, 1, 0, 0}
};
bool sadFace[8][8] = 
{
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};
bool happyFace[8][8] = 
{
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 0, 0, 0, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};
bool noDelay = 0; //certain ball movements require no delay; 1 if no delay needed, 0 otherwise
int colPlayer1, colPlayer2, colPlayer3; //player coordinates
int xVal, yVal; //joystick coordinates
int rowBall = 3, colBall = 6; //ball coordinates
int ballDirection = -1;
bool joyMoved = false;
unsigned long currentMillis;
int gameDelayTime = 500;
int lives = 3;
bool startGame = 0; //1 if the game is on, 0 otherwise 
bool choseLevel = 0; //1 if the player chose the only one level mode of play, 0 otherwise 
bool lostLevel = 0; //1 if the player lost the current level, 0 otherwise
int delayGame(int delayTime) //delays game for a period of delayTime seconds using millis()
{
  if(millis() > currentMillis + delayTime) 
  {
    currentMillis = millis();
    return 1;
  }
  return 0;
}
void displayMatrix() //displays matrix m and the ball
{
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++) lc.setLed(0, row, col, m[row][col]); 
  lc.setLed(0, rowBall, colBall, true);
}
void readMovement() //reads player movement
{
  xVal = analogRead(JOY_X);
  if (joyMoved == false) 
  {
    if(xVal >= 600 && m[7][0] != 1) //player moves right
    {
      int aux = m[7][0];
      for (int col = 0; col < 7; col++) m[7][col] = m[7][col + 1];
      m[7][7] = aux;
    }
    if(xVal < 400 && m[7][7] != 1) //player moves left
    {
      int aux = m[7][7];
      for (int col = 7; col > 0; col--) m[7][col] = m[7][col - 1];
      m[7][0] = aux;
     } 
     joyMoved = true;
   }
   if (xVal >= 400 && xVal < 600) joyMoved = false;
   //updates player coordinates
   for (int col = 0; col < 7; col++)
      {
        if (m[7][col] == 1) 
        {
          colPlayer3 = col;
          break;
        }
      }
   colPlayer2 = colPlayer3 + 1;
   colPlayer1 = colPlayer2 + 1;
}
int hitPlayer() //checks whether the ball will hit the player
{
  if (m[rowBall + 1][colBall] == 1 || m[rowBall + 1][colBall - 1] == 1 || m[rowBall + 1][colBall] == 1) //if ball will hit the player
    {
       noDelay = 1; //no delay needed
       switch (ballDirection) //changes ball direction accordingly
       {
          case -3:
            if (colBall == colPlayer1) ballMovement(1);
            else if (colBall == colPlayer3) ballMovement(2);
            else if (colBall == colPlayer2) ballMovement(3);
            else loseGame();
            break;
         case -1:
            if (colBall == colPlayer1) ballMovement(1);
            else if (colBall == colPlayer3) ballMovement(2);
            else if (colBall == colPlayer2) ballMovement(3);
            else loseGame();
            break;
         case -2:
            if (colBall == colPlayer1) ballMovement(1);
            else if (colBall == colPlayer3) ballMovement(2);
            else if (colBall == colPlayer2) ballMovement(3);
            break;
       }
      return 1;
    }
  //else player will lose current level
  lostLevel = 1;
  loseGame();
  return 0;
}
int hitCeiling() //checks whether the ball will hit the ceiling; if so, changes direction
{
  if (rowBall == 0) //if ball is on the first row (ceiling) 
  {
      noDelay = 1; // no delay needed
      switch (ballDirection) //switches ball direction accordingly
      {
        case 3:
          ballMovement(-3);
          break;
        case 1:
          ballMovement(-2);
          break;
        case 2:
          ballMovement(-1);
          break;
      }
      return 1;
  }
  return 0;
}
int hitSide() //checks whether the ball will hit the sides; if so, changes direction
{
  if (colBall == 7 || colBall == 0) //if ball is on the first column or last column
  {
      switch (ballDirection) //switches ball direction accordingly and does one step ahead of the ballMovement function
      {
        case 1:
          rowBall--;
          colBall--;
          m[rowBall][colBall] = 1;
          ballMovement(2);
          break;
        case 2:
          rowBall--;
          colBall++;
          m[rowBall][colBall] = 1;
          ballMovement(1);
          break;
        case -1:
          rowBall++;
          colBall++;
          m[rowBall][colBall] = 1;
          ballMovement(-2);
          break;
        case -2:
          rowBall++;
          colBall--;
          m[rowBall][colBall] = 1;
          ballMovement(-1);
          break;
      }
      return 1;
  }
  return 0;
}
void ballMovement(int direction) //controls the movement of the ball
{
  ballDirection = direction;
  if (noDelay == 1 || delayGame(gameDelayTime) == 1) 
  {
        if (direction == -1) //down right
          {
            m[rowBall][colBall] = 0; // shuts off the current led
            if ((rowBall + 1 != 7 || hitPlayer() == 0) && hitSide() == 0) //ball can either hit player or sides
            {
              if (lostLevel == 1) //if the player lost the current level
              {
                lostLevel = 0;
                return;
              }
              rowBall++;
              colBall--;
              m[rowBall][colBall] = 1;
            }
          }
          if (direction == -2) //down left
          {
            m[rowBall][colBall] = 0;
            if ((rowBall + 1 != 7 || hitPlayer() == 0) && hitSide() == 0) //ball can either hit player or sides
            {
              if (lostLevel == 1) //if the player lost the current level
              {
                lostLevel = 0;
                return;
              }
              rowBall++;
              colBall++;
              m[rowBall][colBall] = 1;
            }
          }
          if (direction == -3) //down straight
          {
            m[rowBall][colBall] = 0;
            if (rowBall + 1 != 7 || hitPlayer() == 0) //ball can only hit player
            {
              if (lostLevel == 1) //if the player lost the current level
              {
                lostLevel = 0;
                return;
              }
              rowBall++;
              m[rowBall][colBall] = 1;
            }
          }
          if (direction == 3) //up straight
          {
            m[rowBall][colBall] = 0;
            if (hitCeiling() == 0) //ball can only hit ceiling
            {
              rowBall--;
              m[rowBall][colBall] = 1;
            }
          }
          if (direction == 1) //up left
          {
            m[rowBall][colBall] = 0;
            if (hitCeiling() == 0 && hitSide() == 0) //ball can either hit ceiling or sides
            {
              rowBall--;
              colBall++;
              m[rowBall][colBall] = 1;
            }
          }
          if (direction == 2) //up right
          {
            m[rowBall][colBall] = 0;
            if (hitCeiling() == 0 && hitSide() == 0) //ball can either hit ceiling or sides
            {
              rowBall--;
              colBall--;
              m[rowBall][colBall] = 1;
            }
          }
        noDelay = 0; //resets no delay
        displayMatrix();
  }
}
void loseGame() //checks whether the player lost the current level and/or whole game; in the former case, it resets if lives > 0
{
    for (int row = 0; row < 8; row++)
      for (int col = 0; col < 8; col++) lc.setLed(0, row, col, sadFace[row][col]); //displays sad face
    delay(1000); //no influence on the game; displays sad face matrix
    lives--; //decreases lives
    if (lives > 0) 
    {
      lcd.setCursor(2, 0);
      //updates current lives status
      if (lives == 2) lcd.print("2 lives left");
      else lcd.print("1 lives left");
      delay(2000); //no influence on the game; displays lives on lcd
      //resets level
      ballDirection = -1;
      rowBall = 3;
      colBall = 6;
      if (choseLevel == 0) //resets matrix for play all levels mode
      {
        for (int row = 0; row < 8; row++)
          for (int col = 0; col < 8; col++) m[row][col] = initial[row][col];
      }
      else //resets only the player coordinates for play only one level mode
      {
        for (int col = 0; col < 8; col++) m[7][col] = initial[7][col];
      }
      displayMatrix();
      noDelay = 0;
      joyMoved = false;
      if (choseLevel == 0) //if the player chose play all levels mode it starts from level 1 again
      {
        gameDelayTime = 500;
        //updates level on the screen
        lcd.setCursor(4, 1);
        lcd.print("Level 1"); 
      }
      currentMillis = millis();
    }
    else
    {
      //lost game end screen
      lcd.setCursor(2, 0);
      lcd.print("0 lives left");
      lcd.setCursor(3, 1);
      lcd.print("You lose!");
      startGame = 0;
      delay(1500); //no influence on the game; displays lost game end screen
      //will display menu only if the player moves joystick up or down
      yVal = analogRead(JOY_Y);
      while (startGame == 0)
      {
          if (yVal < 200 || yVal > 800)
          {
            lcd.clear();
            startGame = 1;
            displayMenu();
          }
          yVal = analogRead(JOY_Y);
      }
    }
}
void winGame() //checks whether the player won the current level and/or whole game; in the former case, it advances to the next level
{
  int count = 0;
  //counts how many leds are turned on besides the ball
  for (int row = 0; row < 3; row++)
      for (int col = 0; col < 8; col++) 
        if (m[row][col] == 1 && !(rowBall == row && colBall == col)) count++;
  if (count == 0) //no leds are turned on besides the ball
  {
    for (int row = 0; row < 8; row++)
      for (int col = 0; col < 8; col++) lc.setLed(0, row, col, happyFace[row][col]); //displays happy face
    delay(1000); //no influence on the game; displays happy face matrix
    gameDelayTime -= 100; //decreases the game delay so it can advance to the next level
    if (choseLevel == 0 && gameDelayTime >= 100) //only if it's the play all levels mode does it advance to the next level; there are only 5 levels
    {
      //advances to the next level
      ballDirection = -1;
      rowBall = 3;
      colBall = 6;
      for (int row = 0; row < 8; row++)
        for (int col = 0; col < 8; col++) m[row][col] = initial[row][col];
      noDelay = 0;
      joyMoved = false;
      displayMatrix();
      lcd.setCursor(4, 1);
      //updates level on the screen
      switch (gameDelayTime)
      {
        case 500:
          lcd.print("Level 1");
          break;
        case 400:
          lcd.print("Level 2");
          break;
        case 300:
          lcd.print("Level 3");
          break;
        case 200:
          lcd.print("Level 4");
          break;
        case 100:
          lcd.print("Level 5");
          break;
      }
      currentMillis = millis();
    }
    else 
    {
      //win game end screen
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("You win!");
      startGame = 0;
      delay(1500); //no influence on the game; displays win game end screen
      //will display menu only if the player moves joystick up or down
      yVal = analogRead(JOY_Y);
      while (startGame == 0)
      {
          if (yVal < 200 || yVal > 800)
          {
            lcd.clear();
            startGame = 1;
            displayMenu();
          }
          yVal = analogRead(JOY_Y);
      }
    }
  }
}
void displayMenu() //displays game menu
{
  choseLevel = 0; //resets game mode chosen
  startGame = 0; //game is not currently on
  gameDelayTime = 500; //resets game delay
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++) lc.setLed(0, row, col, false); //shuts off matrix
  lcd.clear();
  //displays game modes
  lcd.setCursor(2, 0);
  lcd.print("Choose level"); //play one level only mode
  lcd.setCursor(2, 1);
  lcd.print("Play all"); //play all levels mode
  yVal = analogRead(JOY_Y);
  while (yVal < 200 || yVal > 800) yVal = analogRead(JOY_Y); //waits until the joystick is brought into a neutral position
  while (startGame == 0) //while game isn't on
  {
    if (yVal < 200) //player chose one level only mode
    {
      delay(1000); //no influence on the game; transitions between menu and choose level menu
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("1 2 3 4 5"); //displays the levels
      lcd.setCursor(3, 1);
      lcd.print("^"); //displays the current choice
      int writeCol = 3;
      xVal = analogRead(JOY_X);
      while (yVal < 400) yVal = analogRead(JOY_Y); //waits until the joystick is brought into a neutral position
      int joyMovedMenu = false;
      while (yVal > 200) //game will start once the joystick is moved up
      {
        if (joyMovedMenu == false) 
        {
          if(xVal >= 600) //player moves right in menu
          {
            lcd.setCursor(writeCol, 1);
            lcd.print(" ");
            writeCol += 2;
            if (writeCol > 11) writeCol = 3;
            lcd.setCursor(writeCol, 1);
            lcd.print("^");
          }
          if(xVal < 400) //player moves left in menu
          {
            lcd.setCursor(writeCol, 1);
            lcd.print(" ");
            writeCol -= 2;
            if (writeCol < 3) writeCol = 11;
            lcd.setCursor(writeCol, 1);
            lcd.print("^");
          } 
          joyMovedMenu = true;
        }
        if (xVal >= 400 && xVal < 600) joyMovedMenu = false;
        xVal = analogRead(JOY_X);
        yVal = analogRead(JOY_Y);
      }
      lcd.clear();
      lcd.setCursor(4, 1);
      switch (writeCol) //chooses the level according to the player's choice
      {
        case 3: 
          gameDelayTime = 500;
          lcd.print("Level 1");
          break;
        case 5:
          gameDelayTime = 400;
          lcd.print("Level 2");
          break;
        case 7:
          gameDelayTime = 300;
          lcd.print("Level 3");
          break; 
        case 9:
          gameDelayTime = 200;
          lcd.print("Level 4");
          break;
        case 11:
          gameDelayTime = 100;
          lcd.print("Level 5");
          break;
      }
      startGame = 1;
      choseLevel = 1;
    }
    else if (yVal > 800) 
    {
      lcd.clear();
      lcd.setCursor(4, 1);
      lcd.print("Level 1");
      startGame = 1;
    }
    yVal = analogRead(JOY_Y);
  }
  lcd.setCursor(2, 0);
  lcd.print("3 lives left"); //displays current lives
  delay(1500); //no influence on the game; displays lives
  //starts game
  ballDirection = -1;
  rowBall = 3;
  colBall = 6;
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++) m[row][col] = initial[row][col];
  noDelay = 0;
  joyMoved = false;
  lives = 3;
  lostLevel = 0;
  displayMatrix();
  currentMillis = millis();
}
void setup()
{
  //matrix initialization
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);
  lc.shutdown(0, false);
  lc.setIntensity(0, 5);
  lc.clearDisplay(0);
  //lcd initialization
  pinMode(PWM,OUTPUT);
  analogWrite(PWM,100);
  lcd.begin(16,2);
  lcd.clear();
  displayMenu();
  currentMillis = millis();
}
void loop()
{
  while (startGame == 1) //while game is on
  {
    readMovement();
    ballMovement(ballDirection);
    winGame();
    displayMatrix();
  } 
}
