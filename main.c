#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glut.h>
#include "matfsim.h"
#include "light.h"
#include "image.h"

GLuint listaStolica;

static void initStolice(){
	listaStolica = glGenLists(1);

	glNewList(listaStolica, GL_COMPILE);
		glColor3f(0, 0, 0);
		glPushMatrix();
			/* 4 noge */
			glPushMatrix();
				glTranslatef(-0.074, 0, -0.074);
				glScalef(0.02, 0.5, 0.02);
				glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.074, -0.125, -0.074);
				glScalef(0.02, 0.25, 0.02);
				glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.074, -0.125, 0.074);
				glScalef(0.02, 0.25, 0.02);
				glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.074, 0, 0.074);
				glScalef(0.02, 0.5, 0.02);
				glutSolidCube(1);
			glPopMatrix();
			/* whtevs */
			glPushMatrix();
				glColor3f(0.4, 0.15, 0.1);
				glTranslatef(0, 0.001, 0);
				glScalef(0.17, 0.01, 0.17);
				glutSolidCube(1);
			glPopMatrix();
			/* naslon */
			glPushMatrix();
				glColor3f(0.4, 0.15, 0.1);
				glTranslatef(-0.065, 0.1625, 0);
				glScalef(0.01, 0.125, 0.17);
				glutSolidCube(1);
			glPopMatrix();
		glPopMatrix();
		
	glEndList();
}

static void initTex(){
	/* Objekat koji predstavlja teskturu ucitanu iz fajla. */
    Image * image;

	/* Ukljucuju se teksture. */
    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

	 /* Inicijalizuje se objekat koji ce sadrzati teksture ucitane iz fajla. */
    image = image_init(0, 0);

	/* Generisu se identifikatori tekstura. */
	glGenTextures(3, names);
    /* Kreira se prva tekstura. */

    image_read(image, FILENAME0);

	glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, 
					GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, 
					GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	image_read(image, FILENAME1);

	glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, 
					GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, 
					GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	image_read(image, FILENAME2);

	glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, 
					GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, 
					GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	/* Iskljucujemo aktivnu teksturu */
    glBindTexture(GL_TEXTURE_2D, 0);

	/* Unistava se objekat za citanje tekstura iz fajla. */
    image_done(image);
}

int main(int argc, char **argv){
	setbuf(stdout, NULL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(150, 100);
	glutCreateWindow("MATF Simulator");
	glutFullScreen();

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glutDisplayFunc(on_display);
	glutKeyboardFunc(on_keyboard);
	glutKeyboardUpFunc(on_keyboard_up);
	glutReshapeFunc(on_reshape);
	glutPassiveMotionFunc(on_mouse_motion);
	glutMouseFunc(on_mouse);

	initTex();
	initStolice();

	glClearColor(0.6, 0.6, 0.6, 0);
	glEnable(GL_DEPTH_TEST);

	glLineWidth(3);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutMainLoop();
	
	return 0;
}

static void on_timer(int value){
	if(value == TIMER_ID_MOV){
	
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
			glutTimerFunc(30, on_timer, TIMER_ID_MOV);
	}

	if(value == TIMER_ID_THROW){
		throwAnim += throwAnimSpeed;
		glutPostRedisplay();
		if(throwAnim < throwArmAngleMax){
			glutTimerFunc(30, on_timer, TIMER_ID_THROW);
		}else{
			throwAnim = 0;
			throwing = false;
			carrying = false;
			st->isCarried = false;
			st->isInAir = true;
			/* kancer koji nije probom, mozda zatreba tho */
			//st->xPos = camPosX - 0.1 + cos(-yAngle)* 0.7232 + sin(-yAngle);
			//st->zPos = camPosZ - sin(-yAngle) * 0.7232 + cos(-yAngle);
			xzlen = sqrt(kz*kz+kx*kx);
			/* sve probom uradjeno jadno */
			st->xPos = camPosX + kx/xzlen*(kolikoIspredDaSpusti+0.37);
			st->zPos = camPosZ + kz/xzlen*(kolikoIspredDaSpusti+0.37);
			/* calculato */
			st->yPos = 0.5336;
			st->zAngle = 60;
			st->yAngle = -yAngle;
			throwKZ = kz;
			throwKX = kx;
			
			glutTimerFunc(30, on_timer, TIMER_ID_CHAIR);
		}
		glutPostRedisplay();
	}

	if(value == TIMER_ID_CHAIR){
		if(st->yPos > 0.1){
			throwAnim += chairAnimSpeed;
			xzlen = sqrt(throwKZ*throwKZ+throwKX*throwKX);
			/* bacanje po x i z */
			if(st->xPos < 1.8 && st->xPos > -2.8 && st->zPos < 1.8 && st->zPos > -1.8){
				st->xPos += (throwKX/xzlen)*throwDist/throwXAngle;
				st->zPos += (throwKZ/xzlen)*throwDist/throwXAngle;
				st->zAngle += 23*throwXAngle/100;
			}
			st->zAngle -= 5;
			
			/* bacanje po y */
			st->yPos = 0.5336 + sin(throwAnim)*(throwXAngle/100) - throwAnim/5;
			if(st->zAngle > 360) st->zAngle -= 360;
			//printf("%f\n", st->zAngle);
			glutPostRedisplay();
			glutTimerFunc(10, on_timer, TIMER_ID_CHAIR);
		}else{
			//printf("---------\n");
			/* kolizija stolica - zid */
			if(st->zAngle > 330 || (st->zAngle < 30 && st->zAngle > -30)){
				st->zAngle = 0;
				st->yPos = 0;
			}else if(st->zAngle > 30 && st->zAngle < 180){
				st->zAngle = 90;
				st->yPos = -0.165;
			}else if(st->zAngle > 180 && st->zAngle < 250){
				st->zAngle = 245;
				st->yPos = -0.17;
			}else{
				st->zAngle = 270;
				st->yPos = -0.165;
			}
			//printf(">>> %f\n", st->yPos);
			st->isInAir = false;
			throwAnim = 0;
			glutPostRedisplay();
		}
	}

	if(value == TIMER_ID_TURND){
		if(keyStates['s']){
			yAngle -= adSens;
		}else{
			yAngle += adSens;
		}

		if(yAngle > 360.0){
			yAngle -= 360.0;
		}
		if(yAngle < -360.0){
			yAngle += 360.0;
		}
		
		kx = cos(pi/180.0*yAngle)*sin(pi/180.0*xAngle);
		kz = sin(pi/180.0*yAngle)*sin(pi/180.0*xAngle);    
		
		glutPostRedisplay();
		if(turning)
		glutTimerFunc(30, on_timer, TIMER_ID_TURND);
	}
	
	if(value == TIMER_ID_TURNA){
		if(keyStates['s']){
			yAngle += adSens;
		}else{
			yAngle -= adSens;
		}

		if(yAngle < -360.0){
			yAngle += 360.0;
		}
		if(yAngle > 360.0){
			yAngle -= 360.0;
		}

		kx = cos(pi/180.0*yAngle)*sin(pi/180.0*xAngle);
		kz = sin(pi/180.0*yAngle)*sin(pi/180.0*xAngle);    
		
		glutPostRedisplay();
		if(turning)
			glutTimerFunc(30, on_timer, TIMER_ID_TURNA);
	}

	return;
}

/* ne moze da se pomera kursor dok je pritisnuto dugme problem */
/* ovo je isto kao btn e */
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
				glutTimerFunc(30, on_timer, TIMER_ID_MOV);
			}
			break;
		
		case 's':
		case 'S':
			//printf("on keyboard s\n");
			keyStates['s'] = true;
			if(!moving){
				moving = true;
				glutTimerFunc(30, on_timer, TIMER_ID_MOV);
			}
			break;
		case 'e':
		case 'E':
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
					}
				}
			}
			glutPostRedisplay();
			break;
		case 'Q':
		case 'q':
			if(carrying){
				if(!throwing){
					//carrying = false;
					throwing = true;
					throwXAngle = abs(xAngle - 180);
					if(throwXAngle < 30) throwXAngle = 30;
					if(throwXAngle > 175) throwXAngle = 175;
					throwKX = kx;
					throwKZ = kz;
					st = &stolice[carried];
					glutTimerFunc(30, on_timer, TIMER_ID_THROW);
				}
			}
			glutPostRedisplay();
			break;
		
		/* TODO sedi */
		case 'x':
		case 'X':
			break;

		case 'd':
		case 'D':
			keyStates['d'] = true;
			if(!turning){
				turning = true;
				glutTimerFunc(30, on_timer, TIMER_ID_TURND);
			}
			break;
		case 'a':
		case 'A':
			keyStates['a'] = true;
			if(!turning){
				turning = true;
				glutTimerFunc(30, on_timer, TIMER_ID_TURNA);
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
			//printf("key up w\n");
			keyStates['w'] = false;
			/* ovo se ne izvrsava kada bi trebalo */
			/* on_timer se izvrsi jednom posle pustanja w */
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
		case 'e':
		case 'E':
			/* spusti stolicu */
			//printf("%d\n", carried);
			/* update xpos zpos yangle stolice u odnosu na studenta */	
			/* dodaj stolicu u movedChairs niz */
			if(carrying){
				xzlen = sqrt(kz*kz+kx*kx);
				stolice[carried].xPos = camPosX + kx/xzlen*kolikoIspredDaSpusti;
				stolice[carried].zPos = camPosZ + kz/xzlen*kolikoIspredDaSpusti;
				stolice[carried].yPos = 0;
				stolice[carried].zAngle = 0;
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
		case 'd':
		case 'D':
			keyStates['d'] = false;
			turning = false;
			break;
		case 'a':
		case 'A':
			keyStates['a'] = false;
			turning = false;
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

	//drawAxes();
	drawWalls();
	glDisable(GL_LIGHTING);
	drawStudent();
	glEnable(GL_LIGHTING);
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
					glTranslatef(stolice[i].xPos, stolice[i].yPos, stolice[i].zPos);
					glPushMatrix();
						glRotatef(stolice[i].yAngle, 0, 1, 0);
						glTranslatef(0, 0.25, 0);
						glRotatef(stolice[i].zAngle, 0, 0, 1);
						glDisable(GL_LIGHTING);
						glCallList(listaStolica);
						glEnable(GL_LIGHTING);
					glPopMatrix();
				glPopMatrix();
			}
		}
	}
}

void drawChairs(){
	/* inicijalizujemo structs dok crtamo */
	GLuint i, j;
	glPushMatrix();
		glTranslatef(1.5, 0.25, -1.5);
		xS = 1.5;
		zS = -1.5;
		for(j = 0; j < 3; j++){
			glTranslatef(-1, 0, 0);
			xS -= 1;
			glDisable(GL_LIGHTING);
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
			glEnable(GL_LIGHTING);
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
	/* tabla */
	glBindTexture(GL_TEXTURE_2D, names[2]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(-1, 0, 0);

		glTexCoord2f(0, 0);
		glVertex3f(1.99, 0.6, -1.9);

		glTexCoord2f(1, 0);
		glVertex3f(1.99, 0.6, 1.9);

		glTexCoord2f(1, 1);
		glVertex3f(1.99, 1.8, 1.9);

		glTexCoord2f(0, 1);
		glVertex3f(1.99, 1.8, -1.9);
	glEnd();
	/* zadnji zid */
	glBindTexture(GL_TEXTURE_2D, names[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(1, 0, 0);

		glTexCoord2f(0, 0);
		glVertex3f(-3, 0, 2);
		
		glTexCoord2f(2, 0);
		glVertex3f(-3, 0, -2);
		
		glTexCoord2f(2, 3);
		glVertex3f(-3, 2, -2);
		
		glTexCoord2f(0, 3);
		glVertex3f(-3, 2, 2);
	glEnd();
	/* levi zid */
	glBindTexture(GL_TEXTURE_2D, names[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, 1);

		glTexCoord2f(0, 0);
		glVertex3f(-3, 0, -2);

		glTexCoord2f(2.5, 0);
		glVertex3f(2, 0, -2);

		glTexCoord2f(2.5, 3);
		glVertex3f(2, 2, -2);

		glTexCoord2f(0, 3);
		glVertex3f(-3, 2, -2);
	glEnd();
	/* prednji zid */
	glBindTexture(GL_TEXTURE_2D, names[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(-1, 0, 0);

		glTexCoord2f(0, 0);
		glVertex3f(2, 0, -2);

		glTexCoord2f(2, 0);
		glVertex3f(2, 0, 2);

		glTexCoord2f(2, 3);
		glVertex3f(2, 2, 2);

		glTexCoord2f(0, 3);
		glVertex3f(2, 2, -2);
	glEnd();
	/* desni zid */
	glBindTexture(GL_TEXTURE_2D, names[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);

		glTexCoord2f(0, 0);
		glVertex3f(-3, 0, 2);

		glTexCoord2f(1.875, 0);
		glVertex3f(0.75, 0, 2);

		glTexCoord2f(1.875, 3);
		glVertex3f(0.75, 2, 2);

		glTexCoord2f(0, 3);
		glVertex3f(-3, 2, 2);
	glEnd();
	/* mali zid do vrata */
	glBindTexture(GL_TEXTURE_2D, names[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);

		glTexCoord2f(0, 0);
		glVertex3f(1.5, 0, 2);
		
		glTexCoord2f(0.25, 0);
		glVertex3f(2, 0, 2);

		glTexCoord2f(0.25, 3);
		glVertex3f(2, 2, 2);

		glTexCoord2f(0, 3);
		glVertex3f(1.5, 2, 2);
	glEnd();
	/* mali zid iznad vrata */
	glBindTexture(GL_TEXTURE_2D, names[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(0, 0, -1);

		glTexCoord2f(0.375, 0);
		glVertex3f(1.5, 1.5, 2);

		glTexCoord2f(0, 0);
		glVertex3f(0.75, 1.5, 2);

		glTexCoord2f(0, 0.75);
		glVertex3f(0.75, 2, 2);

		glTexCoord2f(0.375, 0.75);
		glVertex3f(1.5, 2, 2);
	glEnd();
	/* pod */
	glBindTexture(GL_TEXTURE_2D, names[1]);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);

		glTexCoord2f(0, 0);
		glVertex3f(-3, 0, 2);

		glTexCoord2f(4, 0);
		glVertex3f(2, 0, 2);

		glTexCoord2f(4, 3);
		glVertex3f(2, 0, -2);

		glTexCoord2f(0, 3);
		glVertex3f(-3, 0, -2);
    glEnd();

	/* iskljucujemo aktivnu teksturu */
	glBindTexture(GL_TEXTURE_2D, 0);
	
	/* plafon */
	glColor3f(1, 1, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);

		glVertex3f(2, 2, 2);

		glVertex3f(2, 2, -2);

		glVertex3f(-3, 2, -2);

		glVertex3f(-3, 2, 2);
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
		glTranslatef(-0.1, 0, 0);
		if(throwing){
			glPushMatrix();

				/* desna ruka */
				glPushMatrix();
					glColor3f(0.3, 0.3, 0.3);
					glTranslatef(0, 0.95, 0.1);
					glRotatef(60 + throwAnim, 0, 0, 1);
					glTranslatef(0, -0.2, 0);
					glScalef(0.05, 0.4, 0.05);
					glutSolidCube(1);
				glPopMatrix();
				/* leva ruka */
				glPushMatrix();
					glColor3f(0.3, 0.3, 0.3);
					glTranslatef(0, 0.95, -0.1);
					glRotatef(60 + throwAnim, 0, 0, 1);
					glTranslatef(0, -0.2, 0);
					glScalef(0.05, 0.4, 0.05);
					glutSolidCube(1);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(0, 0.95, 0);
					glRotatef(60 + throwAnim, 0, 0, 1);
					glTranslatef(0, -0.35, 0);
					glRotatef(-60 + throwAnim, 0, 0, 1);
					glTranslatef(0.085, -0.23, 0);
					glCallList(listaStolica);
				glPopMatrix();
			glPopMatrix();
		}else if(carrying){
			/* desna ruka */
			glPushMatrix();
				glColor3f(0.3, 0.3, 0.3);
				glTranslatef(0, 0.95, 0.1);
				glRotatef(60, 0, 0, 1);
				glTranslatef(0, -0.2, 0);
				glScalef(0.05, 0.4, 0.05);
				glutSolidCube(1);
			glPopMatrix();
			/* leva ruka */
			glPushMatrix();
				glColor3f(0.3, 0.3, 0.3);
				glTranslatef(0, 0.95, -0.1);
				glRotatef(60, 0, 0, 1);
				glTranslatef(0, -0.2, 0);
				glScalef(0.05, 0.4, 0.05);
				glutSolidCube(1);
			glPopMatrix();
			glPushMatrix();
				//glTranslatef(0.25, 0.33, 0);
				glTranslatef(0, 1.2, 0);
				glRotatef(60, 0, 0, 1);
				glTranslatef(0, -0.35, 0);
				glPushMatrix();
					glRotatef(-60, 0, 0, 1);
					glTranslatef(0.085, -0.48, 0);
					//glDisable(GL_LIGHTING);
					glCallList(listaStolica);
					//glEnable(GL_LIGHTING);
				glPopMatrix();
			glPopMatrix();
		}else{
			/* desna ruka */
			glPushMatrix();
				glColor3f(0.3, 0.3, 0.3);
				glTranslatef(0, 0.95, 0.1);
				glRotatef(-limbAngle, 0, 0, 1);
				glTranslatef(0, -0.2, 0);
				glScalef(0.05, 0.4, 0.05);
				glutSolidCube(1);
			glPopMatrix();
			/* leva ruka */
			glPushMatrix();
				glColor3f(0.3, 0.3, 0.3);
				glTranslatef(0, 0.95, -0.1);
				glRotatef(limbAngle, 0, 0, 1);
				glTranslatef(0, -0.2, 0);
				glScalef(0.05, 0.4, 0.05);
				glutSolidCube(1);
			glPopMatrix();
		}
		/* desna noga */
		glPushMatrix();
			glColor3f(0.2, 0.2, 0.2);
			glTranslatef(0, 0.5, 0.037);
			glRotatef(limbAngle, 0, 0, 1);
			glTranslatef(0, -0.25, 0);
			glScalef(0.1, 0.5, 0.069);
			glutSolidCube(1);
		glPopMatrix();
		/* leva noga */
		glPushMatrix();
			glColor3f(0.2, 0.2, 0.2);
			glTranslatef(0, 0.5, -0.037);
			glRotatef(-limbAngle, 0, 0, 1);
			glTranslatef(0, -0.25, 0);
			glScalef(0.1, 0.5, 0.069);
			glutSolidCube(1);
		glPopMatrix();
		/* trup */
		glPushMatrix();	
			glColor3f(0.6, 0.6, 0.6);
			glTranslatef(0, 0.75, 0);
			glScalef(0.1, 0.5, 0.15);
			glutSolidCube(1);
		glPopMatrix();
	glPopMatrix();
}