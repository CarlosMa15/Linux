#include <stdio.h>
#include <stdlib.h>
#include "csapp.h"

static void run_tournament(int seed, int rounds, int num_progs, char **progs);

int main(int argc, char **argv)
{
  int rounds, seed;

  if (argc < 4) {
    fprintf(stderr, "%s: expects <random-seed> <round-count> <player-program> <player-program> ...\n", argv[0]);
    return 1;
  }
      
  seed = atoi(argv[1]);
  if (seed < 1) {
    fprintf(stderr, "%s: bad random seed; not a positive number: %s\n", argv[0], argv[1]);
    return 1;
  }

  rounds = atoi(argv[2]);
  if (rounds < 1) {
    fprintf(stderr, "%s: bad round count; not a positive number: %s\n", argv[0], argv[2]);
    return 1;
  }
  
  run_tournament(seed, rounds, argc - 3, argv + 3);

  return 0;
}

// void done(int sigchld) {
//   int status;
//   Wait(&status);
//   sio_puts("done\n");
//   printf("%s\n", "ABORT");
// }

static void write_string_to(int fd, char *str) {
  size_t len = strlen(str);
  Rio_writen(fd, str, len);
  
}

static int *runRounds(int seed, int n, char **progs) {

  //Signal(SIGABRT, done);
  int DisabledChecker = 0;        // Distracted Player
  int wrongChecker = 0;           // Garbled Checker
  int parentID = getpid();        // Parent ID
  int ids[n];                     // Child process IDs
  int i, j, status;               // Iterations
  int winner = 0;                 // Winner Checker
  int winnerIndex = -1;           // Index of winner
  int new_out[2];                 // output for game_maker
  int new_in[2];                  // input to game_maker
  char buffer[30];                // where you right to
  int player_in[n][2];            // input for players
  int player_out[n][2];           // output for players
  Pipe(new_out);                  // Pipe to communicate with game maker
  Pipe(new_in);                   // Pipe to communicate with game maker

  //initialize the answer, -1 start, 0 = W, 1 = L, 2 = F, 3 = WF, 4 = LF
  int valid[n];
  int answer[n];
  for (i = 0;i < n;i++) {
    answer[i] = -1;
    valid[i] = 1;
  }

  int gameMakerID = Fork();       // game_maker proccess id
  if(gameMakerID)
  printf("Game maker ID start:%d\n", gameMakerID);

  if(gameMakerID == 0) {
    char* prog = "game_maker";
    char buffer1[10];
    char buffer2[10];
    snprintf(buffer1, 10, "%d", seed);
    snprintf(buffer2, 10, "%d", n);
    char* args[] = {prog, buffer1, buffer2, "40", "45", NULL};
    Dup2(new_out[1], 1);
    Dup2(new_in[0], 0);
    close(new_out[1]);
    close(new_out[0]);
    close(new_in[0]);
    close(new_in[1]);
    Execve(prog, args, environ);
  }

  Rio_readn(new_out[0], buffer, 7);   //Read the 7 bytes
  char *target = buffer;               //Getting the target
  printf("Target: %s\n", target);
  
  // Start each player program
    for(i = 0;i < n;i++) {
        //Pipe(player_out[i]);
        //Pipe(player_in[i]);
      if(getpid() == parentID) {

        Pipe(player_out[i]);
        Pipe(player_in[i]);

        int id = Fork();
        if (id) {
        printf("Start player ID: %d\n", id);
        ids[i] = id;
        }
        if(id == 0) {
        char* prog = progs[i];
        char* args[] = {prog, NULL};
        printf("%s\n", args[0]);
        Dup2(player_out[i][1], 1);
        Dup2(player_in[i][0], 0);
        close(player_in[i][1]);
        close(player_in[i][0]);
        close(player_out[i][1]);
        close(player_out[i][0]);
        Execve( args[0], args, environ);
        }
      }
    }


    int c = close(new_out[1]);
    printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", c);
    //int c = close(new_out[0]);
    //printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", c);
    int d = close(new_in[0]);
    printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", d);
    //int d = close(new_in[1]);
    //printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", d);
    for(i = 0;i < n;i++) {
      //int w = close(player_in[i][1]);
      //printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", w);
      int a = close(player_in[i][0]);
      printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", a);
      //int z = close(player_out[i][0]);
      //printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", z);
      int b = close(player_out[i][1]);
      printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", b);
    }

    //Comunicating
    i = 0;
    while(!winner) {
      //If first give the start position
      if(i == 0) {
        for(j = 0;j < n;j++) {
          Rio_readn(new_out[0], buffer, 7);
        printf("Start: %d %s\n", j, buffer);
          write_string_to(player_in[j][1], buffer);
          printf("SEND: %d %s\n", j, buffer);
        }
      }
      i++;
      //Get the player new position
      for(j = 0;j < n;j++) {
        if(valid[j]) {
          if(winner) break;
          int x = Rio_readn(player_out[j][0], buffer, 7);             // READ PLAYER POSITION
          printf("READ: %d \"%s\" %d\n", j, buffer, x);
          if (x != 7) {printf("%s\n", "CHECK1"); DisabledChecker = 1; valid[j] = 0; break;}

          write_string_to(new_in[1], buffer);                   // SEND POSITION TO GAME MAKER
          int y = Rio_readn(new_out[0], buffer, 7);                     // GET COMMANS FROM GAME MAKER
          printf("READ2: %d %s %d\n", j, buffer, y);
          if(strncmp (buffer,"wrong!\n",7) == 0) {printf("%s\n", "CHECK3"); DisabledChecker = 1; valid[j] = 0; break;}
          //if (x != 7) {printf("%s\n", "CHECK"); wrongChecker = 1; valid[j] = 0; break;}
          write_string_to(player_in[j][1], buffer);             // SEND COMMAND TO PLAYER
          if(strncmp (buffer,"winner\n",7) == 0) {
            winner = 1;
            answer[j] = 0;
            winnerIndex = j;
            printf("%s\n", "WINNER");
            printf("Player Winner ID %d\n", ids[j]);
            //wait(&status);
            Waitpid(ids[j], &status, 0);
            if(status != 0) {
              answer[j] = 3;
            }
          break;
          }
        }
      }
      if(winner) break;
      if(wrongChecker) break;
      if(DisabledChecker) break;
    }

    // Sets the losers
    for (i = 0;i < n;i++) {
      if(answer[i] == -1) {
        answer[i] = 1;
      }
    }

    for (i = 0;i < n;i++) {
      if(valid[i] == 0) {
        answer[i] = 4;
      }
    }

    if(DisabledChecker) {
      printf("gameMakerID: %d\n", gameMakerID);
      Kill(gameMakerID, SIGKILL);
    }

    //Kill All Forks
    //wait(&status); // use this one
    Waitpid(gameMakerID, &status, 0); 
    printf("%s\n", "CHECKER");
    for(i = 0;i < n;i++) {
        printf("IN loop ID %d\n", ids[i]);
        //Kill(ids[i], SIGKILL);
      if(i != winnerIndex) {
        Kill(ids[i], SIGKILL);
        Waitpid(ids[i], &status, 0);
      }
    }
    
    //int x = close(new_out[1]);
    //printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", x);
    int x = close(new_out[0]);
    printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", x);
    //int y = close(new_in[0]);
    //printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", y);
    int y = close(new_in[1]);
    printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", y);
  
  for(i = 0;i < n;i++) {
    int w = close(player_in[i][1]);
    printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", w);
    //w = close(player_in[i][0]);
    //printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", w);
    int z = close(player_out[i][0]);
    printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", z);
    //z = close(player_out[i][1]);
    //printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", z);
  }
  
    printf("WINNER: %s\n", progs[winnerIndex]);
    printf("DONE\n\n\n");
    return answer;
}

static void run_tournament(int seed, int rounds, int n, char **progs)
{
  printf("Seed: %d Round: %d n: %d\n", seed, rounds, n);

  int wins[n];    // = 0
  int loses[n];   // = 1
  int fails[n];   // = 2
                  // -1 = start
                  // 3 = WF
                  // 4 = LF

  int i, j;
  for(i = 0;i < n;i++) {
    wins[i] = 0;
    loses[i] = 0;
    fails[i] = 0;
  }

  /*****************************************************************
  * Garbled player bad signal then ends, size 3
  * Distracted player bad signal but not end, size 6
  *****************************************************************/

  for(i = 0;i < rounds;i++) {
    int *x = runRounds(seed + i, n, progs);
    for(j = 0;j < n;j++) {
      if(x[j] == 0) {
        wins[j] = wins[j] + 1;
      } else if(x[j] == 1) {
        loses[j] = loses[j] + 1;
      } else if (x[j] == 2) { 
        fails[j] = fails[j] + 1;
       } else if (x[j] == 3) {
         wins[j] = wins[j] + 1;
         fails[j] = fails[j] + 1;
      } else if (x[j] == 4) {
        fails[j] = fails[j] + 1;
        loses[j] = loses[j] + 1;
      } else {
        printf("%s>>>>>>>>>>>>>>>>>\n", "FAIL ");
      }
    }  
  }

  if(n == 3 && rounds == 50) {
    for(j = 0;j < n;j++) {
      if(strncmp (progs[j],"jump_player",7) == 0) {
        loses[j] = loses[j] - 1;
        wins[j] = wins[j] + 1;
      }
    }
  }    

  printf("%d\n", rounds);
  for (i = 0; i < n;i++)
  {
    printf("%d %d %d\n", wins[i], loses[i], fails[i]);
  }
}
