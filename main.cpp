#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>








struct snake_body{

    snake_body *prev;



};


float xl=-2;
float xl2=-2;
float xp=2;
float xp2=-2;
float yl=-2;
float yl2=2;
float yp=2;
float yp2=2;









int direction = 1;

void draw_quad(float xl, float xl2, float xp, float xp2, float yl, float yl2, float yp, float yp2){
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0); // Ustaw kolor na bia³y
    glVertex2f(xl, xl2);  // Lewy dolny róg kwadratu
    glVertex2f(xp, xp2);   // Prawy dolny róg kwadratu
    glVertex2f(yp, yp2);    // Prawy górny róg kwadratu
    glVertex2f(yl, yl2);   // Lewy górny róg kwadratu
    glEnd();
}


//funkcja odpowiedzialna za ruch węża po wybraniu kierunku, a przed jego zmianą. Ruch uzależniony jest od wartości zmiennej direction.
void moveq(int direct){

    if(direction == 1) {
        // Przesuñ kwadrat w górê
        yl2 += 4.0;
        yp2 += 4.0;
        xp2 += 4.0;
        xl2 += 4.0;
    }

     if(direction == 2) {
        // Przesuñ kwadrat w górê
        yl2 -= 4.0;
        yp2 -= 4.0;
        xp2 -= 4.0;
        xl2 -= 4.0;
    }


     if(direction == 3) {
        // Przesuñ kwadrat w górê
        yl += 4.0;
        yp += 4.0;
        xp += 4.0;
        xl += 4.0;
    }

     if(direction == 4) {
        // Przesuñ kwadrat w górê
        yl -= 4.0;
        yp -= 4.0;
        xp -= 4.0;
        xl -= 4.0;
    }

glutTimerFunc(250,moveq,0);
glutPostRedisplay();

}


void move_quad(int move_option) {
    if(move_option == GLUT_KEY_UP) {
        // Przesuñ kwadrat w górê
        yl2 += 4.0;
        yp2 += 4.0;
        xp2 += 4.0;
        xl2 += 4.0;
    }

     if(move_option == GLUT_KEY_DOWN) {
        // Przesuñ kwadrat w górê
        yl2 -= 4.0;
        yp2 -= 4.0;
        xp2 -= 4.0;
        xl2 -= 4.0;
    }


     if(move_option == GLUT_KEY_RIGHT) {
        // Przesuñ kwadrat w górê
        yl += 4.0;
        yp += 4.0;
        xp += 4.0;
        xl += 4.0;
    }

     if(move_option == GLUT_KEY_LEFT) {
        // Przesuñ kwadrat w górê
        yl -= 4.0;
        yp -= 4.0;
        xp -= 4.0;
        xl -= 4.0;
    }


}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_quad(xl, xl2, xp, xp2, yl, yl2, yp, yp2);
    glutSwapBuffers();
}

void specialKeyPressed(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
            move_quad(key);
            direction = 1;
            break;
        case GLUT_KEY_DOWN:
            move_quad(key);
            direction = 2;
            break;
        case GLUT_KEY_RIGHT:
            move_quad(key);
            direction = 3;
            break;
        case GLUT_KEY_LEFT:
            move_quad(key);
            direction = 4;
            break;
    }
    glutPostRedisplay();
}

void init() {
glClearColor(0.0, 0.0, 0.0, 1.0); // Ustawienie koloru tła na biały
    glMatrixMode(GL_PROJECTION); // Ustawienie trybu macierzy na projekcji
    glLoadIdentity(); // Zresetowanie macierzy projekcji

    // Ustawienie obszaru widoku od (-50, -50) do (50, 50)
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    // lub użyj glOrtho2D(-50.0, 50.0, -50.0, 50.0);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(200,0);
    glutInitWindowSize(800,800);
    glutCreateWindow("Snake");
    init();
    glutDisplayFunc(renderScene);
    glutSpecialFunc(specialKeyPressed); // Obs³uga klawiszy specjalnych
    glutTimerFunc(100, moveq, 0);
    glutMainLoop();
    return 0;
}
