#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>
#include "matfsim.h"

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("MATF Simulator");

	glutWarpPointer(window_width/2, window_height/2);
	glutDisplayFunc(on_display);
	glutKeyboardFunc(on_keyboard);
	glutReshapeFunc(on_reshape);
	glutPassiveMotionFunc(on_mouse_motion);

	glClearColor(0.6, 0.6, 0.6, 0);
	glEnable(GL_DEPTH_TEST);

	glLineWidth(3);

	glutMainLoop();
	
	return 0;
}

static void on_mouse_motion(int x, int y){
	
	/* menjamo uglove u zavisnosti od pozicije kursora */
	x -= window_width/2;
	y -= window_height/2;
	xAngle += y/mouseSens;
	yAngle += x/mouseSens;
	
	if(yAngle > 360.0/mouseSens){
        yAngle -= 360.0/mouseSens;
    }
    if(yAngle < -360.0/mouseSens){
        yAngle += 360.0/mouseSens;
    }
    /* necemo frontflip da radimo */
	if(xAngle > 179.0/mouseSens){
        xAngle = 179.0/mouseSens;
    }
	/* backflip takodje no */
    if(xAngle < 1.0/mouseSens){
        xAngle = 1.0/mouseSens;
    } 
                    
    /* prebacimo u radijane pa radimo sa sfernim */ 
    kx = cos(pi/180.0*yAngle*mouseSens)*sin(pi/180.0*xAngle*mouseSens);
    ky = cos(pi/180.0*xAngle*mouseSens);
    kz = sin(pi/180.0*yAngle*mouseSens)*sin(pi/180.0*xAngle*mouseSens);    
	
	glutPostRedisplay();
}

static void on_keyboard(unsigned char key, int x, int y){
    switch (key) {
    	case 27:
        	exit(0);
        	break;
		case 'w':
		case 'W':
			/* necemo da nam brzina kretanja zavisi od y ose*/
			xzlen = sqrt(kz*kz+kx*kx);
			camPosX += kx*(1/xzlen)/moveSens;
			camPosZ += kz*(1/xzlen)/moveSens;
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
		60,									// ugao
		window_width/(float)window_height,	// aspect ratiom
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

	/* koordinatne ose */
	drawAxes();

	/* neki patos nesto */
	glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
            glVertex3f(5, 0, -5);
            glVertex3f(-5, 0, -5);
            glVertex3f(-5, 0, 5);
            glVertex3f(5, 0, 5);
    glEnd();

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