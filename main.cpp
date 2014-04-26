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

#define NONE 0
#define LOGOS 1
#define MAIN_MENU 2
#define NEW_GAME_SCREEN 3
#define STORY 4
#define SCENE1p1 10
#define SCENE1p2 11

int currentGameState = NONE;

void gotoMainMenu(){
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

SDL_Surface* story1 = NULL, *story2 = NULL, *story3 = NULL;

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

int init_sdl() {
    logs = fopen("log.log","w");
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);

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
void loadGame(){}
void playGame(){}

void gotoStory(){currentGameState=STORY;}

void continueLastGame() {
    loadGame();
    playGame();
}
bool justenterdedstory;
void playNewGame() {
    gotoStory();
    justenterdedstory=true;
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

    if((story1      = load("res/story/story1.png"))==NULL)
        return 1;
    if((story2      = load("res/story/story2.png"))==NULL)
        return 1;
    if((story3      = load("res/story/story3.png"))==NULL)
        return 1;
}

bool clickedButton(button b, int x, int y){
    bool wx = b.b.x<=x&&(b.b.x+b.b.w)>=x;
    bool wy = b.b.y<=y&&(b.b.y+b.b.h)>=y;
    return wx&&wy&&(b.gameState==currentGameState);
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
            keys[e.key.keysym.scancode]=true;
            break;
        }
        case SDL_KEYUP: {
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
                    for(int i=0;i<200;i++){
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
                break;
            }

            case STORY: {
                if(justenterdedstory){
                    justenterdedstory=false;
                    showandwaitforinput(story1);
                    showandwaitforinput(story2);
                    showandwaitforinput(story3);
                }
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
