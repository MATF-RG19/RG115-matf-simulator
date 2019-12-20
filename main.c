#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>
#include "matfsim.h"
#include "light.h"

GLuint listaStolica;

static void initStolice(){
	listaStolica = glGenLists(1);

	glNewList(listaStolica, GL_COMPILE);
		glColor3f(0, 0, 0);
		glPushMatrix();
			/* 4 noge */
			glPushMatrix();
				glTranslatef(-0.074, 0.25, -0.074);
				glScalef(0.02, 0.5, 0.02);
				glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.074, 0.125, -0.074);
				glScalef(0.02, 0.25, 0.02);
				glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.074, 0.125, 0.074);
				glScalef(0.02, 0.25, 0.02);
				glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.074, 0.25, 0.074);
				glScalef(0.02, 0.5, 0.02);
				glutSolidCube(1);
			glPopMatrix();
			/* whtevs */
			glPushMatrix();
				glColor3f(0.4, 0.15, 0.1);
				glTranslatef(0, 0.251, 0);
				glScalef(0.17, 0.01, 0.17);
				glutSolidCube(1);
			glPopMatrix();
			/* naslon */
			glPushMatrix();
				glColor3f(0.4, 0.15, 0.1);
				glTranslatef(-0.065, 0.4125, 0);
				glScalef(0.01, 0.125, 0.17);
				glutSolidCube(1);
			glPopMatrix();
		glPopMatrix();
		
	glEndList();
}

int main(int argc, char **argv){
	setbuf(stdout, NULL);
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
	glutMouseFunc(on_mouse);

	initStolice();

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
		//printf("on move w\n");
		/* necemo da nam brzina kretanja zavisi od y ose*/
		xzlen = sqrt(kz*kz+kx*kx);
		camPosX += kx/xzlen*moveSens;
		camPosZ += kz/xzlen*moveSens;
		camPosY += limbAngle*bobSens;
	}

	if(keyStates['s']){
		//printf("on move s\n");
		xzlen = sqrt(kz*kz+kx*kx);
		camPosX -= kx/xzlen*moveSens;
		camPosZ -= kz/xzlen*moveSens;
		camPosY += limbAngle*bobSens;
	}

	glutPostRedisplay();

	if(moving)
		glutTimerFunc(30, on_move, value);
}

/* TODO */
/* ne moze da se pomera kursor dok je pritisnuto dugme */
static void on_mouse(int button, int state, int x, int y){
	GLuint i;
	switch (button)	{
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN){
			/* kopirano iz keyboard down case p */
			for(i = 0; i < MAX_STOLICA; i++){
				sx = stolice[i].xPos;
				sz = stolice[i].zPos;
				rastojanje = sqrt((sx-camPosX)*(sx-camPosX)+(sz-camPosZ)*(sz-camPosZ));
				//printf("%d. %f\n", i, rastojanje);
				
				/* ako jeste namesti studenta da nosi ako ne nista */
				if(rastojanje < dovoljnaBlizina){
					/* plus provera da li je stolica ispred studenta
					ugao izmedju kx+kz vektora kamere i pos stolice minus pos studenta   */
					smsx = sx-camPosX;
					smsz = sz-camPosZ;
					ugao = acos((smsx*kx+smsz*kz)/(sqrt(kz*kz+kx*kx)*sqrt(smsx*smsx+smsz*smsz)));
					//printf("%f %f\n", ugao, dovoljniUgao);
					if(ugao < dovoljniUgao){
						/* ako stolica nije moved onda brMoved++ i stolica.moved = true */
						if(!stolice[i].moved){
							brMoved++;
							stolice[i].moved = true;
						}
						stolice[i].isCarried = true;
						carried = i;
						carrying = true;
						break;
					}
				}
			}
			glutPostRedisplay();
		}else if(state == GLUT_UP){
			if(carrying){
				/* kopirano iz keyboard up case p */
				xzlen = sqrt(kz*kz+kx*kx);
				stolice[carried].xPos = camPosX + kx/xzlen*kolikoIspredDaSpusti;
				stolice[carried].zPos = camPosZ + kz/xzlen*kolikoIspredDaSpusti;
				stolice[carried].yAngle = -yAngle;
				stolice[carried].isCarried = false;
				//printf("%f, %f, %f \n", stolice[carried].xPos, stolice[carried].zPos, stolice[carried].yAngle);
				//printf("%f %f %f\n", camPosX, camPosZ, yAngle);
				carrying = false;
				carried = -1;
				glutPostRedisplay();
			}
		}
		break;

	default:
		break;
	}
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
	GLuint i;
    switch (key) {
    	case 27:
        	exit(0);
        	break;
		case 'w':
		case 'W':
			//printf("on keyb w\n");
			keyStates['w'] = true;
			if(!moving){
				moving = true;
				glutTimerFunc(30, on_move, TIMER_ID_MOV);
			}
			break;
		
		case 's':
		case 'S':
			//printf("on keyboard s\n");
			keyStates['s'] = true;
			if(!moving){
				moving = true;
				glutTimerFunc(30, on_move, TIMER_ID_MOV);
			}
			break;
		case 'p':
		case 'P':
			for(i = 0; i < MAX_STOLICA; i++){
				sx = stolice[i].xPos;
				sz = stolice[i].zPos;
				rastojanje = sqrt((sx-camPosX)*(sx-camPosX)+(sz-camPosZ)*(sz-camPosZ));
				//printf("%d. %f\n", i, rastojanje);
				
				/* ako jeste namesti studenta da nosi ako ne nista */
				if(rastojanje < dovoljnaBlizina){
					/* plus provera da li je stolica ispred studenta
					ugao izmedju kx+kz vektora kamere i pos stolice minus pos studenta   */
					smsx = sx-camPosX;
					smsz = sz-camPosZ;
					ugao = acos((smsx*kx+smsz*kz)/(sqrt(kz*kz+kx*kx)*sqrt(smsx*smsx+smsz*smsz)));
					//printf("%f %f\n", ugao, dovoljniUgao);
					if(ugao < dovoljniUgao){
						/* ako stolica nije moved onda brMoved++ i stolica.moved = true */
						if(!stolice[i].moved){
							brMoved++;
							stolice[i].moved = true;
						}
						stolice[i].isCarried = true;
						carried = i;
						carrying = true;
						break;
					}
				}
			}
			glutPostRedisplay();
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
			//printf("key up w\n");
			keyStates['w'] = false;
			/* ovo se ne izvrsava kada bi trebalo */
			/* on_move se izvrsi jednom posle pustanja w */
			/* reseno u drawStudent funkciji */
			//limbAngle = 0;
			moving = false;
			break;
		case 's':
		case 'S':
			//printf("key up s\n");
			keyStates['s'] = false;
			moving = false;
			break;
		/* TODO sedi */
		case 'e':
		case 'E':
			break;
		case 'p':
		case 'P':
			/* spusti stolicu */
			//printf("%d\n", carried);
			/* update xpos zpos yangle stolice u odnosu na studenta */	
			/* dodaj stolicu u movedChairs niz */
			if(carrying){
				xzlen = sqrt(kz*kz+kx*kx);
				stolice[carried].xPos = camPosX + kx/xzlen*kolikoIspredDaSpusti;
				stolice[carried].zPos = camPosZ + kz/xzlen*kolikoIspredDaSpusti;
				stolice[carried].yAngle = -yAngle;
				stolice[carried].isCarried = false;
				//movedChairs[brMoved-1] = &stolice[carried];
				//printf("%f, %f, %f \n", stolice[carried].xPos, stolice[carried].zPos, stolice[carried].yAngle);
				//printf("%f %f %f\n", camPosX, camPosZ, yAngle);
				carrying = false;
				carried = -1;
				glutPostRedisplay();
			}
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

	initLights();
	setMaterial();

	glEnable(GL_COLOR_MATERIAL);

	drawAxes();
	drawWalls();
	drawStudent();
	drawChairs();
	drawMovedChairs();

	glDisable(GL_LIGHTING);
	drawCrosshair();
	glEnable(GL_LIGHTING);
	

	/* ovo radi kako treba ovde, neka ga */
	glutWarpPointer(window_width/2, window_height/2);
	glutPostRedisplay();
	glutSwapBuffers();
}

void drawMovedChairs(){
	GLuint i;
	for(i = 0; i < MAX_STOLICA; i++){
		//printf("%f, %f, %f \n", movedChairs[i].xPos, movedChairs[i].zPos, movedChairs[i].yAngle);
		//printf("%f %f %f\n", camPosX, camPosZ, yAngle);
		if(stolice[i].moved){
			if(!stolice[i].isCarried){
				glPushMatrix();
					glTranslatef(stolice[i].xPos, 0, stolice[i].zPos);
					glRotatef(stolice[i].yAngle, 0, 1, 0);
					//glDisable(GL_LIGHTING);
					glCallList(listaStolica);
					//glEnable(GL_LIGHTING);
				glPopMatrix();
			}
		}
	}
}

void drawChairs(){
	/* inicijalizujemo structs dok crtamo */
	GLuint i, j;
	glPushMatrix();
		glTranslatef(1.5, 0, -1.5);
		xS = 1.5;
		zS = -1.5;
		for(j = 0; j < 3; j++){
			glTranslatef(-1, 0, 0);
			xS -= 1;
			//glDisable(GL_LIGHTING);
			for(i = 0; i < 6; i++){
				idStolice = i + j * 6;
				
				if(!stolice[idStolice].moved){
					glCallList(listaStolica);
					/* ovo se posle prvog puta izvrsava bez potrebe ali ok */
					stolice[idStolice].xPos = xS;
					stolice[idStolice].zPos = zS;
					//printf("%d %f %f\n", idStolice, xS, zS);
				}
				glTranslatef(0, 0, 0.4);
				zS += 0.4;
			}
			glTranslatef(0, 0, -2.4);
			zS -= 2.4;
			//glEnable(GL_LIGHTING);
		}
	glPopMatrix();
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
	/* zadnji zid */
	glColor3f(0.2, 1, 0.2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-3, 0, 2);
		glVertex3f(-3, 0, -2);
		glVertex3f(-3, 2, -2);
		glVertex3f(-3, 2, 2);
	glEnd();
	/* levi zid */
	glColor3f(0.3, 0.3, 0.3);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-3, 0, -2);
		glVertex3f(2, 0, -2);
		glVertex3f(2, 2, -2);
		glVertex3f(-3, 2, -2);
	glEnd();
	/* prednji zid */
	glColor3f(0.4, 0.4, 0.4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(2, 0, -2);
		glVertex3f(2, 2, -2);
		glVertex3f(2, 2, 2);
		glVertex3f(2, 0, 2);
	glEnd();
	/* desni zid */
	glColor3f(0.1, 0.1, 0.1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(-3, 0, 2);
		glVertex3f(0.75, 0, 2);
		glVertex3f(0.75, 2, 2);
		glVertex3f(-3, 2, 2);
	glEnd();
	/* mali zid do vrata */
	glColor3f(0.5, 0.5, 0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glVertex3f(1.5, 0, 2);
		glVertex3f(2, 0, 2);
		glVertex3f(2, 2, 2);
		glVertex3f(1.5, 2, 2);
	glEnd();
	/* mali zid iznad vrata */
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
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
	if(!keyStates['w'] && !keyStates['s']){
		limbAngle = 0;
		camPosY = 1; 
	}

	/* kolizija */
	if(camPosX >= 1.9) camPosX -= 0.05;
	else if(camPosX <= -2.9) camPosX += 0.05;
	else if(camPosZ >= 1.9) camPosZ -=  0.05;
	else if(camPosZ <= -1.9) camPosZ += 0.05;

	glPushMatrix();
		glTranslatef(camPosX, 0, camPosZ);
		glRotatef(-yAngle, 0, 1, 0);
		if(carrying){
			/* desna ruka */
			glPushMatrix();
				glColor3f(0.3, 0.3, 0.3);
				glTranslatef(-0.1, 0.95, 0.1);
				glRotatef(60, 0, 0, 1);
				glTranslatef(0, -0.2, 0);
				glScalef(0.05, 0.4, 0.05);
				glutSolidCube(1);
			glPopMatrix();
			/* leva ruka */
			glPushMatrix();
				glColor3f(0.3, 0.3, 0.3);
				glTranslatef(-0.1, 0.95, -0.1);
				glRotatef(60, 0, 0, 1);
				glTranslatef(0, -0.2, 0);
				glScalef(0.05, 0.4, 0.05);
				glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.25, 0.33, 0);
				//glDisable(GL_LIGHTING);
				glCallList(listaStolica);
				//glEnable(GL_LIGHTING);
			glPopMatrix();
			}else{
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
		}
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
		/* trup */
		glPushMatrix();	
			glColor3f(0.6, 0.6, 0.6);
			glTranslatef(-0.1, 0.75, 0);
			glScalef(0.1, 0.5, 0.15);
			glutSolidCube(1);
		glPopMatrix();
	glPopMatrix();
}