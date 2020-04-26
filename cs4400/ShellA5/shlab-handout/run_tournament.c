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

/*
static char *read_to_string(int fd) {
  size_t size = 4097, amt = 0;
  char buffer[size];
  ssize_t got;

  while (1) {
    got = Read(fd, buffer + amt, size - amt);
    if (!got) {
      buffer[amt] = 0;
      return strdup(buffer);
    }
    amt += got;
    if (amt > (size - 1))
      app_error("received too much output");
  }
}
*/

static void write_string_to(int fd, char *str) {
  size_t len = strlen(str);
  Rio_writen(fd, str, len);
  
}

static int runRounds(int seed, int n, char **progs) {
	int parentID = getpid();  		// Parent ID
	int ids[n];						// Child process IDs
	int i, j;						// Iterations
	int winner = 0;					// Winner Checker
	int winnerIndex = -1;			// Index of winner
	int new_out[2];					// output for game_maker
	int new_in[2];					// input to game_maker
	char buffer[30];				// where you right to
	int player_in[n][2];            // input for players
	int player_out[n][2];			// output for players
	Pipe(new_out);					// Pipe to communicate with game maker
	Pipe(new_in);					// Pipe to communicate with game maker
	int gameMakerID = Fork();		// game_maker proccess id

	if(gameMakerID == 0) {
		char* prog = "game_maker";
		char buffer1[10];
		char buffer2[10];
		snprintf(buffer1, 10, "%d", seed);
		snprintf(buffer2, 10, "%d", n);
		char* args[] = {prog, buffer1, buffer2, "40", "45", NULL};
		Dup2(new_out[1], 1);
		Dup2(new_in[0], 0);
		Execve(prog, args, environ);
	}

	Rio_readn(new_out[0], buffer, 7);   //Read the 7 bytes
	char *target = buffer;               //Getting the target
	printf("Target: %s\n", target);

	//Getting the staring position of players
	for(i = 0;i < n;i++) {
		Pipe(player_out[i]);
		Pipe(player_in[i]);
	}
	
	// Start each player program
   	for(i = 0;i < n;i++) {
   		if(getpid() == parentID) {
   			int id = Fork();
   			ids[i] = id;
   			if(id == 0) {
   			char* prog = progs[i];
   			char* args[] = {prog, NULL};
   			//printf("%s\n", args[0]);
   			Dup2(player_out[i][1], 1);
   			Dup2(player_in[i][0], 0);
   			Execve( args[0], args, environ);
   			}
   		}
   	}

   	//Comunicating
   	i = 0;
   	while(!winner) {
   		//If first give the start position
   		if(i++ == 0) {
   			for(j = 0;j < n;j++) {
   				Rio_readn(new_out[0], buffer, 7);
				printf("Start: %d %s\n", j, buffer);
   				write_string_to(player_in[j][1], buffer);
   				printf("SEND: %d %s\n", j, buffer);
   			}
   		}

   		//Get the player new position
   		for(j = 0;j < n;j++) {
   			if(winner) break;
   			Rio_readn(player_out[j][0], buffer, 7); 	         // READ PLAYER POSITION
   			printf("READ: %d %s\n", j, buffer);			
			write_string_to(new_in[1], buffer);			// SEND POSITION TO GAME MAKER
			Rio_readn(new_out[0], buffer, 7);			    // GET COMMANS FROM GAME MAKER
			printf("READ2: %d %s\n", j, buffer);
			write_string_to(player_in[j][1], buffer);	// SEND COMMAND TO PLAYER
			if(strncmp (buffer,"winner\n",7) == 0) {
				winner = 1;
				winnerIndex = j;
				printf("%s\n", "WINNER");
				break;
			}
   		}
   		if(winner) break;
   	}

   	for(i = 0;i < n;i++) {
   		if(i != winnerIndex) {
   			kill(ids[i], SIGKILL);
   		}
   	}
   	
   	int x = close(new_out[1]);
   	printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", x);
	int y = close(new_in[0]);
	printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", y);
	
	for(i = 0;i < n;i++) {
		int w = close(player_in[i][1]);
		printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", w);
		int z = close(player_out[i][0]);
		printf("%d>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", z);
	}
	
   	printf("DONE\n\n\n");
   	return winnerIndex;
}

/**********************************************************************
* rounds = number of rounds
* n = the number of players
* progs =  Array of player programs
* Parents id = 1 if parent process is done
* getpid() = get id, getppid() = get parent id
* Fork() returns 0 to child(itself), id to parent
**********************************************************************/
static void run_tournament(int seed, int rounds, int n, char **progs)
{
	fprintf(stderr, "Seed: %d Round: %d n: %d\n", seed, rounds, n );

	int wins[n];
	int i;
	for(i = 0;i < n;i++) {
		wins[i] = 0;
	}

	for(i = 0;i < rounds;i++) {
		int x = runRounds(seed + i, n, progs);
		printf("WINNER: %s\n", progs[x]);
		wins[x] = wins[x] + 1;	
	}

	printf("%d\n", rounds);
	for (i = 0; i < n;i++)
	{
		printf("%d %d %d\n", wins[i], rounds - wins[i], 0);
	}
}