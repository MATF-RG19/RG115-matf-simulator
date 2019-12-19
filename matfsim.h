#ifndef _MATFSIM_H
#define _MATFSIM_H

#define pi 3.141592653589793
#define MAX_STOLICA 18

#define TIMER_ID_MOV 0

static int window_width, window_height;

static float camPosX = 0.0;
static float camPosY = 1.0;
static float camPosZ = 0.0;

/* not used
static float camCenterX = 0.0;
static float camCenterY = 0.0;
static float camCenterZ = 0.0;
*/

static float camUpX = 0;
static float camUpY = 1;
static float camUpZ = 0;

static float kx = 1;
static float ky = 0;
static float kz = 0;
static float xzlen = 0;

// uglovi rotacije oko x i y ose
static float xAngle = 90;
static float yAngle = 0;

static float limbAngle = 0;
static float limbSpeed = 6;
static float limbMaxAngle = 40;
static int limbSign = 1;

static bool keyStates[256] = {0};
static float moveSens = 0.06;
static float mouseSens = 0.8;
static float bobSens = 0.0001;

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboard_up(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_move(int value);
static void on_mouse_motion(int x, int y);
static void on_mouse(int button, int state, int x, int y);

bool moving = false;
bool carrying = true; // zakucano zbog testiranja

void drawAxes();
void drawWalls();
void drawCrosshair();
void drawStudent();
void drawChairs();
void drawMovedChairs();

int brMoved = 0;
int idStolice;
float xS, zS;

typedef struct Stolica{
	int yAngle, xPos, zPos;	
	bool moved;
	bool isCarried;
}Stolica;

Stolica stolice[MAX_STOLICA];
Stolica movedChairs[MAX_STOLICA];

/* Bitmapa */
GLubyte crosshair[24] = {
   	0x0f, 0x00,	/* 0000 1111 0000 0000 */
	0x0f, 0x00,	/* 0000 1111 0000 0000 */
	0x0f, 0x00,	/* 0000 1111 0000 0000 */
	0x0f, 0x00,	/* 0000 1111 0000 0000 */
    0xff, 0xf0, /* 1111 1111 1111 0000 */
    0xff, 0xf0, /* 1111 1111 1111 0000 */
    0xff, 0xf0, /* 1111 1111 1111 0000 */
    0xff, 0xf0, /* 1111 1111 1111 0000 */
   	0x0f, 0x00,	/* 0000 1111 0000 0000 */
	0x0f, 0x00,	/* 0000 1111 0000 0000 */
	0x0f, 0x00,	/* 0000 1111 0000 0000 */
	0x0f, 0x00,	/* 0000 1111 0000 0000 */
};

#endif