#include <Arduino.h>
#include <TFT_eSPI.h>
#include "bg4.h"
#include "mc.h"
#include "tile.h"
#include "bg5.h"
#include "ZeleniTile.cpp"
#include "stack.cpp"


TFT_eSPI tft = TFT_eSPI();
TFT_eSprite zelenaTla[] = { TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft)};
TFT_eSprite modraTla[] = {TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft), TFT_eSprite(&tft)};
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite mc = TFT_eSprite(&tft);

TFT_eSprite Mtla1 = TFT_eSprite(&tft);
TFT_eSprite check = TFT_eSprite(&tft);
TFT_eSprite scr = TFT_eSprite(&tft);
TFT_eSprite backgroundEnd = TFT_eSprite(&tft);

int button_levo = 0;
int button_desno = 35;

ZeleniTile* ZelTile[5];
ModriTile* ModTile[5];

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
  
  pinMode(35, INPUT_PULLUP);
  pinMode(0,  INPUT_PULLUP);

  

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

  Mtla1.createSprite(32,6);
  Mtla1.setSwapBytes(false);
  Mtla1.pushImage(0,0,32,6,tile[0]);

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
  int temp = rand()%100;
  for(int i = 0; i < score; i++){
    int temp2 = rand()%100;
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
          Serial.print("false");
        }
        ModTile[i]->x -= 2;
      }else{
        if(ModTile[i]->x > 109){
          ModTile[i]->levo = true;
          Serial.print("true");
        }
        ModTile[i]->x += 2;
      }
    }
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

void generateTiles() {
  int i;
  if (upI[0] == true) {
    i = 1;
  } else {
    i = 0;
  }

  for (; i < stZelenih; i++) {
    if (ZelTile[i]->y > 240) {
      if (ModTile[i]->y > 240) {  
          upI[i] = false;  
        }
      if(upI[i]){
        continue;
      }

      if (!mogoceModra()) {
        if (!upI[i]) {
          ZelTile[i]->x = (rand() % 103);
          ZelTile[i]->y = -10;
        }
      } else {
        if (!upI[i]) {  
          upI[i] = true;
          ModTile[i]->x = (rand() % 103);
          ModTile[i]->y = -10;
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
    }
  }else if((y < 80) && (y > 60)){
    y+=4;
    for(int i = 0; i < 5; i++){
      ZelTile[i]->y+=4;
      ModTile[i]->y += 4;
    }
  }else if(y < 60){
    y+=6;
    for(int i = 0; i < 5; i++){
      ZelTile[i]->y+=6;
      ModTile[i]->y += 6;
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

void loop(){
  jumpTime++;

  if(digitalRead(button_desno) == 0){
    move(true);
  }
  if(digitalRead(button_levo) == 0){
    move(false);
  }
  
  premakniModro();
  generateTiles();
  jump();
  dajDol();
  smrt();
  slika();
}