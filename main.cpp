#include<stdio.h>
#include <cstdlib>
#include<time.h>
#include<string.h>
#include <string>
#define GL_SILENCE_DEPRECATION
#include<GLUT/glut.h>
#include<cmath>
using namespace std;
//int axis = 0;
//float theta[3] = { 0,0,0 };
bool start = false;
bool gameover = false;
bool jump = false;
unsigned int m_screenWidth = 750;
unsigned int m_screenHeight = 750;
int color_var = 0;
void* m_glutFont = GLUT_BITMAP_TIMES_ROMAN_24;
float player_y = 0.35;
float viewer[3] = { 0,0,2 };
float player[3] = { 0,player_y,0 };
float jumpsin = 0;
float enemypos[100];
float enemypos2[100];
float enemypos3[100];
float y_bounce=0;
float map_len = 400;
int immunity=0;
int lives=3;
GLfloat mat_cube[] = { 1.0f,1.0f,1.0f };
GLfloat mat_enemy[] = { 0.8f,0.1f,0.1f };
GLfloat mat_floor[] = { 0.6f,0.6f,0.6f };

GLfloat mat_amb[] = { 0.7f,0.7f,0.7f,1.0f };
GLfloat mat_diff[] = { 0.7f,0.7f,0.7f,1.0f };
GLfloat mat_spec[] = { 0.7f,0.7f,0.7f,1.0f };
GLfloat mat_shine[] = { 50.0f };


//GLfloat lightintensity[] = { 0.9f,0.9f,0.9f,1.0f };
GLfloat lightintensity[] = { 0.0f,0.0f,1.0f,1.0f };
GLfloat lightintensity1[] = { 0.0f,0.9f,0.9f,1.0f };
//GLfloat lightposition[] = { viewer[0],viewer[1],viewer[2],0.0f };
GLfloat lightposition[] = { 0,2,-5,1.0f };
GLfloat lightposition1[] = { viewer[0],viewer[1],viewer[2],0.0f };
GLfloat lightpositionarr[100][4];
//float v[8][3] = { {-1,-1,-1},{-1,1,-1},{1,1,-1},{1,-1,-1},{-1,-1,1},{-1,1,1},{1,1,1},{1,-1,1} };
void display();
void drawStair(float x, float y);
void drawSpectator(float x, float y, float z);

void printStairs()
{
    drawStair(6, 0);
    drawStair(-6, 0);
    drawStair(8, 0.5);
    drawStair(-8, 0.5);
    drawStair(10, 1);
    drawStair(-10, 1);
}
void printSpectators()
{
    for (int i = -3; i > -map_len / 2; i -= 2)
    {
        
        drawSpectator(4.5, 0.5+y_bounce, i);
        drawSpectator(-4.5, 0.5+y_bounce, i);
        drawSpectator(6, 1+y_bounce, i+1);
        drawSpectator(-6, 1+y_bounce, i+1);
        drawSpectator(7.5, 1.5+y_bounce, i);
        drawSpectator(-7.5, 1.5+y_bounce, i);
    }
}
void drawStair(float x, float y)
{
    glPushMatrix();
    glTranslated(x, y, 0);
    glScaled(6, 1, map_len);
    glutSolidCube(1.0);
    glPopMatrix();
}
void drawSpectator(float x, float y, float z)
{
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(0.3, 1, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(x, y+0.76, z+0.45);
    glRotatef(30, 1, 0, 0);
    glScaled(0.1, 0.3, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(-x, y+0.76, z-0.45);
    glRotatef(-30, 1, 0, 0);
    glScaled(0.1, 0.3, 0.1);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glPushMatrix();
    glTranslated(x, y+0.8, z);
    //glScaled(0.3, 1, 0.3);
    glutSolidSphere(0.25,16,16);
    glPopMatrix();
}
void drawText(const std::string& text, const unsigned int x, const unsigned int y)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_screenWidth, 0, m_screenHeight, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(x, y);
    for (const char c : text)
        glutBitmapCharacter(m_glutFont, (int)c);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
void printText(string s)
{
    //printf("Entering");
    glColor3f(1.0, 0.0, 1.0);
    glRasterPos2f(0.1f, 0.1f);
    int len = int(s.length());
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    }
}
void keyboard(unsigned char key, int x, int y)
{
    printf("key = %c", key);
    if (key == 's')
    {
        start = true;
        printf("Game Started");
    }
}
void keys(int key, int x, int y)
{
    if (start)
    {
        if (key == GLUT_KEY_LEFT) player[0] -= 1.4;
        if (key == GLUT_KEY_UP) if (!jump) jump = true;
        if (key == GLUT_KEY_RIGHT) player[0] += 1.4;
    }
}
void init()
{
    srand(time(0));
    for (int i = 0; i < 100; i++)
    {
        enemypos[i] = (rand() % 3 - 1) * 1.4;
        enemypos2[i] = (rand() % 3 - 1) * 1.4;
        enemypos3[i] = (rand() % 3 - 1) * 1.4;
    }
}

void move()
{
    if (start)
    {
        player[2] -= 0.1;
        immunity--;
        printf("%d\n",lives);
    }
    if(lives<1)
    {
        start=false;
        gameover = true;
    }
    display();
}

void changeMaterial(float rgb[3], float sh)
{
    for (int i = 0; i < 3; i++)
    {
        mat_amb[i] = rgb[i];
        mat_diff[i] = rgb[i];
        mat_spec[i] = rgb[i];
    }
    mat_shine[0] = sh;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);
}
void ground(double thickness)
{
    glPushMatrix();
    glTranslated(0, 0, 0);
    glScaled(5, thickness, map_len);
    glutSolidCube(1.0);
    glPopMatrix();
}
void drawPlayer(double size)
{
    glPushMatrix();
    glTranslated(player[0], player[1], player[2]);
    glScaled(1, 0.7, 1);
    glutSolidCube(1.0);
    glPopMatrix();
}
void drawEnemyCube(float pos_x, float pos_z)
{
    glPushMatrix();
    glTranslated(pos_x, player_y, pos_z);
    glScaled(1, 0.7, 1);
    glutSolidCube(1.0);
    glPopMatrix();
}
void jumpfun()
{
    if (jumpsin > 180)
    {
        jumpsin = 0;
        player[1] = player_y;
        jump = false;
    }
    player[1] = player_y + sin(jumpsin * 3.1415 / 180);
    //printf("%lf    %lf\n",sin(jumpsin),player[1]);
    jumpsin += 3;
}
void drawPointLight(GLfloat p[], GLfloat li[])
{
    //printf("%f %f %f\n", p[0], p[1], p[2]);
    glPushMatrix();
       //glTranslatef(p[0], p[1], p[2]);
        glLightfv(GL_LIGHT1, GL_POSITION, p);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, li);
        glLightfv(GL_LIGHT1, GL_AMBIENT, li);
        glLightfv(GL_LIGHT1, GL_SPECULAR, li);
        const GLfloat c[] = { 100 };
        glLightfv(GL_LIGHT1, GL_SPOT_EXPONENT, c);
        GLfloat a[] = {0,0,-5};
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, a);
        const GLfloat b[] = {30};
        glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, b);
    
        
    glPopMatrix();

}
void drawEnemies()
{
    for (int i = -6, j = 0; i > -map_len / 2; i -= 12, j++)
    {

        if (i < player[2] + 0.5)
        {
            drawEnemyCube(enemypos[j], float(i));
            if ((int(player[0]) == int(enemypos[j]) && i > player[2]) && (player[1] - player_y < 0.55))
            {
                if(immunity<0)
                {
                    lives--;
                    immunity=7;
                }
            }
               
            if (j % 6 == 0)
            {
                drawEnemyCube(enemypos3[j], float(i));
                if ((int(player[0]) == int(enemypos3[j]) && i > player[2]) && (player[1] - player_y < 0.55))
                {
                    if(immunity<0)
                    {
                        lives--;
                        immunity=7;
                    }
                    
                }
                  
            }
            if (j % 3 == 0)
            {
                drawEnemyCube(enemypos2[j], float(i));
                if ((int(player[0]) == int(enemypos2[j]) && i > player[2]) && (player[1] - player_y < 0.55))
                {
                    if(immunity<0)
                    {
                        lives--;
                        immunity=7;
                    }
                }
            }
            
        }

    }

}
void drawLightPoles()
{
    for (int i = -6, j = 0; i > -map_len / 2; i -= 12, j++)
    {

        if (i < player[2] + 0.5)
        {
            //drawPointLight(lightposition);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lightintensity);
        }
    }
}

void changeColor()
{

     if (mat_floor[color_var] >= 1.0)
        {
            mat_floor[color_var] = 1;
            color_var++;
            if (color_var == 2)
            {
                color_var = 0;
                mat_floor[0] =0;
                mat_floor[1] =0;
                mat_floor[2] = 0;
                mat_floor[rand() % 3]=1;
            }
            
       }

    mat_floor[color_var] += 0.01;

}

void display()
{

    //lightposition[0] = viewer[0];
    //lightposition[1] = viewer[1];
    //lightposition[2] = viewer[2];
    //glLightfv(GL_LIGHT0, GL_POSITION, lightposition);
    
    

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2, 2, -2, 2, 1, 10);
    //glOrtho(-4, 4, -4, 4, 4, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    viewer[0] = player[0];
    viewer[1] = player[1] + 1;
    viewer[2] = player[2] + 1.5;
    //gluLookAt(0, viewer[1], viewer[2],0, player[1], player[2], 0.0, 1.0, 0.0);
    gluLookAt(viewer[0], viewer[1], viewer[2], player[0], player[1], player[2], 0.0, 1.0, 0.0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //drawPointLight(lightposition, lightintensity);

    //changeColor();
    changeMaterial(mat_floor, 50);
    glPushMatrix();
    ground(0.02);
    glPopMatrix();

    changeMaterial(mat_floor, 50);
    printStairs();
    printSpectators();

    changeMaterial(mat_cube, 80);
    if (jump)
    {
        jumpfun();
    }
    glPushMatrix();
    drawPlayer(1);
    glPopMatrix();
    //printf("%f", player[2]);
    changeMaterial(mat_enemy, 80);
    glPushMatrix();
    drawEnemies();
    glPopMatrix();
    //changeMaterial(mat_cube, 80);
    glDisable(GL_LIGHTING);
    if (!start&&!gameover)
    {
        drawText("Press 's' or Raise your right hand to start", 180, 600);
    }
    if(gameover)
    {
        drawText("GAME OVER", 300, 600);
    }
    string score = "Score: "+to_string(-int(player[2])) ;
    drawText(score, 325, 700);
    string life = "Lives: ";
    for (int i=0; i<lives; i++) {
        life+="O";
    }
    drawText(life,50, 700);
    glEnable(GL_LIGHTING);
    glutSwapBuffers();
    glFlush();

}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(m_screenWidth, m_screenHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("ABHIGYAN AND DUBEY ENDLESS RUNNER");

    //int x = 1000, y = 1000, n = 4, ok;
    //ok = stbi_info("a.png", &x, &y, &n);
    //printf("%d", ok);
    init();
    glutDisplayFunc(display);

    glutSpecialFunc(keys);
    glutIdleFunc(move);
    glutKeyboardFunc(keyboard);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
