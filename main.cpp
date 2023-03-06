#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

class Piece;
void RedWon(RenderWindow& window);
void WhiteWon(RenderWindow& window);
void DrawTiles(RenderWindow& window);
void RedPiece(RenderWindow& window);
void WhitePiece(RenderWindow& window);
void Highlight(RenderWindow& window, int x, int y);
void countRed(RenderWindow& window, Piece* RedPieces);
void countWhite(RenderWindow& window, Piece* WhitePieces);
Piece* FindPiece(int x, int y, Piece* RedPieces, Piece* WhitePieces);
void Setup(RenderWindow& window, Piece* RedPieces, Piece* WhitePieces);
int MovePiece(int x, int y, Piece* s_Piece, Piece* RedPieces, Piece* WhitePieces, int* turn);

class Piece {
public:
    int x;
    int y;
    float radius = 30;
    bool isAlive;
    bool isKing = false;
    Color color;

    void Draw(RenderWindow& window)
    {
        if (isAlive)
        {
            CircleShape shape(radius);
            shape.setFillColor(color);
            if (isKing)
            {
                shape.setOutlineThickness(3.f);
                shape.setOutlineColor(Color::Yellow);
            }
            shape.setPosition(Vector2f(x * 75 + (75 - radius * 2) / 2, y * 75 + (75 - 2 * radius) / 2));
            window.draw(shape);
        }
    }
};

int main()
{
    RenderWindow window(VideoMode(900, 600, 32), "Checkers", Style::Default);
    Piece RedPieces[12];
    Piece WhitePieces[12];
    Piece* SelectedPiece;
    bool selected = false;
    int turn = 1;

    Clock clock;
    Time time;
    stringstream ss;

    int m = 0;
    int s = 0;
    int count = 0;

    Font fnt;
    fnt.loadFromFile("BUTTERSHINE SERIF.otf");

    Text txt("", fnt);
    txt.setCharacterSize(60);

    Setup(window, RedPieces, WhitePieces);

    for (int i = 0;i < 12;i++)
    {
        WhitePieces[i].color = Color::White;
        RedPieces[i].color = Color::Red;
    }

    while (window.isOpen())
    {
        Event evnt;
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::MouseButtonReleased:
                if (evnt.mouseButton.button == Mouse::Left)
                {
                    selected = !selected;
                }
            }
        }

        for (int i = 0; i < 12; i++)
        {
            if (RedPieces[i].y == 0)
            {
                RedPieces[i].isKing = true;
            }
            if (WhitePieces[i].y == 7)
            {
                WhitePieces[i].isKing = true;
            }
        }

        if (selected)
        {
            int x = Mouse::getPosition(window).x / 75;
            int y = Mouse::getPosition(window).y / 75;
            if (FindPiece(x, y, RedPieces, WhitePieces) && (FindPiece(x, y, RedPieces, WhitePieces)->color == Color::Red && turn == 1 || FindPiece(x, y, RedPieces, WhitePieces)->color == Color::White && turn == 2))
            {
                if (FindPiece(x, y, RedPieces, WhitePieces) == SelectedPiece)
                {
                    SelectedPiece = NULL;
                }
                else
                {
                    SelectedPiece = FindPiece(x, y, RedPieces, WhitePieces);
                }
                selected = false;
            }
            else if (SelectedPiece != NULL && MovePiece(x, y, SelectedPiece, RedPieces, WhitePieces, &turn))
            {
                selected = false;
                SelectedPiece = NULL;
            }
            selected = false;
        }

        window.clear();

        ss.str("");

        time = clock.getElapsedTime();
        s = time.asSeconds();
        m = s / 60;
        s = s - (m * 60);

        ss << "Time: ";

        if (m < 10)
        {
            ss << " 0" << m;
        }
        else
        {
            ss << " : " << m;
        }
        if (s < 10)
        {
            ss << " : " << " 0 " << s;
        }
        else
        {
            ss << ":" << s;
        }

        DrawTiles(window);

        WhitePiece(window);

        RedPiece(window);

        countRed(window, RedPieces);
        countWhite(window, WhitePieces);

        if (SelectedPiece != NULL)
        {
            Highlight(window, SelectedPiece->x, SelectedPiece->y);
        }

        for (int i = 0;i < 12;i++)
        {
            WhitePieces[i].Draw(window);
            RedPieces[i].Draw(window);
        }

        for (int i = 0; i < 12; i++)
        {
            if (RedPieces[i].isAlive == true)
                break;
            if (i == 11)
            {
                txt.setPosition(300, 250);
                txt.setFillColor(Color::White);
                WhiteWon(window);

                if (count != 1)
                {
                    txt.setString(ss.str());
                    count++;
                }
                window.draw(txt);
            }
        }

        for (int i = 0; i < 12; i++)
        {
            if (WhitePieces[i].isAlive == true)
                break;
            if (i == 11)
            {
                txt.setPosition(300, 250);
                txt.setFillColor(Color::Red);
                RedWon(window);

                if (count != 1)
                {
                    txt.setString(ss.str());
                    count++;
                }
                window.draw(txt);
            }
        }

        window.display();
    }

    return 0;
}

struct name
{
    int size;
};

void DrawTiles(RenderWindow& window)
{
    name TileSize;
    TileSize.size = 8;
    RectangleShape tile;
    tile.setSize(Vector2f(75.f, 75.f));
    for (int i = 0;i < TileSize.size;i++)
    {
        for (int j = 0;j < TileSize.size;j++)
        {
            tile.setPosition(Vector2f(75 * i, 75 * j));
            if ((i + j) % 2 == 0)
            {
                tile.setFillColor(Color::White);
            }
            else
                tile.setFillColor(Color::Black);

            window.draw(tile);
        }
    }
}

void Highlight(RenderWindow& window, int x, int y)
{
    RectangleShape tile;
    tile.setSize(Vector2f(75.f, 75.f));
    tile.setFillColor(Color::Green);
    tile.setPosition(Vector2f(75 * x, 75 * y));
    window.draw(tile);
}

void Setup(RenderWindow& window, Piece* RedPieces, Piece* WhitePieces)
{
    int m = 0;
    for (int i = 0;i < 3;i++)
    {
        for (int j = (int)!(i % 2 & 1);j < 8;j += 2)
        {
            WhitePieces[m].isAlive = true;
            WhitePieces[m].x = j;
            WhitePieces[m].y = i;
            m++;
        }
    }

    m = 0;
    for (int i = 0;i < 3;i++)
    {
        for (int j = (int)(i % 2 & 1);j < 8;j += 2)
        {
            RedPieces[m].isAlive = true;
            RedPieces[m].x = j;
            RedPieces[m].y = 7 - i;
            m++;
        }
    }
}

Piece* FindPiece(int x, int y, Piece* RedPieces, Piece* WhitePieces)
{
    for (int i = 0; i < 12;i++)
    {
        if (RedPieces[i].x == x && RedPieces[i].y == y)
        {
            if (RedPieces[i].isAlive)
            {
                return &RedPieces[i];
            }
        }
        if (WhitePieces[i].x == x && WhitePieces[i].y == y)
        {
            if (WhitePieces[i].isAlive)
            {
                return &WhitePieces[i];
            }
        }
    }
    return NULL;
}

void KillPiece(int x, int y, Piece* RedPieces, Piece* WhitePieces, int* turn)
{
    FindPiece(x, y, RedPieces, WhitePieces)->isAlive = false;
}

int MovePiece(int x, int y, Piece* s_Piece, Piece* RedPieces, Piece* WhitePieces, int* turn) {
    if (s_Piece->color == Color::Red || s_Piece->color == Color::White && s_Piece->isKing)
    {
        if (x == s_Piece->x - 1 && y == s_Piece->y - 1)
        {
            if (!FindPiece(x, y, RedPieces, WhitePieces))
            {
                *turn = ((*turn == 1) ? 2 : 1);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        if (x == s_Piece->x + 1 && y == s_Piece->y - 1)
        {
            if (!FindPiece(x, y, RedPieces, WhitePieces))
            {
                *turn = ((*turn == 1) ? 2 : 1);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        if (x == s_Piece->x - 2 && y == s_Piece->y - 2)
        {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x + 1, y + 1, RedPieces, WhitePieces) != NULL && FindPiece(x + 1, y + 1, RedPieces, WhitePieces)->color != s_Piece->color)
            {
                KillPiece(x + 1, y + 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y - 2)
        {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x - 1, y + 1, RedPieces, WhitePieces) != NULL && FindPiece(x - 1, y + 1, RedPieces, WhitePieces)->color != s_Piece->color)
            {
                KillPiece(x - 1, y + 1, RedPieces, WhitePieces, turn);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
    }
    if (s_Piece->color == Color::White || s_Piece->color == Color::Red && s_Piece->isKing)
    {
        if (x == s_Piece->x - 1 && y == s_Piece->y + 1)
        {
            if (!FindPiece(x, y, RedPieces, WhitePieces))
            {
                *turn = ((*turn == 1) ? 2 : 1);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }
        }
        if (x == s_Piece->x + 1 && y == s_Piece->y + 1)
        {
            if (!FindPiece(x, y, RedPieces, WhitePieces))
            {
                *turn = ((*turn == 1) ? 2 : 1);
                s_Piece->x = x;
                s_Piece->y = y;
                return 1;
            }

        }
        if (x == s_Piece->x - 2 && y == s_Piece->y + 2)
        {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x + 1, y - 1, RedPieces, WhitePieces) != NULL && FindPiece(x + 1, y - 1, RedPieces, WhitePieces)->color != s_Piece->color)
            {
                s_Piece->x = x;
                s_Piece->y = y;
                KillPiece(x + 1, y - 1, RedPieces, WhitePieces, turn);
                return 1;
            }
        }
        if (x == s_Piece->x + 2 && y == s_Piece->y + 2)
        {
            if (!FindPiece(x, y, RedPieces, WhitePieces) && FindPiece(x - 1, y - 1, RedPieces, WhitePieces) != NULL && FindPiece(x - 1, y - 1, RedPieces, WhitePieces)->color != s_Piece->color)
            {
                s_Piece->x = x;
                s_Piece->y = y;
                KillPiece(x - 1, y - 1, RedPieces, WhitePieces, turn);
                return 1;
            }
        }
    }
    return 0;
}

void WhitePiece(RenderWindow& window)
{
    Font font;
    font.loadFromFile("BUTTERSHINE SERIF.otf");

    Text text;
    text.setPosition(650, 50);
    text.setFont(font);
    text.setFillColor(Color::White);
    text.setCharacterSize(20);
    text.setStyle(Text::Bold);
    text.setString("White Pieces out: ");

    window.draw(text);
}

void RedPiece(RenderWindow& window)
{
    Font font;
    font.loadFromFile("BUTTERSHINE SERIF.otf");

    Text text;
    text.setPosition(650, 400);
    text.setFont(font);
    text.setFillColor(Color::White);
    text.setCharacterSize(20);
    text.setStyle(Text::Bold);
    text.setString("Red Pieces out: ");

    window.draw(text);
}

void countRed(RenderWindow& window, Piece* RedPieces)
{
    Font font;
    font.loadFromFile("BUTTERSHINE SERIF.otf");

    Text text;
    text.setFont(font);
    text.setFillColor(Color::White);
    text.setPosition(720, 430);
    text.setCharacterSize(20);
    text.setStyle(Text::Bold);

    int count1 = 0;
    for (int i = 0; i < 12; i++)
    {
        if (RedPieces[i].isAlive == false)
            count1++;
    }
    stringstream ss;
    ss << count1;

    text.setString(ss.str().c_str());
    window.draw(text);
}

void countWhite(RenderWindow& window, Piece* WhitePieces)
{
    Font font;
    font.loadFromFile("BUTTERSHINE SERIF.otf");

    Text text;
    text.setFont(font);
    text.setFillColor(Color::White);
    text.setPosition(740, 80);
    text.setCharacterSize(20);
    text.setStyle(Text::Bold);

    int count = 0;
    for (int i = 0; i < 12; i++)
    {
        if (WhitePieces[i].isAlive == false)
            count++;
    }
    stringstream ss;
    ss << count;

    text.setString(ss.str().c_str());
    window.draw(text);
}

void RedWon(RenderWindow& window)
{
    Texture background;
    background.loadFromFile("image.jpg");
    Sprite sprite(background);


    Font font;
    font.loadFromFile("BUTTERSHINE SERIF.otf");
    Text text;
    text.setFont(font);
    text.setPosition(200, 150);
    text.setFillColor(Color::Red);
    text.setCharacterSize(60);
    text.setStyle(Text::Bold);
    text.setString("Red Player Won!!");

    window.draw(sprite);
    window.draw(text);
}

void WhiteWon(RenderWindow& window)
{
    Texture background;
    background.loadFromFile("image.jpg");
    Sprite sprite(background);

    Font font;
    font.loadFromFile("BUTTERSHINE SERIF.otf");
    Text text;

    text.setFont(font);
    text.setPosition(150, 150);
    text.setFillColor(Color::White);
    text.setCharacterSize(60);
    text.setStyle(Text::Bold);
    text.setString("White Player Won!!");

    window.draw(sprite);
    window.draw(text);
}