#include <iostream>
#include <map>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;
	 
class Base {
  public:
    int stun = 0;
    int poison = 0;
    int burn = 0;
    int shield = 0;
    int dodge = 0;
    int withstand = 0;
    int regen = 0;
    int strength = 0;
};
	 
class Move: public Base {
  public:
    int damage = 0;
    int heal = 0;
    int cleanse = 0;
    int dispel = 0;
    int tankify = 0;
    int streak = 0;
    int repeat = 1;
    Move(int dam, int s, int p, int b, int d, int sk, int re) {
      damage = dam;
      stun = s;
      poison = p;
      burn = b;
      dispel = d;
      streak = sk;
      repeat = re;
    }
    Move(int h, int s, int c, int d, int w, int r, int t, int st, int sk) {
       heal = h;
       shield = s;
       cleanse = c;
       dodge = d;
       withstand = w;
       regen = r;
       tankify = t;
       strength = st;
       streak = sk;
    }
    Move(int dam, int h, int st, int p, int b, int s, int c, int d, int w, int r, int sk, int re) {
      damage = dam;
      heal = h;
      stun = st;
      poison = p;
      burn = b;
      shield = s;
      cleanse = c;
      dispel = d;
      withstand = w;
      regen = r;
      streak = sk;
      repeat = re;
    }
    Move(int s) {
       switch(s) {
         case 0:
          damage = 0;
          streak = 1;
         case 1:
           damage = 1;
           streak = 2;
      }
    }
    Move() {}
};

class Player: public Base {
  public:
    int maxhp = 20;
    int hp = 20;
    int turn;
    Player(int h, int t) {
      hp = h;
      maxhp = h;
      turn = t;
    }
    void sethp(int h) {
      hp = h;
      maxhp = h;
    }
    void dispel(int d) {
      if (strength)
        strength -= d;
      if (dodge)
        dodge -= d;
      if (withstand) 
        withstand -= d;
      if (regen)
        regen -= d;
      if (shield)
        shield -= 2 * d;
    }
    void cleanse(int c) {
      if (poison) 
        poison -= c;
      if (burn)
        burn -= c;
    }
};

int strengthTest(int i) {
  if (i) 
    return 2;
  else 
    return 1;
}

void game() {
	srand((unsigned) time(0));
  map<string, Move> moves;
  //attack moves: damage, stun, poison, burn, dispel, streak, repeat
  //support moves: heal, shield, cleanse, dodge, withstand, regen, tankify, strength, streak
  //basic attack moves
  moves["sword"] = Move(1,0,0,0,0,1,1);
  moves["multisword"] = Move(1,0,0,0,0,2,3);
  moves["cannon"] = Move(5,0,0,0,0,3,1);
  moves["superstrike"] = Move(8,0,0,0,0,4,1);
  moves["reaper"] = Move(12,0,0,0,4,5,1);
  moves["cheat_codes"] = Move(20,0,0,0,10,6,1);
  //complex attack moves
  moves["dispel"] = Move(0,0,0,0,1,1,1);
  moves["zap"] = Move(2,1,0,0,0,2,1);
  moves["intoxicate"] = Move(1,0,3,0,0,2,1);
  moves["fireball"] = Move(3,0,0,2,0,3,1);
  moves["stun"] = Move(0,3,0,0,0,3,1);
  moves["asian_slap"] = Move(2,2,0,0,0,3,1);
  moves["blizzard"] = Move(3,3,0,0,0,4,1);
  //support moves
  moves["cleanse"] = Move(0,0,1,0,0,0,0,0,1);
  moves["heal"] = Move(2,0,1,0,0,0,0,0,2);
  moves["strength"] = Move(0,0,0,0,0,0,0,1,2);
  moves["shield"] = Move(0,3,0,0,0,0,0,0,2);
  moves["tankify"] = Move(1,0,0,0,0,0,1,0,2);
  moves["withstand"] = Move(0,0,0,0,1,0,0,0,2);
  moves["dodge"] = Move(0,0,0,2,0,0,0,0,3);
  //fancy moves
  //special moves
  Player p1(1,1);
  Player p2(1,2);
  Player* user = &p1;
  Player* target = &p2;
  int streak = 0;
  cout << "Welcome to Chance! \n";
  int hp;
  cout << "Enter Player HP: \n";
	cin >> hp;
  if (!hp) {
    hp = 20;
    cout << "Invalid Input; 20 hp will be used\n";
  }
  p1.sethp(hp);	
  p2.sethp(hp);
  string uinput;
  bool moving = false;
  while (p1.hp > 0 && p2.hp > 0) {
    cout << "\nPlayer " << (*user).turn << " Turn\n" << p1.hp << " - " << p2.hp << "\n";
    if ((*user).regen) {
      cout << "Player " << (*user).turn << " regenerated 1 health\n";
      (*user).hp++;
      (*user).regen--;
      if ((*user).hp > (*user).maxhp)
        (*user).hp--;
    }
    moving = true;
    if ((*user).stun) {
      cout << "Player " << (*user).turn << " was stunned\n";
      (*user).stun--;
      moving = false;
    }
    while (moving) {
      cin >> uinput;
      if (uinput == "s" || uinput == ".") {
        if (rand() % 2 || !(rand() % 5)) {
          streak++;
          cout << "Streak Successful\n";
        } else { 
          cout << "Streak Failed\n";
          moving = false;
        }
      } else if (streak >= moves[uinput].streak && moves[uinput].streak) {
        if (moves[uinput].tankify) {
            cout << "Player " << (*user).turn << " tankified " << moves[uinput].tankify << " hp\n";
            (*user).maxhp += moves[uinput].tankify;
        }
        if (moves[uinput].cleanse) {
            cout << "Player " << (*user).turn << " was cleansed for " << moves[uinput].cleanse << "\n";
              (*user).cleanse(moves[uinput].cleanse);
          }
          if (moves[uinput].dispel) {
              cout << "Player " << (*target).turn << " was dispelled for " << moves[uinput].dispel << "\n";
              (*target).dispel(moves[uinput].dispel);
          }
          if (moves[uinput].heal) {
            cout << "Player " << (*user).turn << " healed for " << moves[uinput].heal << "\n";
            (*user).hp += moves[uinput].heal;
            if ((*user).hp > (*user).maxhp)
              (*user).hp = (*user).maxhp;
          }
          if (moves[uinput].strength) {
            cout << "Player " << (*user).turn << " gained strength for " << moves[uinput].strength << " attack(s)\n";
            (*user).strength += moves[uinput].strength;
          }
          if (moves[uinput].dodge) {
            cout << "Player " << (*user).turn << " can dodge " << moves[uinput].dodge << " attack(s)\n";
            (*user).dodge += moves[uinput].dodge;
          }
          if (moves[uinput].withstand) {
            cout << "Player " << (*user).turn << " can withstand " << moves[uinput].withstand << " time(s)\n";
            (*user).withstand += moves[uinput].withstand;
          }
          if (moves[uinput].regen) {
            cout << "Player " << (*user).turn << " gained regeneration for " << moves[uinput].regen << " turns\n";
            (*user).regen += moves[uinput].regen;
          }
          if (moves[uinput].shield > (*user).shield) {
            cout << "Player " << (*user).turn << " acquired a shield with a durability of " << moves[uinput].shield << "\n";
            (*user).shield = moves[uinput].shield;
          }
          if (moves[uinput].poison) {
            cout << "Player " << (*target).turn << " was poisoned for " << moves[uinput].poison << " turns\n";
            (*target).poison += moves[uinput].poison;
          }
          if (moves[uinput].burn) {
            cout << "Player " << (*target).turn << " was burnt for " << moves[uinput].burn << " turns\n";
            (*target).burn += moves[uinput].burn;
          }
          if (moves[uinput].stun > (*target).stun) {
            cout << "Player " << (*target).turn << " was stunned for " << moves[uinput].stun << " turns\n";
            (*target).stun = moves[uinput].stun;
          }
          if (!moves[uinput].damage)
            break;
          for (int i = 0; i<moves[uinput].repeat; i++) {
            if (moves[uinput].damage > 3 && (*target).dodge) {
              (*target).dodge--;
              cout << "Player " << (*target).turn << " dodged the attack\n";
            }
            else if (moves[uinput].damage <= 3 && (*target).withstand) {
              (*target).withstand--;
              cout << "Player " << (*target).turn << " withstood the attack\n";
              moving = false;
              break;
            } else {
              if ((*user).strength)
                cout << "Player " << (*user).turn << " used strength\n";
              if ((*target).shield) {
                if (strengthTest((*user).strength) * moves[uinput].damage < ((*target).shield)) {
                  cout << "Player " << (*target).turn << "'s shield took " << strengthTest((*user).strength)*moves[uinput].damage << " damage\n";
                  (*target).shield -= strengthTest((*user).strength) * moves[uinput].damage;
                } else {
                  cout << "Player " << (*target).turn << "'s shield Broke\n";
                  if (strengthTest((*user).strength)*moves[uinput].damage-(*target).shield > 0) {
                    (*target).hp -= strengthTest((*user).strength)*moves[uinput].damage-(*target).shield;
                    cout << "Player " << (*target).turn << " took " << strengthTest((*user).strength)*moves[uinput].damage-(*target).shield << " damage\n";
                  }
                  (*target).shield = 0;
                }
              } else {
                cout << "Player " << (*target).turn << " took " << strengthTest((*user).strength)*moves[uinput].damage-(*target).shield << " damage\n";
                (*target).hp -= strengthTest((*user).strength)*moves[uinput].damage;
              }
              if ((*user).strength)
                (*user).strength--;
          }
          moving = false;
        }//end of move
      } else if (moves[uinput].streak) //end of input
        cout << "Insufficient Streak\n";
      else
        cout << "Invalid Input\n";
    }//end of turn
    if ((*user).poison) {
      cout << "Player " << (*user).turn << " took 1 damage from poison\n";
      (*user).hp--;
      (*user).poison--;
    }
    if ((*user).burn) {
      cout << "Player " << (*user).turn << " took 2 damage from burning\n";
      (*user).hp -= 2;
      (*user).burn--;
    }
    streak = 0;
    user = target;
    if (user == &p1)
      target = &p2;
    else
      target = &p1;
  }//end of game
  if (p1.hp < 1 && p2.hp < 1)
    cout << "Draw\n";
  else if (p1.hp > 0)
    cout << "Player 1 Wins\n";
  else
    cout << "Player 2 Wins\n";
}

int main() {
  game();
  return 0;
}
