#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <locale.h>
#define NUM_SEGMENTS 100


/**
 * @file
 * @brief Prosty Snake Game w OpenGL.
 */

/**
 * @brief Struktura przechowująca współrzędne kwadratów tworzących ciało węża oraz wskaźnik na poprzedzający kwadrat.
 */
struct snake_body{

    snake_body *prev;
    float lbx;
    float lby;
    float rbx;
    float rby;
    float lux;
    float luy;
    float rux;
    float ruy;
};



/**
 * Zmienna przechowująca informację o koordynatach x pokarmu.
 */
int x;

/**
 * Zmienna przechowująca informację o koordynatach y pokarmu.
 */
int y;

/**
 * Flaga określająca, czy gra jest zakończona.
 * Jeśli wartość wynosi 1, gra się zakończyła.
 */
int game_over = 0;

/**
 * Flaga określająca, czy sterowanie jest włączone.
 * Jeśli wartość wynosi 1, sterowanie jest włączone.
 */
int isControlEnabled = 1;

/**
 * Zmienna przechowująca wynik gracza.
 */
int score = 0;

/**
 * Stała określająca szerokość okna.
 */
const int windowWidth = 800;

/**
 * Stała określająca wysokość okna.
 */
const int windowHeight = 800;

/**
 * Stała określająca rozmiar siatki gry.
 */
const int gridSize = 50;

/**
 * Zmienna określająca aktualne menu.
 * Wartości:
 * 0 - ekran początkowy,
 * 1 - gra w trakcie,
 * 2 - ekran gry,
 * 3 - ekran końcowy.
 */
int menu = 0;

/**
 * Zmienna określająca aktualny kierunek poruszania się węża.
 * Wartości:
 * 1 - góra,
 * 2 - dół,
 * 3 - prawo,
 * 4 - lewo.
 */
int direction = 1;



/**
 * Wskaźnik na pierwszy element kolejki reprezentującej ciało węża.
 */
snake_body *header;

/**
 * Wskaźnik na drugi element kolejki reprezentującej ciało węża.
 */
snake_body *second;

/**
 * Wskaźnik na trzeci element kolejki reprezentującej ciało węża.
 */
snake_body *third;

/**
 * Wskaźnik na ostatni element kolejki reprezentującej ciało węża.
 */
snake_body *last;

/**
 * Wskaźnik pomocniczy wykorzystywany do iteracji po elementach kolejki reprezentującej ciało węża.
 */
snake_body *elem;






/**
 * @brief Funkcja odpowiedzialna za wyznaczanie nowych współrzędnych pokarmu. Wyznacza ona dwie losowe współrzędne i porównuje je ze współrzędnymi wszystkich kwadratów tworzących węża. Jeśli współrzędne nachodzą na siebie losuje nowe.
 */
void generate_food(){

int check = 0;

snake_body *current = last;

do {
    x = rand()%97 - 48;
    y = rand()%97 - 48;
    current = last;
    check = 0;
    while(current->prev != NULL){
        if(fabs(x-((current->rbx + current->lbx)/2)) <= 4 && fabs(y-((current->rby+current->ruy)/2) <= 4)){
           check = 1;
           break;
    }
    current = current->prev;
}
} while(check==1);


}






/**
 * @brief Funkcja odpowiedzialna za inicjalizację węża. Alokuje ona pamięć dla trzech kwadratów, wyznaczając im współrzędne na środku ekranu. Przypisuje ona do wskaźnika last adres trzeciego kwadratu. Na koniec wywołuje ona funkcję generate_food tworzącą pokarm.
*/
void initialize_snake(){

    header = (snake_body *)malloc(sizeof(snake_body));
    header->lbx = -2;
    header->lby = -2;
    header->rbx = 2;
    header->rby = -2;
    header->lux = -2;
    header->luy = 2;
    header->rux = 2;
    header->ruy = 2;
    header->prev = NULL;

    second = (snake_body *)malloc(sizeof(snake_body));
    second->prev = header;
    second->lbx = -2;
    second->lby = -6;
    second->rbx = 2;
    second->rby = -6;
    second->lux = -2;
    second->luy = -2;
    second->rux = 2;
    second->ruy = -2;

    third = (snake_body *)malloc(sizeof(snake_body));
    third->prev = second;
    third->lbx = -2;
    third->lby = -10;
    third->rbx = 2;
    third->rby = -10;
    third->lux = -2;
    third->luy = -6;
    third->rux = 2;
    third->ruy = -6;

    score = 0;
    last = third;


    generate_food();
}







/**
 * @brief Funkcja odpowiedzialna za dodanie nowego kwadratu na końcu węża. Alokuje ona pamięć na strukturę snake_body, przypisuje do wskaźnika new_tail, który zostaje dodany do kolejki przechowującej ciało węża.
*/
void add_tail(){

    snake_body *new_tail = (snake_body *)malloc(sizeof(snake_body));
    *new_tail = *last;
    new_tail->prev = last;
    last = new_tail;

}







/**
 * @brief Funkcja odpowiedzialna za rysowanie kwadratu będącego częścią węża. Jako parametr przyjmuje ona wskaźnik do ostatniego elementu kolejki. W funkcji nadany zostaje kolor kwadratu. Następnie przypisuje się mu współrzędne przekazanego wskaźnika.
*
* @param s Wskażnik na ostatni element kolejki.
*/
void draw_quad(struct snake_body *s){
    glBegin(GL_QUADS);
    glColor3f(0.2, 0.6, 1);
    glVertex2f(s->lbx, s->lby);
    glVertex2f(s->rbx, s->rby);
    glVertex2f(s->rux, s->ruy);
    glVertex2f(s->lux, s->luy);
    glEnd();
}






/**
 * @brief Funkcja odpowiedzialna za rysowanie pokarmu. Funkcja korzysta ze współrzędnych x i y wyznaczonych przez funkcję generate_food() i na ich podstawie rysuje koło w odpowiednim kolorze w odpowiednim miejscu ekranu.
*/
void draw_food(){

    float radius = 2.0;


    glBegin(GL_TRIANGLE_FAN);
     glColor3f(0.2f, 0.6f, 1.0f);
    glVertex2f(x, y);
    for (int i = 0; i <= NUM_SEGMENTS; i++) {
        float theta = 2.0f * M_PI * i / NUM_SEGMENTS;
        float x_coord = x + radius * cosf(theta);
        float y_coord = y + radius * sinf(theta);
        glVertex2f(x_coord, y_coord);
    }
    glEnd();
}






/**
 * @brief Funkcja odpowiedzialna za sprawdzenie czy głowa węża dotknęła pokarm. W jej ciele wyznaczony zostaje środek wertykalny i horyzontalny głowy węża. Następnie jego współrzędne zostają porównane ze współrzędnymi środka pokarmu.
* Jeśli odległość środka głowy do środka pokarmu jest mniejsza niż 4 funkcja wywołuje funkcję generate_food(),  która wyznacza nowe współrzędne pokarmu. Inkrementuje ona wartość zmiennej score oraz wywołuje funkcję add_tail()
* dodającą nową część węża.
*/
void check_food(){

    float midx_head = (header->rbx + header->lbx)/2;
    float midy_head = (header->rby + header->ruy)/2;

    if(fabs(midx_head-x)<4 && fabs(midy_head - y)<4){
        generate_food();
        score++;
        add_tail();
}}





/**
 * @brief Funkcja typu int sprawdzająca kontakt węża ze ścianami. Jeśli współrzędne węża wykraczają poza współrzędne ścian ekranu, funkcja zwraca wartość 1, w przeciwnym razie zwraca 0.
*/
int check_wall_hit(){

    if(header->rbx > 50) return 1;

    if(header->lbx < -50) return 1;

    if(header->ruy > 50) return 1;

    if(header->rby < -50) return 1;

    return 0;

}





/**
 * @brief Funkcja odpowiedzialna za ruch węża. Iteruje ona po członach węża aktualizując współrzędne kwadratów poprzez przypisanie im współrzędnych poprzedzających ich członów poprzez utworzony wskaźnik current (do którego na początku przypisany jest wartość wskaźnika last.
*/
void move_snake() {
    snake_body *current = last;
    while (current != NULL && current->prev != NULL) {
        current->lbx = current->prev->lbx;
        current->lby = current->prev->lby;
        current->rbx = current->prev->rbx;
        current->rby = current->prev->rby;
        current->lux = current->prev->lux;
        current->luy = current->prev->luy;
        current->rux = current->prev->rux;
        current->ruy = current->prev->ruy;
        current = current->prev;
    }

}




/**
 * @brief Funkcja odpowiedzialna za tworzenie tekstu z aktualnym rezultatem, którego wartość jest brana ze zmiennej globalnej score.
*/
void display_result(){

    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(-49.0f, 47.0f);
    char scoreText[10];
    sprintf(scoreText, "Score: %d", score);
    for (char* c = scoreText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

}





/**
 * @brief Funkcja odpowiedzialna za zapisywanie najlepszego rezultatu w pliku tekstowym na Pulpicie. Początkowo próbuje otworzyć plik do odczytu. Jeśli taki nie istnieje to otwiera plik do zapisu jednocześnie go tworząc i zapisując wynik.
* Jeśli plik istnieje, a wynik uzyskany w czasie gry jest lepszy,niż zapisany rekord to zamyka plik do odczytu i otwiera go do zapisu po czym zapisuje nowy rekord i zamyka plik.
*/
int check_record(){

        FILE *file;
        char filename[] = "C:/Users/peter/Desktop/record_book.txt";

        file = fopen(filename, "r");
        if(file == NULL){
            file = fopen(filename, "w");
            if(file == NULL){
                perror("Nie ma takiego pliku!");
                return 0;
            }
            else{
                fprintf(file, "%d", score);
                fclose(file);
                return 1;
            }
        }
        else{
            int rekord;
            fscanf(file, "%d", &rekord);
            fclose(file);
            if(score > rekord){
                file = fopen(filename, "w");
                if(file == NULL){
                    perror("Nie można otworzyć pliku!");
                    return 0;
                }
                fprintf(file, "%d", score);
                fclose(file);
                return 1;
            }
        }
        return 0;

}









/**
 * @brief Funkcja typu int sprawdzająca kontakt głowy węża z innymi częściami jego ciała. Najpierw tworzy przechodni wskaźnik, do którego przypisuje zawartość wskaźnika last. Następnie iteruje po wszystkich członach węża sprawdzając czy odległość środka głowy do środka
* danego członu jest mniejsza niż 4. Jeśli tak - funkcja zwraca 1, jeśli nie - zwraca 0.
*/
int check_suicide(){

    snake_body *current = last;
    int check = 0;

    float midx_head = (header->rbx + header->lbx)/2;
    float midy_head = (header->rby + header->ruy)/2;

    while(current->prev != NULL){
        float midx_cur = (current->rbx + current->lbx)/2;
        float midy_cur = (current->rby + current->ruy)/2;
        if(fabs(midx_head - midx_cur) < 4 && fabs(midy_head-midy_cur) < 4){
            check = 1;
            break;
        }
        current = current->prev;
    }

    if(check == 1)
        return 1;
    return 0;
}













/**
 * @brief Funkcja zwrotna odpowiedzialna za ruch węża. Na początku aktualizuje współrzędne wszystkich członów poza głową poprzez wywołanie funkcji move_snake(). Następnie aktualizuje współrzędne głowy węża w zależności od wartości zmiannej direction.
* Po zaaktualizowaniu współrzędnych funkcja sprawdza czy nie doszło do zderzenia ze ścianą bądż z ciałem węża. Jeśli doszło zmienia wartość zmiennej game_over na 1, a menu na 3. Jeśli nie doszło do zderzenia funkcja sprawdza czy doszło do kontaktu z jedzeniem,
* odświeża obraz oraz ponownie wykonuje metodę glutTimerFunc z rekursyjnym wywołaniem funkcji moveq().
*/
void moveq(int direct){

    move_snake();

    if (direction == 1) {
        header->lby += 4.0; header->rby += 4.0; header->luy += 4.0; header->ruy += 4.0;
    } else if (direction == 2) {
        header->lby -= 4.0; header->rby -= 4.0; header->luy -= 4.0; header->ruy -= 4.0;
    } else if (direction == 3) {
        header->rbx += 4.0; header->lbx += 4.0; header->rux += 4.0; header->lux += 4.0;
    } else if (direction == 4) {
        header->rbx -= 4.0; header->lbx -= 4.0; header->rux -= 4.0; header->lux -= 4.0;
    }
    if (check_suicide() || check_wall_hit()) {
            game_over = 1;
            glutPostRedisplay();
            menu = 3;
        }

      if(!game_over){
            check_food();
            glutPostRedisplay();
            check_food();
            glutTimerFunc(250, moveq, 0);
      }

    }






/**
 * @brief Funkcja odpowiedzialna za zmianę kierunku węża. Aktualizuje ona współrzędne ciała węża w taki sam sposób jak funkcja moveq(). Następnie odświeża obraz i sprawdza kontakt z pokarmem.
*/
void change_direction(int direct){

    move_snake(); // Move all segments before moving the head

    if (direction == 1) { // Up
        header->lby += 4.0; header->rby += 4.0; header->luy += 4.0; header->ruy += 4.0;
    } else if (direction == 2) { // Down
        header->lby -= 4.0; header->rby -= 4.0; header->luy -= 4.0; header->ruy -= 4.0;
    } else if (direction == 3) { // Right
        header->rbx += 4.0; header->lbx += 4.0; header->rux += 4.0; header->lux += 4.0;
    } else if (direction == 4) { // Left
        header->rbx -= 4.0; header->lbx -= 4.0; header->rux -= 4.0; header->lux -= 4.0;
    }
    glutPostRedisplay();
    check_food();
}







/**
 * @brief Funkcja ustawiająca początkowe wartości odpowiednich zmiennych globalnych (na początku gry oraz przy zresetowaniu). Ustawia direction na 1, menu na 2 oraz game_over na 0.Wywołuje metodę initialize_snake() oraz glutTimerFunc (z wywołaniem metody moveq()).
*/
void start_game(){

    direction = 1;
    initialize_snake();
    glutTimerFunc(100, moveq, 0);
    menu = 2;
    game_over = 0;


}



/**
 * @brief Funkcja odpowiedzialna za dealokację pamięci wszystkich kwadratów tworzących ciało węża.
*/
void delete_snake(){

    struct snake_body *current;
    while(last != NULL){
            current = last;
            last = last->prev;
            free(current);
    }

}







/**
 * @brief Funkcja odpowiedzialna za wyrenderowanie sceny. Jeśli wartość zmiennej menu wynosi 0 wyświetla ekran początkowy. Jeśli menu wynosi 1 wykonuje funkcję start_game(), jeśli menu wynosi 2 wyświetla planszę gry oraz wywołuje funkcje rysujące węża, pokarm i wynik.
* Jeśli menu wynosi 3 funkcja wyświetla ekran końcowy, na którym wyświetlany jest wynik oraz opcja ponownego rozpoczęcia gry lub zakończenia gry. Funkcja wywołuje też wtedy funkcję check_rekord() i jeśli rekord został pobity dodaje odpowiednią informację na ekranie.
*/
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    if(menu==0){
            glColor3f(0.0, 0.0, 0.0);


            glColor3f(1.0, 1.0, 1.0);
            glRasterPos2f(-10, 0);
            char scoreText[13];
            sprintf(scoreText, "ZACZNIJ GRE!");
            for (char* c = scoreText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
            glutSwapBuffers();
        }


else if (menu==1){
         start_game();
}
else if (menu ==2)
{

    int numSquares = 25;
    float squareSize = (float)gridSize * 2 / numSquares;

    for (int i = 0; i < numSquares; ++i) {
        for (int j = 0; j < numSquares; ++j) {
            if ((i + j) % 2 == 0) {
              //  glColor3f(0.65, 0.17, 0.17);
                  glColor3f(0.8, 0.8, 0.8);
            } else {
                glColor3f(0.2, 0.2, 0.2);
            }

            float x = i * squareSize - gridSize;
            float y = j * squareSize - gridSize;

            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + squareSize, y);
            glVertex2f(x + squareSize, y + squareSize);
            glVertex2f(x, y + squareSize);
            glEnd();
        }
    }

    elem = last;
    while(elem!= NULL){
    draw_quad(elem);
    elem = elem->prev;
    }

    display_result();
    draw_food();
    glutSwapBuffers();
}


else if(menu==3){
    glColor3f(0.0, 0.0, 0.0);


        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(-15.0f, 20.0f);

        char scoreText[30];
        sprintf(scoreText, "Koniec gry! Wynik: %d", score);
        for (char* c = scoreText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }

        int rekord = check_record();
        if(rekord == 1){
            glRasterPos2f(-15.0f, 10.0f);

            char scor[30];
            sprintf(scor, "Nowy rekord!");
            for (char* c = scor; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
        }


        glRasterPos2f(-15.0f,-3.0f);
        char odnowa[15];
        sprintf(odnowa, "Zacznij od nowa!");
        for(char *c = odnowa; *c != '\0'; c++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }

        glRasterPos2f(-15.0f,-9.0f);
        char koniec[15];
        sprintf(koniec, "Zakoncz gre!");
        for(char *c = koniec; *c != '\0'; c++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
        }

    glutSwapBuffers();
        }


}









/**
 * @brief Funkcja zwrotna odpowiedzialna za obsługę klawiszy. Obsługiwane klawisze to up, down, left i right. Po ich wciśnięciu zostaje zaaktualizowana wartość zmiennej direction oraz wykonana metoda change_direction().
*/
void specialKeyPressed(int key, int x, int y) {
    if (!isControlEnabled) return;
    switch(key) {
        case GLUT_KEY_UP:
            if(direction != 2 && direction != 1){
            direction = 1;
            change_direction(key);}
            break;
        case GLUT_KEY_DOWN:
            if(direction != 1 && direction != 2){
            direction = 2;
            change_direction(key);}
            break;
        case GLUT_KEY_RIGHT:
            if(direction != 4 && direction != 3){
            direction = 3;
            change_direction(key);}
            break;
        case GLUT_KEY_LEFT:
            if(direction != 3 && direction != 4){
            direction = 4;
            change_direction(key);}
            break;
    }
   // glutPostRedisplay();
}




/**
 * @brief Funkcja odpowiedzialna za obsługę naciśnięć przycisków myszy. Obsługuje ona lewy przycisk myszy w dwóch wypadkach. Pierwszy dotyczy ekranu początkoweogo (menu równe 0). Wciśnięcie napisu rozpoczynającego grę ustawia menu na 1.
* Drugi dotyczy sytuacji, gdy wyświetlany jest ekran końcowy (menu równa się 3). Wciśnięcie odpowiednich napisów wywołuje metodę delete_snake() i albo ustawia wartość menu na 1, bądź kończy grę poprzez metodę exit(0).
*/
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
      float scaledX = ((float)x / windowWidth) * 100.0 - 50.0;
      float scaledY = ((float)(windowHeight - y) / windowHeight) * 100.0 - 50.0;

        if (menu == 0 && scaledX >= -10 && scaledX <= 10 && scaledY >= 0&& scaledY <= 2.3) {
            menu = 1;
            glutPostRedisplay();
        }
        if (menu == 3 && scaledX >= -15 && scaledX <= 7 && scaledY >= -4&& scaledY <= -2) {
            delete_snake();
            menu = 1;
            glutPostRedisplay();
        }
        if (menu == 3 && scaledX >= -15 && scaledX <= 3 && scaledY >= -10&& scaledY <= -6) {
            delete_snake();
            exit(0);
        }

    }

}






/**
 * @brief Funkcja inicjująca matrycę ekranu. Ustawia kolor tła na biały, tryb macierzy na projekcji, resetuje macierz projekcji, ustawia obszar widoku od -50 do 50 (wertykalnie i horyzontalnie).
*/
void init() {
glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
}



/**
 * @brief Funkcja main inicjalizuje aplikację graficzną "Snake" korzystając z biblioteki OpenGL i GLUT. Ustawia lokalizację na język polski, inicjalizuje tryb wyświetlania z podwójnym buforowaniem i kolorem RGB,
* ustawia początkową pozycję oraz rozmiar okna, tworzy okno o tytule "Snake", wywołuje funkcję inicjalizacyjną, przypisuje funkcje obsługi wyświetlania, klawiszy specjalnych oraz przycisków myszy,
* a następnie rozpoczyna główną pętlę przetwarzania zdarzeń GLUT.
*/
int main(int argc, char** argv){

    setlocale(LC_CTYPE, "Polish");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(200,0);
    glutInitWindowSize(800,800);
    glutCreateWindow("Snake");
    init();
    glutDisplayFunc(renderScene);
    glutSpecialFunc(specialKeyPressed);
    glutMouseFunc(mouseButton);
    glutMainLoop();
    return 0;
}
