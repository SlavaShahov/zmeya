#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace sf;

const int N = 30, M = 20; //N - �����, M - ������
const int ts = 25; //������ ������� ������

int F;// ���������� �������� �� ����� ������
//1- ������
//2- ������� ������
//3- ��������
//4- �����
//5- �����

int dir = 2, num = 4; //���������� dir �������� �� �������, ���������� num �������� �� ����� ������
/*��� �������������� ������ ��� ������ ���������� dir
dir = 1 -> ����
dir = 2 -> �����
dir = 3 -> �����
dir = 0 -> ����*/

bool game = true; //���������� game �������� �� ����������� ����

int Score = 0;//���������� �������� �� ��������� ����

//��������� ������ �������� �� � �����
struct Snake
{
    int x, y;
    //� ��������� ������� ���������� x, y
    //��� �������� �� ���������� ������
} s[600];//s[] - ��� ������ ������ ������

//��������� ������
struct Fruct
{
    int x, y;
    //����� ���������� ������
} f;

//������� Logic �������� �� ���� ����
void Logic()
{
    //����� ��������� ��������� ����������
    //������ �������� ���� ������
    for (int i = num; i > 0; i--) {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    //�������� ������
    if (dir == 0)
        s[0].y += 1; //����
    if (dir == 1)
        s[0].x -= 1; //�����
    if (dir == 2)
        s[0].x += 1; //�����
    if (dir == 3)
        s[0].y -= 1; //�����

    //�������� �� ��, ���� ������ ������ ����� ������ ������, �� ��� ����������� �� ���� ����� ������ �������
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

    //���� ������ ������� ������
    if ((s[0].x == f.x) && (s[0].y == f.y)) {
        num++; //������ �����
        Score++;//������������� ���������� ����� 
        F = 1 + rand() % 5;//�������� ���������� ��� ������ ������
        bool found = false;
        while (!found) {//����� ��� �������� �� ��, ����� ������ �� ���������� � ���� ������, � �������� ����� ���������� ������
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

    //����� ��������
    for (int i = 1; i < num; i++) //���� for ��� ���� ������ ������
        //���� ������ ������������ �� ����� �����
        if ((s[0].x == s[i].x) && (s[0].y == s[i].y))
            game = false; //������ �������� ���������� game �� false
}


int main()
{
    int W = 100;//���������� �������� �� ��, ������� ����� ������� �����, ����� ��������
    ofstream RecF1;//����� ��� ������ �������
    ifstream RecF2;//����� ��� ������ �������
    int Rec;//���������� ����� �������� �������� ������� �� �����

    RecF2.open("Record.txt"); //��������� ���� ��� ������ 
    RecF2 >> Rec; //���������� Rec ��������� �������� ������� �� �����
    RecF2.close(); //��������� ����

    srand(time(NULL));

    RenderWindow window(VideoMode(ts * N, ts * M + 150), "Snake!");//��������� ����

    //� ������� ������� ��� �����, ��� ����� ������������ ��������� ���� � ������, �������
    RectangleShape rectangle(Vector2f(ts * N, ts * M + 150));//���� ������ ���, ����� �������������
    rectangle.setFillColor(Color(0, 51, 153));//����� ���� ��������������

    //��� ���������� ��������
    /*
    Texture t; <- ������� ���������� ��� ��������
    t.loadFromFile("������������ ��������"); <- � ���������� ��������
    ��������� ��������
    Sprite tiles(t); <- ����� ��������� ���������� ������� �
    ��������� ��������*/

    //������ ��������� �������� ���� 
    Texture t;
    t.loadFromFile("D:/VS Progect/zmeya/Paint/tiles.png");
    Sprite tiles(t);

    //������ �������� ������
    Texture sn;
    sn.loadFromFile("D:/VS Progect/zmeya/Paint/zmii.png");
    Sprite snake(sn);

    Texture FRUIT;//������ �������� �������, ������� ����� �������� � switch

    //������ ������� gameover
    Texture go;
    go.loadFromFile("D:/VS Progect/zmeya/Paint/Game over.png");
    Sprite gameover(go);
    gameover.setPosition(90, 170);

    //������ ������� youwin
    Texture yw;
    yw.loadFromFile("D:/VS Progect/zmeya/Paint/You win.png");
    Sprite youwin(yw);
    youwin.setPosition(90, 170);

    //������ ����� � ����� ��� ������� ���������� ����� � ����� 
    Font font;
    font.loadFromFile("Vogue Bold.ttf");
    Text text("", font, 40);

    //��������� ���������� ������ � ��� ���
    f.x = rand() % N;
    f.y = rand() % M;
    F = 1 + rand() % 5;

    Clock clock; //���������� clock ����� ��� ������� �������
    float timer = 0, delay = 0.12;
    //timer � delay �������� �� �������� ������
    //timer ��������, ����� ����� ����� ������ ���������
    //delay - �� ��������

    while (window.isOpen())
    {
        srand(time(NULL));
        bool Pause = false;
        while (Pause == false) {
            //������� �����
            float time = clock.getElapsedTime().asSeconds();
            clock.restart();
            timer += time;

            //�������, ���������� �� ����� �� ���������
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {//���� ������� ����� ������� �������� ���� ��� ���� ���� ������ ������� esc, �� ������� �� ���������
                    window.close();
                    return 0;
                }

            }

            //���������� ������� ����� ����������
            if (Keyboard::isKeyPressed(Keyboard::Left)) //���� ������� ����� ������
                dir = 1;
            if (Keyboard::isKeyPressed(Keyboard::Right)) //���� ������� ������ ������
                dir = 2;
            if (Keyboard::isKeyPressed(Keyboard::Up)) //���� ������� ����� ������
                dir = 3;
            if (Keyboard::isKeyPressed(Keyboard::Down)) //���� ������� ���� ������
                dir = 0;

            //����� �������� �� ������ ����
            if ((timer > delay && game) && (timer > delay && Score != W)) {
                timer = 0; //���������� �����
                Logic(); //�������� ������ ����
            }

            window.clear();

            window.draw(rectangle);//������ ���

            //��� ���������� ����� ������������� ����
            //����� ���� for
            for (int i = 0; i < N; i++)
                for (int j = 0; j < M; j++) {
                    tiles.setPosition(i * ts, j * ts); //�������
                    window.draw(tiles); //������
                }

            //������ ������
            for (int i = 0; i < num; i++) {
                if (i != 0) //������ ���� ������
                    snake.setTextureRect(IntRect(0, 0, ts, ts));
                else //������ ������ ������
                    snake.setTextureRect(IntRect(dir * ts, ts, ts, ts));

                // �� ����� ��������� ������ ����������
                if (!game && i == 1)
                    snake.setTextureRect(IntRect(dir * ts, ts * 2, ts, ts));

                snake.setPosition(s[i].x * ts, s[i].y * ts); //����� �������
                //��� ������ ������
                window.draw(snake);
            }

            //����� ������
            switch (F) {
            case 1:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/ahyennie apple.png");//��������� ������
                break;
            case 2:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/goldenapple.png");//��������� ������� ������
                break;
            case 3:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/apelsin.png");//��������� ��������
                break;
            case 4:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/grysha.png");//��������� �����
                break;
            case 5:
                FRUIT.loadFromFile("D:/VS Progect/zmeya/Paint/arbyz.png");//��������� �����
                break;
            }
            Sprite fruit(FRUIT);//������ ������ �������� ������
            fruit.setPosition(f.x * ts, f.y * ts);//����� ������� ������
            window.draw(fruit);//������ �����

            ostringstream PlayerScoreString;//��������� ���������� ��� ������ ��������� �����
            PlayerScoreString << Score;//������� � �� ����� �����(��������� ������)
            text.setString("Score:" + PlayerScoreString.str());//����� ������ ������ � �������� ������ ������� .str()
            text.setFillColor(Color::Green);//����� ���� ������
            text.setPosition(20, 550);//����� ������� ������
            window.draw(text);//������ �����

            //������ �� ����� ������: ������� ������ �� ����� �� �������, ���� �� �� ����� ������ ��������� �� ���� �����
            if (Score <= Rec) {
                ostringstream RecordString;//��������� ���������� ��� ������ �������
                RecordString << Rec;//������� � �� �������� �� �����(��������� ������)
                text.setString("Record:" + RecordString.str());//����� ������ ������ � �������� ������ ������� .str()
                text.setFillColor(Color::Green);//����� ���� ������
                text.setPosition(515, 550);//����� ������� ������
                window.draw(text);//������ �����
            }
            else {
                ostringstream RecordString;//��������� ���������� ��� ������ �������
                RecordString << Score;//������� � �� ����� ��������� �� ���� �����(��������� ������)
                text.setString("New Record:" + RecordString.str());//����� ������ ������ � �������� ������ ������� .str()
                text.setFillColor(Color::Green);//����� ���� ������
                text.setPosition(515, 550);//����� ������� ������
                window.draw(text);//������ �����
            }

            //���� �� ��������, �� ������ �������� ������� youwin
            if (Score == W) {
                window.draw(youwin);
                RecF1.open("Record.txt");//��������� ���� ��� ������ �������
                RecF1 << Score;
                RecF1.close();
            }
            //���� �� ���������, �� ������ �������� ������� gameover
            if (!game) {
                window.draw(gameover);
                if (Rec < Score) {//���� ��������� ���� ������ �������, �� ������ ���������������
                    RecF1.open("Record.txt");
                    RecF1 << Score;
                    RecF1.close();
                }
            }

            window.display();

            //���� � ��� ��������� ��� ������, ����� �� ��������
            if (!game || Score == W) {
                break;
            }

            //��������� �����
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {//���� ������ enter
                Pause = true;
                while (Pause == true) {//���� � ��� �����, ������ ����� pause ������� ������
                    text.setString("Pause");
                    text.setPosition(325, 550);
                    text.setFillColor(Color::Red);
                    window.draw(text);
                    window.display();
                    if (Keyboard::isKeyPressed(Keyboard::Escape)) {//����� �� ��������� � ����� ����� ������� esc
                        window.close();
                        return 0;
                    }
                    if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {//������������ � ����
                        Pause = false;
                    }
                }
            }
        }
    }
}