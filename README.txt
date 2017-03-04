Ideas:
Download openings from some openings book: consider: http://www.samsoft.org.uk/reversi/openings.htm

Heuristics options:
1) Maximize number of future good possible moves for us (minimax?) (maybe take fewest pieces at the beginning?
- but make sure we're not wiped out since apparently that is possible)
2) Minimize number of future good possible moves for opponent (or maximize number of future bad moves for opponent?)
(maybe force them to take most number of pieces at the beginning)
3) "Power spots" - corners, edges, center (4 center squares?), cluster our disks, avoid frontier discs,
avoid taking edges too soon??? - http://berg.earthlingz.de/ocd/strategy3.php?lang=en

Speed:
Dynamic programming? - save what positions have been analyzed in some sort of table
Parallelization?
