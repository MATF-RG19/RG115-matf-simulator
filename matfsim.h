#ifndef _MATFSIM_H
#define _MATFSIM_H

#define pi 3.141592653589793
#define MAX_STOLICA 18
#define TIMER_ID_MOV 0
#define TIMER_ID_THROW 1
#define TIMER_ID_CHAIR 2
#define TIMER_ID_TURNA 3
#define TIMER_ID_TURND 4

/* imena fajlova sa teksturama */
#define FILENAME0 "wall.bmp"
#define FILENAME1 "floor.bmp"
#define FILENAME2 "tabla.bmp"

/* identifikatori tekstura */
static GLuint names[3];

static int window_width, window_height;

static float camPosX = 0.0;
static float camPosY = 1.0;
static float camPosZ = 0.0;

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
static float adSens = 3;

static void initTex();
static void initStolice();

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_keyboard_up(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_mouse_motion(int x, int y);
static void on_mouse(int button, int state, int x, int y);

void drawAxes();
void drawWalls();
void drawCrosshair();
void drawStudent();
void drawChairs();
void drawMovedChairs();

/* ---------- ZA STOLICE ------------ */
GLuint listaStolica;

static float dovoljnaBlizina = 0.3;
static float dovoljniUgao = pi/8;

bool moving = false;
bool carrying = false;
bool throwing = false;
bool turning = false;

static float throwAnim = 0;
static float throwAnimSpeed = 4;
static float chairAnimSpeed = 0.1;
static float throwArmAngleMax = 30;
static float throwDist = 1;
static float throwXAngle = 0;
static float throwKX, throwKZ;

int brMoved = 0;
int idStolice;
float xS, zS;  // pozicija stolice koja se koristi za inicijalizaciju struct u drawChairs
int carried = -1;
float rastojanje, ugao, sx, sz, smsx, smsz; // vars za podizanje stolice
float kolikoIspredDaSpusti = 0.18;

typedef struct Stolica{
	float zAngle, yAngle, xPos, zPos, yPos;	
	bool moved;
	bool isCarried;
	bool isInAir;

}Stolica;

Stolica stolice[MAX_STOLICA];
/* ptr na stolicu koja se baca */
Stolica* st;
/* ----------------------------------- */

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