#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<cstdio>
#include<cstdlib>
#ifdef WIN32
#include<windows.h>
#endif // WIN32

FILE* logs;

bool running;
bool justEnteredMMenu;

#define logoMillis 2000

#define walkTime 100
#define standTime 500

#define walkSpeed 2

#define NONE 0
#define LOGOS 1
#define MAIN_MENU 2
#define NEW_GAME_SCREEN 3
#define STORY  4
#define STORY2 5
#define SCENE1p1 11
#define SCENE1p2 12
#define SCENE1p3 13
#define MISSION1 101
#define DIGDOWN 102

int currentGameState = NONE;

void saveGame(){
    FILE* save = fopen("save","w");
    fprintf(save,"%d\n",currentGameState);
    if(currentGameState==DIGDOWN){
        printf("%d %d",levelWidth,levelHeight);
        for(int i=0;i<levelWidth;i++)
            for(int j=0;j<levelHeight;j++)
                printf("%c",' '+level[i][j]);
    }
}

void gotoMainMenu(){
    if(currentGameState>3)
        saveGame();
    currentGameState = MAIN_MENU;
    justEnteredMMenu = true;
}

SDL_Window* gWin = NULL;
SDL_Surface* gWinSrf = NULL;

//LOGOS & SPLASH
SDL_Surface* LD29_logo1 = NULL;
SDL_Surface* LD29_logo2 = NULL;
SDL_Surface* LD29_splash = NULL;

SDL_Surface* ioanD_logo = NULL;

SDL_Surface* winIcon = NULL;

SDL_Surface* gameTitle = NULL;

SDL_Surface* playButton = NULL, *quitButton = NULL;
SDL_Surface* newGame = NULL, *continueGame = NULL;
SDL_Surface* mainMenu = NULL;

SDL_Surface* scene1bars = NULL;
SDL_Surface* scene1nobars = NULL;

SDL_Surface* alpha1 = NULL;

SDL_Surface* playerMole = NULL;

SDL_Surface* story[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
int part;
SDL_Surface* SCENE1P3 = NULL;

SDL_Surface* Mission1[] = {NULL, NULL};
SDL_Surface* mission1bossdialogue = NULL;

#define WIDTH 640
#define HEIGHT 480

int init_sdl();
int loadMedia();
void gameLoop();
void cleanUp() {
    running = false;
    SDL_Quit();
    IMG_Quit();

    SDL_DestroyWindow(gWin);
    SDL_FreeSurface(gWinSrf);
}

bool waliking = false;

bool keys[512];

struct button {
    SDL_Rect b;
    SDL_Surface* s;
    void (*trigger)();
    int gameState;
} buttons[256];
int buttoncount;

SDL_Surface* load(char* url){
    SDL_Surface* result=NULL;
    result = IMG_Load(url);
    return result;
}

void openLDWebPage() {
#ifdef WIN32
    ShellExecute(NULL, "open", "http://www.ludumdare.com/compo", NULL, NULL, SW_SHOWNORMAL);
#else
    system("x-www-browser http://www.ludumdare.com/compo");
#endif // WIN32
    return;
}

void quit() {
    running = false;
}

int main(int argc, char* argv[]){
    if(init_sdl()==1)
        return 1;
    loadMedia();
    gameLoop();
    return 0;
}

int location;
int playerdir;
bool walking;
int x, y;

int init_sdl() {
    logs = fopen("log.log","w");
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    gWin = SDL_CreateWindow("Extinct",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
    if(gWin == NULL){
        fprintf(logs,"could not create window!!!");
        return 1;
    }

    gWinSrf = SDL_GetWindowSurface(gWin);
    if(gWinSrf == NULL){
        fprintf(logs,"Could not get the window's surface!!!");
        return 1;
    }
}
void loadGame() {

}
void playGame(bool newGame){
    currentGameState=DIGDOWN;
    location=newGame; // I use location as the level now
}

void gotoStory(){
    currentGameState=STORY;
}

void continueLastGame() {
    loadGame();
    playGame(false);
}
void playNewGame() {
    gotoStory();
}

void nothing(){}
bool firstTimeNGS;
void play() {
    currentGameState = NEW_GAME_SCREEN;
    firstTimeNGS = true;
}

int loadMedia(){
    if((LD29_logo1  = load("res/logo/ld29l1.png"))==NULL)
        return 1;
    if((LD29_logo2  = load("res/logo/ld29l2.png"))==NULL)
        return 1;
    if((ioanD_logo  = load("res/logo/mylogo.png"))==NULL)
        return 1;
    if((LD29_splash = load("res/logo/ld29sp.png"))==NULL)
        return 1;

    if((scene1bars  = load("res/scenes/scene1_bars.png"))==NULL)
        return 1;
    if((scene1nobars= load("res/scenes/scene1_nobars.png"))==NULL)
        return 1;

    if((winIcon     = load("res/icon.png"))==NULL)
        return 1;

    if((continueGame= load("res/buttons/continueGame.png"))==NULL)
        return 1;
    if((newGame     = load("res/buttons/newGame.png"))==NULL)
        return 1;
    if((mainMenu    = load("res/buttons/mainMenu.png"))==NULL)
        return 1;

    if((playerMole  = load("res/sprites/MolePlayer.png"))==NULL)
        return 1;

    if((gameTitle   = load("res/logo/gameTitle.png"))==NULL)
        return 1;

    if((playButton  = load("res/buttons/playButton.png"))==NULL)
        return 1;
    if((quitButton  = load("res/buttons/quitButton.png"))==NULL)
        return 1;

    if((alpha1      = load("res/alpha1blackpixel.png"))==NULL)
        return 1;

    if((story[1]    = load("res/story/story1.png"))==NULL)
        return 1;
    if((story[2]    = load("res/story/story2.png"))==NULL)
        return 1;
    if((story[3]    = load("res/story/story3.png"))==NULL)
        return 1;
    if((SCENE1P3    = load("res/scenes/scene1_p3.png"))==NULL)
        return 1;

    if((Mission1[0] = load("res/scenes/mission1_0.png"))==NULL)
        return 1;
    if((Mission1[1] = load("res/scenes/mission1_1.png"))==NULL)
        return 1;
    if((mission1bossdialogue = load("res/scenes/mission1_bossdial.png"))==NULL)
        return 1;
    if((story[4]    = load("res/story/story4.png"))==NULL)
        return 1;
    if((story[5]    = load("res/story/story5.png"))==NULL)
        return 1;
    return 0;
}

bool clickedButton(button b, int x, int y){
    bool wx = b.b.x<=x&&(b.b.x+b.b.w)>=x;
    bool wy = b.b.y<=y&&(b.b.y+b.b.h)>=y;
    return wx&&wy&&(b.gameState==currentGameState);
}

bool movement_key(int keysym){
    if(keysym==SDLK_d||keysym==SDLK_RIGHT){
        playerdir=0;//right
        return true;
    }
    if(keysym==SDLK_q||keysym==SDLK_a||keysym==SDLK_LEFT){
        playerdir=1;
        return true;
    }
    return false;
}

void handleEvent(SDL_Event e){
    switch(e.type){
        case SDL_QUIT: {
            if(currentGameState==MAIN_MENU)
                quit();
            else
                gotoMainMenu();
            break;
        }
        case SDL_KEYDOWN: {
            if(movement_key(e.key.keysym.sym))
                walking = true;
            keys[e.key.keysym.scancode]=true;
            break;
        }
        case SDL_KEYUP: {
            if(movement_key(e.key.keysym.sym))
                walking = false;
            keys[e.key.keysym.scancode]=false;
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            int mx = e.button.x, my = e.button.y;
            for(int i=0;i<buttoncount;i++){
                if(clickedButton(buttons[i],mx,my))
                    buttons[i].trigger();
            }
        }
    }
}

void makeButton(SDL_Surface* sprite, SDL_Rect rect, void (*trig)(), int gameState){
    buttons[buttoncount].s=sprite;
    buttons[buttoncount].b=rect;
    buttons[buttoncount].trigger = *trig;
    buttons[buttoncount].gameState = gameState;
    buttoncount++;
}

void tick(){
    SDL_Event e;
    while(SDL_PollEvent(&e))
        handleEvent(e);
}

void render() {
    for(int i=0;i<buttoncount;i++){
        if(buttons[i].gameState==currentGameState)
            SDL_BlitScaled(buttons[i].s,NULL,gWinSrf,&buttons[i].b);
    }
    SDL_UpdateWindowSurface(gWin);

}

void showandwaitforinput(SDL_Surface* s) {
    SDL_BlitScaled(s,NULL,gWinSrf,&gWinSrf->clip_rect);
    SDL_UpdateWindowSurface(gWin);
    SDL_Event e;
    while(true)
        while(SDL_PollEvent(&e))
            if(e.type==SDL_KEYDOWN)
                return;
}

void initButtons(){
    SDL_Rect buttonRect;

    buttonRect = {WIDTH/2-256,HEIGHT/2-132,256,128};
    makeButton(continueGame,buttonRect,&continueLastGame,NEW_GAME_SCREEN);
    buttonRect = {WIDTH/2,HEIGHT/2-132,256,128};
    makeButton(newGame,buttonRect,&playNewGame,NEW_GAME_SCREEN);
    buttonRect = {WIDTH/2-64,HEIGHT/2+32,128,64};
    makeButton(mainMenu,buttonRect,&gotoMainMenu,NEW_GAME_SCREEN);
    buttonRect={WIDTH/2-128,10,256,64};
    makeButton(gameTitle,buttonRect,&nothing, MAIN_MENU);
    buttonRect={WIDTH/2-64,80,128,64};
    makeButton(playButton,buttonRect,&play, MAIN_MENU);
    buttonRect={WIDTH/2-64,150,128,64};
    makeButton(quitButton,buttonRect,&quit, MAIN_MENU);
    buttonRect = {WIDTH-256,HEIGHT-128,256,128};
    makeButton(LD29_splash,buttonRect,&openLDWebPage, MAIN_MENU);

}
bool firstTimeSCENE1 = true;
long long scene1timer = -1;

void renderPlayer(int dir, bool walking, int x, int y){
    SDL_Rect r = {x,y,64,128};
    int X;
    if(walking)
        X=128+SDL_GetTicks()/walkTime%4*64;
    SDL_Rect k = {X,dir*128,64,128};
    SDL_BlitSurface(playerMole,&k,gWinSrf,&r);
}

bool left(){
    return keys[SDL_SCANCODE_A]||keys[SDL_SCANCODE_Q];
}
bool right(){
    return keys[SDL_SCANCODE_D];
}
bool up() {
    return keys[SDL_SCANCODE_W]||keys[SDL_SCANCODE_Z];
}

void gameLoop() {
    SDL_SetWindowIcon(gWin,winIcon);
    int ticks = 0; //-> should allways be 60 (sometimes 59 or 61)
    int frames = 0;
    long long timer = SDL_GetTicks();
    long long lastTime = SDL_GetTicks();
    const double ms = 1000.0/60.0;
    double delta = 0;
    running = true;
    initButtons();
    while(running){
        SDL_FillRect(gWinSrf,&gWinSrf->clip_rect,0);
        long long now = SDL_GetTicks();
        switch(currentGameState){
            case NONE: {
                currentGameState = LOGOS;
                break;
            }
            case LOGOS: {

                //LD29 logo1
                SDL_BlitScaled(LD29_logo1,NULL,gWinSrf,&gWinSrf->clip_rect);
                SDL_UpdateWindowSurface(gWin);
                long long timer = SDL_GetTicks();
                bool keypressed = false;
                while(SDL_GetTicks()-timer<=logoMillis&&!keypressed){
                    SDL_Event e;
                    while(SDL_PollEvent(&e))
                        if(e.type==SDL_KEYDOWN||e.type==SDL_QUIT)
                            keypressed=true;
                    SDL_UpdateWindowSurface(gWin);
                }
                if(!keypressed)
                    for(int i=0;i<200;i++){
                        SDL_BlitScaled(alpha1,NULL,gWinSrf,&gWinSrf->clip_rect);
                        SDL_BlitScaled(alpha1,NULL,gWinSrf,&gWinSrf->clip_rect);
                        SDL_UpdateWindowSurface(gWin);
                    }
                SDL_FillRect(gWinSrf,&gWinSrf->clip_rect,0);
                SDL_UpdateWindowSurface(gWin);

                //LD29 logo2
                SDL_BlitScaled(LD29_logo2,NULL,gWinSrf,&gWinSrf->clip_rect);
                SDL_UpdateWindowSurface(gWin);
                timer = SDL_GetTicks();
                keypressed = false;
                while(SDL_GetTicks()-timer<=logoMillis&&!keypressed){
                    SDL_Event e;
                    while(SDL_PollEvent(&e))
                        if(e.type==SDL_KEYDOWN||e.type==SDL_QUIT)
                            keypressed=true;
                    SDL_UpdateWindowSurface(gWin);
                }
                if(!keypressed)
                    for(int i=0;i<200;i++) {
                        SDL_BlitScaled(alpha1,NULL,gWinSrf,&gWinSrf->clip_rect);
                        SDL_BlitScaled(alpha1,NULL,gWinSrf,&gWinSrf->clip_rect);
                        SDL_UpdateWindowSurface(gWin);
                    }
                SDL_FillRect(gWinSrf,&gWinSrf->clip_rect,0);
                SDL_UpdateWindowSurface(gWin);


                //MY LOGO!!!
                SDL_BlitScaled(ioanD_logo,NULL,gWinSrf,&gWinSrf->clip_rect);
                SDL_UpdateWindowSurface(gWin);
                timer = SDL_GetTicks();
                keypressed = false;
                while(SDL_GetTicks()-timer<=logoMillis&&!keypressed){
                    SDL_Event e;
                    while(SDL_PollEvent(&e))
                        if(e.type==SDL_KEYDOWN||e.type==SDL_QUIT)
                            keypressed=true;
                    SDL_UpdateWindowSurface(gWin);
                }
                if(!keypressed)
                    for(int i=0;i<200;i++){
                        SDL_BlitScaled(alpha1,NULL,gWinSrf,&gWinSrf->clip_rect);
                        SDL_BlitScaled(alpha1,NULL,gWinSrf,&gWinSrf->clip_rect);
                        SDL_UpdateWindowSurface(gWin);
                    }
                SDL_FillRect(gWinSrf,&gWinSrf->clip_rect,0);
                SDL_UpdateWindowSurface(gWin);

                gotoMainMenu();
            }
            case MAIN_MENU: {
                SDL_Rect r = {100,100,256,128};
                SDL_BlitSurface(mission1bossdialogue,NULL,gWinSrf,&r);
                SDL_FillRect(gWinSrf,&gWinSrf->clip_rect,0xffd5ad);
                break;

            }
            /*case GAME: {
                int standingframe = SDL_GetTicks() / standTime % 2;
                int walkingframe = SDL_GetTicks() / walkTime % 4;
                SDL_Rect standrect, standrectd, walkrect, walkrectd;
                standrect  = {standingframe*64,0,64,128};
                standrectd = {0,0,64,128};
                SDL_BlitSurface(playerMole,&standrect,gWinSrf,&standrectd);
                walkrect  = {walkingframe*64,0,64,128};
                walkrectd = {100,0,64,128};
                SDL_BlitSurface(playerMole,&walkrect,gWinSrf,&walkrectd);
                break;
            }*/

            case SCENE1p1: {
                SDL_Event e;
                SDL_BlitSurface(scene1bars,NULL,gWinSrf,NULL);
                render();
                while(SDL_PollEvent(&e))
                    if(e.type==SDL_MOUSEBUTTONDOWN) {
                        int mx=e.button.x, my=e.button.y;
                        if(mx>=468&&my>=254&&mx<=521&&my<=334){
                            SDL_Rect pmr = {SDL_GetTicks()/standTime%2*64,0,64,128}, tr = {400,200,64,128};
                            SDL_BlitSurface(playerMole,&pmr,gWinSrf,&tr);
                            scene1timer=SDL_GetTicks();
                        }
                    } else if(e.type==SDL_QUIT)
                        gotoMainMenu();
                if(SDL_GetTicks()-scene1timer>2000&&scene1timer!=-1)
                    currentGameState=SCENE1p2;
                else{
                    if((SDL_GetTicks()-scene1timer)%42==0&&scene1timer!=-1){
                        SDL_Rect gwsr = {400,200,64,128}, pml = {SDL_GetTicks() / standTime % 2 * 64, 0, 64, 128};
                        SDL_BlitSurface(playerMole,&pml,gWinSrf,&gwsr);
                        render();
                    }
                }
                break;
            }

            case SCENE1p2: {
                SDL_BlitSurface(scene1nobars,NULL,gWinSrf,NULL);
                SDL_Rect gwsr = {400,200,64,128}, pml = {SDL_GetTicks() / standTime % 2 * 64, 128, 64, 128};
                SDL_BlitSurface(playerMole,&pml,gWinSrf,&gwsr);
                if(SDL_GetTicks()-scene1timer>6000)
                    currentGameState=SCENE1p3;
                break;
            }

            case SCENE1p3: {
                SDL_Rect r = {0,0,640,480};
                SDL_BlitSurface(SCENE1P3,NULL,gWinSrf,&r);
                SDL_Event e;
                while(SDL_PollEvent(&e)){
                    if(e.type==SDL_KEYDOWN) {
                        currentGameState=MISSION1;
                        location = 0;
                        x=550;
                        y=250;
                        playerdir = 0;
                    }
                }
                break;
            }

            case MISSION1: {
                SDL_BlitSurface(Mission1[location],NULL,gWinSrf,NULL);
                if(left()&&!right()&&x>0)
                    if(location==0||x>200)
                        x-=walkSpeed;
                    else{
                        SDL_Rect rct = {200,150,256,128};
                        SDL_BlitSurface(mission1bossdialogue,NULL,gWinSrf,&rct);
                        renderPlayer(1,false,x,y);
                        render();
                        SDL_Delay(5000);
                        currentGameState = STORY2;
                    }
                if(right()&&!left()&&x<WIDTH-64)
                    x+=walkSpeed;
                if(x>=30&&x<=120 && location == 0) {
                    location = 1;
                    x=450;
                }
                if(x>=512&&location==1){
                    location = 0 ;
                    x=550;
                }
                renderPlayer(playerdir,walking&&(left()!=right())&&(location==0||x>200),x,y);
                break;
            }

            case STORY2: {
                SDL_BlitSurface(story[part],NULL,gWinSrf,NULL);
                SDL_Event e;
                while(SDL_PollEvent(&e))
                    if(e.type==SDL_KEYDOWN)
                        part++;
                if(part==6)
                    playGame(true);
                break;
            }

            case STORY: {
                SDL_BlitSurface(story[part],NULL,gWinSrf,NULL);
                SDL_Event e;
                while(SDL_PollEvent(&e))
                    if(e.type==SDL_KEYDOWN)
                        part++;
                if(part==3)
                    currentGameState=SCENE1p1;
                //scene1timer=-1;
                break;
            }
            case NEW_GAME_SCREEN: {
                SDL_FillRect(gWinSrf,&gWinSrf->clip_rect,0xffd5ad);
                break;
            }
        }
            delta += (now-lastTime)/ms;
            lastTime = SDL_GetTicks();
            while(delta>=1.0){
                tick();
                ticks++;
                delta = delta - 1.0;
            }
            render();
            frames++;
            if(SDL_GetTicks()-timer>1000){
                ticks = frames = 0;
                timer += 1000;
            }
        }
}
