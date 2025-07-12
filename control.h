#ifndef CONTROL_H
#define CONTROL_H

Move getEngineMove();

//Update bitboards and boardArr
void executeMove(const Move& m);//update global game state, assumes valid move.
void unexecuteMove(const Move& m, int destSquare);//Restore board state but not metadata
/*Move execution/unexcution procedure:

metadataT backupMd = MD;
int destSquare = boardArr[m.dest];
executeMove(m);
//process
MD = backupMd;
unexecuteMove(m, destSquare);

*/

/*Takes isInCheck to avoid repeated work, the color passed is the color to move
Return values:
- 2: Game continuing
- 1: White wins
- 0: Draw
- -1: Black wins*/
int gameEnd(int color, int isInCheck, int numMoves);

#endif