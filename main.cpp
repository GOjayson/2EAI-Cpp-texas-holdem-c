#include <iostream>
#include <string.h>
#define DEBUG_PRINT
#include "texasholdem.h"




unsigned int previousBetOfPlayer[MAX_PLAYERS_IN_GAME];

int meineFunktion( struct Game * game, struct Player * player, unsigned int totalBet )
{
	//printf( "raised amount = %d\n", totalBet - player->bet );

	for( int i = 0 ; i < game->playersSize ; i++ )
	{
		//printf( "betting ratio of player %s is %f%% and is %f%% of my capital and raised %d chips\n",
		//		game->players[i]->name,
		//		(float) game->players[i]->bet / game->players[i]->chips * 100,
		//		(float) game->players[i]->bet / player->chips * 100,
		//		game->players[i]->bet - previousBet[i] );
		previousBetOfPlayer[i] = game->players[i]->bet;
	}

	if( rand() % 5 > 0 ) //--> 0 1 2 3 4 --> 4/5 --> 80%
	{
		if( rand() % 5 == 0 ) //--> 0 1 2 3 4 --> 1/5 --> 20% of 80% ==> 16%
		{
			return( ( player->chips / 2 ) - totalBet ); //half in
		}
		return( 0 );
	}
	return( -1 );
}

int willYouRaise( struct Game * game, struct Player * player, unsigned int totalBet )
{
	switch( player->ID )
	{
	case 0:
		return( 1 ); //Allways raise with 1 !
		break;
	case 1:
	{
		PokerRank temp = getMyHandRank( player->hand );
		if( temp.category >= THREE_OF_A_KIND )
		{ //When I have a three of a kind or higher, I go all-in
			return( player->chips );
		}
		return( 0 ); //Else I will always call
		break;
	}
	case 2:
	{
		static unsigned int previousRound = 0;
		static unsigned int previousBet = 0;
		if( previousRound != game->round )
		{ //When the previous game round is different from the current game round, we are in a new game and we take the big blind
			previousBet = game->blind * 2;
		}
		if( totalBet > previousBet * 2 )
		{ //When the total bet is more than double the previous known bet, we fold
			return( -1 );
		}
		previousBet = totalBet; //safe the current bet als previous known bet
		return( 0 ); //Else we call
		break;
	}
	case 3:
		if( totalBet > ( player->chips / 10 ) )
		{ //When the current bet is bigger than 10% of my chips, I will fold
			return( -1 );
		}
		else
		{ //Else I call
			return( 0 );
		}
		break;
	case 4:
		if( player->hand->cards[0]->rank == 1 || player->hand->cards[1]->rank == 1 )
		{ //When I got at least an ACE in my hand, I will raise with 10% of my hand !
			return( player->chips / 10 );
		}
		else
		{ //Else I will fold
			return( -1 );
		}
	case 5:
		if( player->hand->cards[0]->suit == player->hand->cards[1]->suit )
		{ //When the two cards in my hand have the same suit
			if( table[0] == nullptr )
			{ //And there is nothing on the table yet, raise with 10 !
				return( 10 );
			}
			if( table[3] == nullptr )
			{ //Or the forth card is not on the table yet
				if( player->hand->cards[0]->suit == table[0]->suit || player->hand->cards[0]->suit == table[1]->suit || player->hand->cards[0]->suit == table[2]->suit )
				{ //And there is at least one of the suits that matched our double suit, raise 100 ! (we are going for the flush)
					return( 100 );
				}
			}
			if( totalBet > player->bet + 5 )
			{ //Or unless the bet has been raisen 5 more than what I had bet already, I fold
				return( -1 );
			}
			return( 0 ); //Else I will call
		}
		else
		{ //Else I fold
			return( -1 );
		}
	case 6:
		return( meineFunktion( game, player, totalBet ) );
		break;
	default:
		return( 0 ); //Call all the time
		break;
	}
	return( 0 );
}

int main( void )
{
	Game game;
	makeNewDeck( &game );

	Player p1;
	strcpy( p1.name, "Alice" );
	p1.ID = 1;
	Player p2;
	strcpy( p2.name, "Bob" );
	p2.ID = 2;
	Player p3;
	strcpy( p3.name, "Carla" );
	p3.ID = 3;
	Player p4;
	strcpy( p4.name, "Danny" );
	p4.ID = 4;
	Player p5;
	strcpy( p5.name, "Eric" );
	p5.ID = 5;
	Player p6;
	strcpy( p6.name, "Fuhrer" );
	p6.ID = 6;
	Player helloThisIsMe;
	strcpy( helloThisIsMe.name, "!!! THIS IS ME !!!" );
	helloThisIsMe.ID = 0;

	addPlayerToGame( &game, &p1 );
	addPlayerToGame( &game, &p2 );
	addPlayerToGame( &game, &p3 );
	addPlayerToGame( &game, &p4 );
	addPlayerToGame( &game, &p5 );
	addPlayerToGame( &game, &p6 );
	addPlayerToGame( &game, &helloThisIsMe );

	playGame( &game, 1 );
	printf( "The winner is %s with %d chips !", game.players[0]->name, game.players[0]->chips );
	return 0;
}

