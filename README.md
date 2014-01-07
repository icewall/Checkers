Author : Marcin 'Icewall' Noga


Description
===========
Checkers game with AI implemented with usage of [Minimax]( http://en.wikipedia.org/wiki/Minimax ) algorithm and also [Alpha–beta pruning](http://en.wikipedia.org/wiki/Alpha–beta_pruning) for optymalization
purpose.

How good CPU is ?
=================
Current implementation with usage of alpha-beta allow CPU to predict 8 moves in reasonable amount of time.
The AI inteligence level You can edit in cpu.cpp:

```cpp
Line 33: 
	m_maxDepth = 8;
```	

#Strategy
AI needs to have some kind of descrption of board fields to be able to evaluate both moves: own and human one.
This data for "evaluation function" can be find in cpu.cpp:

```cpp
=======
Line: 14

int g_tableWeight[] = 	
                     { 4, 4, 4, 4,
											 4, 3, 3, 3,
											 3, 2, 2, 4,
											 4, 2, 1, 3,
											 3, 1, 2, 4,
											 4, 2, 2, 3,
											 3, 3, 3, 4,
											 4, 4, 4, 4};

int g_tableSimpleWeight [] =
                     { 3, 3, 3, 3,
											 3,	2, 2, 2,
											 2, 1, 1, 3,
											 3, 1, 1, 2,
											 2, 1, 1, 3,
											 3, 1, 1, 2,
											 2, 2, 2, 3,
											 3, 3, 3, 3};
```											 
=======
```											 
Currently CPU uses **g_tableWeight**.

#What repo contains ?
**bin/** for those people who would like to faced with CPU rapidly, there is ready to launch Warcaby.exe file. 


#Final notes
Please keep in mind that project was written during one week ( limit set by prof on university ) so code is not perfect, but I hope You will enjoy playing and find some interesting pieces of code for yourself ;).

Enjoy
