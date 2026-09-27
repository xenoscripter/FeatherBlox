#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct { SDL_Rect r; const char *label; } Button;

static int command_exists(const char *cmd) {
    char test[256];
    snprintf(test, sizeof(test), "command -v %s >/dev/null 2>&1", cmd);
    return system(test) == 0;
}
static int cordial_installed(void) {
    return system("flatpak info io.github.luohoa97.Cordial >/dev/null 2>&1") == 0;
}
static void install_info(void) {
    puts("Install Cordial:");
    puts("flatpak remote-add --if-not-exists cordial https://luohoa97.github.io/cordial/cordial.flatpakrepo");
    puts("flatpak install cordial io.github.luohoa97.Cordial");
}
static int launch_cordial(void) {
    puts("Starting Roblox through Cordial...");
    fflush(stdout);
    return system("flatpak run io.github.luohoa97.Cordial");
}
static void draw_text(SDL_Renderer *ren, TTF_Font *font, const char *s, int x, int y) {
    SDL_Color c={245,240,255,255};
    SDL_Surface *surf=TTF_RenderUTF8_Blended(font,s,c);
    if(!surf) return;
    SDL_Texture *tex=SDL_CreateTextureFromSurface(ren,surf);
    SDL_Rect dst={x,y,surf->w,surf->h};
    SDL_RenderCopy(ren,tex,NULL,&dst);
    SDL_DestroyTexture(tex); SDL_FreeSurface(surf);
}
static void draw_button(SDL_Renderer *ren, TTF_Font *font, Button *b, int hover) {
    SDL_SetRenderDrawColor(ren, hover?150:95, hover?65:30, hover?230:130, 255);
    SDL_RenderFillRect(ren,&b->r);
    SDL_SetRenderDrawColor(ren,205,140,255,255);
    SDL_RenderDrawRect(ren,&b->r);
    SDL_Surface *surf=TTF_RenderUTF8_Blended(font,b->label,(SDL_Color){255,255,255,255});
    if(surf) {
        SDL_Texture *tex=SDL_CreateTextureFromSurface(ren,surf);
        SDL_Rect d={b->r.x+(b->r.w-surf->w)/2,b->r.y+(b->r.h-surf->h)/2,surf->w,surf->h};
        SDL_RenderCopy(ren,tex,NULL,&d);
        SDL_DestroyTexture(tex); SDL_FreeSurface(surf);
    }
}
int main(void) {
    if(SDL_Init(SDL_INIT_VIDEO)!=0) { fprintf(stderr,"SDL: %s\n",SDL_GetError()); return 1; }
    if(TTF_Init()!=0) { fprintf(stderr,"SDL_ttf: %s\n",TTF_GetError()); SDL_Quit(); return 1; }

    SDL_Window *win=SDL_CreateWindow("FeatherBlox 2",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,720,440,SDL_WINDOW_SHOWN);
    SDL_Renderer *ren=SDL_CreateRenderer(win,-1,SDL_RENDERER_SOFTWARE);
    if(!win || !ren) { fprintf(stderr,"Window/renderer failed: %s\n",SDL_GetError()); TTF_Quit(); SDL_Quit(); return 1; }

    const char *fonts[]={
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
    };
    TTF_Font *title=NULL,*font=NULL;
    for(size_t i=0;i<sizeof(fonts)/sizeof(fonts[0]);i++) {
        title=TTF_OpenFont(fonts[i],28); font=TTF_OpenFont(fonts[i],18);
        if(title && font) break;
        if(title) TTF_CloseFont(title); if(font) TTF_CloseFont(font);
        title=font=NULL;
    }
    if(!font || !title) { fprintf(stderr,"No usable font found.\n"); SDL_DestroyRenderer(ren); SDL_DestroyWindow(win); TTF_Quit(); SDL_Quit(); return 1; }

    Button launch={{70,145,580,62},"Launch Roblox"};
    Button status={{70,225,280,52},"Check Runtime"};
    Button install={{370,225,280,52},"Install Cordial"};
    Button quit={{70,300,580,52},"Quit"};

    int running=1;
    while(running) {
        SDL_Event e;
        int mx,my;
        SDL_GetMouseState(&mx,&my);
        while(SDL_PollEvent(&e)) {
            if(e.type==SDL_QUIT) running=0;
            if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) {
                SDL_Point p={e.button.x,e.button.y};
                if(SDL_PointInRect(&p,&launch)) {
                    if(!command_exists("flatpak")) {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"FeatherBlox 2","Flatpak is missing. Install flatpak first.",win);
                    } else if(!cordial_installed()) {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING,"Cordial not installed","Click 'Install Cordial' first, then launch Roblox.",win);
                    } else {
                        running=0;
                        launch_cordial();
                    }
                } else if(SDL_PointInRect(&p,&status)) {
                    char msg[256];
                    snprintf(msg,sizeof(msg),"Flatpak: %s\nCordial: %s\nVulkan required by FeatherBlox: NO\nRoblox backend: Cordial",
                        command_exists("flatpak")?"available":"missing",
                        cordial_installed()?"installed":"not installed");
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Runtime Status",msg,win);
                } else if(SDL_PointInRect(&p,&install)) {
                    install_info();
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,"Install Cordial",
                        "Run the two Cordial Flatpak commands shown in the terminal, then return here.",win);
                } else if(SDL_PointInRect(&p,&quit)) running=0;
            }
        }
        SDL_SetRenderDrawColor(ren,10,5,18,255); SDL_RenderClear(ren);
        draw_text(ren,title,"FeatherBlox 2",70,45);
        draw_text(ren,font,"Lightweight Roblox launcher • OpenGL/GLES target • no Vulkan dependency",70,90);
        draw_button(ren,font,&launch,SDL_PointInRect(&(SDL_Point){mx,my},&launch));
        draw_button(ren,font,&status,SDL_PointInRect(&(SDL_Point){mx,my},&status));
        draw_button(ren,font,&install,SDL_PointInRect(&(SDL_Point){mx,my},&install));
        draw_button(ren,font,&quit,SDL_PointInRect(&(SDL_Point){mx,my},&quit));
        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }
    TTF_CloseFont(title); TTF_CloseFont(font); SDL_DestroyRenderer(ren); SDL_DestroyWindow(win);
    TTF_Quit(); SDL_Quit(); return 0;
}
