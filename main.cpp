#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

using namespace sf;

const int hSide=40;
const int vSide=40;
const int scrLimit=4;
const int blockSize=17;
const int width=hSide*blockSize;
const int hight=vSide*blockSize;

int direction=1;
int lastFrameDirection=direction;
int headsNum=5;
int lifes=5;
int score=0;

int aplx,aply;


struct snake {
    int x;
    int y=scrLimit;
} s[100];

void appleGenerate(){
    srand(time(0));
    aplx = rand()%hSide;
    aply = rand()%(vSide-scrLimit)+scrLimit;
}

void appleCheckAndEat(float *delay, Sound *sound){
    if( s[0].x==aplx && s[0].y==aply ){
        appleGenerate();
        headsNum++;
        score+=10;
        *delay-=0.002;
        sound->play();
    }
}

void borderCheck (){
    if ( (s[0].x ==  hSide ) && direction==0 ) s[0].x=0;
    if ( (s[0].x ==  -1) && direction==2 ) s[0].x=hSide-1; // 5alletha -1 me4 0 3a4an howa bye3mel check 2abl ma ye3mel draw, fa beltaly hay8ayyar el position 2abl el draw, fana zawwedt el 7odod belzabt zay fekret >,>=
    if ( (s[0].y ==  vSide) && direction==1 ) s[0].y=scrLimit;
    if ( (s[0].y ==  scrLimit-1) && direction==3 ) s[0].y=vSide-1; // nafs fekret el comment elly fo2
}

void collisionCheck (RenderWindow *window,Sound *a){
    for (int i=1;i<headsNum;i++)
        if (s[0].x==s[i].x && s[0].y==s[i].y){
            headsNum=i;
            lifes--;
            a->play();
            if (lifes==0){
                window->close();
                printf("Game Over")
                ;
            }
        }
}

void et7arrak (){

    //body moving
    for (int i=headsNum;i>0;i--){
        s[i].x = s[i-1].x;
        s[i].y = s[i-1].y;
    }

    //head moving
    if (direction == 0) s[0].x +=1;
    if (direction == 1) s[0].y +=1;
    if (direction == 2) s[0].x -=1;
    if (direction == 3) s[0].y -=1;
}

void directionControl (){
    if ( Keyboard::isKeyPressed(Keyboard::Right) &&  lastFrameDirection!= 2 )
        direction=0;
    if ( Keyboard::isKeyPressed(Keyboard::Down)  &&  lastFrameDirection!= 3 )
        direction=1;
    if ( Keyboard::isKeyPressed(Keyboard::Left)  &&  lastFrameDirection!= 0 )
        direction=2;
    if ( Keyboard::isKeyPressed(Keyboard::Up)  && lastFrameDirection!= 1 )
        direction=3;
}

int main()
{
    RenderWindow window(VideoMode(width,hight),"Game");
    window.setFramerateLimit(60);

    Texture t1,t2,t3;
    t1.loadFromFile("Images/white.png");
    t2.loadFromFile("Images/red.png");
    t3.loadFromFile("Images/green.png");

    Sprite sprite1(t1);
    Sprite sprite2(t2);
    Sprite sprite3(t3);
    Sprite sprite4(t3);

    ////////////////// sounds \\\\\\\\\\\\\\\\\\\

/*
    Music music;
    music.openFromFile("music.wav");
    music.play();*/

    SoundBuffer sweet1,sweet2,snake,ay;
    Sound swe1,swe2,sn,a;
    sweet1.loadFromFile("Sounds/Sweet.wav");
    snake.loadFromFile("Sounds/Snake.wav");
    ay.loadFromFile("Sounds/Ay.wav");
    swe1.setBuffer(sweet1);
    sn.setBuffer(snake);
    a.setBuffer(ay);
    sn.play();
    /////////////////////////////////////////////

    appleGenerate();

    float delay=0.1;
    float timer=0;
    float time;
    Clock clock;

    while (window.isOpen()){

        /////// close check & event throwing \\\\\\\\

        Event e;
        while (window.pollEvent(e))
            if (e.type == e.Closed)
                window.close();
        /////////////////////////////////////////////

        directionControl();

        ////////// timing & draw conditions\\\\\\\\\\\

        time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer+=time;

        if (timer>=delay){
            et7arrak();
            borderCheck();
            collisionCheck(&window,&a);
            appleCheckAndEat(&delay, &swe1);
            timer=0;
            lastFrameDirection=direction;
        }

        //////////////////// draw \\\\\\\\\\\\\\\\\\\\

        window.clear();

        //BackGround draw
        for (int i=scrLimit;i<vSide;i++)
            for (int j=0;j<hSide;j++){
                window.draw(sprite1);
                sprite1.setPosition(j*blockSize,i*blockSize);
            }

        //Snake draw
        sprite3.setPosition(s[0].x*blockSize , s[0].y*blockSize);
        window.draw(sprite3);
        for (int i=1;i<headsNum;i++){
                sprite2.setPosition( s[i].x*blockSize , s[i].y*blockSize);
                window.draw(sprite2);
        }

        //Apple draw
        sprite4.setPosition(aplx*blockSize , aply*blockSize);
        window.draw(sprite4);

        //Text draw
        Font font;
        font.loadFromFile("arial.TTF");

        int delayInt =101-1000*delay;

        Text text1("Lives: " + std::to_string(lifes) +
                    "\t\tScore: " + std::to_string(score)+
                      "\t\tSpeed: " + std::to_string(delayInt),font,35);
        window.draw(text1);

        window.display();


    }
}
