/*************************************************************

  This sketch shows how to read values from Virtual Pins

  App project setup:
    Slider widget (0...100) on Virtual Pin V1
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings

#define BLYNK_AUTH_TOKEN            "2O8Epc3mdJjSAnKWY1-_0miPVc9NShRS"
//#define BLYNK_DEBUG 
#include <FastLED.h>
#include <pixeltypes.h>
#define DATA_PIN D2
#define BRIGHTNESS  100
#define NUM_LEDS 30
//#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;
struct CRGB leds[NUM_LEDS];


char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "WiFi Atas TB Al Kautsar";
char pass[] = "t4m4n_b3l4j4r";

int button;
int selectedEffect;
int led;
int r, g, b;

void off(){
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  FastLED.clear();
}
BLYNK_WRITE(V1)
{
   r = param[0].asInt(); // assigning incoming value from pin V1 to a variable
   g = param[1].asInt(); // assigning incoming value from pin V1 to a variable
   b = param[2].asInt(); // assigning incoming value from pin V1 to a variable
  

  if ((r!=0) or (g!=0) or (b!=0)){    
    fill_solid( leds, NUM_LEDS, CRGB(r,g,b));
    FastLED.show();
    button = 0;
    Blynk.virtualWrite(V3, 0);
  }
  else if ((r==0) and (g==0) and (b==0)) {
    button = 0;
    Blynk.virtualWrite(V3, 0);
    off();
  }

}
BLYNK_WRITE(V2){  
  led = param.asInt();
  if (led !=1){
    Blynk.virtualWrite(V1, 0,0,0);
    button=0;
    r = 0;
    g = 0;
    b = 0;
    Blynk.virtualWrite(V3, 0);
    off();
    
  }

}
BLYNK_WRITE(V3){
   button = param.asInt();
   if (button == 1) Blynk.virtualWrite(V1, 0,0,0);
   
//  if ( int button ==1 and param.asInt()) 

}

void DEbug(){
  Serial.print("rgb ");
  Serial.print(r);
  Serial.print(g);
  Serial.println(b);
  Serial.print("button ");
  Serial.println(button);
}
BLYNK_CONNECTED() {

  // You can also update individual virtual pins like this:
  Blynk.syncVirtual(V1, V2, V3);

}
void nextPattern(){
  ChangePalettePeriodically();
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  
  FillLEDsFromPaletteColors( startIndex);
  
  FastLED.show();
  FastLED.delay(1000 / 100);

}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );  // GRB ordering is typical
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();


}

void loop()
{
  Blynk.run();
//  DEbug();
  if (button == 1){
//    Blynk.virtualWrite(V1, 0,0,0);
    nextPattern();
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
    // 'black out' all 16 palette entries...
    fill_solid( currentPalette, 16, CRGB::Black);
    // and set every fourth one to white.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
