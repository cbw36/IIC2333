

struct card
{
  char* value;
  char* shape;
  int used;
};

typedef struct card Card;

struct player
{
  int id;
  char* name;
  Card** hand;
  int pot;
  int bet;
};

typedef struct player Player;
