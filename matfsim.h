#ifndef _MATFSIM_H
#define _MATFSIM_H

#define pi 3.141592653589793

static int window_width, window_height;

static float camPosX = 0.0;
static float camPosY = 1.0;
static float camPosZ = 0.0;

static float camCenterX = 0.0;
static float camCenterY = 0.0;
static float camCenterZ = 0.0;

static float camUpX = 0;
static float camUpY = 1;
static float camUpZ = 0;

static float kx = -1;
static float ky = 0;
static float kz = -1;
static float xzlen = 0;

// uglovi rotacije oko x i y ose
static float xAngle = 180;
static float yAngle = 0;

static bool keyStates[256] = {0};
static float moveSens = 30;
static float mouseSens = 0.5;

static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_mouse_motion(int x, int y);

void drawAxes();

#endif