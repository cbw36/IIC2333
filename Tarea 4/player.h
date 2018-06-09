

struct card
{
  int value;
  char* shape;
};

typedef struct card Card;

struct player
{
  int id;
  char* name;
  Card* hand;
  int pot;
  int bet;
};

typedef struct player Player;
