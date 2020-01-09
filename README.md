# Nine Men’s Morris

Nine Men’s Morris engine and GUI developed from the ground up using C and FreeGLUT.

https://www.youtube.com/watch?v=xaaOT-JRM80

## :memo: Description

Nine Men’s Morris is an ancient game, popular in Ancient Rome and the medieval period. The oldest building that has a Nine Men’s Morris board carved into its stones is an Egyptian temple at Kurna, Egypt, which dates to roughly 1400 BCE. The game is also know as Mill (probably because the shape of the board looks like a windmill), Merrels or in serbian Mica. The name Nine Men’s Morris seems to have been originated by Shakespeare in his play, A Midsummer Night’s Dream. It's been reintroduced to the wider audiences as a mini game in Assassin's Creed III developed by Ubisoft.

## :wrench: Installation and running the game

```bash
make
./9mm
```
>You need g++ compiler and freeglut!

## :video_game: Rules

* There are 2 players in the game.
* The board consists of a grid of 3 squares one inside the other, with lines connecting the 24 intersections on which pieces can be placed.
* Each player has 9 pieces, called “men” of a given color.
* Each player tries to form a “mill”, which is 3 of their pieces in one row connected by the line. To form a mill, the pieces can be placed either horizontally or vertically, but not at at a 90 degree angle, and they must be connected by lines.
* The game begins when all pieces are off the board.
* There are 3 phases to the game:
  * Phase 1: Placing the men onto the board
    * The players take turns placing one piece at a time onto the board, onto the empty dots.
    * During this phase each player tries to prevent the opponent from forming a mill. However, if a mill was formed anyway, then the player who formed a mill gets to remove one of the opponent’s pieces, of their choice, from the board. The piece removed has to be not in a mill itself. If all of the opponent’s pieces form a mill, then any of the opponent’s pieces can be removed.
    * The act of removing an opponent’s piece after forming a mill is called, “pounding”.
  * Phase 2: Moving the men on the board
    * Once all of the pieces have been placed onto the board, the players begin moving their pieces, in alternating moves, one dot at a time to try to form mills in the same fashion as in the first phase.
    * In the second phase pieces can only move to an adjacent dot, and pieces cannot jump over each other or skip dots if more than one are available in a row.
    * A player is allowed to move a piece out of a mill and then moving it back on the next move to form the mill again. If this happens it is considered as if they formed a new mill and they get to remove one of the opponent’s pieces.
  * Phase 3: Flying the men across the board
    * When one of the players has been reduced down to 3 last pieces, phase 2 ends and phase 3 begins for that player.
    * In phase 3, the limitation of moving only to an adjacent dot is removed, and player can move their pieces to any available dot, even if that requires skipping dots or jumping over other pieces. Such movement is called “flying”.
    * Phase 3 gives an advantage to the losing player.
  * The game ends when one of the players is reduced to two pieces, and cannot any longer form mills, or if a player has no legal moves to make, making their opponent the winner in either situation.
