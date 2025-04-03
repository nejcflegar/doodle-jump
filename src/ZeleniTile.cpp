class ZeleniTile{
    public: int x;
    public: int y;
    public: ZeleniTile(int x, int y){
        this->x = x;
        this->y = y;
    }
};
class ModriTile{
    public: int x;
    public: int y;
    public: bool levo = false;
    public: ModriTile(int x, int y){
        this->x = x;
        this->y = y;
    }

};
        
class RjaviTile{
    public: int x;
    public: int y;
    public: bool pada = false;
    public: bool up = false;
    public: RjaviTile(int x, int y){
        this->x = x;
        this->y = y;
    }
};

class Metek{
    public: int x;
    public: int y;
    public: int stSlike = 0; 
    public: bool uporabljen = false;
    public: Metek(int x, int y){
        this->x = x;
        this->y = y;
    }
};

class Posast{
    public: int x;
    public: int y;
    public: int premik;
    public: bool levo;
    public: bool uporabljen;
    public: Posast(int x, int y){
        this->x = x;
        this->y = y;
        this->uporabljen = false;
        this->levo = true;
        this->premik = 10;
    }
};