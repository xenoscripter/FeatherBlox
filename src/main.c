#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int command_exists(const char *cmd)
{
    char test[256];
    snprintf(test, sizeof(test), "command -v %s >/dev/null 2>&1", cmd);
    return system(test) == 0;
}

static int flatpak_app_installed(const char *app)
{
    char test[512];
    snprintf(test, sizeof(test),
             "flatpak info %s >/dev/null 2>&1", app);
    return system(test) == 0;
}

static int launch_cordial(void)
{
    printf("FeatherBlox Launcher: starting Cordial...\n");
    fflush(stdout);
    return system("flatpak run io.github.luohoa97.Cordial");
}

static void show_status(void)
{
    printf("\n=== FeatherBlox Launcher ===\n");
    printf("Roblox runtime: Cordial\n");
    printf("Cordial installed: %s\n",
           flatpak_app_installed("io.github.luohoa97.Cordial") ? "yes" : "no");
    printf("Flatpak available: %s\n",
           command_exists("flatpak") ? "yes" : "no");

    if (!command_exists("flatpak")) {
        printf("\nInstall Flatpak first:\n");
        printf("  sudo apt install flatpak\n");
    } else if (!flatpak_app_installed("io.github.luohoa97.Cordial")) {
        printf("\nInstall the Roblox-compatible runtime:\n");
        printf("  flatpak remote-add --if-not-exists cordial https://luohoa97.github.io/cordial/cordial.flatpakrepo\n");
        printf("  flatpak install cordial io.github.luohoa97.Cordial\n");
    }
}

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "FeatherBlox Launcher",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 360, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 18, 10, 28));
    SDL_UpdateWindowSurface(window);

    printf("\nFeatherBlox Launcher\n");
    printf("---------------------\n");
    printf("[ENTER] Launch Roblox through Cordial\n");
    printf("[I]     Show Cordial installation commands\n");
    printf("[C]     Check runtime\n");
    printf("[ESC]   Quit\n");

    int running = 1;
    int launch_requested = 0;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;

                if (key == SDLK_ESCAPE) {
                    running = 0;
                } else if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
                    launch_requested = 1;
                    running = 0;
                } else if (key == SDLK_c) {
                    show_status();
                } else if (key == SDLK_i) {
                    printf("\nCordial installation:\n");
                    printf("  flatpak remote-add --if-not-exists cordial https://luohoa97.github.io/cordial/cordial.flatpakrepo\n");
                    printf("  flatpak install cordial io.github.luohoa97.Cordial\n");
                }
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    if (!launch_requested)
        return 0;

    if (!command_exists("flatpak")) {
        fprintf(stderr, "Flatpak is not installed. Run: sudo apt install flatpak\n");
        return 1;
    }

    if (!flatpak_app_installed("io.github.luohoa97.Cordial")) {
        fprintf(stderr, "Cordial is not installed. Run:\n");
        fprintf(stderr, "  flatpak remote-add --if-not-exists cordial https://luohoa97.github.io/cordial/cordial.flatpakrepo\n");
        fprintf(stderr, "  flatpak install cordial io.github.luohoa97.Cordial\n");
        return 1;
    }

    return launch_cordial();
}
