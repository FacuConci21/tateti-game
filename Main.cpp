#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <array>
#include <vector>

struct SCursor {
    short quadrant;
    short x;
    short y;
};

struct SQuadrant {
    short id;
    short player_id;
};

const HANDLE H_CONSOLE = GetStdHandle(STD_OUTPUT_HANDLE);
const short GRWIDTH = 15;
const short GRHEIGHT = 15;
short player;
std::vector<short> ocuped_quadrants;
std::array<SQuadrant, 9> quadrants_list;

void GoToXY(int, int);

void Restart();

void LoadQuadrants();

void GameRect();

void GameDrawCrosses();

void GameNameCrosses();

void SetGameQuadrantPosition(SCursor*);

bool IsOcuped(short);

short WinnerVerify();

void EndGameMessage(short);

short rows();

short columns();

short diagonals();

using namespace std;

int main()
{
    player = 1;
    SCursor sCursor = { 1,8,8 };
    ocuped_quadrants = {};
    short winner = 0;
    bool inGame = true;
    POINTS cursor = { 30,30 };
    SQuadrant sq;
    LoadQuadrants();

    /*      DRAWING THE STAGE       */
    system("cls");

    GameRect();

    GameNameCrosses();

    GameDrawCrosses();

    //      GAME LOOP
    while (inGame)
    {
        GoToXY(0, 0); cout << "turn player " << player << endl;
        SetGameQuadrantPosition(&sCursor);
        
        if (sCursor.quadrant < 1) sCursor.quadrant = 1;
        if (sCursor.quadrant > 9) sCursor.quadrant = 9;

        GoToXY(sCursor.x, sCursor.y);

        switch (_getch())
        {
        case 13:
            if (ocuped_quadrants.size() < 9)
            {
                if (player == 1) {
                    cout << "X";
                    quadrants_list[sCursor.quadrant - 1].player_id = 1;
                    player = 2;
                }
                else {
                    cout << 'O';
                    quadrants_list[sCursor.quadrant - 1].player_id = 2;
                    player = 1;
                }
                ocuped_quadrants.push_back(sCursor.quadrant);
            }   
            else { inGame = false; }

            sCursor.quadrant++;
            break;

        case 27:
            inGame = false;
            break;
        case 244:
            break;
        case 72: // up arrow key
            sCursor.quadrant -= 3;
            break;
        case 80: // down arrow key
            sCursor.quadrant += 3;
            break;
        case 75: // left arrow key
            sCursor.quadrant--;
            break;
        case 77: // right arrow key
            sCursor.quadrant++;
            break;
        default:
            break;
        }

        winner = WinnerVerify();
        if (winner > 0) inGame = false;

    }

    EndGameMessage(winner);

    Restart();
    
    //system("pause>o");
    return 0;
}

void Restart()
{
    GoToXY(0, 30);
    cout << "Restart? (y/n)";
    const char ans = _getch();

    if (ans == 'N' || ans == 'n')
    {
        return;
    }
    else if (ans == 'y' || ans == 'Y')
    {
        main();
    }
    else
    {
        Restart();
    }
}

void GoToXY(int _x, int _y)
{
    COORD dwPosition;

    dwPosition.X = _x;
    dwPosition.Y = _y;

    SetConsoleCursorPosition(H_CONSOLE, dwPosition);
}

void LoadQuadrants()
{
    for (short i = 0; i < 9; i++)
    {
        SQuadrant q = { i + 1, NULL };
        quadrants_list[i] = q;
    }
}

void GameRect()
{
    short x0 = 5, y0 = 5;

    for (short i = 0; i < GRWIDTH; i++) // top line
    {
        GoToXY(x0 + i, y0 ); cout << (char)219;
    }
    cout << endl;

    for (short i = 1; i < GRHEIGHT - 1; i++) // parallels segments
    {
        GoToXY(x0, y0 + i); cout << (char)219;
        
        GoToXY(x0 + GRWIDTH -1, y0 + i); cout << (char)219;    
    }
    cout << endl;

    for (short i = 0; i < GRWIDTH; i++) // bot line
    {
        GoToXY(x0 + i, GRHEIGHT + y0 -1); cout << (char)219;
    }
    cout << endl;
}

void GameNameCrosses()
{
    POINTS lineA = { 10, 4 }, lineB = { 14, 4 }, lineC = { 4, 10 }, lineD = { 4, 14 };

    GoToXY(lineA.x, lineA.y); cout << 'A';
    GoToXY(lineB.x, lineB.y); cout << 'B';
    GoToXY(lineC.x, lineC.y); cout << 'C';
    GoToXY(lineD.x, lineD.y); cout << 'D';
}

void GameDrawCrosses()
{
    POINTS lineA = { 5, 0 }, lineB = { 9, 0 }, lineC = { 0, 5 }, lineD = { 0, 9 };
    short x0 = 5, y0 = 5;

    for (short i = 2;  i < GRHEIGHT-2;  i++)
    {
        GoToXY(x0 + lineA.x, y0 + i); cout << (char)219;
        GoToXY(x0 + lineB.x, y0 + i); cout << (char)219;
    }

    for (short i = 2; i < GRWIDTH - 2; i++)
    {
        GoToXY(x0 + i, y0 + lineC.y); cout << (char)219;
        GoToXY(x0 + i, y0 + lineD.y); cout << (char)219;
    }
}

bool IsOcuped(short next_cursor_pos)
{
    for (size_t i = 0; i < ocuped_quadrants.size(); i++)
    {
        if (next_cursor_pos == ocuped_quadrants[i]) return true;
    }
    return false;
}

void SetGameQuadrantPosition(SCursor* game_cursor)
{
    if (IsOcuped(game_cursor->quadrant)) return;

    switch (game_cursor->quadrant)
    {
    case 1:
        game_cursor->x = 8;
        game_cursor->y = 8;
        break;
    case 2:
        game_cursor->x = 12;
        game_cursor->y = 8;
        break;
    case 3:
        game_cursor->x = 16;
        game_cursor->y = 8;
        break;
    case 4:
        game_cursor->x = 8;
        game_cursor->y = 12;
        break;
    case 5:
        game_cursor->x = 12;
        game_cursor->y = 12;
        break;
    case 6:
        game_cursor->x = 16;
        game_cursor->y = 12;
        break;
    case 7:
        game_cursor->x = 8;
        game_cursor->y = 16;
        break;
    case 8:
        game_cursor->x = 12;
        game_cursor->y = 16;
        break;
    case 9:
        game_cursor->x = 16;
        game_cursor->y = 16;
        break;
    default:
        break;
    }
}

short WinnerVerify()
{
    short diagonal_winner = diagonals();
    short row_winner = rows();
    short column_winner = columns();

    if (diagonal_winner > 0)
    {
        return diagonal_winner;
    }
    else if (row_winner > 0)
    {
        return row_winner;
    }
    else if (column_winner)
    {
        return column_winner;
    }
    else
    {
        return 0;
    }
}

void EndGameMessage(short winner)
{
    GoToXY(7, 23);
    cout << "GAME OVER" << endl;

    GoToXY(7, 24);
    (winner != 0) ?
        cout << "WINNER: player " << winner << endl :
        cout << " NO WINNER " << endl;
    
}

short rows()
{
    for (size_t f = 0; f < 7; f += 3)
    {
        if (quadrants_list[f].player_id != NULL)
        {
            if (quadrants_list[f].player_id == quadrants_list[f + 1].player_id &&
                quadrants_list[f].player_id == quadrants_list[f + 2].player_id)
            {
                return quadrants_list[f].player_id;
            }
        }
        else
        {
            continue;
        }
    }

    return 0;
}

short columns()
{
    for (size_t c = 0; c < 3; c++)
    {
        if (quadrants_list[c].player_id != NULL)
        {
            if (quadrants_list[c].player_id == quadrants_list[c + 3].player_id &&
                quadrants_list[c].player_id == quadrants_list[c + 6].player_id)
            {
                return quadrants_list[c].player_id;;
            }
        }
        else
        {
            continue;
        }
    }

    return 0;
}

short diagonals()
{
    if (quadrants_list[4].player_id == NULL) return false;

    if (quadrants_list[0].player_id == quadrants_list[4].player_id &&
        quadrants_list[0].player_id == quadrants_list[8].player_id)
    {
        return quadrants_list[0].player_id;
    }

    if (quadrants_list[2].player_id == quadrants_list[4].player_id &&
        quadrants_list[2].player_id == quadrants_list[6].player_id)
    {
        return quadrants_list[2].player_id;
    }

    return 0;
}
