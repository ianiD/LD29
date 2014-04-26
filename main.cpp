#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<cstdio>

FILE* logs;

bool running;

#define logoMillis 2000

#define NONE 0
#define LOGOS 1
#define MAIN_MENU 2
#define GAME 3

int currentGameState = NONE;

SDL_Window* gWin = NULL;
SDL_Surface* gWinSrf = NULL;

//LOGOS & SPLASH
SDL_Surface* LD29_logo1 = NULL;
SDL_Surface* LD29_logo2 = NULL;
SDL_Surface* LD29_splash = NULL;

SDL_Surface* ioanD_logo = NULL;

SDL_Surface* winIcon = NULL;

SDL_Surface* playButton = NULL, *quitButton = NULL;

SDL_Surface* alpha1 = NULL;

#define WIDTH 640
#define HEIGHT 480

int init_sdl();
int loadMedia();
void gameLoop();
void cleanUp();

SDL_Surface* load(char* url){
    SDL_Surface* result=NULL;
    result = IMG_Load(url);
    return result;
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

    gWin = SDL_CreateWindow("Ludum dare 29",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
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

int loadMedia(){
    if((LD29_logo1  = load("res/logo/ld29l1.png"))==NULL)
        return 1;
    if((LD29_logo2  = load("res/logo/ld29l2.png"))==NULL)
        return 1;
    if((ioanD_logo  = load("res/logo/mylogo.png"))==NULL)
        return 1;
    if((LD29_splash = load("res/logo/ld29sp.png"))==NULL)
        return 1;

    if((winIcon     = load("res/icon.png"))==NULL)
        return 1;

    if((playButton  = load("res/buttons/playButton.png"))==NULL)
        return 1;
    if((quitButton  = load("res/buttons/quitButton.png"))==NULL)
        return 1;

    if((alpha1      = load("res/alpha1blackpixel.png"))==NULL)
        return 1;
}

void gameLoop() {
    running = true;
    while(running){
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
                        if(e.type==SDL_KEYDOWN)
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
                        if(e.type==SDL_KEYDOWN)
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
                        if(e.type==SDL_KEYDOWN)
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

                currentGameState = MAIN_MENU;
            }
        }
    }
}
