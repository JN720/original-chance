#include <iostream>
#include <map>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;
	 
class Base {
  public:
    int stun = 0;
    int poison = 0;
    int burn = 0;
    int shield = 0;
    int dodge = 0;
    int guard = 0;
    int regen = 0;
    int strength = 0;
};

class Player: public Base {
  public:
    int maxhp = 20;
    int hp = 20;
    int turn;
    int scorebank = 0;
    int accelerate = 0;
    vector<string> ds = {};
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
      if (strength < 0)
        strength = 0;
      if (dodge)
        dodge -= d;
      if (dodge < 0)
        dodge = 0;
      if (guard) 
        guard -= d;
      if (guard < 0)
        guard = 0;
      if (regen)
        regen -= d;
      if (regen < 0)
        regen = 0;
      if (shield)
        shield -= 2 * d;
      if (shield < 0)
        shield = 0;
      if (scorebank > 1) {
        scorebank -= floor(d/2);
        if (scorebank < 1)
          scorebank = 1;
      }
    }
    void cleanse(int c) {
      if (poison) 
        poison -= c;
      if (poison < 0)
        poison = 0;
      if (burn)
        burn -= c;
      if (burn < 0)
        burn = 0;
    }
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
    int isspecial = 0;
    string line;
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
       guard = w;
       regen = r;
       tankify = t;
       strength = st;
       streak = sk;
    }
    Move(int dam, int h, int st, int p, int b, int s, int c, int d, int dg, int w, int r, int sk, int re) {
      damage = dam;
      heal = h;
      stun = st;
      poison = p;
      burn = b;
      shield = s;
      cleanse = c;
      dispel = d;
      dodge = dg;
      guard = w;
      regen = r;
      streak = sk;
      repeat = re;
    }
    Move(int s, Player *user, Player *target) {
       isspecial = s;
       switch(s) {
        case 0:
          break;
        case 1: //arrow
          if (!(rand() % 4)) {
            damage = 5;
            stun = 1;
            line = "arrow crits\n";
          } else if (!(rand() % 4)) {
            damage = 3;
            line = "arrow hits\n";
          }
          else if (!(rand() % 4)) {
            damage = 1;
            line = "arrow grazes\n";
          }
          else
            line = "arrow misses\n";
          streak = 2;
          break;
        case 2: //backstab
          if (rand() % 2) {
            damage = 1;
            line = "backstab failed; resorted to standard knife attack\n";
          }
          else {
            damage = 4;
            stun = 1;
            line = "backstab successful\n";
          }
          streak = 2;
          break;
        case 3: //metallize
          if ((*target).poison) {
            line = to_string((*target).poison) + " poison metallized\n";
            damage = (*target).poison * 2;
            (*target).poison = 0; 
          } else
            line = "metallization unsuccessful\n";
          streak = 3;
          break;
        case 4: //blade_flurry
          damage = 1;
          streak = 3;
          repeat = rand() % 11;
          break;
        case 5: //blade_storm
          damage = 1;
          streak = 4;
          repeat = rand() % 19;
          break;
        case 6: //accelerate
          (*user).accelerate++;
          line = "Player " + to_string((*user).turn) + "'s rate is now " + to_string((*user).accelerate) + "\n";
          damage = (*user).accelerate;
          streak = 3;
          break;
      }
    }
    Move() {}
    string toString() {
      string rvalue = "\nStreak: " + to_string(streak) + "\n";
      if (damage) 
        rvalue += "Damage: " + to_string(damage) + "\n";
      if (heal) 
        rvalue += "Heal: " + to_string(heal) + "\n";
      if (stun) 
        rvalue += "Stun: " + to_string(stun) + "\n";    
      if (poison) 
        rvalue += "Poison: " + to_string(poison) + "\n";
      if (burn)
        rvalue += "Burn: " + to_string(burn) + "\n";
      if (shield)
        rvalue += "Shield: " + to_string(shield) + "\n";
      if (cleanse) 
        rvalue += "Cleanse: " + to_string(cleanse) + "\n";
      if (dispel) 
        rvalue += "Dispel: " + to_string(dispel) + "\n";
      if (dodge) 
        rvalue += "Dodge: " + to_string(dodge) + "\n";
      if (guard) 
        rvalue += "Guard: " + to_string(guard) + "\n";
      if (regen) 
        rvalue += "Regeneration: " + to_string(regen) + "\n";
      return rvalue;
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
  //basic attack moves
  moves["sword"] = Move(1,0,0,0,0,1,1);
  moves["multisword"] = Move(1,0,0,0,0,2,3);
  moves["cannon"] = Move(5,0,0,0,0,3,1);
  moves["superstrike"] = Move(9,0,0,0,0,4,1);
  moves["reaper"] = Move(12,0,0,0,4,5,1);
  moves["parry_this"] = Move(14,0,0,0,0,5,1);
  moves["cheat_codes"] = Move(20,0,0,0,10,6,1);
  moves["secret_move"] = Move(0,0,0,0,0,7,1);
  //complex attack moves
  moves["gas"] = Move(0,0,1,0,0,1,1);
  moves["timeout"] = Move(0,1,0,0,0,1,1);
  moves["dispel"] = Move(0,0,0,0,1,2,1);
  moves["zap"] = Move(2,1,0,0,0,2,1);
  moves["intoxicate"] = Move(1,0,3,0,0,2,1);
  moves["duodispel"] = Move(0,0,0,0,2,3,1);
  moves["fireball"] = Move(3,0,0,2,0,3,1);
  moves["stun"] = Move(0,3,0,0,0,3,1);
  moves["asian_slap"] = Move(3,2,0,0,0,3,1);
  moves["hax"] = Move(3,0,0,0,0,3,2);
  moves["micronuke"] = Move(3,0,3,1,0,3,1);
  moves["blizzard"] = Move(5,4,0,0,0,4,1);
  moves["asphyxiate"] = Move(1,0,15,0,0,4,1);
  moves["deep_freeze"] = Move(0,9,0,0,0,5,1);
  //support moves: heal, shield, cleanse, dodge, guard, regen, tankify, strength, streak
  moves["cleanse"] = Move(0,0,1,0,0,0,0,0,1);
  moves["multicleanse"] = Move(0,0,3,0,0,0,0,0,1);
  moves["heal"] = Move(2,0,1,0,0,0,0,0,2);
  moves["strength"] = Move(0,0,0,0,0,0,0,1,2);
  moves["shield"] = Move(0,3,0,0,0,0,0,0,2);
  moves["tankify"] = Move(1,0,0,0,0,0,1,0,2);
  moves["regenerate"] = Move(0,0,0,0,0,4,0,0,2);
  moves["guard"] = Move(0,0,0,0,1,0,0,0,2);
  moves["dodge"] = Move(0,0,0,2,0,0,0,0,3);
  moves["mega_shield"] = Move(0,12,0,0,0,0,0,0,4);
  moves["mega_heal"] = Move(9,0,3,0,0,0,0,0,4);
  moves["mega_tankify"] = Move(5,0,0,0,0,0,5,0,4);
  moves["restore"] = Move(14,0,8,0,0,0,0,0,5);
  moves["divinity"] = Move(5,5,20,2,1,10,0,0,5);
  //fancy moves: damage, heal, stun, poison, burn, shield, cleanse, dispel, guard, regen, streak, repeat
  moves["metacleanse"] = Move(1,0,0,0,1,0,1,0,0,0,0,2,1);
  moves["vampirize"] = Move(3,2,0,0,0,0,0,0,0,0,0,3,1);
  moves["tide"] = Move(0,0,0,0,1,0,2,1,0,0,0,3,1);
  moves["prism"] = Move(1,1,1,1,1,1,1,1,1,1,1,4,1);
  //ignore this
  Player p1(1,1), p2(1,2);
  //special moves
  moves["arrow"] = Move(1, &p1, &p2);
  moves["backstab"] = Move(2, &p1, &p2);
  moves["metallize"] = Move(3, &p1, &p2);
  moves["blade_flurry"] = Move(4, &p1, &p2);
  moves["blade_storm"] = Move(5, &p1, &p2);
  moves["accelerate"] = Move(6, &p1, &p2);
  
  string moveNames[47] = {"sword","multisword","cannon","superstrike","reaper","cheat_codes","gas","timeout","dispel","zap","intoxicate","duodispel","fireball","stun","asian_slap","hax","micronuke","blizzard","asphyxiate","deep_freeze","parry_this","cleanse","multicleanse","heal","strength","shield","tankify","regenerate","guard","dodge","mega_shield","mega_heal","mega_tankify","restore","divinity","metacleanse","vampirize","tide","prism","arrow","backstab","metallize","secret_move","blade_flurry","blade_storm", "accelerate","scorebank"};

  Player* user = &p1;
  Player* target = &p2;
  int streak = 0, hp, luck, gamemode = 1;
  cout << "Welcome to Chance! \n";
  cout << "Enter Player HP: \n";
  cin >> hp;
  if (hp < 0)
    hp = 20;
  cout << "\nEnter Luck: (Default Value is 10)\n";
  cin >> luck;
  cout << "\nEnter Gamemode: (1 for standard, 2 for draft)\n";
  cin >> gamemode;
  if (gamemode == 0)
    gamemode = 1;

  p1.sethp(hp);	
  p2.sethp(hp);
  p1.ds.clear();
  p2.ds.clear();
  p1.accelerate = 2;
  p2.accelerate = 2;
  string uinput = "";
  bool moving = false;
  bool dc;
  int dsize;
  if (gamemode == 2) {   
    int drafting;
    string *moveCheck;
    cout << "\nEnter Number of Moves for Draft:\n";
    cin >> dsize;
    for (int i = 0; i < dsize; i++) {
      drafting = 1;
      while (drafting != 3) {
        cout << "\nEnter Player " + to_string(drafting) + "'s Chosen Move:\n";
        cin >> uinput;
        if (find(begin(moveNames),end(moveNames),uinput) == end(moveNames) && uinput != "moves" && uinput.substr(0,5) != "info_")
          cout << "Invalid Input\n";
        else if (uinput == "moves") {
          for (int k = 1; k < 7; k++) {
          cout << k << "-Streak Moves:\n";
            for (int l = 0; l < sizeof(moveNames)/sizeof(moveNames[0]); l++) {
              if (moves[moveNames[l]].streak == k)
                cout << moveNames[l] << " ";
            }
            cout << "\n";
          }
        } else if (uinput.substr(0,5) == "info_") {
          bool b;
          for (int m = 0; m < sizeof(moveNames)/sizeof(moveNames[0]); m++) {
            if (moveNames[m] == uinput.substr(5)) {
              if (moves[uinput.substr(5)].isspecial) {
                cout << "\nThis is a special move with varying outcomes.\n\n";
                b = true;
                break;
              } else {
                cout << moves[uinput.substr(5)].toString();
                b = true;
                break;
              }
            }
          }
          if (!b)
            cout << "Invalid Input\n";
        } else {
          if (drafting == 1)
            p1.ds.push_back(uinput);
          else
            p2.ds.push_back(uinput);
          drafting++;
        }
      }
    }
  }

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
      dc = gamemode != 2 || (find(begin((*user).ds),end((*user).ds),uinput) != end((*user).ds));
      if (uinput == "s" || uinput == ".") {
        if (rand() % 100 > 50 - luck) {
          streak++;
          cout << "Streak Successful\n";
        } else { 
          cout << "Streak Failed\n";
          moving = false;
        }
      
      } else if(uinput == "moves") {
        if (gamemode != 2) {  
          for (int k = 1; k < 7; k++) {
            cout << k << "-Streak Moves:\n";
            for (int l = 0; l < sizeof(moveNames)/sizeof(moveNames[0]); l++) {
              if (moves[moveNames[l]].streak == k)
                cout << moveNames[l] << " ";
            }
            cout << "\n";
          }
        } else {
            cout << "\nYour Moves:\n";
            for (int i = 0; i < dsize; i++) {
              cout << (*user).ds[i] + " ";
            }
            cout << "\n\nEnemy Moves:\n";
            for (int i = 0; i < dsize; i++) {
              cout << (*target).ds[i] + " ";
            }
            cout << "\n\n";
        }
        cout << "Type \"info_move\" to Get Info About a Move\nex. info_sword\n\n";
      } else if (uinput.substr(0,5) == "info_") {
          bool b;
          for (int m = 0; m < sizeof(moveNames)/sizeof(moveNames[0]); m++) {
            if (moveNames[m] == uinput.substr(5)) {
              
              if (moves[uinput.substr(5)].isspecial) {
                cout << "\nThis is a special move with varying outcomes.\n\n";
                b = true;
                break;
              } else {
                cout << moves[uinput.substr(5)].toString();
                b = true;
                break;
              }
            }
          }
          if (!b)
            cout << "Invalid Input\n";
      } else if (streak > 1 && uinput == "deposit" && (*user).scorebank) {
        (*user).scorebank += floor(streak/2);
        cout << "deposit successful; scorebank now has a balance of " << (*user).scorebank << "\n";
        moving = false;
      } else if (uinput == "deposit" && (*user).scorebank)
        cout << "Insufficient Streak\n";
      else if ((*user).scorebank && uinput == "withdraw") {
        streak += (*user).scorebank;
        (*user).scorebank = 0;
        cout << "withdraw successful; streak is now " << streak << "\n";
      }
      else if (streak > 3 && uinput == "scorebank" && dc) {
        (*user).scorebank = 1;
        cout << "scorebank initialized\n";
        moving = false;
      }
      else if (streak >= moves[uinput].streak && moves[uinput].streak && dc) {
        if (moves[uinput].isspecial) {
          moves[uinput] = Move(moves[uinput].isspecial, user, target);
          cout << moves[uinput].line;
        }
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
          if (moves[uinput].guard) {
            cout << "Player " << (*user).turn << " can guard " << moves[uinput].guard << " time(s)\n";
            (*user).guard += moves[uinput].guard;
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
            else if (moves[uinput].damage <= 3 && (*target).guard) {
              (*target).guard--;
              cout << "Player " << (*target).turn << " guarded the attack\n";
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
      } else if (moves[uinput].streak && dc) //end of input
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
    cout << "\nDraw\n";
  else if (p1.hp > 0)
    cout << "\nPlayer 1 Wins\n";
  else
    cout << "\nPlayer 2 Wins\n";
}

int main() {
  game();
  return 0;
}
