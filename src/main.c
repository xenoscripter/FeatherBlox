#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

static int command_exists(const char *cmd)
{
    char test[256];
    snprintf(test, sizeof(test), "command -v %s >/dev/null 2>&1", cmd);
    return system(test) == 0;
}

static int cordial_installed(void)
{
    return system("flatpak info io.github.luohoa97.Cordial >/dev/null 2>&1") == 0;
}

static void print_install(void)
{
    puts("");
    puts("Install Cordial:");
    puts("  flatpak remote-add --if-not-exists cordial https://luohoa97.github.io/cordial/cordial.flatpakrepo");
    puts("  flatpak install cordial io.github.luohoa97.Cordial");
    puts("");
}

static void print_status(void)
{
    puts("");
    puts("=== FeatherBlox 2 status ===");
    printf("Flatpak: %s\n", command_exists("flatpak") ? "available" : "missing");
    printf("Cordial: %s\n", cordial_installed() ? "installed" : "not installed");
    puts("FeatherBlox Vulkan dependency: none");
    puts("Roblox backend: Cordial");
    puts("");
}

static int launch_cordial(void)
{
    puts("Starting Roblox through Cordial...");
    fflush(stdout);
    return system("flatpak run io.github.luohoa97.Cordial");
}

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "FeatherBlox 2",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 360, SDL_WINDOW_SHOWN
    );

    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 12, 7, 20));
    SDL_UpdateWindowSurface(window);

    puts("");
    puts("FeatherBlox 2");
    puts("----------------");
    puts("[ENTER] Launch Roblox");
    puts("[C]     Check runtime");
    puts("[I]     Installation commands");
    puts("[ESC]   Quit");

    int running = 1;
    int launch = 0;

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
                    launch = 1;
                    running = 0;
                } else if (key == SDLK_c) {
                    print_status();
                } else if (key == SDLK_i) {
                    print_install();
                }
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    if (!launch)
        return 0;

    if (!command_exists("flatpak")) {
        fprintf(stderr, "Flatpak is missing. Run: sudo apt install flatpak\n");
        return 1;
    }

    if (!cordial_installed()) {
        fprintf(stderr, "Cordial is not installed.\n");
        print_install();
        return 1;
    }

    return launch_cordial();
}
