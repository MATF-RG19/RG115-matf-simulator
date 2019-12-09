#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>
#include "matfsim.h"

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(150, 100);
	glutCreateWindow("MATF Simulator");

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glutDisplayFunc(on_display);
	glutKeyboardFunc(on_keyboard);
	glutKeyboardUpFunc(on_keyboard_up);
	glutReshapeFunc(on_reshape);
	glutPassiveMotionFunc(on_mouse_motion);

	glClearColor(0.6, 0.6, 0.6, 0);
	glEnable(GL_DEPTH_TEST);

	glLineWidth(3);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutMainLoop();
	
	return 0;
}

static void on_move(int value){
	if(value != TIMER_ID_MOV)
		return;
	if(limbAngle > limbMaxAngle || limbAngle < -limbMaxAngle)
		limbSign = -limbSign;

	limbAngle += limbSign*limbSpeed; 

	if(keyStates['w']){
		/* necemo da nam brzina kretanja zavisi od y ose*/
		xzlen = sqrt(kz*kz+kx*kx);
		camPosX += kx/xzlen*moveSens;
		camPosZ += kz/xzlen*moveSens;
	}

	glutPostRedisplay();

	if(moving)
		glutTimerFunc(30, on_move, value);
}

static void on_mouse_motion(int x, int y){
	
	/* menjamo uglove u zavisnosti od pozicije kursora */
	x -= window_width/2;
	y -= window_height/2;
	xAngle += y*mouseSens;
	yAngle += x*mouseSens;
	
	if(yAngle > 360.0){
        yAngle -= 360.0;
    }
    if(yAngle < -360.0){
        yAngle += 360.0;
    }
    /* necemo frontflip da radimo */
	if(xAngle > 179.0){
        xAngle = 179.0;
    }
	/* backflip takodje no */
    if(xAngle < 1.0){
        xAngle = 1.0;
    } 
                    
    /* prebacimo u radijane pa radimo sa sfernim */ 
    kx = cos(pi/180.0*yAngle)*sin(pi/180.0*xAngle);
    ky = cos(pi/180.0*xAngle);
    kz = sin(pi/180.0*yAngle)*sin(pi/180.0*xAngle);    
	
	glutPostRedisplay();
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
    	case 27:
        	exit(0);
        	break;
		case 'w':
		case 'W':
			keyStates['w'] = true;
			if(!moving){
				moving = true;
				glutTimerFunc(30, on_move, TIMER_ID_MOV);
			}

			break;
		
		default:
			break;
    }

	glutPostRedisplay();
}

static void on_keyboard_up(unsigned char key, int x, int y){
    switch (key) {
		case 'w':
		case 'W':
			keyStates['w'] = false;
			/* ovo se ne izvrsava kada bi trebalo */
			/* on_move se izvrsi jednom posle pustanja w */
			/* reseno u drawStudent funkciji */
			//limbAngle = 0;
			moving = false;
			break;
		
		default:
			break;
    }

	glutPostRedisplay();
}

static void on_reshape(int width, int height){
	window_width = width;
	window_height = height;

	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		80,									// ugao
		window_width/(float)window_height,	// aspect ratio
		0.01,								// near clipping plane distance
		50.0								// far clipping plane distance
	);
}

static void on_display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPosX, camPosY, camPosZ,				// eye xyz
			  camPosX + kx, camPosY + ky, camPosZ + kz,	// ref point xyz
			  camUpX, camUpY, camUpZ);					// up vector

	drawAxes();
	drawWalls();
	drawStudent();
	drawCrosshair();

	/* ovo radi kako treba ovde, neka ga */
	glutWarpPointer(window_width/2, window_height/2);
	glutPostRedisplay();
	glutSwapBuffers();
}

void drawAxes(){
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(7, 0, 0);

		glColor3f(0, 1, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 7, 0);

		glColor3f(0, 0, 1);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 7);
	glEnd();
};

void drawWalls(){

	glColor3f(0.2, 0.2, 0.2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-3, 0, 2);
		glVertex3f(-3, 0, -2);
		glVertex3f(-3, 2, -2);
		glVertex3f(-3, 2, 2);
	glEnd();
	
	glColor3f(0.3, 0.3, 0.3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-3, 0, -2);
		glVertex3f(2, 0, -2);
		glVertex3f(2, 2, -2);
		glVertex3f(-3, 2, -2);
	glEnd();

	glColor3f(0.4, 0.4, 0.4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(2, 0, -2);
		glVertex3f(2, 2, -2);
		glVertex3f(2, 2, 2);
		glVertex3f(2, 0, 2);
	glEnd();

	glColor3f(0.1, 0.1, 0.1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-3, 0, 2);
		glVertex3f(0.75, 0, 2);
		glVertex3f(0.75, 2, 2);
		glVertex3f(-3, 2, 2);
	glEnd();

	glColor3f(0.5, 0.5, 0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(1.5, 0, 2);
		glVertex3f(2, 0, 2);
		glVertex3f(2, 2, 2);
		glVertex3f(1.5, 2, 2);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(0.75, 1.5, 2);
		glVertex3f(1.5, 1.5, 2);
		glVertex3f(1.5, 2, 2);
		glVertex3f(0.75, 2, 2);
	glEnd();
	/* pod */
	glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
            glVertex3f(5, 0, -5);
            glVertex3f(-5, 0, -5);
            glVertex3f(-5, 0, 5);
            glVertex3f(5, 0, 5);
    glEnd();
	
}

void drawCrosshair(){
	/* sacuvamo matrice, promenimo na paralelnu proj, crtamo crosshair pa vratimo na staro */
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, window_width, 0, window_height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(0, 1, 0);
	glRasterPos2i(window_width/2 - 8, window_height/2 - 8);
	glBitmap(12, 12, 0, 0, 0, 0, crosshair);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawStudent(){
	/* kad pustimo w udovi se nekad ne vrate na pocetnu poziciju */
	/* ovo to resava */
	if(!keyStates['w']){
		limbAngle = 0;
	}
	glPushMatrix();
		glTranslatef(camPosX, 0, camPosZ);
		glRotatef(-yAngle, 0, 1, 0);
		/* desna ruka */
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef(-0.1, 0.95, 0.1);
			glRotatef(-limbAngle, 0, 0, 1);
			glTranslatef(0, -0.2, 0);
			glScalef(0.05, 0.4, 0.05);
			glutSolidCube(1);
		glPopMatrix();
		/* leva ruka */
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.3);
			glTranslatef(-0.1, 0.95, -0.1);
			glRotatef(limbAngle, 0, 0, 1);
			glTranslatef(0, -0.2, 0);
			glScalef(0.05, 0.4, 0.05);
			glutSolidCube(1);
		glPopMatrix();
		/* desna noga */
		glPushMatrix();
			glColor3f(0.2, 0.2, 0.2);
			glTranslatef(-0.1, 0.5, 0.037);
			glRotatef(limbAngle, 0, 0, 1);
			glTranslatef(0, -0.25, 0);
			glScalef(0.1, 0.5, 0.069);
			glutSolidCube(1);
		glPopMatrix();
		/* leva noga */
		glPushMatrix();
			glColor3f(0.2, 0.2, 0.2);
			glTranslatef(-0.1, 0.5, -0.037);
			glRotatef(-limbAngle, 0, 0, 1);
			glTranslatef(0, -0.25, 0);
			glScalef(0.1, 0.5, 0.069);
			glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();	
			glColor3f(0.6, 0.6, 0.6);
			glTranslatef(-0.1, 0.75, 0);
			glScalef(0.1, 0.5, 0.15);
			glutSolidCube(1);
		glPopMatrix();
	glPopMatrix();
}