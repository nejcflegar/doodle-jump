#include <Arduino.h>
#include <TFT_eSPI.h>

class stack{
    public: int vredstiZeleneX[10] = {rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103};
    public: int vredstiZeleneY[10] = {200,150,100,50,0,-50,-100,-150,-200,-250}; 

    public: int ZpopX(){
        int temp = vredstiZeleneX[0];
        for(int i = 0; i < 9; i++){
            vredstiZeleneX[i] = vredstiZeleneX[i+1]; 
        }
        vredstiZeleneX[9] = rand()%103;
        return temp;
    }
    public: int ZpopY(){
        int temp = vredstiZeleneY[0];
        for(int i = 0; i < 9; i++){
            vredstiZeleneY[i] = vredstiZeleneY[i+1]; 
        }
        if((rand()%2) == 1){
            vredstiZeleneY[9] = -250 + rand()%20;
        }else{
            vredstiZeleneY[9] = -250 - rand()%20;
        }
        return temp;
    }

    public: int vredstiModriX[10] = {rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103,rand()%103};
    public: int vredstiModriY[10] = {0,-50,-100,-150,-200,-250};

    public: int MpopX(){
        int temp = vredstiModriX[0];
        for(int i = 0; i < 9; i++){
            vredstiModriX[i] = vredstiModriX[i+1]; 
        }
        vredstiModriX[9] = rand()%103;
        return temp;
    }
    public: int MpopY(){
        int temp = vredstiModriY[0];
        for(int i = 0; i < 9; i++){
            vredstiModriY[i] = vredstiModriY[i+1]; 
        }
        if((rand()%2) == 1){
            vredstiModriY[9] = -250 + rand()%20;
        }else{
            vredstiModriY[9] = -250 - rand()%20;
        }
        return temp;
    }

};
