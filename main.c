#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


#define MAX_LENGTH 20

void reduce_rating(const char *username, int reduction);
void clear_screen();
int kbhit(void);
void flush_input() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
void give_badge(const char *username, int badge_id) {
  FILE *fptr = fopen("user.txt", "r");
  if (!fptr) {
    perror("Error opening user.txt");
    return;
  }

  char line[MAX_LENGTH];
  int found = 0;
  int line_number = 0;
  int user_line = -1;

  while (fgets(line, sizeof(line), fptr)) {
    line[strcspn(line, "\n")] = 0;
    if (strcmp(line, username) == 0) {
      found = 1;
      user_line = line_number;
      break;
    }
    line_number++;
  }

  fclose(fptr);

  if (found) {
    fptr = fopen("badges.txt", "r+");
    if (!fptr) {
      perror("Error opening badges.txt");
      return;
    }

    for (int i = 0; i <= user_line; i++) {
      fgets(line, sizeof(line), fptr);
    }

    int badges = atoi(line);
    if (!(badges & (1 << (badge_id - 1)))) {
      badges |= (1 << (badge_id - 1));

      fseek(fptr, 0, SEEK_SET);
      for (int i = 0; i < user_line; i++) {
        fgets(line, sizeof(line), fptr);
      }

      fprintf(fptr, "%d\n", badges);
    }

    fclose(fptr);
  } else {
    printf("User not found\n");
  }
}
int main(void) {
 
  printf("Hello, this is Fir - The Game. You can play Fir with various "
         "modifiers.\nThis is Version 0.9.0 (The Game Update)\n");
  /*
  Version 0.9.0
  This Game Is Called Fir - The Game Update
  This supports blindfold mode and we created few badges, and few minor rule
  settings To modify minor rule settings, update settings.txt

  settings.txt contains minor settings.
  games.txt contains the ratings.
  user.txt contains all users.

  Try giving privelleges to the files, so that you can modify them.
  this Game is made by Imagment Studios and feel free to remix or fork.
  */

  while (1) {
    int command;
  here:
    // clear screen

    sleep(5);
    clear_screen();
    printf("\n[1] Register a new user\n[2] Play\n[3] See Records\n[4] "
           "Exit\n[5] Help\n-----------------\n[6] Badges\n\n");
    printf("Enter a command: ");
    if (scanf("%d", &command) != 1) {
      printf("Invalid input. Please enter a number.\n");
      flush_input();
      continue;
    }
    flush_input();

    if (command == 1) {
      char username[MAX_LENGTH];
      printf("\n\nEnter a username: ");
      if (scanf("%s", username) != 1) {
        printf("Invalid input.\n");
        flush_input();
        continue;
      }

      FILE *fptr = fopen("user.txt", "r");
      if (!fptr) {
        perror("Error opening user.txt");
        continue;
      }

      char line[MAX_LENGTH];
      int found = 0;

      while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, username) == 0) {
          found = 1;
          break;
        }
      }

      fclose(fptr);

      if (found) {
        printf("Username already exists!\n");
      } else {
        printf("Welcome, %s!\n", username);
        fptr = fopen("user.txt", "a");
        if (!fptr) {
          perror("Error opening user.txt for writing");
          continue;
        }
        fprintf(fptr, "%s\n", username);
        fclose(fptr);

        fptr = fopen("games.txt", "a");
        if (!fptr) {
          perror("Error opening games.txt for writing");
          continue;
        }
        fprintf(fptr, "10\n");
        fclose(fptr);

        // open badges.txt and write 0
        fptr = fopen("badges.txt", "a");
        if (!fptr) {
          perror("Error opening badges.txt for writing");
        }
        fprintf(fptr, "0\n");
        fclose(fptr);
      }
    } else if (command == 4) {
      printf("\n\nGoodbye!\n");
      exit(0);
    } else if (command == 3) {
      char username[MAX_LENGTH];
      printf("\n\nInput Username: ");
      if (scanf("%s", username) != 1) {
        printf("Invalid input.\n");
        flush_input();
        continue;
      }

      FILE *fptr = fopen("user.txt", "r");
      if (!fptr) {
        perror("Error opening user.txt");
        continue;
      }

      char line[MAX_LENGTH];
      int found = 0;
      int line_number = 0;
      int user_line = -1;

      while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, username) == 0) {
          found = 1;
          user_line = line_number;
          break;
        }
        line_number++;
      }

      fclose(fptr);

      if (found) {
        fptr = fopen("games.txt", "r");
        if (!fptr) {
          perror("Error opening games.txt");
          continue;
        }

        for (int i = 0; i <= user_line; i++) {
          fgets(line, sizeof(line), fptr);
        }

        printf("\nFir Rating Of %s: %s\n", username, line);
        int fir = atoi(line);
        if (fir <= 4)
          printf("%s is Unranked\n", username);
        else if (fir >= 5 && fir <= 9)
          printf("%s is Bronze\n", username);
        else if (fir >= 10 && fir <= 30)
          printf("%s is Silver\n", username);
        else if (fir >= 31 && fir <= 50)
          printf("%s is Gold\n", username);
        else if (fir >= 51 && fir <= 100)
          printf("%s is Platinum\n", username);
        else if (fir >= 101 && fir <= 500)
          printf("%s is Diamond\n", username);
        else if (fir >= 501 && fir <= 1000)
          printf("%s is Master\n", username);
        else
          printf("%s is Grand Master\n", username);
        fclose(fptr);
      } else {
        printf("User not found\n");
      }
    } else if (command == 2) {
      printf("\nYou are trying to play with a human.\nPlease customize your "
             "game.\n");

      int size;
      do {
        printf("Select the size of the board (9~40): ");
        if (scanf("%d", &size) != 1) {
          printf("Invalid input. Please enter a number.\n");
          flush_input();
          continue;
        }
        flush_input();
        if (size < 9 || size > 40) {
          printf("Invalid input. Please try again.\n");
        }
      } while (size < 9 || size > 40);
      size--;

      int player;
      do {
        printf("Select the number of players (2~5): ");
        if (scanf("%d", &player) != 1) {
          printf("Invalid input. Please enter a number.\n");
          flush_input();
          continue;
        }
        flush_input();
        if (player < 2 || player > 5) {
          printf("Invalid input. Please try again.\n");
        }
      } while (player < 2 || player > 5);

      int time;
      do {
        printf("Choose your time in seconds (300~1200): ");
        if (scanf("%d", &time) != 1) {
          printf("Invalid input. Please enter a number.\n");
          flush_input();
          continue;
        }
        flush_input();
        if (time < 300 || time > 1200) {
          printf("Invalid input. Please try again.\n");
        }
      } while (time < 300 || time > 1200);

      int player_clocks[player];
      for (int i = 0; i < player; i++) {
        player_clocks[i] = time;
      }

      char player_name[5][20] = {""};
      for (int i = 0; i < player; i++) {
        printf("Enter the name of player %d: ", i + 1);
        if (scanf("%s", player_name[i]) != 1) {
          printf("Invalid input.\n");
          flush_input();
          continue;
        }
        for (int j = 0; j < i; j++) {
          if (strcmp(player_name[i], player_name[j]) == 0) {
            printf("Username already exists! Please try again.\n");
            i--;
            break;
          }
        }
      }
      int gamemode = 0;
      do {
        printf("Choose your gamemode\n[1]: Normal [2]: Blindfold\n");
        if (scanf("%d", &gamemode) != 1) {
          printf("Invalid input. Please enter a number.\n");
          flush_input();
          continue;
        }
      } while (gamemode != 1 && gamemode != 2);
      int seeds = 0;
      if (gamemode == 1) {
        do {
          printf("Choose your setting seeds. 0 is default. (0~3)\n");
          if (scanf("%d", &seeds) != 1) {
            printf("Invalid input. Please enter a number.\n");
            flush_input();
            continue;
          }

        } while (seeds < 0 && seeds > 3);
      }
      printf("\nThe game begins!\nThis is a game of\n\n");
      for (int i = 0; i < player; i++) {
        printf("[%s]\n", player_name[i]);
        reduce_rating(player_name[i], 3);
      }
      printf("Remember, make 5 in a row and you will win.\n");
      printf("This game costed 3 Points. And good luck.\n");

      // Waits 3 seconds and clears the screen
      sleep(3);
      clear_screen();

      int board[46][46] = {0};
      int turn = -1;
      int pointer_x = size / 2, pointer_y = size / 2;

      while (1) {
        int win = 0;
        int filled=0;
       
        if (filled == size * size) {
          printf("This is a draw. The board is entirely filled\n");
          for (int i = 0; i < player; i++) {
            reduce_rating(player_name[i], -4);
          }
          break;
        } else if (filled >= size * size * 3 / 4) {
          // Award all players badge 3
          for (int i = 0; i < player; i++) {
            give_badge(player_name[i], 3);
          }
        }

        turn++;
        turn %= player;
        printf("It is %s's turn. Remaining time: %d seconds.\n",
               player_name[turn], player_clocks[turn]);
        sleep(1);

        while (player_clocks[turn] > 0) {
          // Reduce clock
          sleep(1);
          player_clocks[turn]--;
          // Clear screen
          clear_screen();
          if (gamemode != 2) {
            for (int i = 0; i <= size; i++) {
              for (int j = 0; j <= size; j++) {
                if (pointer_x == i && pointer_y == j) {
                  printf("🔘");
                  continue;
                }
                  if (board[i][j] == 0) 
                      printf("⚫");
                    else if (board[i][j] == 1)
                      printf("🔴");
                    else if (board[i][j] == 2)
                      printf("🔵");
                    else if (board[i][j] == 3)
                      printf("🟡");
                    else if (board[i][j] == 4)
                      printf("🟢");
                    else if (board[i][j] == 5)
                      printf("🟣");
                
              }
              printf("\n");
            }
          } else {
            printf("This is blindfold mode.\nThe location of the pointer is %d "
                   "%d\n",
                   pointer_x, pointer_y);
            if (board[pointer_x][pointer_y] != 0) {
              printf("There is a token under the pointer\n");
            } else {
              printf("There is no token under the pointer\n");
            }
            printf("\n\n");
          }
          // Handle user input for moving the pointer
          if (kbhit()) {
            char input = getchar();
            if (input == 'A' || input == 'a') {
              if (pointer_y > 0) {
                pointer_y--;
              }
            } else if (input == 'D' || input == 'd') {
              if (pointer_y < size) {
                pointer_y++;
              }
            } else if (input == 'W' || input == 'w') {
              if (pointer_x > 0) {
                pointer_x--;
              }
            } else if (input == 'S' || input == 's') {
              if (pointer_x < size) {
                pointer_x++;
              }
            } else if (input == ' ' || input == '\r') {
              if (board[pointer_x][pointer_y] == 0) {
                board[pointer_x][pointer_y] = turn + 1;
                break;
              }
            } else if (input == 'J' || input == 'j') {
              int a, b;
              printf("You pressed the Jumping key (J) Enter the coordinates: ");
              if (scanf("%d %d", &a, &b) != 2) {
                printf("Invalid Input.\n");
                flush_input();
                continue;
              }
              a--;
              b--;

              // If a and b are in the range
              if (a >= 0 && a <= size && b >= 0 && b <= size) {
                pointer_x = a;
                pointer_y = b;
              } else {
                printf("Invalid coordinates.\n");
              }
            } else if (input == 'x' || input == 'X') {
              // Draw
              char draw[80] = "";
              printf("Before you type this command, you should have asked "
                     "other players if they wanted to draw.\n If you want to "
                     "draw, type 'I would Like to draw and I've already asked "
                     "other players about this'\n\n");
              scanf("%s", draw);
              if (strcmp(draw, "I would Like to draw and I've already asked "
                               "other player about this") == 0) {
                printf("You have drawn. You can't draw again");
                for (int i = 0; i < player; i++) {
                  reduce_rating(player_name[i], -4);
                }
                goto here;
              }
            } else if (input == '0') {
              break;
            }
          }
        }

        if (player_clocks[turn] <= 0) {
          printf("Time's up for %s!\n", player_name[turn]);
          player_clocks[turn] = 50;
          for (int i = 0; i <= size; i++) {
            for (int j = 0; j <= size; j++) {
              if (board[i][j] == turn + 1) {
                board[i][j] = 0;
              }
            }
          }
          sleep(2);
        }
        for (int i = 0; i <= size; i++) {
          for (int j = 0; j <= size; j++) {
            // Check Horizontal
            if (j <= size - 5) {
              if (board[i][j] == turn + 1 && board[i][j + 1] == turn + 1 &&
                  board[i][j + 2] == turn + 1 &&
                  board[i][j + 3] == turn + 1 &&
                  board[i][j + 4] == turn + 1) {
                if (seeds % 2 == 0) {
                  win = 1;
                } else if (seeds%4<2 && j + 5 <= size && board[i][j + 5] == turn + 1) {
                  win = 1;
                } else if ( seeds%4>=2 && ! ( (j + 5 <= size && board[i][j + 5]==turn+1 )|| ( j-1>=0 && board[i][j-1]==turn+1 ))) {
                  win = 1;
                }
              }
            }
            // Check Vertical
            if (i <= size - 5) {
              if (board[i][j] == turn + 1 && board[i + 1][j] == turn + 1 &&
                  board[i + 2][j] == turn + 1 &&
                  board[i + 3][j] == turn + 1 &&
                  board[i + 4][j] == turn + 1) {
                if (seeds % 2 == 0) {
                  win = 1;
                } else if (seeds%4<2 &&i + 5 <= size && board[i + 5][j] == turn + 1) {
                  win = 1;
                } else if ( seeds%4>=2 && ! ( (i + 5 <= size && board[i+5][j]==turn+1 )|| ( i-1>=0 && board[i-1][j]==turn+1 ))) {
                  win = 1;
                }
              }
            }
            // Check Diagonal (Top-Left to Bottom-Right)
            if (i <= size - 5 && j <= size - 5) {
              if (board[i][j] == turn + 1 &&
                  board[i + 1][j + 1] == turn + 1 &&
                  board[i + 2][j + 2] == turn + 1 &&
                  board[i + 3][j + 3] == turn + 1 &&
                  board[i + 4][j + 4] == turn + 1) {
                if (seeds % 2 == 0) {
                  win = 1;
                } else if (i + 5 <= size && j + 5 <= size &&
                           board[i + 5][j + 5] == turn + 1) {
                  win = 1;
                } 
              }
            }
            // Check Diagonal (Top-Right to Bottom-Left)
            if (i <= size - 5 && j >= 4) {
              if (board[i][j] == turn + 1 &&
                  board[i + 1][j - 1] == turn + 1 &&
                  board[i + 2][j - 2] == turn + 1 &&
                  board[i + 3][j - 3] == turn + 1 &&
                  board[i + 4][j - 4] == turn + 1) {
                if (seeds % 2 == 0) {
                  win = 1;
                } else if (i + 5 <= size && j - 5 >= 0 &&
                           board[i + 5][j - 5] == turn + 1) {
                  win = 1;
                }
              }
            }
          }
        }

        if (win == 1) {
          printf("Player %s wins!\n", player_name[turn]);
          // Award the winning player badge 1 and 2
          give_badge(player_name[turn], 1);
          
          for (int i = 0; i < player; i++) {
            if (i != turn) {
              reduce_rating(player_name[i], -1);
              give_badge(player_name[i], 2);
            } else {
              reduce_rating(player_name[i], -player*4);
            }
          }
          break;
        }
        // check if three quaters of the game board is filled
        
      }
    } else if (command == 5) {
      printf("\n\nThis is a help menu.\nTo play a game, just press 2 after "
             "registration (to register, press 1).\nThere is a tier system in "
             "this game.\nThe higher the tier, the better the player.\n\n0~4 "
             "Unranked\n5~9 Bronze\n10~30 Silver\n31~50 Gold\n51~100 "
             "Platinum\n101~500 Diamond\n501~1000 Master\n\nIf someone beats "
             "another player, their ratings could change.\n\nBy the way, have "
             "you tried Fir In Processing? (Check processing.txt)\n\n");
    } else if (command == 6) {
      char username[MAX_LENGTH];
      printf("\n\nInput Username: ");
      if (scanf("%s", username) != 1) {
        printf("Invalid input.\n");
        flush_input();
        continue;
      }

      FILE *fptr = fopen("user.txt", "r");
      if (!fptr) {
        perror("Error opening user.txt");
        continue;
      }

      char line[MAX_LENGTH];
      int found = 0;
      int line_number = 0;
      int user_line = -1;
      int fir;
      while (fgets(line, sizeof(line), fptr)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, username) == 0) {
          found = 1;
          user_line = line_number;
          break;
        }
        line_number++;
      }

      fclose(fptr);

      if (found) {
        fptr = fopen("badges.txt", "r");
        if (!fptr) {
          perror("Error opening badges.txt");
          continue;
        }

        for (int i = 0; i <= user_line; i++) {
          fgets(line, sizeof(line), fptr);
        }

        fir = atoi(line);
        printf("Badge ID: %d\n", fir);

        fclose(fptr);
      } else {
        printf("User not found.\n");
        goto here;
      }
      printf("These are the lists of the badges:\n");
      printf("------------------------------------\n");
      printf("✅ ");
      printf("Welcome - Visit the game\n");
      if (fir % 2 >= 1)
        printf("✅ ");
      printf("First to win - Win for the first time\n");
      if (fir % 4 >= 2)
        printf("✅ ");
      printf("Challenger - lose for the first time\n");
      if (fir % 8 >= 4)
        printf("✅ ");
      printf("Neck and the Neck - Fill the 3/4 of the board\n");

    } else {
      printf("Invalid command. Please try again. (Press 5 For help)\n");
    }
  }
}
void reduce_rating(const char *username, int reduction) {
  FILE *fptr = fopen("user.txt", "r");
  if (!fptr) {
    perror("Error opening user.txt");
    return;
  }

  char line[MAX_LENGTH];
  int found = 0;
  int line_number = 0;
  int user_line = -1;

  while (fgets(line, sizeof(line), fptr)) {
    line[strcspn(line, "\n")] = 0;
    if (strcmp(line, username) == 0) {
      found = 1;
      user_line = line_number;
      break;
    }
    line_number++;
  }

  fclose(fptr);

  if (found) {
    fptr = fopen("games.txt", "r+");
    if (!fptr) {
      perror("Error opening games.txt");
      return;
    }

    // Read all ratings into memory
    char ratings[100][MAX_LENGTH];
    int count = 0;

    while (fgets(ratings[count], sizeof(ratings[count]), fptr)) {
      ratings[count][strcspn(ratings[count], "\n")] = 0;
      count++;
    }

    // Reduce the rating
    int fir = atoi(ratings[user_line]);
    fir -= reduction;

    if (fir < 0)
      fir = 0; // Ensure rating does not go below 0

    // Update the rating in memory
    snprintf(ratings[user_line], MAX_LENGTH, "%d", fir);

    // Write all ratings back to the file
    freopen("games.txt", "w", fptr);
    for (int i = 0; i < count; i++) {
      fprintf(fptr, "%s\n", ratings[i]);
    }

    fclose(fptr);
  }
}

void clear_screen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}
int kbhit(void) {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
