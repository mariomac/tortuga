#ifndef TORTUGA_H_INCLUDED
#define TORTUGA_H_INCLUDED

#include <SDL/SDL.h>

// Cambios con respecto a versión de grwin
// pausar_lapiz ha pasado a ser velocidad_tortuga
// falta por implementar dibujo_libre

// Definiciones de constantes simbolicas

// Colores
#define BLACK   0
#define WHITE   1
#define RED     2
#define BLUE    3
#define GREEN   4
#define GRAY    5

//estado del lápiz
#define PEN_DOWN 1
#define PEN_UP   0

// velocidades
#define VELOCIDAD_LENTA 2
#define VELOCIDAD_MEDIA 4
#define VELOCIDAD_RAPIDA 18
#define VELOCIDAD_MAXIMA 600

//visibilidad del puntero
#define OFF 0
#define ON  1
// Estas son las funciones de la tortuga para practicas

void girar(float grados);
void avanzar(float distancia);
void posicion_inicial(float posX, float posY);
void angulo_inicial(float grados);
void punta_lapiz(int color, int grosor);
void estado_lapiz(int estado);
void velocidad_tortuga(int velocidad);
void puntero_lapiz(int visible);

// Aqui acaban las funciones de la tortuga para practicas
// A partir de aqui, todo son funciones internas

#include <math.h>




#define FRAMES_SECOND 30

#define DELAY 1000 / FRAMES_SECOND

#define BUFFERWIDTH 800
#define BUFFERHEIGHT 600
#define TURTLE_STEPS 10
#define TBW 33

#define PIXELS_AVANCE 20

char tbc[TBW][TBW] = {
    "             XXXXXX             ",
    "            X......X            ",
    "           X........X           ",
    "  XXXX    X..X....X..X          ",
    " X....X   X..X... X..X          ",
    "X.....X   X..........X          ",
    "X......X   X........X      XXXX ",
    "X.......X  XXX....XXX     X....X",
    " X.......XXXooXXXXooXX   X.....X",
    "  XX.....XooXooooooXooX X......X",
    "    X...XoooXooooooXoooX......X ",
    "     X.XoooooXooooXoooooX....X  ",
    "      XXXXooooXXXXooooXXX...X   ",
    "       XooXXoXooooXoXXooXXX     ",
    "       XooooXooooooXooooX       ",
    "      XoooooXooooooXoooooX      ",
    "      XoooXXXooooooXXXoooX      ",
    "      XXXXoooXooooXoooXXXX      ",
    "      XoooooooXXXXoooooooX      ",
    "      XXXXoooXooooXoooXXXX      ",
    "      XoooXXXooooooXXXoooX      ",
    "       XoooXoXooooXoXoooX       ",
    "       XoooXoXooooXoXoooXXX     ",
    "      XXooXoooXooXoooXooX...X   ",
    "     X.XXXooooXXXXooooXXX....X  ",
    "    X...XooXXXooooXXXooX......X ",
    "  XX.....XXoooXooXoooXX X......X",
    " X.......XXooooXXooooX   X.....X",
    "X.......X  XXXXooXXXX     X....X",
    "X......X      XXXX         XXXX ",
    "X.....X        X..X             ",
    " X...X          XX.X            ",
    "  XXXX            XXXX          "
};

typedef struct {
    float posX;
    float posY;
    float rotation;
    float velocidad;
    int step;
    Uint32 color;
    int grosor;
    int estadoLapiz;
    int visible;
} Ttortuga;


SDL_Surface* screen;
Ttortuga turtle;

SDL_Surface* tablero; //borrar
SDL_Rect dstrect; //borrar

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
int thread_func(void *unused);
void processEvents();
void line(int x0, int x1, int y0, int y1);

void dibujaTortuga(Ttortuga *t) {
    float rad = (2 * 3.141592653589793 * t->rotation) / 360;
    int x,y;
    float sx,sy;

    Uint32 pixel, transparent;
    transparent = SDL_MapRGB(screen->format, 255,0,255);
    for(y = -4 ; y <= TBW + 4; y++) {
        for(x= -4 ; x <= TBW + 4; x++) {
            //sx = (x - t->posX) * cos(rad) + (y - t->posY) * sin(rad) + TBW/2;
            //sy = (y - t->posY) * cos(rad) - (x - t->posX) * sin(rad) + TBW/2;
            sx = (x - TBW/2) * cos(rad) + (y - TBW/2) * sin(rad) + TBW/2;
            sy = (y - TBW/2) * cos(rad) - (x - TBW/2) * sin(rad) + TBW/2;
            if(t->step > TURTLE_STEPS / 2) {
                sy = TBW - sy;
            }
            if(sx >= 0 && sy >= 0 && sx < TBW && sy < TBW) {
                switch(tbc[TBW-1-(int)sx][(int)sy]) {
                    case 'X':
                        pixel=SDL_MapRGB(screen->format,0,0,0);
                        break;
                    case 'o':
                        pixel=SDL_MapRGB(screen->format,0,100,0);
                        break;
                    case '.':
                        pixel=SDL_MapRGB(screen->format,0,255,0);
                        break;
                    default:
                        pixel = transparent;
                        break;
                }
            } else {
                pixel = transparent;
            }
            if(pixel != transparent) {
                putpixel(screen,x + t->posX - TBW/2,BUFFERHEIGHT - (y + t->posY-TBW/2),pixel);
            }

        }
    }

}

void inicializar() {
    int x,y;
    Uint32 pixel;
    Uint32 transparent;
    SDL_PixelFormat fmt;

    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window

    screen = SDL_SetVideoMode(BUFFERWIDTH, BUFFERHEIGHT, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return;
    }

    fmt = *(screen->format);
    // load an image
    tablero = SDL_CreateRGBSurface(SDL_HWSURFACE,BUFFERWIDTH,BUFFERHEIGHT,
                    fmt.BitsPerPixel,
                  fmt.Rmask,fmt.Gmask,fmt.Bmask,fmt.Amask );
    if (!tablero)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return;
    }

    pixel = SDL_MapRGB(screen->format,250,240,240);
    SDL_LockSurface(tablero);
    for(y = 0 ; y < BUFFERHEIGHT ; y++) {
        for(x=0 ; x < BUFFERWIDTH ; x++) {
            putpixel(tablero,x,y,pixel);
        }
    }
    SDL_UnlockSurface(tablero);

    // centre the bitmap on screen
    dstrect.x = (screen->w - tablero->w) / 2;
    dstrect.y = (screen->h - tablero->h) / 2;

    turtle.posX=10;
    turtle.posY=10;
    turtle.rotation=0;
    turtle.velocidad=VELOCIDAD_MEDIA;
    turtle.step = 0;
    turtle.color = 0;
    turtle.grosor = 3;
    turtle.estadoLapiz = PEN_DOWN;
    turtle.visible = ON;

    // Hacer el color rosa como transparente
    transparent = SDL_MapRGB(screen->format, 255,0,255);
    SDL_SetColorKey( screen, SDL_RLEACCEL | SDL_SRCCOLORKEY, transparent);
}

void dibujaTablero() {
        // DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw bitmap
        SDL_BlitSurface(tablero, 0, screen, &dstrect);

        // DRAWING ENDS HERE
        if(turtle.visible) {
            dibujaTortuga(&turtle);
        }
        //SDL_BlitSurface(turtle.img, (SDL_Rect*)NULL, screen, &turtle.pos);

        // finally, update the screen :)
        SDL_Flip(screen);
}

void freeResources() {
    SDL_FreeSurface(tablero);
}
void finalizar() {
    SDL_Event event;
    while(1) {
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                freeResources();
                exit(EXIT_SUCCESS);
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        freeResources();
                        exit(EXIT_SUCCESS);
                    }
                    break;
                }
            } // end switch
        } // end of message processing
        dibujaTablero();
        SDL_Delay(DELAY);
    }
}


void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void processEvents() {
    // message processing loop
    turtle.step = (turtle.step + 1) % TURTLE_STEPS;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // check for messages
        switch (event.type)
        {
            // exit if the window is closed
        case SDL_QUIT:
            freeResources();
            exit(EXIT_SUCCESS);
            break;

            // check for keypresses
        case SDL_KEYDOWN:
            {
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    freeResources();
                    exit(EXIT_SUCCESS);
                }
                break;
            }
        } // end switch
    } // end of message processing
    dibujaTablero();
    SDL_Delay(DELAY);
}

void girar(float angulo) {
    float init = turtle.rotation;
    float giro = 0;
    float pasos = 0;
    do {
        pasos++;
        if(abs(pasos * turtle.velocidad) >= abs(angulo)) {
            giro = angulo;
        } else {
            giro = pasos * (angulo > 0 ? turtle.velocidad : -turtle.velocidad);
        }
        turtle.rotation = init + giro;

        processEvents();
    } while(abs(pasos * turtle.velocidad) < abs(angulo));
}

void posicion_inicial(float posX, float posY) {
    turtle.posX = posX * PIXELS_AVANCE;
    turtle.posY = posY * PIXELS_AVANCE;
}

void angulo_inicial(float grados) {
    turtle.rotation = grados;
}

void punta_lapiz(int color, int grosor) {
    switch(color) {
        case BLACK:   turtle.color=SDL_MapRGB(tablero->format,0,0,0); break;
        case WHITE:   turtle.color=SDL_MapRGB(tablero->format,255,255,255); break;
        case RED:     turtle.color=SDL_MapRGB(tablero->format,255,0,0); break;
        case BLUE:    turtle.color=SDL_MapRGB(tablero->format,0,0,255); break;
        case GREEN:   turtle.color=SDL_MapRGB(tablero->format,0,255,0); break;
        case GRAY:    turtle.color=SDL_MapRGB(tablero->format,128,128,128); break;
    }
    turtle.grosor = grosor;
}

void avanzar(float distancia) {
    float abs = distancia * PIXELS_AVANCE;
    float a;
    float initx = turtle.posX;
    float inity = turtle.posY;
    float lastx = initx;
    float lasty = inity;
    float pasos = 0;

    float rad = (3.141592653589793f * turtle.rotation) / 180.0f;
    do {
        pasos++;
        if(turtle.velocidad * pasos >= abs) {
            a = abs;
        } else {
            a = turtle.velocidad * pasos;
        }
        lastx = turtle.posX;
        lasty = turtle.posY;
        turtle.posX = initx + a * cos(rad);
        turtle.posY = inity + a * sin(rad);
        if(turtle.estadoLapiz) line(lastx,lasty,turtle.posX,turtle.posY);
        processEvents();
    } while (turtle.velocidad * pasos < abs);
}

void rasterCircle(int x0, int y0)
{
  int f = 1 - turtle.grosor;
  int ddF_x = 1;
  int ddF_y = -2 * turtle.grosor;
  int x = 0;
  int y = turtle.grosor;
  int i;

  putpixel(tablero,x0, y0 + turtle.grosor,turtle.color);
  putpixel(tablero,x0, y0 - turtle.grosor,turtle.color);
  putpixel(tablero,x0 + turtle.grosor, y0,turtle.color);
  putpixel(tablero,x0 - turtle.grosor, y0,turtle.color);

  while(x < y)
  {
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
    if(f >= 0)
    {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    for(i = -x ; i <=x ; i++) {
        putpixel(tablero,x0 + i, y0 + y,turtle.color);
        putpixel(tablero,x0 + i, y0 - y,turtle.color);
    }
    for(i = -y ; i <= y ; i++) {
        putpixel(tablero,x0 + i, y0 + x,turtle.color);
        putpixel(tablero,x0 + i, y0 - x,turtle.color);
    }
    for(i = -turtle.grosor ; i <= turtle.grosor ; i++) {
        putpixel(tablero,x0 - i , y0, turtle.color);
    }
    /*
    putpixel(tablero,x0 - x, y0 + y,turtle.color);
    putpixel(tablero,x0 + x, y0 - y,turtle.color);
    putpixel(tablero,x0 - x, y0 - y,turtle.color);
    putpixel(tablero,x0 + y, y0 + x,turtle.color);
    putpixel(tablero,x0 - y, y0 + x,turtle.color);
    putpixel(tablero,x0 + y, y0 - x,turtle.color);
    putpixel(tablero,x0 - y, y0 - x,turtle.color);
    */
  }
}

void line(int x1,int y1,int x2,int y2)
{
  int dx, dy, i, e;
  int incx, incy, inc1, inc2;
  int x,y;

  dx = x2 - x1;
  dy = y2 - y1;

  if(dx < 0) dx = -dx;
  if(dy < 0) dy = -dy;
  incx = 1;
  if(x2 < x1) incx = -1;
  incy = 1;
  if(y2 < y1) incy = -1;
  x=x1;
  y=y1;

  if(dx > dy)
    {
      rasterCircle(x,BUFFERHEIGHT-y);
      e = 2*dy - dx;
      inc1 = 2*( dy -dx);
      inc2 = 2*dy;
      for(i = 0; i < dx; i++)
      {
         if(e >= 0)
         {
            y += incy;
            e += inc1;
         }
         else e += inc2;
         x += incx;
         rasterCircle(x,BUFFERHEIGHT-y);

      }
   }
   else
   {
      rasterCircle(x,BUFFERHEIGHT-y);

      e = 2*dx - dy;
      inc1 = 2*( dx - dy);
      inc2 = 2*dx;
      for(i = 0; i < dy; i++)
      {
        if(e >= 0)
        {
           x += incx;
           e += inc1;
        }
        else e += inc2;
        y += incy;
      rasterCircle(x,BUFFERHEIGHT-y);

    }
  }
 }

void estado_lapiz(int estado) {
    turtle.estadoLapiz = estado;
}

void velocidad_tortuga(int velocidad) {
    turtle.velocidad = velocidad;
}

void puntero_lapiz(int visible) {
    turtle.visible = visible;
}

#endif // TORTUGA_H_INCLUDED
