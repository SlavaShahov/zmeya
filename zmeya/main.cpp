#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace sf;

const int N = 30, M = 20; //N - длина, M - ширина
const int ts = 25; //Размер каждого плитки

int F;// Переменная отвечает за выбор фрукта
//1- яблоко
//2- золотое яблоко
//3- апельсин
//4- груша
//5- арбуз

int dir = 2, num = 4; //переменная dir отвечает за поворот, переменная num отвечает за длину змейки
/*Как поварачивается змейка при помощи переменной dir
dir = 1 -> лево
dir = 2 -> право
dir = 3 -> вверх
dir = 0 -> вниз*/

bool game = true; //переменная game отвечает за продолжение игры

int Score = 0;//переменная отвечает за набранные очки

//Структура змейки отвечает за её длину
struct Snake
{
    int x, y;
    //В структуре указаны переменные x, y
    //они отвечают за координаты плитки
} s[600];//s[] - это каждая плитка змейки

//Структура яблоки
struct Fruct
{
    int x, y;
    //Задаём координаты яблоки
} f;

//Функция Logic отвечает за саму игру
void Logic()
{
    //Здесь находятся первичные координаты
    //Просто рисуется сама змейка
    for (int i = num; i > 0; i--) {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    //движение змейки
    if (dir == 0)
        s[0].y += 1; //вниз
    if (dir == 1)
        s[0].x -= 1; //влево
    if (dir == 2)
        s[0].x += 1; //право
    if (dir == 3)
        s[0].y -= 1; //вверх

    //Проверка на то, если змейка уходит через стенку экрана, то она возращается на поле через другую сторону
    if (s[0].x > N - 1) {
        s[0].x = 0;
    }
    if (s[0].x < 0) {
        s[0].x = N - 1;
    }
    if (s[0].y > M - 1) {
        s[0].y = 0;
    }
    if (s[0].y < 0) {
        s[0].y = M - 1;
    }

    //Если змейка поедает яблоко
    if ((s[0].x == f.x) && (s[0].y == f.y)) {
        num++; //Змейка растёт
        Score++;//Увеличивается количество очков 
        F = 1 + rand() % 5;//Меняется переменная для выбора фрукта
        bool found = false;
        while (!found) {//Здесь идёт проверка на то, чтобы яблоко не появлялось в теле змейки, и задаются новые координаты яблока
            f.x = rand() % N;
            f.y = rand() % M;
            found = true;
            for (int i = 0; i < num; i++) {
                if ((f.x * ts == s[i].x * ts) && (f.y * ts == s[i].y * ts)) {
                    found = false;
                    break;
                }
            }
        }
    }

    //Здесь проигрыш
    for (int i = 1; i < num; i++) //цикл for для всех плиток змейки
        //Если змейка сталкивается со своим телом
        if ((s[0].x == s[i].x) && (s[0].y == s[i].y))
            game = false; //меняем значение переменной game на false
}


int main()
{
    int W = 100;//Переменная отвечает за то, сколько нужно набрать очков, чтобы выиграть
    ofstream RecF1;//Поток для записи рекорда
    ifstream RecF2;//Поток для чтения рекорда
    int Rec;//Переменная будет получать значение рекорда из файла

    RecF2.open("Record.txt"); //Открываем файл для чтения 
    RecF2 >> Rec; //Переменная Rec принимает значение рекорда из файла
    RecF2.close(); //Закрываем файл

    srand(time(NULL));

    RenderWindow window(VideoMode(ts * N, ts * M + 150), "Snake!");//Открываем окно

    //Я захотел сделать фон места, где будут отображаться набранные очки и рекорд, цветным
    RectangleShape rectangle(Vector2f(ts * N, ts * M + 150));//буду делать это, рисуя прямоугольник
    rectangle.setFillColor(Color(0, 51, 153));//задаю цвет прямоугольника

    //Как ставляется картинка
    /*
    Texture t; <- Задаётся переменная для текстуры
    t.loadFromFile("Расположение картинки"); <- в переменную текстуры
    загружаем картинку
    Sprite tiles(t); <- Задаём следующию переменную спрайта и
    загружаем текстуру*/

    //Ставим картнинку плиточек поля 
    Texture t;
    t.loadFromFile("D:/VS Progect/zmeya/Paint/tiles.png");
    Sprite tiles(t);

    //Ставим картинку змейки
    Texture sn;
    sn.loadFromFile("D:/VS Progect/zmeya/Paint/zmii.png");
    Sprite snake(sn);

    Texture FRUIT;//Создаём текстуру фруктов, которые будут меняться в switch

    //Ставим надпись gameover
    Texture go;
    go.loadFromFile("D:/VS Progect/zmeya/Paint/Game over.png");
    Sprite gameover(go);
    gameover.setPosition(90, 170);

    //Ставим надпись youwin
    Texture yw;
    yw.loadFromFile("D:/VS Progect/zmeya/Paint/You win.png");
    Sprite youwin(yw);
    youwin.setPosition(90, 170);

    //Создаём шрифт и текст для надписи количества очков и паузы 
    Font font;
    font.loadFromFile("Vogue Bold.ttf");
    Text text("", font, 40);

    //первичные координаты фрукта и его вид
    f.x = rand() % N;
    f.y = rand() % M;
    F = 1 + rand() % 5;

    Clock clock; //переменная clock нужна для ирового времени
    float timer = 0, delay = 0.12;
    //timer и delay отвечают за скорость змейки
    //timer отвечает, через какое время змейка двигается
    //delay - за задержку

    while (window.isOpen())
    {
        srand(time(NULL));
        bool Pause = false;
        while (Pause == false) {
            //Игровое время
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;

            //События, отвечающие за выход из программы
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {//Если событие равно нажатию крестика окна или если была нажата клавиша esc, то выходим из программы
                    window.close();
                    return 0;
                }

            }

            //Управление змейкой через клавиатуру
            if (Keyboard::isKeyPressed(Keyboard::Left)) //Если стрелка влево нажата
                dir = 1;
            if (Keyboard::isKeyPressed(Keyboard::Right)) //Если стрелка вправо нажата
                dir = 2;
            if (Keyboard::isKeyPressed(Keyboard::Up)) //Если стрелка вверх нажата
                dir = 3;
            if (Keyboard::isKeyPressed(Keyboard::Down)) //Если стрелка вниз нажата
                dir = 0;

            //Здесь отвечает за работу игры
            if ((timer > delay && game) && (timer > delay && Score != W)) {
                timer = 0; //Возвращаем время
                Logic(); //Вызываем работу игры
            }

            window.clear();

            window.draw(rectangle);//рисуем фон

            //Для прорисовки всего графичесского окна
            //Пишем цикл for
            for (int i = 0; i < N; i++)
                for (int j = 0; j < M; j++) {
                    tiles.setPosition(i * ts, j * ts); //Позиция
                    window.draw(tiles); //Рисуем
                }

            //Рисуем змейку
            for (int i = 0; i < num; i++) {
                if (i != 0) //Рисуем тело змейки
                    snake.setTextureRect(IntRect(0, 0, ts, ts));
                else //Рисуем голову змейки
                    snake.setTextureRect(IntRect(dir * ts, ts, ts, ts));

                // Во время проигрыша голова изменяется
                if (!game && i == 1)
                    snake.setTextureRect(IntRect(dir * ts, ts * 2, ts, ts));

                snake.setPosition(s[i].x * ts, s[i].y * ts); //Задаём позицию
                //для плиток змейки
                window.draw(snake);
            }

            //Выбор фрукта
            switch (F) {
            case 1:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/ahyennie apple.png");//загружаем яблоко
                break;
            case 2:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/goldenapple.png");//загружаем золотое яблоко
                break;
            case 3:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/apelsin.png");//загружаем апельсин
                break;
            case 4:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/grysha.png");//загружаем грушу
                break;
            case 5:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/arbyz.png");//загружаем арбуз
                break;
            }
            Sprite fruit(FRUIT);//создаём спарйт текстуры фрукта
            fruit.setPosition(f.x * ts, f.y * ts);//задаём позицию фрукта
            window.draw(fruit);//рисуем фрукт

            ostringstream PlayerScoreString;//Объявляем переменную для вывода набранных очков
            PlayerScoreString << Score;//Заносим в неё число очков(формируем строку)
            text.setString("Score:" + PlayerScoreString.str());//Задаём строку тексту и вызываем строку методом .str()
            text.setFillColor(Color::Green);//задаём цвет текста
            text.setPosition(20, 550);//Задаём позицию строке
            window.draw(text);//рисуем текст

            //Выводи на экран рекорд: выводим рекорд из файла до момента, пока он не будет меньше набранных за игру очков
            if (Score <= Rec) {
                ostringstream RecordString;//Объявляем переменную для вывода рекорда
                RecordString << Rec;//Заносим в неё Значение из файла(формируем строку)
                text.setString("Record:" + RecordString.str());//Задаём строку тексту и вызываем строку методом .str()
                text.setFillColor(Color::Green);//задаём цвет текста
                text.setPosition(515, 550);//Задаём позицию строке
                window.draw(text);//рисуем текст
            }
            else {
                ostringstream RecordString;//Объявляем переменную для вывода рекорда
                RecordString << Score;//Заносим в неё число набранных за игру очков(формируем строку)
                text.setString("New Record:" + RecordString.str());//Задаём строку тексту и вызываем строку методом .str()
                text.setFillColor(Color::Green);//задаём цвет текста
                text.setPosition(515, 550);//Задаём позицию строке
                window.draw(text);//рисуем текст
            }

            //Если мы выиграли, то будеть выдовать надпись youwin
            if (Score == W) {
                window.draw(youwin);
                RecF1.open("Record.txt");//открываем файл для записи рекорда
                RecF1 << Score;
                RecF1.close();
            }
            //Если мы проиграли, то будеть выдовать надпись gameover
            if (!game) {
                window.draw(gameover);
                if (Rec < Score) {//если набранные очки больше рекорда, то рекорд перезаписывется
                    RecF1.open("Record.txt");
                    RecF1 << Score;
                    RecF1.close();
                }
            }

            window.display();

            //Если у нас проигрышь или победа, пауза не рисуется
            if (!game || Score == W) {
                break;
            }

            //Реализуем Паузу
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {//Если нажали enter
                Pause = true;
                while (Pause == true) {//Пока у нас пауза, рисуем слово pause красным цветом
                    text.setString("Pause");
                    text.setPosition(325, 550);
                    text.setFillColor(Color::Red);
                    window.draw(text);
                    window.display();
                    if (Keyboard::isKeyPressed(Keyboard::Escape)) {//Выход из программы с паузы через нажатие esc
                        window.close();
                        return 0;
                    }
                    if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {//Возвращаемся к игре
                        Pause = false;
                    }
                }
            }
        }
    }
}