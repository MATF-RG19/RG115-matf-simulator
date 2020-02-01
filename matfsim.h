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
GLuint names[3];

int window_width, window_height;

float camPosX = 0.0;
float camPosY = 1.0;
float camPosZ = 0.0;

float camUpX = 0;
float camUpY = 1;
float camUpZ = 0;

float kx = 1;
float ky = 0;
float kz = 0;
float xzlen = 0;

// uglovi rotacije oko x i y ose
float xAngle = 90;
float yAngle = 0;


float limbAngle = 0;
float limbSpeed = 6;
float limbMaxAngle = 40;
int limbSign = 1;

bool keyStates[256] = {0};
float moveSens = 0.06;
float mouseSens = 0.8;
float bobSens = 0.0001;
float adSens = 3;

void initTex();
void initStolice();

void on_display(void);
void on_keyboard(unsigned char key, int x, int y);
void on_keyboard_up(unsigned char key, int x, int y);
void on_reshape(int width, int height);
void on_timer(int value);
void on_mouse_motion(int x, int y);

void drawAxes();
void drawWalls();
void drawCrosshair();
void drawStudent();
void drawChairs();
void drawMovedChairs();

/* ---------- ZA STOLICE ------------ */
GLuint listaStolica;

float dovoljnaBlizina = 0.3;
float dovoljniUgao = pi/8;

bool moving = false;
bool carrying = false;
bool throwing = false;
bool turning = false;

float throwAnim = 0;
float throwAnimSpeed = 4;
float chairAnimSpeed = 0.1;
float throwArmAngleMax = 30;
float throwDist = 1;
float throwXAngle = 0;
float throwKX, throwKZ;

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