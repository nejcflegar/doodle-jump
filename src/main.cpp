#include <Arduino.h>
#include <TFT_eSPI.h>
#include "bg4.h"
#include "mc.h"
#include "tile.h"
#include "bg5.h"
#include "ZeleniTile.cpp"
#include "stack.cpp"
#include "brown.h"
#include "strelV2.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite zelenaTla[] = { TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft)};
TFT_eSprite modraTla[] = {TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft)};
TFT_eSprite rjavaTla[] = {TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft)};
TFT_eSprite strelSlika[] = {TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft)};
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite mc = TFT_eSprite(&tft);

TFT_eSprite check = TFT_eSprite(&tft);
TFT_eSprite scr = TFT_eSprite(&tft);
TFT_eSprite backgroundEnd = TFT_eSprite(&tft);

int button_levo = 0;
int button_desno = 35;
int button_strel = 12;

boolean stel_debounce = HIGH;

ZeleniTile* ZelTile[5];
ModriTile* ModTile[5];
RjaviTile* RjaTile[5];
Metek* strel[10];

int prevAtGroundY = 9999;
int score = 0;
int x = 0;
int y = 208;
int jumpTime = 0;
int atGroundY;
boolean atGround = true;
boolean atTileGround = false;
int stZelenih = 5;
int stModrih = 0;
stack sklad;
bool upI[5] = {false, false, false, false, false};


bool CoinFlip() {
  return rand() % 2 == 1;
}

void setup(){
  Serial.begin(9600);


  for(int i = 0; i < 5; i++){
    ZelTile[i] = new ZeleniTile(rand()%103,rand()%15+(230-50*i));
  }

  for(int i = 0; i < 5; i++){
    ModTile[i] = new ModriTile(rand()%103, 999);
  }
  
  for (int i = 0; i < 5; i++) {
    RjaTile[i] = new RjaviTile(rand() % 103, 999);
  }
  
  for(int i = 0; i < 10; i++){
    strel[i] = new Metek(-1119,-1119);
  }

  pinMode(button_levo, INPUT_PULLUP);
  pinMode(button_desno,  INPUT_PULLUP);
  pinMode(button_strel, INPUT_PULLUP);
  


  tft.setSwapBytes(true);
  tft.init();

  background.createSprite(135,240);
  background.setSwapBytes(true);
  background.pushImage(0,0,135,240,bg4);
  background.pushSprite(0,0);

  backgroundEnd.createSprite(135,240);
  backgroundEnd.setSwapBytes(true);
  backgroundEnd.pushImage(0,0,135,240,bg5);
  backgroundEnd.pushSprite(0,0);

  for(int i = 0; i < 5; i++){
    zelenaTla[i].createSprite(32,6);
    zelenaTla[i].setSwapBytes(false);
    zelenaTla[i].pushImage(0,0,32,6,tile[1]);
  }

  for(int i = 0; i < 5; i++){
    modraTla[i].createSprite(32,6);
    modraTla[i].setSwapBytes(false);
    modraTla[i].pushImage(0,0,32,6,tile[0]);
  }

  for(int i = 0; i < 5; i++){
    rjavaTla[i].createSprite(32,6);
    rjavaTla[i].setSwapBytes(false);
    rjavaTla[i].pushImage(0,0,32,6,brown[0]);
  }

  for(int i = 0; i < 10; i++){
    strelSlika[i].createSprite(8,8);
    strelSlika[i].setSwapBytes(false);
    strelSlika[i].pushImage(0,0,8,8,strelV2[0]);
  }

  mc.createSprite(32,32);
  mc.setSwapBytes(false);
  mc.pushImage(0,0,32,32,jaz[0]);
  mc.pushToSprite(&background,0,0,TFT_WHITE);

  scr.createSprite(50,50);
  scr.setTextSize(3);
  scr.setTextColor(TFT_BLACK,TFT_WHITE);
}
bool yay = false;
bool mogoceModra(){
  yay = false;
  int temp = rand()%500;
  for(int i = 0; i < score; i++){
    int temp2 = rand()%500;
    if(temp2 == temp){
      yay = true;
    }
  }
  return yay;
}

void slika(){
  background.pushImage(0,0,135,240,bg4);
  
  for(int i = 0; i < 5; i++){
    zelenaTla[i].pushToSprite(&background,ZelTile[i]->x,ZelTile[i]->y,TFT_WHITE);
  }
  
  for(int i = 0; i < 5; i++){
    modraTla[i].pushToSprite(&background,ModTile[i]->x,ModTile[i]->y,TFT_WHITE);
  }

  for(int i = 0; i < 5; i++){
    rjavaTla[i].pushToSprite(&background,RjaTile[i]->x,RjaTile[i]->y,TFT_WHITE);
  }

  for(int i = 0; i < 10; i++){
    strelSlika[i].pushToSprite(&background,strel[i]->x,strel[i]->y,TFT_WHITE);
  }

  mc.pushToSprite(&background,x,y,TFT_WHITE);
  background.pushSprite(0,0);
}

void move(boolean a){
  if(a){
    mc.pushImage(0,0,32,32,jaz[0]);
    mc.pushToSprite(&background,x,y,TFT_WHITE);
    x +=4;
    if(x > 125){
      x = -20;
    }
  }else{
    mc.pushImage(0,0,32,32,jaz[1]);
    mc.pushToSprite(&background,x,y,TFT_WHITE);
    x -=4;
    if(x < -20){
      x = 125;
    }
  }
}

void premakniModro(){
  for(int i = 0; i < 5; i++){
    if(upI[i]){
      if(ModTile[i]->levo){
        if(ModTile[i]->x < 0){
          ModTile[i]->levo = false;
        }
        ModTile[i]->x -= 2;
      }else{
        if(ModTile[i]->x > 109){
          ModTile[i]->levo = true;
        }
        ModTile[i]->x += 2;
      }
    }
  }
}

void genRjava(){
  for(int i = 0; i < 5; i++){
    if(RjaTile[i]->up){
    }else{
      RjaTile[i]->x = rand()%103;
      RjaTile[i]->y = -35;
      RjaTile[i]->up = true;
      break;
    }
  }
}

void mogoceRjava(){
  if(rand()%10 == rand()%10){
    genRjava();
  }
}



void checkTile(){
  
  for(int i = 0; i < 5; i++){
    if((-32 < (ZelTile[i]->x + 16 - (x + 16))) && ((ZelTile[i]->x + 16 - (x + 16)) < 32)){
      if(-2 < (ZelTile[i]->y - (y + 32)) && (ZelTile[i]->y - (y + 32) < 4)){
        if(jumpTime > 30 ){
          atGround = true;
          if((prevAtGroundY - y) > 5){
            score++;
            prevAtGroundY = y;
          }
        }
      }
    }
  }

  for(int i = 0; i < 5; i++){
    if(upI[i]){
      if((-32 < (ModTile[i]->x + 16 - (x + 16))) && ((ModTile[i]->x + 16 - (x + 16)) < 32)){
        if(-2 < (ModTile[i]->y - (y + 32)) && (ModTile[i]->y - (y + 32) < 4)){
          if(jumpTime > 30 ){
            atGround = true;
            if((prevAtGroundY - y) > 5){
              score++;
              prevAtGroundY = y;
            }
          }
        }
      }
    }
  }

  for(int i = 0; i < 5; i++){
    if(RjaTile[i]->up){
      if((-32 < (RjaTile[i]->x + 16 - (x + 16))) && ((RjaTile[i]->x + 16 - (x + 16)) < 32)){
        if(-2 < (RjaTile[i]->y - (y + 32)) && (RjaTile[i]->y - (y + 32) < 4)){
          RjaTile[i]->pada = true;
        }
      }
    }
  }
}

void jump(){
  if(atGround == true){
    atGround = false;
    jumpTime = 0;
  }else{
    if(jumpTime < 30){

      if(jumpTime <10){
        y -= 4;
      }else if(jumpTime <25){
        y -= 3;
      }else{
        y -= 1;
      }
      
    }else{

      if(jumpTime <35){
        y += 1;
      }else if(jumpTime <50){
        y += 3;
      }else{
        y += 4;
      }
      if(y >208){
        y = 208;
        atGround = true;
      }
    }
    checkTile();
  }
}

int poglejTaZadno(){
  int y = 999;
  for(int i = 0; i < 5; i++){
    if(ZelTile[i]->y < y){
      y = ZelTile[i]->y;
    }
  } 
  for(int i = 0; i < 5; i++){
    if(ModTile[i]->y < y){
      y = ModTile[i]->y;
    }
  } 

  return y - 50;
}

void generateTiles() {

  for (int i = 0; i < 5; i++) {
    if (ZelTile[i]->y > 240) {
      if (ModTile[i]->y > 240){  
          upI[i] = false; 
          mogoceRjava(); 
        }
      if(upI[i]){
        continue;
      }

      if (!mogoceModra()) {
        if (!upI[i]) {
          ZelTile[i]->x = (rand() % 103);
          ZelTile[i]->y = poglejTaZadno();
        }
      } else {
        if (!upI[i]) {  
          upI[i] = true;
          ModTile[i]->x = (rand() % 103);
          ModTile[i]->y = poglejTaZadno();
        }
      }
    }
  }
}

void dajDol(){
  if((y < 114) && (y > 80)){
    if(jumpTime <10){
        y += 1;
        prevAtGroundY +=1;
      }else if(jumpTime <25){
        y += 3;
        prevAtGroundY +=3;
      }else{
        y += 4;
        prevAtGroundY +=4;
      }
    for(int i = 0; i < 5; i++){
      ZelTile[i]->y += 3;
      ModTile[i]->y += 3;
      RjaTile[i]->y +=3;
    }
    for(int i = 0; i < 10; i++){
      if(strel[i]->uporabljen){
        strel[i]->y += 3;
      }
    }
  }else if((y < 80) && (y > 60)){
    y+=4;
    for(int i = 0; i < 5; i++){
      ZelTile[i]->y+=4;
      ModTile[i]->y += 4;
      RjaTile[i]->y +=4;
    }
    for(int i = 0; i < 10; i++){
      if(strel[i]->uporabljen){
        strel[i]->y += 4;
      }
    }
  }else if(y < 60){
    y+=6;
    for(int i = 0; i < 5; i++){
      ZelTile[i]->y+=6;
      ModTile[i]->y += 6;
      RjaTile[i]->y +=6;
      for(int i = 0; i < 10; i++){
        if(strel[i]->uporabljen){
          strel[i]->y += 6;
        }
      }
    }
  }

}

void padanjeRjave(){
  for(int i = 0; i < 5; i++){
    if(RjaTile[i]->pada && RjaTile[i]->up){
      rjavaTla[i].pushImage(0,0,32,6,brown[1]);
      RjaTile[i]->y += 3;
      if(RjaTile[i]->y > 250){
        RjaTile[i]->up = false;
        RjaTile[i]->pada = false;
        rjavaTla[i].pushImage(0,0,32,6,brown[0]);
      }
    }
  }
}

void smrtLoop(){
  smrtLoop();
}

void smrt(){
  if((y == 208) && (score != 0)){
    scr.fillSprite(TFT_WHITE);
    scr.drawString(String(score),0,0);
    scr.pushToSprite(&backgroundEnd,85,155,TFT_WHITE);
    backgroundEnd.pushSprite(0,0);
    smrtLoop();
  }
}

void streljacina(){
  mc.pushImage(0,0,32,32,jaz[2]);
  mc.pushToSprite(&background,x,y,TFT_WHITE);
  for(int i = 0; i < 10; i++){
    if(!strel[i]->uporabljen){
      strel[i]->x = x+12;
      strel[i]->y = y-12;
      strel[i]->uporabljen = true;
      break;
    }
  }
}

void premikanjeStrela(){
  for(int i = 0; i < 10; i++){
    if(strel[i]->uporabljen){
      strel[i]->y -=6;
      if(strel[i]->y < -10){
        strel[i]->uporabljen = false;
      }
    }
  }
}

void loop(){
  jumpTime++;

  if(digitalRead(button_desno) == 0){
    move(true);
  }
  if(digitalRead(button_levo) == 0){
    move(false);
  }
  if (digitalRead(button_strel) == LOW) {
    if (stel_debounce == HIGH) {
      streljacina();
    }
    stel_debounce = LOW;
  } else {
    stel_debounce = HIGH; 
  }


  premikanjeStrela();
  premakniModro();
  padanjeRjave();
  generateTiles();
  jump();
  dajDol();
  smrt();
  slika();
}