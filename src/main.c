#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 960
#define HEIGHT 540

typedef struct {
    float x, y, z;
    float yaw, pitch;
} Player;

static void cube(float x, float y, float z, float s)
{
    float h = s * 0.5f;
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);

    glColor3f(.55f,.25f,.85f);
    glVertex3f(-h,h,-h); glVertex3f(h,h,-h);
    glVertex3f(h,h,h); glVertex3f(-h,h,h);

    glColor3f(.18f,.10f,.25f);
    glVertex3f(-h,-h,h); glVertex3f(h,-h,h);
    glVertex3f(h,-h,-h); glVertex3f(-h,-h,-h);

    glColor3f(.45f,.18f,.72f);
    glVertex3f(-h,-h,h); glVertex3f(-h,h,h);
    glVertex3f(h,h,h); glVertex3f(h,-h,h);

    glColor3f(.35f,.12f,.58f);
    glVertex3f(h,-h,-h); glVertex3f(h,h,-h);
    glVertex3f(-h,h,-h); glVertex3f(-h,-h,-h);

    glColor3f(.40f,.15f,.65f);
    glVertex3f(-h,-h,-h); glVertex3f(-h,h,-h);
    glVertex3f(-h,h,h); glVertex3f(-h,-h,h);

    glColor3f(.62f,.28f,.90f);
    glVertex3f(h,-h,h); glVertex3f(h,h,h);
    glVertex3f(h,h,-h); glVertex3f(h,-h,-h);

    glEnd();
    glPopMatrix();
}

static void ground(void)
{
    glBegin(GL_QUADS);
    for (int z=-16; z<16; ++z) {
        for (int x=-16; x<16; ++x) {
            float c = ((x+z)&1) ? .10f : .13f;
            glColor3f(c,c,c+.03f);
            glVertex3f((float)x,0,(float)z);
            glVertex3f((float)x+1,0,(float)z);
            glVertex3f((float)x+1,0,(float)z+1);
            glVertex3f((float)x,0,(float)z+1);
        }
    }
    glEnd();
}

static void setup(int w, int h)
{
    if (h < 1) h = 1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0,(double)w/(double)h,.1,250.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_FLAT);
}

static void render(const Player *p)
{
    glClearColor(.025f,.02f,.05f,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-p->pitch,1,0,0);
    glRotatef(-p->yaw,0,1,0);
    glTranslatef(-p->x,-p->y,-p->z);

    ground();

    for (int x=-4; x<=4; ++x)
        cube((float)x,.5f,-5,1);

    for (int z=-4; z<=2; ++z)
        cube(4,.5f,(float)z,1);

    cube(0,.5f,-2,1);
    cube(1,1.5f,-2,1);
    cube(2,.5f,-2,1);
}

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        fprintf(stderr,"SDL_Init failed: %s\n",SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);

    SDL_Window *window = SDL_CreateWindow(
        "FeatherBlox",
        SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
        WIDTH,HEIGHT,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

    if (!window) {
        fprintf(stderr,"Window creation failed: %s\n",SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        fprintf(stderr,"OpenGL context failed: %s\n",SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(0);

    printf("FeatherBlox started\n");
    printf("OpenGL: %s\n",glGetString(GL_VERSION));
    printf("Renderer: %s\n",glGetString(GL_RENDERER));

    Player p={0,1.7f,6,0,8};
    int running=1;
    Uint64 previous=SDL_GetPerformanceCounter();

    SDL_SetRelativeMouseMode(SDL_TRUE);

    while (running) {
        Uint64 now=SDL_GetPerformanceCounter();
        float dt=(float)((double)(now-previous)/
                         (double)SDL_GetPerformanceFrequency());
        previous=now;
        if (dt>.05f) dt=.05f;

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type==SDL_QUIT) running=0;
            if (e.type==SDL_KEYDOWN &&
                e.key.keysym.sym==SDLK_ESCAPE) running=0;

            if (e.type==SDL_MOUSEMOTION) {
                p.yaw += e.motion.xrel*.12f;
                p.pitch += e.motion.yrel*.12f;
                if (p.pitch>89) p.pitch=89;
                if (p.pitch<-89) p.pitch=-89;
            }
        }

        const Uint8 *keys=SDL_GetKeyboardState(NULL);
        float speed=keys[SDL_SCANCODE_LSHIFT]?7.0f:4.0f;
        float move=speed*dt;
        float yaw=p.yaw*(float)M_PI/180.0f;
        float fx=sinf(yaw), fz=-cosf(yaw);
        float rx=cosf(yaw), rz=sinf(yaw);

        if (keys[SDL_SCANCODE_W]) { p.x+=fx*move; p.z+=fz*move; }
        if (keys[SDL_SCANCODE_S]) { p.x-=fx*move; p.z-=fz*move; }
        if (keys[SDL_SCANCODE_A]) { p.x-=rx*move; p.z-=rz*move; }
        if (keys[SDL_SCANCODE_D]) { p.x+=rx*move; p.z+=rz*move; }
        if (keys[SDL_SCANCODE_SPACE]) p.y+=move;
        if (keys[SDL_SCANCODE_LCTRL]) p.y-=move;

        int w,h;
        SDL_GetWindowSize(window,&w,&h);
        setup(w,h);
        render(&p);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
