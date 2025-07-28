#include "iGraphics.h"
#include <math.h>
#include <queue>
#include <string>
#define MAX_BALLS 78
#define MAX_QUEUE_SIZE 100
int level = 1;
int State = 0;
bool soundOn = true;
int volumeLevel = 5;
int unlockedLevel = 1;
bool page = true;

int scores[10]; // Array to store the top 10 scores

int score = 0;
int bg_sound = -1;
#define MAX_ROW 10
#define MAX_COLUMN 20
int row, column;
#include <isound.h>
using namespace std;
#include <iostream>
char username[100] = ""; // Stores the username
int charIndex = 0;       // Tracks the current index of the entered characters
bool isTyping = false;
int inputBoxX = 300;
int inputBoxY = 350;
int inputBoxWidth = 200;
int inputBoxHeight = 30;
char names[10][40];
void startingpage()
{
    page = false;
}
struct Ball
{
    int x;
    int y;
    char color;
    bool visited;
};
Ball ball_position[78];
Ball sending_ball_position[75];
int sent_ball = 1;
int ball_radius = 30;
bool sticked_ball = false;
#define MAX_PATH_POINTS 50
int game_start = 0;
int num = 0;
struct Path
{
    int x;
    int y;
};
Path ball_path[MAX_PATH_POINTS];

int targetX = 400, targetY = 75;
int speed = 25;
bool ballMoving = false;

char select_random_color()
{
    int random_value = rand() % 3;
    if (random_value == 0)
    {
        return 'R';
    }
    else if (random_value == 1)
    {
        return 'G';
    }
    else
    {
        return 'B';
    }
}
void MainMenu()
{
    iSetTransparentColor(255, 255, 0, 0.7);
    iFilledRectangle(220, 448, 300, 35);
    iSetColor(0, 0, 0);
    iTextBold(300, 460, "START GAME", GLUT_BITMAP_HELVETICA_18);
    iSetTransparentColor(255, 255, 0, 0.7);
    iFilledRectangle(220, 408, 300, 35);
    iSetColor(0, 0, 0);
    iTextBold(300, 420, "LEADERBOARD", GLUT_BITMAP_HELVETICA_18);
    iSetTransparentColor(255, 255, 0, 0.7);
    iFilledRectangle(220, 368, 300, 35);
    iSetColor(0, 0, 0);
    iTextBold(300, 380, "HELP", GLUT_BITMAP_HELVETICA_18);
    iSetTransparentColor(255, 255, 0, 0.7);
    iFilledRectangle(220, 328, 300, 35);
    iSetColor(0, 0, 0);
    iTextBold(300, 340, "SOUND", GLUT_BITMAP_HELVETICA_18);
    iSetTransparentColor(255, 255, 0, 0.7);
    iFilledRectangle(220, 288, 300, 35);
    iSetColor(0, 0, 0);
    iTextBold(300, 300, "EXIT GAME", GLUT_BITMAP_HELVETICA_18);
}
void startgame()
{
    game_start = 1;
}
void resumegame()
{
    iSetColor(255, 255, 0);
    iText(70, 450, "RESUME GAME-PRESS 'b' TO GO BACK", GLUT_BITMAP_HELVETICA_18);
}
void Settings()
{
    iSetColor(255, 255, 0);
    iText(30, 420, "SETTINGS-PRESS 'b' TO GO BACK", GLUT_BITMAP_HELVETICA_18);
}
void load_leaderboard()
{
    FILE *file = fopen("leaderboard.txt", "r");
    if (file == NULL)
    {
        return; // Exit if the file doesn't exist
    }

    int i = 0;
    // Reading name and score pairs from the file
    while (fscanf(file, "%s %d", names[i], &scores[i]) != EOF && i < 10)
    {
        i++;
    }
    fclose(file);

    // Sort the scores in descending order, and also sort names accordingly
    for (int i = 0; i < 9; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (scores[i] < scores[j])
            {
                // Swap scores
                int temp_score = scores[i];
                scores[i] = scores[j];
                scores[j] = temp_score;

                // Swap names accordingly
                char temp_name[40];
                strcpy(temp_name, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], temp_name);
            }
        }
    }

    // Debugging: Print the loaded and sorted names and scores
    
}

void save_leaderboard()
{
    // Load the existing leaderboard from the file
    load_leaderboard();

    // Add the new score and name to the leaderboard
    scores[9] = score;  // Add the current score at the last position
    strcpy(names[9], username);  // Add the current username at the last position

    // Sort the leaderboard by score in descending order
    for (int i = 0; i < 9; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (scores[i] < scores[j])
            {
                // Swap scores
                int temp_score = scores[i];
                scores[i] = scores[j];
                scores[j] = temp_score;

                // Swap names accordingly
                char temp_name[40];
                strcpy(temp_name, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], temp_name);
            }
        }
    }

    // If there are more than 10 entries, remove the oldest one
    if (scores[9] != 0)
    {
        for (int i = 9; i > 0; i--)
        {
            scores[i] = scores[i - 1];
            strcpy(names[i], names[i - 1]);
        }
    }

    // Open the file to overwrite with the updated leaderboard
    FILE *file = fopen("leaderboard.txt", "w");  // Change 'a' to 'w' to overwrite
    if (file == NULL)
    {
        return;
    }

    // Write the sorted leaderboard back to the file
    for (int i = 0; i < 10; i++)
    {
        fprintf(file, "%s %d\n", names[i], scores[i]);
    }

    fclose(file);
}


bool is_valid_name(const char *name)
{
    // Check if the name is empty
    if (name == NULL || name[0] == '\0')
        return false;

    // Check if the name contains only alphanumeric characters
    for (int i = 0; name[i] != '\0'; i++)
    {
        if (!isalnum(name[i])) // If the character is not alphanumeric, return false
            return false;
    }

    return true; // Name is valid
}

void Leaderboard()
{
    load_leaderboard(); // Load the leaderboard from the file

    // Display the leaderboard
    iSetColor(255, 255, 0);
    iTextBold(220, 530, "LEADERBOARD - PRESS 'b' TO GO BACK", GLUT_BITMAP_HELVETICA_18);
    iSetTransparentColor(255, 255, 0, 0.4);
    iFilledRectangle(150, 130, 450, 380);
    iSetTransparentColor(255, 255, 0, 0.4);
    iFilledRectangle(150, 460, 450, 50);
    iSetTransparentColor(0, 0, 0, 1);
    iTextBold(180, 480, "RANK", GLUT_BITMAP_HELVETICA_18);
    iTextBold(300, 480, "NAME", GLUT_BITMAP_HELVETICA_18);
    iTextBold(500, 480, "SCORE", GLUT_BITMAP_HELVETICA_18);

    int y = 440;

    // Loop through and display top 10 scores with their names
    for (int i = 0; i < 10; i++)
    {
        char rank[10];
        sprintf(rank, "%d.", i + 1);
        iTextBold(180, y, rank, GLUT_BITMAP_HELVETICA_18);
        iTextBold(300, y, names[i], GLUT_BITMAP_HELVETICA_18);
        char scoreStr[10];
        sprintf(scoreStr, "%d", scores[i]);
        iTextBold(500, y, scoreStr, GLUT_BITMAP_HELVETICA_18);
        y = y - 30;
    }
}

void Help()
{
    iSetColor(255, 255, 0);
    iTextBold(30, 460, "HELP-PRESS 'b' TO GO BACK", GLUT_BITMAP_HELVETICA_18);
    iSetTransparentColor(0, 0, 255, 0.3);
    iFilledRectangle(25, 320, 760, 120);
    iSetColor(0, 255, 255);
    iTextBold(30, 420, "USE MOUSE LEFT BUTTON TO FIX THE BALL WHERE IT CAN POP OUT SAME COLORED BALL");
    iTextBold(30, 390, "TRY NOT TO FIX THE MOUSE ON A DIFFIERENT COLORED BALL");
    iTextBold(30, 360, "AVOID MISSING THE SAME COLORED BALL AND ALSO TRY TO FINISH THE GAME TAKING AS LEAST NUMBER OF");
    iTextBold(30, 330, "BALLS AS POSSIBLE");
    iSetColor(255, 255, 0);
     iSetTransparentColor(0, 0, 255, 0.3);
    iFilledRectangle(25, 100, 760, 120);
    iSetColor(0, 255, 255);
    iTextBold(30, 200, "                                                              Game Credit:-",GLUT_BITMAP_HELVETICA_18);
    iTextBold(30, 170, "Developer-1: A.K.M. Ashikul Islamn",GLUT_BITMAP_HELVETICA_18);
    iTextBold(30, 140, "Developer-2: Tonmoy Saha",GLUT_BITMAP_HELVETICA_18);
    iTextBold(30, 110, "Adviser:- Ashrafur Rahman",GLUT_BITMAP_HELVETICA_18);
}
void SaveGame()
{
    iSetColor(255, 255, 0);
    iText(30, 460, "SAVE/LOAD-PRESS 'b' TO GO BACK", GLUT_BITMAP_HELVETICA_18);
    iText(30, 420, "SAVE PROGRESS-[COMING SOON]");
    iText(30, 390, "LOAD LAST PROGRESS-[COMING SOON]");
}
void sound()
{
    iSetColor(255, 255, 0);
    iText(30, 460, "Press Right keyboard button for increasing volume and left keyboard button for decreasing", GLUT_BITMAP_HELVETICA_18);
    iSetTransparentColor(0, 0, 0, 0.8);
    iFilledRectangle(150, 380, 200, 10);
    iSetTransparentColor(0, 0, 0, 0.8);
    iFilledRectangle(150, 380, 0, 10);
    iSetColor(255, 255, 0);
    iLine(150, 385, 350, 385);
    string vol = "MUSIC VOLUME:" + to_string(volumeLevel);
    iText(30, 380, vol.c_str());
    if (soundOn)
    {
        /* code */
        iText(30, 340, "SOUND EFFECTS:ON");
    }
    else
    {
        iText(30, 340, "SOUND EFFECTS:OFF");
    }
}

void exitgame()
{
    exit(0);
}

void base_set_ball()
{

    sending_ball_position[sent_ball].x = 400;
    sending_ball_position[sent_ball].y = 75;
    char selected_color = select_random_color();
    sending_ball_position[sent_ball].color = selected_color;
}
void set_ball()
{
    if (level == 1)
    {

        int x1 = 40, y1 = 670;
        for (int i = 1; i < 14; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x1, y1, a};
            x1 = x1 + ball_radius * 2;
        }
        int x2 = 70, y2 = 617;
        for (int i = 14; i < 26; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x2, y2, a};
            x2 = x2 + ball_radius * 2;
        }
        int x3 = 40, y3 = 564;
        for (int i = 26; i < 39; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x3, y3, a};
            x3 = x3 + ball_radius * 2;
        }
        int x4 = 70, y4 = 511;
        for (int i = 39; i < 51; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x4, y4, a};
            x4 = x4 + ball_radius * 2;
        }
    }
    else if (level == 2)
    {
        int x1 = 40, y1 = 670;
        for (int i = 1; i < 14; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x1, y1, a};
            x1 = x1 + ball_radius * 2;
        }
        int x2 = 70, y2 = 617;
        for (int i = 14; i < 26; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x2, y2, a};
            x2 = x2 + ball_radius * 2;
        }
        int x3 = 40, y3 = 564;
        for (int i = 26; i < 39; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x3, y3, a};
            x3 = x3 + ball_radius * 2;
        }
        int x4 = 70, y4 = 511;
        for (int i = 39; i < 51; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x4, y4, a};
            x4 = x4 + ball_radius * 2;
        }
        int x5 = 40, y5 = 458;
        for (int i = 51; i < 64; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x5, y5, a};
            x5 = x5 + ball_radius * 2;
        }
    }

    else if (level == 3)
    {
        int x1 = 40, y1 = 670;
        for (int i = 1; i < 14; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x1, y1, a};
            x1 = x1 + ball_radius * 2;
        }
        int x2 = 70, y2 = 617;
        for (int i = 14; i < 26; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x2, y2, a};
            x2 = x2 + ball_radius * 2;
        }
        int x3 = 40, y3 = 564;
        for (int i = 26; i < 39; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x3, y3, a};
            x3 = x3 + ball_radius * 2;
        }
        int x4 = 70, y4 = 511;
        for (int i = 39; i < 51; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x4, y4, a};
            x4 = x4 + ball_radius * 2;
        }
        int x5 = 40, y5 = 458;
        for (int i = 51; i < 64; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x5, y5, a};
            x5 = x5 + ball_radius * 2;
        }
        int x6 = 70, y6 = 405;
        for (int i = 64; i < 76; i++)
        {
            char a = select_random_color();
            ball_position[i] = {x6, y6, a};
            x6 = x6 + ball_radius * 2;
        }
    }
}
void check_level_advance()
{
    int counter = 0;  // This will count the number of balls still on the screen

    // Loop through all ball positions
    for (int i = 0; i < 78; i++)  // Maximum balls that can be on screen (or adjust based on your game's logic)
    {
        if (ball_position[i].x >= 0 && ball_position[i].y >= 0) // Ball is still on the screen
        {
            counter++;
        }
    }

    // If no balls are left on the screen, increase the level
    if (counter == 0)
    {
        level++;  // Advance to the next level
        
        // Call set_ball() to set the new balls for the next level
        set_ball();  
    }
   
}


void check_ball_position_and_neighbors(int ball_number, char color)
{

    int line_num = 1, balls_in_line, count = 0, last_ball_in_line = 0;
    if (ball_position[ball_number].visited == true)
    {
    }
    else
    {
        ball_position[ball_number].visited = true;

        // Find the line the ball is in
        while (1)
        {
            balls_in_line = (line_num % 2 == 1) ? 13 : 12;
            last_ball_in_line = count + balls_in_line;

            if (ball_number <= last_ball_in_line)
            {
                break;
            }

            count = last_ball_in_line;
            line_num++;
        }

        int ball_position_in_line = ball_number - count;

        // Check the neighboring balls in the line
        if (ball_position_in_line > 1 && ball_position_in_line < balls_in_line)
        {

            // Check left ball's color
            if (ball_position_in_line - 1 >= 0)
            {
                char left_ball_color = ball_position[ball_number - 1].color;
                if (left_ball_color == color && ball_position[ball_number - 1].visited != true)
                {
                    ball_position[ball_number - 1].x = -1000;
                    ball_position[ball_number - 1].y = -1000;
                    num++;
                    check_ball_position_and_neighbors(ball_number - 1, color);
                }
            }

            // Check right ball's color
            if (ball_position_in_line + 1 < balls_in_line)
            {
                char right_ball_color = ball_position[ball_number + 1].color;
                if (right_ball_color == color && ball_position[ball_number + 1].visited != true)
                {
                    ball_position[ball_number + 1].x = -1000;
                    ball_position[ball_number + 1].y = -1000;
                    num++;
                    check_ball_position_and_neighbors(ball_number + 1, color);
                }
            }
        }
        else if (ball_position_in_line == 1)
        {
            // If the ball is the first in the line, no left ball

            // Check right ball's color
            if (ball_position_in_line + 1 < balls_in_line)
            {
                char right_ball_color = ball_position[ball_number + 1].color;
                if (right_ball_color == color && ball_position[ball_number + 1].visited != true)
                {
                    ball_position[ball_number + 1].x = -1000;
                    ball_position[ball_number + 1].y = -1000;
                    num++;
                    check_ball_position_and_neighbors(ball_number + 1, color);
                }
            }
        }
        else if (ball_position_in_line == balls_in_line)
        {
            // If the ball is the last in the line, no right ball

            // Check left ball's color
            if (ball_position_in_line - 1 >= 0)
            {
                char left_ball_color = ball_position[ball_number - 1].color;
                if (left_ball_color == color && ball_position[ball_number - 1].visited != true)
                {
                    ball_position[ball_number - 1].x = -1000;
                    ball_position[ball_number - 1].y = -1000;
                    num++;
                    check_ball_position_and_neighbors(ball_number - 1, color);
                }
            }
        }

        // Check for balls above and below the current ball
        if (line_num > 1) // If there is a line above
        {
            int ball_above_1 = ball_number - 12;
            int ball_above_2 = ball_number - 13;

            if (ball_above_1 >= 0 && ball_above_1 < 78)
            {
                char above_ball_color_1 = ball_position[ball_above_1].color;
                if (above_ball_color_1 == color && ball_position[ball_above_1].visited != true)
                {
                    ball_position[ball_above_1].x = -1000;
                    ball_position[ball_above_1].y = -1000;
                    num++;
                    check_ball_position_and_neighbors(ball_above_1, color);
                }
            }

            if (ball_above_2 >= 0 && ball_above_2 < 78)
            {
                char above_ball_color_2 = ball_position[ball_above_2].color;
                if (above_ball_color_2 == color && ball_position[ball_above_2].visited != true)
                {
                    ball_position[ball_above_2].x = -1000;
                    ball_position[ball_above_2].y = -1000;
                    num++;
                    check_ball_position_and_neighbors(ball_above_2, color);
                }
            }
        }

        if (line_num < 4) // If there is a line below
        {
            int ball_below_1 = ball_number + 12;
            int ball_below_2 = ball_number + 13;

            if (ball_below_1 >= 0 && ball_below_1 < 78)
            {
                char below_ball_color_1 = ball_position[ball_below_1].color;
                if (below_ball_color_1 == color && ball_position[ball_below_1].visited != true)
                {
                    ball_position[ball_below_1].x = -1000;
                    ball_position[ball_below_1].y = -1000;
                    num++;
                    check_ball_position_and_neighbors(ball_below_1, color);
                }
            }

            if (ball_below_2 >= 0 && ball_below_2 < 78)
            {
                char below_ball_color_2 = ball_position[ball_below_2].color;
                if (below_ball_color_2 == color && ball_position[ball_below_2].visited != true)
                {
                    ball_position[ball_below_2].x = -1000;
                    ball_position[ball_below_2].y = -1000;
                    num++;
                    check_ball_position_and_neighbors(ball_below_2, color);
                }
            }
        }
    }
}

void calculate_ball_path(int mx, int my)
{
    for (int i = 0; i < MAX_PATH_POINTS; i++)
    {
        ball_path[i].x = -1;
        ball_path[i].y = -1;
    }

    int startX = sending_ball_position[sent_ball].x;
    int startY = sending_ball_position[sent_ball].y;

    int steps = MAX_PATH_POINTS;
    int dx = mx - startX;
    int dy = my - startY;

    for (int i = 0; i < steps; i++)
    {
        ball_path[i].x = startX + (dx * i) / steps;
        ball_path[i].y = startY + (dy * i) / steps;
    }
}
void finish_game()
{
    // Check if the sending ball has reached the base (y <= 90)
    if (sending_ball_position[sent_ball].y <= 150)
    {
        State = 9;
    }
}

void reset_game()
{
    level = 1;
    game_start = 0;
    score = 0; // Reset the score

    ballMoving = false;
    sticked_ball = false;

    sent_ball = 0;
    sending_ball_position[sent_ball].x = 400;
    sending_ball_position[sent_ball].y = 75;

    targetX = 400;
    targetY = 75;

    unlockedLevel = 1;
    volumeLevel = 5;

    // Clear the balls
    for (int i = 0; i < MAX_BALLS; i++)
    {
        ball_position[i].x = 0;
        ball_position[i].y = 0;
        ball_position[i].color = ' ';
        ball_position[i].visited = false; // Reset visited state as well
    }

    set_ball(); // Reinitialize balls
}

double calculate_distance(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void move_ball_towards_target()
{
    int dx = targetX - sending_ball_position[sent_ball].x;
    int dy = targetY - sending_ball_position[sent_ball].y;

    double distance = sqrt(dx * dx + dy * dy);

    if (distance > speed)
    {
        double ratio = speed / distance;
        sending_ball_position[sent_ball].x += dx * ratio;
        sending_ball_position[sent_ball].y += dy * ratio;
    }
    else
    {
        sending_ball_position[sent_ball].x = targetX;
        sending_ball_position[sent_ball].y = targetY;
    }

    int ballCenterX = sending_ball_position[sent_ball].x;
    int ballCenterY = sending_ball_position[sent_ball].y;

    if (ballCenterX + ball_radius > iScreenWidth)
    {
        sending_ball_position[sent_ball].x = iScreenWidth - ball_radius;
        dx = -dx;
    }
    else if (ballCenterX - ball_radius < 0)
    {
        sending_ball_position[sent_ball].x = ball_radius;
        dx = -dx;
    }

    if (ballCenterY + ball_radius > iScreenHeight)
    {

        ballMoving = false;
        sent_ball++;
        sending_ball_position[sent_ball].x = 400;
        sending_ball_position[sent_ball].y = 75;
        sending_ball_position[sent_ball].color = select_random_color();

        return;
    }

    if (sending_ball_position[sent_ball].y <= ball_radius)
    {
        sending_ball_position[sent_ball].y = ball_radius;
        ballMoving = false;
    }

    targetX = sending_ball_position[sent_ball].x + dx;
    targetY = sending_ball_position[sent_ball].y + dy;

    for (int i = 0; i < MAX_BALLS; i++)
    {
        if (i == sent_ball)
            continue;

        int dxBall = sending_ball_position[sent_ball].x - ball_position[i].x;
        int dyBall = sending_ball_position[sent_ball].y - ball_position[i].y;
        double distanceBetweenBalls = sqrt(dxBall * dxBall + dyBall * dyBall);

        if (distanceBetweenBalls <= ball_radius * 2)
        {
            if (sending_ball_position[sent_ball].color == ball_position[i].color)
            {
                num++;

                check_ball_position_and_neighbors(i, ball_position[i].color);

                ball_position[i].x = -1000;
                ball_position[i].y = -1000;

                sending_ball_position[sent_ball].x = -1000;
                sending_ball_position[sent_ball].y = -1000;

                score += num;
                num = 0;
                sent_ball = (sent_ball + 1) % MAX_BALLS;

                sending_ball_position[sent_ball].x = 400;
                sending_ball_position[sent_ball].y = 75;
                sending_ball_position[sent_ball].color = select_random_color();

                ballMoving = false;

                return;
            }
            else
            {
                sending_ball_position[sent_ball].x = ball_position[i].x;
                sending_ball_position[sent_ball].y = ball_position[i].y - 2 * ball_radius;

                ballMoving = false;
                finish_game();
                sent_ball++;
                sending_ball_position[sent_ball].x = 400;
                sending_ball_position[sent_ball].y = 75;
                sending_ball_position[sent_ball].color = select_random_color();

                return;
            }
        }
    }

    for (int i = 0; i < sent_ball; i++)
    {
        int dxBall = sending_ball_position[sent_ball].x - sending_ball_position[i].x;
        int dyBall = sending_ball_position[sent_ball].y - sending_ball_position[i].y;
        double distanceBetweenBalls = sqrt(dxBall * dxBall + dyBall * dyBall);

        if (distanceBetweenBalls <= ball_radius * 2)
        {
            if (sending_ball_position[sent_ball].color == sending_ball_position[i].color)
            {

                sending_ball_position[i].x = -1000;
                sending_ball_position[i].y = -1000;

                sending_ball_position[sent_ball].x = -1000;
                sending_ball_position[sent_ball].y = -1000;

                sent_ball = (sent_ball + 1) % MAX_BALLS;

                sending_ball_position[sent_ball].x = 400;
                sending_ball_position[sent_ball].y = 75;
                sending_ball_position[sent_ball].color = select_random_color();

                ballMoving = false;

                return;
            }
            else
            {
                sending_ball_position[sent_ball].x = sending_ball_position[i].x;
                sending_ball_position[sent_ball].y = sending_ball_position[i].y - 2 * ball_radius;

                ballMoving = false;
                finish_game();
                sent_ball++;
                sending_ball_position[sent_ball].x = 400;
                sending_ball_position[sent_ball].y = 75;
                sending_ball_position[sent_ball].color = select_random_color();

                return;
            }
        }
    }
}

void draw_dotted_line(int startX, int startY, int endX, int endY)
{
    int numDots = 50;
    int dx = endX - startX;
    int dy = endY - startY;
    for (int i = 0; i < numDots; i++)
    {
        int x = startX + (dx * i) / numDots;
        int y = startY + (dy * i) / numDots;
        if (i % 2 == 0)
        {
            if (sending_ball_position[sent_ball].color == 'R')
            {
                iShowImage(x, y, "reddot.png");
            }
            else if (sending_ball_position[sent_ball].color == 'G')
            {
                iShowImage(x, y, "greendot.png");
            }
            else if (sending_ball_position[sent_ball].color == 'B')
            {
                iShowImage(x, y, "bluedot.png");
            }
        }
    }
}

void iDraw()
{
    iClear();
    if (page)
    {
        iShowImage(0, 0, "game.bmp");
        return;
    }
    iShowImage(0, 0, "background.bmp");

    if (State == 0)
        MainMenu();
    else if (State == 2)
        Leaderboard();
    else if (State == 9)
    {
        iClear();
        iShowImage(0, 0, "gameover.png");
    }
    else if (State == 7)
    {

        iSetColor(255, 255, 255);                                                                       // White color for prompt text
        iText(inputBoxX, inputBoxY + inputBoxHeight + 10, "Enter your name", GLUT_BITMAP_HELVETICA_18); // Prompt for username

        // Draw the input box with a border for better visibility
        iSetColor(255, 255, 255);                                              // White color for input box
        iFilledRectangle(inputBoxX, inputBoxY, inputBoxWidth, inputBoxHeight); // Filled box
        iSetColor(0, 0, 0);                                                    // Black border for the input box
        iRectangle(inputBoxX, inputBoxY, inputBoxWidth, inputBoxHeight);       // Border for the input box

        // Draw the entered username inside the input box
        iSetColor(0, 0, 0);                                                       // Black color for the text
        iText(inputBoxX + 10, inputBoxY + 8, username, GLUT_BITMAP_HELVETICA_18); // Display the entered username

        // Show a blinking cursor if user is typing
        if (isTyping)
        {
            // Blink cursor at the end of the entered text (in the input box)
            iSetColor(0, 0, 0); // Black color for the cursor
            iLine(inputBoxX + 10 + (charIndex * 10), inputBoxY + 5, inputBoxX + 10 + (charIndex * 10), inputBoxY + inputBoxHeight - 5);
        }
    }
    else if (State == 1)
    {
        iClear();

        iShowImage(0, 0, "background.jpg");
        iSetTransparentColor(255, 255, 0, 0.7);
        iFilledRectangle(15, 10, 135, 35);
        iSetColor(0, 0, 0);
        iTextBold(30, 20, "press b to go home", GLUT_BITMAP_HELVETICA_12);
        iSetColor(255, 0, 0);
        string score_1 = "Score:" + to_string(score);
        iText(700, 10, score_1.c_str(), GLUT_BITMAP_TIMES_ROMAN_24);

        if (ballMoving)
        {
            move_ball_towards_target();
            check_level_advance();
        }
        for (int i = 0; i <= sent_ball; i++)
        {
            if (sending_ball_position[i].x >= 0 && sending_ball_position[i].y >= 0)
            {
                if (sending_ball_position[i].color == 'R')
                {
                    iSetColor(255, 0, 0);
                    iFilledCircle(sending_ball_position[i].x, sending_ball_position[i].y, ball_radius);
                }
                else if (sending_ball_position[i].color == 'G')
                {
                    iSetColor(0, 255, 0);
                    iFilledCircle(sending_ball_position[i].x, sending_ball_position[i].y, ball_radius);
                }
                else if (sending_ball_position[i].color == 'B')
                {
                    iSetColor(0, 0, 255);
                    iFilledCircle(sending_ball_position[i].x, sending_ball_position[i].y, ball_radius);
                }
            }
        }
        if (!ballMoving && ball_path[0].x != -1 && ball_path[0].y != -1)
        {
            draw_dotted_line(sending_ball_position[sent_ball].x, sending_ball_position[sent_ball].y, ball_path[MAX_PATH_POINTS - 1].x, ball_path[MAX_PATH_POINTS - 1].y);
        }

        for (int i = 1; i < 78; i++)
        {
            if (ball_position[i].x >= 0 && ball_position[i].y >= 0)
            {
                if (ball_position[i].color == 'R')
                {
                    iSetColor(255, 0, 0);
                    iFilledCircle(ball_position[i].x, ball_position[i].y, ball_radius);
                }
                else if (ball_position[i].color == 'G')
                {
                    iSetColor(0, 255, 0);
                    iFilledCircle(ball_position[i].x, ball_position[i].y, ball_radius);
                }
                else if (ball_position[i].color == 'B')
                {
                    iSetColor(0, 0, 255);
                    iFilledCircle(ball_position[i].x, ball_position[i].y, ball_radius);
                }
            }
        }
        startgame();
    }
    else if (State == 3)
        Help();
    else if (State == 4)
        sound();
    else if (State == 5)
        exitgame();
    if (State == 4) // Volume control screen
    {
        int barX = 150;     // X-coordinate of the volume bar
        int barY = 380;     // Y-coordinate of the volume bar
        int barWidth = 200; // Width of the volume bar
        int barHeight = 10; // Height of the volume bar
        int maxVolume = 10; // Maximum volume level

        // Draw the active volume (green)
        iSetColor(8, 227, 19); // Green color for the active volume
        iFilledRectangle(barX, barY, volumeLevel * (barWidth / maxVolume), barHeight);

        // Draw the inactive volume (red)
        iSetColor(227, 9, 27); // Red color for the inactive volume
        iFilledRectangle(barX + volumeLevel * (barWidth / maxVolume), barY,
                         (maxVolume - volumeLevel) * (barWidth / maxVolume), barHeight);

        // Draw the boundary of the volume bar
        iSetColor(0, 0, 0); // Black color for border
        iRectangle(barX, barY, barWidth, barHeight);
    }
}

void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (State == 7)
        {
            if (mx >= inputBoxX && mx <= inputBoxX + inputBoxWidth && my >= inputBoxY && my <= inputBoxY + inputBoxHeight)
            {
                isTyping = true; // Start typing when clicked inside the box
            }
            else
            {
                isTyping = false; // Stop typing if clicked outside the box
            }
        }

        if (game_start == 1)
        {

            targetX = mx;
            targetY = my;
            calculate_ball_path(mx, my);
            ballMoving = true;
        }
    }
    if (page)
        return;
    int adjustedY = my;
    if (State == 9)
    {
        if (mx >= 245 && mx <= 554)
        {
            if (my >= 171 && my < 266)
            {
                State = 0;
            }
        }
    }

    if (State == 0)
    {
        if (State == 0)
        {
            if (mx >= 280 && mx <= 500)
            {
                if (adjustedY >= 460 && adjustedY <= 480)
                    State = 7;
                else if (adjustedY >= 420 && adjustedY <= 440)
                    State = 2;
                else if (adjustedY >= 380 && adjustedY <= 400)
                    State = 3;
                else if (adjustedY >= 340 && adjustedY <= 360)
                    State = 4;
                else if (adjustedY >= 300 && adjustedY <= 320)
                    State = 5;
            }
        }
    }
}

void iMouseMove(int mx, int my)
{
    if (game_start == 1)
    {
        if (!ballMoving)
        {
            calculate_ball_path(mx, my);
        }
    }
}

void iKeyboard(unsigned char key)
{
    if (State == 7)
    { // If the user is typing their name
        if (isTyping)
        {
            if (key == 13)
            { // Enter key
                isTyping = false;

                // Save the username and score to the leaderboard
                // Save the score and leaderboard

                State = 1;
                reset_game();
                base_set_ball();
            }
            else if (key == 8)
            { // Backspace key
                if (charIndex > 0)
                {
                    charIndex--;
                    username[charIndex] = '\0';
                }
            }
            else if (key >= 32 && key <= 126)
            { // Printable character
                if (charIndex < 99)
                {
                    username[charIndex] = key;
                    charIndex++;
                    username[charIndex] = '\0';
                }
            }
        }
    }

    if (key == 'b')
    { // When 'b' is pressed
        // Save the score and update leaderboard before going back to the main menu
        save_leaderboard();
        charIndex = 0;
        username[charIndex] = '\0'; // Clear username
        State = 0;                  // Go back to the main menu
        reset_game();               // Reset the game state
        base_set_ball();            // Set the ball for the next game
    }
}

void iSpecialKeyboard(unsigned char key)
{
    if (State == 4) // Volume control screen
    {

        int maxVolume = 10; // Maximum volume level

        if (key == GLUT_KEY_LEFT) // Left arrow key
        {
            // Decrease volume if not at minimum
            if (volumeLevel > 0)
            {
                volumeLevel--;
            }
            iDecreaseVolume(bg_sound, 10);
            if (volumeLevel < 0)
                volumeLevel = 0;
        }
        else if (key == GLUT_KEY_RIGHT) // Right arrow key
        {
            // Increase volume if not at maximum
            if (volumeLevel < maxVolume)
            {
                volumeLevel++;
            }
            iIncreaseVolume(bg_sound, 10);
            if (volumeLevel > 10)
                volumeLevel = 10;
        }
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv); // Initialize GLUT
    srand(time(NULL));     // Seed random number generator

    // Set the starting page after 5000 ms
    iSetTimer(5000, startingpage);

    // Set up the game ball
    base_set_ball();
    set_ball();

    // Initialize sound
    iInitializeSound();
    bg_sound = iPlaySound("background.wav", true, 50); // Play background music in a loop at 50% volume

    // Initialize the window
    iInitialize(800, 700, "Bouncing Balls");

    // Initialize ball visited status
    for (int i = 0; i < 78; i++)
    {
        ball_position[i].visited = false;
    }
    for (int i = 0; i < 78; i++)
    {
        ball_position[i].x=-1000;
        ball_position[i].y=-1000;        

    }
    

    glutMainLoop(); // Start the main game loop

    return 0;
}

void iMouseDrag(int mx, int my)
{
}

void iMouseWheel(int dir, int mx, int my)
{
}
