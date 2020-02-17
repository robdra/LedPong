#include <Adafruit_NeoPixel.h>
#include <math.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    4
// Which pins are connected to the player buttons
#define playerOnePin    7
#define playerTwoPin    8

unsigned long int prevMillSec = 0;

int LED_COUNT = 59;
int playerOneLives = 3;
int playerTwoLives = 3;
int playerOnePaddle = LED_COUNT-playerOneLives;
int playerTwoPaddle = playerTwoLives-1;
int pos = 0;
int speed = 100;
int prevPos = 0;
int activate = 0;

bool countUp = true;
bool playerOneActive = false;
bool playerOnePressed = false;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
        clock_prescale_set(clock_div_1);
    #endif
    // END of Trinket-specific code.

    pinMode(playerOnePin, INPUT);
    Serial.begin(9600);
    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop()
{
    game(countUp, playerOneLives);
}

void ball(bool countUp, int playerOneLives)
{
    unsigned long int currMillSec = millis();
    
    if(countUp)
    {
        if(currMillSec - prevMillSec >= speed)
        {
            prevMillSec = currMillSec;
            activate++;
            
            strip.setPixelColor(pos, strip.Color(255, 0, 0));         //  Set pixel's color (in RAM)
            strip.show();
            strip.setPixelColor(prevPos, strip.Color(0, 0, 0));         //  Set pixel's color (in RAM)
            strip.show();
            prevPos = pos;
            pos++;
            
            if(pos > playerOnePaddle)
            {
                playerOnePressed = false;
                strip.setPixelColor(prevPos, strip.Color(0, 0, 0));         //  Set pixel's color (in RAM)
                strip.show();
                playerOneLives--;
                countUp = true;
                Serial.println("Lost life");
                game(countUp, playerOneLives);
                if(playerOneLives = 0)
                {
                    //gameOver();
                }
            }
        }
    }
    else
    {
        if(currMillSec - prevMillSec >= speed)
        {
            if(pos < playerTwoPaddle)
            {
                playerTwoLives--;
                countUp = false;
                if(playerOneLives = 0)
                    //gameOver();

                game(countUp, playerOneLives);
            }
            prevMillSec = currMillSec;
            activate++;
            if(activate == 1)
            {
                playerOneActive = true;
            }
            strip.setPixelColor(pos, strip.Color(255, 0, 0));         //  Set pixel's color (in RAM)
            strip.show();
            strip.setPixelColor(prevPos, strip.Color(0, 0, 0));         //  Set pixel's color (in RAM)
            strip.show();
            prevPos = pos;
            pos--;
        }
    }
}

void player1(bool &countUp, int &playerOneLives, int &playerOnePaddle)
{
    int active = digitalRead(playerOnePin);

    if(active == HIGH)
    {
        playerOnePressed = true;
    }

    if(playerOnePressed && active == LOW)
    {
        playerOnePressed = false;

        if(playerOnePaddle == pos)
        {
            countUp = false;
            pos = playerOnePaddle-1;
            activate = 0;
            playerOneActive = false;
        }
        else
        {
           playerOneLives--;
           playerOneActive = false;
           game(countUp, playerOneLives);
           
        }
    }
}

void game(bool &countUp, int &playerOneLives)
{
    clear();
    
    pos = LED_COUNT/2;
    prevPos = 0;
    playerOnePaddle = LED_COUNT-playerOneLives;
    playerTwoPaddle = playerTwoLives-1;
    activate = 0;

    strip.setPixelColor(playerOnePaddle, strip.Color(0, 255, 0));         //  Set pixel's color (in RAM)
    strip.show();
    strip.setPixelColor(playerTwoPaddle, strip.Color(0, 255, 0));         //  Set pixel's color (in RAM)
    strip.show();
    Serial.println("Game");
    Serial.println(playerOneLives);
    while(true)
    {
        player1(countUp, playerOneLives, playerOnePaddle);
        ball(countUp, playerOneLives);
        //player2();
    }
}

void clear()
{
    for(int i = 0; i<LED_COUNT; i++)
    {
        strip.setPixelColor(i, strip.Color(0, 0, 0));         //  Set pixel's color (in RAM)
        strip.show();
    }
}