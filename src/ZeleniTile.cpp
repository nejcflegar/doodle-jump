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