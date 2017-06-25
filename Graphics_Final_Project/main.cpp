//
//  main.cpp
//  Homework_buzz
//
//  Created by GaryNoh on 2017. 6. 17..
//  Copyright © 2017년 Macbook. All rights reserved.
//

#include <opengl/gl.h>
#include <glut/glut.h>
#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <deque>
#include <algorithm>
#include <vector>

using namespace std;


//변수 그룹1
float GL_PI = 3.14/180.0;

GLfloat xRotated, yRotated, zRotated;
GLdouble radius=1;

//변수 그룹2
float phi, theta;
float phir, phir20, thetar;
float PI = atan(1.0)*4;
float c		 = PI/180.0;
float x, y, z;

//변수 그룹3
int rot = 0;
int rot_arm = 0;
int rot_toggle = 1;
int rot_arm_toggle = 1;

int q = 0;

//캐릭터 위치 변수
float pos_x = 0, pos_y =0, pos_z = 9;

//시점 변수 (초기에는 y,z 시점이 3 만큼 이동)
float view_x = 0, view_y =10, view_z = 10;

double radian = 3.14/180;
bool token = false;

int inc_rotate = 0;
int delay = 80;

//장애물 x, y
bool isNewEnemey = true;
int enemyType;


struct ENEMY{
    int type;
    float x;
    float y;
    float z;
    float r,g,b;
};

struct BULLET{
    float origin_x;
    float x, y, z;
};

//enemy 의 목록을 가지고 있는 큐
vector<struct ENEMY> enemyList;
//vector<struct ENEMY> enemyList;
vector<struct BULLET> bulletList;

long long int playTime;



void createNewEnemy(){
    struct ENEMY newEnemy;
    
    
    //x, y, z 설정
    srand((unsigned int)time(NULL));
    //x 값 음수, 양수
    if((rand()%2) == 0){
        srand((unsigned int)time(NULL));
        newEnemy.x = -((int) rand()%10);
    }
    else{
        newEnemy.x = (int) rand()%10;
    }
    
    newEnemy.y = 0;
    newEnemy.z = -50;
    srand((unsigned int)time(NULL));
    newEnemy.type = rand()%5;
    
    newEnemy.r = (rand()%256)/255.0;
    newEnemy.g = (rand()%256)/255.0;
    newEnemy.b = (rand()%256)/255.0;
    //vector 에 추가
    enemyList.push_back(newEnemy);
    
    printf("----------------- new enemy : %.1f, %.1f, %.1f\n",newEnemy.x, newEnemy.y, newEnemy.z);
}

void draw(int i){
    glPushMatrix();
    //z 값이 10 넘은 enemy 는 제거
    glTranslatef(enemyList[i].x, 0, enemyList[i].z);
    
    glColor3f(enemyList[i].r, enemyList[i].g, enemyList[i].b);
    printf("enemy %d appear in %.1f, %.1f, %.1f\n", enemyList[i].type, enemyList[i].x, enemyList[i].y, enemyList[i].z);
    enemyList[i].z+=0.5;
    
    switch (enemyList[i].type) {
        case 0:
            glutSolidSphere(1, 20, 20);
            //glutWireSphere(1, 4, 4);
            break;
        case 1:
            glutSolidOctahedron();
            //glutWireOctahedron();
            break;
        case 2:
            glutSolidTeapot(1);
            //glutWireTeapot(1);
            break;
        case 3:
            glutSolidCube(1);
            //glutWireCube(1);
            break;
        default:
            break;
    }
    //drawBox(10, GL_QUADS);
    glPopMatrix();
}



//x 와 z 값만 바뀐다
void drawEnemy(){
    
    //enemyList 에 있는 enemy들을 그려준다
    int numOfEnemy = (int)enemyList.size();
    printf("num of enemy : %d\n", numOfEnemy);
    printf("----------dosomething-----------\n");
    
    for (int i = 0; i < numOfEnemy; i++) {
        draw(i);
    }
    
    if(enemyList.size() > 0 && enemyList[0].z > 10) {
        enemyList.erase(enemyList.begin(), enemyList.begin()+1);
    }
    printf("---------end dosomething--------\n");
    
}


/*
 미사일 생성
 */
void createBullet(int x_orientation, float origin_x){
    //새 bullet 생성
    struct BULLET newBullet;
    switch (x_orientation) {
        case 0:
            newBullet.x = -2;
            break;
        case 1:
            newBullet.x = 2;
            break;
    }
    newBullet.y = 0;
    newBullet.z = 9;
    newBullet.origin_x = origin_x;
    bulletList.push_back(newBullet);
    
}

/*
 미사일 발사
 */
void drawShooting(){
    
    int numOfBullets = (int)bulletList.size();
    for(int i = 0 ; i < numOfBullets; i++){
        glPushMatrix();
        glColor3f((rand()%256)/255.0, (rand()%256)/255.0, (rand()%256)/255.0);
        glTranslatef(bulletList[i].origin_x + bulletList[i].x, bulletList[i].y, (bulletList[i].z -= 0.5) );
        glutWireSphere(0.1, 10, 10);
        printf("bullet pos : %.1f %.1f %.1f\n", bulletList[i].origin_x + bulletList[i].x, bulletList[i].y, bulletList[i].z);
        glPopMatrix();
    }
    if(bulletList.size() > 0 && bulletList[0].z < -50) {
        bulletList.erase(bulletList.begin(), bulletList.begin()+1);
    }
}

/*
 배경 : 우주 그라데이션
 */
void drawBackground(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glBegin(GL_QUADS);
    //red color
    glColor3f(0.0,0.0,0.0);
    glVertex2f(-1.0, 1.0);
    glVertex2f(-1.0,-1.0);
    //blue color
    glColor3f(33.0/255,0,32.0/255);
    glVertex2f(1.0,-1.0);
    glVertex2f(1.0, 1.0);
    glEnd();
}


/*
 타이머 콜백 함수
 */
void timer(int t){
    //obs_z+=0.1;
    //createNewEnemy();
    glutPostRedisplay();
    glutTimerFunc(delay, timer, t);
    
    if(++playTime > 60){
        playTime = 0;
        printf("난이도 상승\n");
        if(delay > 10) delay-=10;
        else delay = 5;
    }
    if(playTime == 30){
        createNewEnemy();
    }
}


/*
 초기화 함수
 */
void myinit()
{
    
    glLoadIdentity(); // 단위행렬
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    //glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 200.0);
    glEnable ( GL_DEPTH_TEST );
    
}

/*
 반원그리는 함수
 */
void drawHalfSphere(){
    for (phi=-80; phi<=0; phi+=10.0){
        phir	 = c*phi;
        phir20 = c*(phi+20);
        glBegin(GL_QUAD_STRIP);
        for (theta=-180.0; theta<=180.0; theta+=10.0){
            thetar = c*theta;
            x=sin(thetar)*cos(phir);
            y=cos(thetar)*cos(phir);
            z=sin(phir);
            glVertex3f(x,y,z);
            x=sin(thetar)*cos(phir20);
            y=cos(thetar)*cos(phir20);
            z=sin(phir20);
            glVertex3f(x,y,z);
        }
        glEnd();
    }
}

/*
 빠르게 원통그리는 함수
 */
void drawQuickCylinder(float top, float base, float height){
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, top, base, height ,32,32);
}

/*
 원통그리는 함수
 */
void drawCylinder(){
    printf("%f %f %f\n",x,y,z);
    glPushMatrix();
    glColor3f(1, 0, 0);
    glBegin(GL_QUAD_STRIP);
    float phi90 = c*phi;
    float phi9020 = c*(phi90+20);
    for (phi=10; phi<=10; phi+=10.0)
    {
        phi90 = c*phi;
        phi9020 = c*(phi90+30);
        for(theta= -180.0; theta<=180; theta+=10.0){
            thetar = c*theta;
            x=sin(thetar)*cos(phi90);
            y=cos(thetar)*cos(phi90);
            z=sin(phi90);
            glVertex3f(x,y,z);
            x=sin(thetar)*cos(phi9020);
            y=cos(thetar)*cos(phi90);
            z=sin(phi9020);
            glVertex3f(x,y,z);
            if(theta >= 0)
                glColor3f(0, 1, 0.6);
        }
    }
    glEnd();
    glPopMatrix();
}

void checkShot(){
    int margin = 2;
    for(int i = 0; i < (int)enemyList.size(); i++){
        for(int j = 0; j < (int)bulletList.size(); j++){
            printf("checking shot\n");
            if(abs(bulletList[j].origin_x + bulletList[j].x - enemyList[i].x) < margin &&
               abs(bulletList[j].z - enemyList[i].z) < margin){
                printf("************************************shot!*******************************\n");
                //장애물제거
                enemyList.erase(enemyList.begin() + i);
                //총알 제거
                bulletList.erase(bulletList.begin() + j);
                //break loops
                i = 10000;
                break;
            }
        }
    }
}

void checkDie(){
    if(enemyList.size() > 0){
        int margin = 2;
        printf("buzz pos : %.1f, %.1f, %.1f\n",pos_x, pos_y, pos_z);
        if(abs(enemyList[0].x - pos_x) < margin && abs(enemyList[0].z - pos_z) < margin){
            printf("^^^^^^^^^^^^^^^^^^^^DIE^^^^^^^^^^^^^^^^^^^^\n");
            exit(1);
        }
    }
    
}

/*
 화면에 그리는 함수
 */
void onDisplay(){
    
    //중요!
    //    물체를 그려서 사용자가 지정한 특정 좌표에 그리기 위해서는 위치변환을 통해 좌표를 지정해야 한다. 하지만 처음 물체를 그릴 때에는 상관없지만, 두
    //    째 물체를 그릴 때에는 그 전의 변경된 행렬이 남아 있기 때문에 단위행렬로 초기화를 해주어야 한다.
    //    그러나 이 함수의 문제점은 이전의 상태를 기억하지 못함. 그냥 날려버림. 때문에 상태를 넣었다 뺄 수 있는 행렬스택 사용. glPushMatrix,
    //    glPopMatrix
    glLoadIdentity(); // load an identity matrix
    glClear(GL_COLOR_BUFFER_BIT);
    //배경
    drawBackground();
    
    //projection
    glMatrixMode(GL_PROJECTION);
    glFrustum(-2.0, 2.0, -2.0, 2.0, 2.0, 200.0);
    //glRotatef(90, 1, 0, 0);
    
    //시점 설정
    gluLookAt(view_x, view_y, view_z, 0, 0, 0, 0, 1, 0);
    
    //버즈
    glPushMatrix();
    //버즈 위치 조정
    glTranslatef(pos_x, pos_y, pos_z);
    //초록색 헬맷 그리기
    glColor3f(0, 1, 0);
    drawHalfSphere();
    
    //헬멧 속 살색 머리 그리기
    glPushMatrix();
    glColor3f(245/255.0, 211/255.0, 200/255.0);
    glScalef(0.6, 0.6, 0.6); //헬멧의 60%
    drawHalfSphere();
    glPopMatrix();
    
    //회색 팔1 그리기
    //glColor3f(212/255.0, 214/255.0, 213/255.0);
    //팔1
    glPushMatrix(); //팔1 따로 저장
    //x축으로 1만큼 이동해서 그리기
    glTranslated(1, 0, 0);
    glRotatef(90, 0, 1, 0);
    //drawShooting();
    drawQuickCylinder(0.15, 0.15, 2.0);
    glPopMatrix();
    
    glColor3f(212/255.0, 100/255.0, 100/255.0);
    //회색 팔2 그리기
    glPushMatrix();
    glTranslated(-1, 0, 0);
    glRotatef(-90, 0, 1, 0);
    drawQuickCylinder(0.15, 0.15, 2.0);
    glPopMatrix();
    
    //회색 몸통 그리기
    glPushMatrix();
    glColor3f(223/255.0, 226/255.0, 232/255.0);
    drawQuickCylinder(1.0, 1.0, 1.5);
    glPopMatrix();
    
    //다리1 그리기
    glPushMatrix();
    glColor3f(212/255.0, 214/255.0, 213/255.0);
    //뒤집기
    glRotatef(rot, 1, 0, 0);
    //다리1 위치로 이동
    glTranslated(-0.3, 0, 1.5);
    drawQuickCylinder(0.2, 0.2, 1.7);
    glPopMatrix();
    
    //다리2 그리기
    glPushMatrix();
    //뒤집기
    glRotatef(-rot, 1, 0, 0);
    //다리2 위치로 이동
    glTranslated(0.3, 0, 1.5);
    drawQuickCylinder(0.2, 0.2, 1.7);
    glPopMatrix();
    glPopMatrix();
    
    //멀리서 생성되는 장애물
    drawEnemy();
    drawShooting();
    
    //enemy 와 bullet 이 부딪혔는지 확인
    checkShot();
    checkDie();
    
    //drawShooting();
    
    //중심큐브
    glColor3f(0.5, 0.5, 0.5);
    glutWireCube(1);
    
    
    
    glutSwapBuffers();
}

/*
 키보드가 눌리면 불리는 콜백
 */
int angle = 1;
void OnKeyboardPressed(unsigned char key, int x, int y){
    switch (key) {
        case 'a':
            printf("key left\n");
            pos_x-=0.5;
            break;
        case 'd':
            printf("key right\n");
            pos_x+=0.5;
            break;
        case 'z':
            view_x = 5*cos(2*3.14);
            view_z = 10+(5*sin(2*3.14));
            break;
        case 'v':
            view_x = -  5*cos(2*3.14);
            view_z = 10+(5*sin(2*3.14));
            break;
        case 'Z':
            view_x = 0;
            view_z = 10;
            break;
        case 'k':
            //left shooting
            printf("shoot\n");
            createBullet(0, pos_x);
            break;
        case 'l':
            //right shooting
            printf("shooting\n");
            createBullet(1, pos_x);
            break;
        default:
            printf("etc...\n");
            break;
    }
    printf("%.1f %.1f %.1f\n",pos_x, pos_y, pos_z);
}

/*
 윈도우 크기 조정했을 때 불리는 콜백 함수
 @param : width 와 height 는 꼭 필요한 파라미터임
 */
void OnResize(int width, int height){
    glutReshapeWindow(1000, 700);
}


int main(int argc, char **argv) {
    //glut library 초기화
    glutInit(&argc, argv);
    //bit mask 초기화 : double buffer window, RGBA mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    //윈도우 사이즈 초기화(초깃값 300, 300)
    glutInitWindowSize(1000, 700);
    //윈도우 시작위치 초기화 (처음에는 -1, -1)
    glutInitWindowPosition(0, 0);
    //윈도우 생성
    glutCreateWindow("Toy Story");
    
    glutReshapeFunc(OnResize);
    
    //display 콜백 함수 등록
    glutDisplayFunc(onDisplay);
    //키보드 함수 등록
    glutKeyboardFunc(OnKeyboardPressed);
    //딜레이와 함께 timer 콜백 함수 등록
    glutTimerFunc(delay, timer, 0);
    //초기화
    //glRotatef(90, 1, 0, 0);
    myinit();
    //화면 전체를 작게함
    
    glutMainLoop();
    return 0;
}
