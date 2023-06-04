/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: MSCS
 *
 * Created on 12 March 2020, 8:38 AM
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>
#include <time.h>
#include "board.h"
#include "agent.h"

using namespace std;

int main(int argc, char** argv) {
        ofstream roundsFile("roundsResultsData.txt");
        for(int r=0; r<5; r++){
            cout<<"---------Round-"+to_string(r+1)+"----------------"<<endl;
            time_t now = time(NULL);
            srand(now);
            cout<<"Seed: "<<now<<endl;
            board abalone;
            //queue<board> gameStates;
            //gameStates.push(abalone);
            string state(abalone);
            cout<<"Initial State:"<<endl<<state;
            agent* w = new randAgent('O');
            agent* b = new distAgent('@');
            movement* pom=nullptr;//previous opponent's move
            movement* mym=nullptr;//player's (whoever is playing) chosen move
            char c='O'; // represents who is playing

            /*
                while none of the players have their number of marbles off the board == 6
            */
            int i=0; 
            ofstream sessionFile("game-"+to_string(r+1)+".txt");
            while(abalone.inPlay()){
                try{
                    if(c=='O'){  // white moves
                        movement mm = w->move(pom,abalone,i+1);
                        mym = new movement(mm);
                        if(pom)
                            delete pom;
                        pom = new movement(mm);
                    }
                    else{ // black moves
                        movement mm = b->move(pom,abalone,i+1);
                        mym = new movement(mm);
                        if(pom)
                            delete pom;
                        pom = new movement(mm);
                    }
                }
                catch(const string& s){
                    cout<<s;
                    return 1;
                }
                bool valid=abalone.executeMove(c,mym->l,mym->n,mym->fd,mym->md);
                cout<<"Move "<<i+1<<": "<<c<<","<<mym->l<<","<<mym->n<<","<<mym->fd<<","<<mym->md<<endl;
                i++;
                if(valid){
                    string state(abalone);
                    //gameStates.push(abalone);
                    cout<<"Moves: "<<i<<endl;
                    cout<<"Next State:"<<endl<<state;
                    sessionFile<<"Move "<<i<<","<<c<<","<<mym->l<<","<<mym->n<<","<<mym->fd<<","<<mym->md<<endl;
                }
                else{
                    cout<<"Invalid move!!"<<endl;
                }
                if(c=='O')
                    c='@';
                else
                    c='O';
                delete mym;
                SLP(1);
                //system(CL);
            }
            // myFile<<"*******************MOVES WE DID********************"<<endl;
            // for(int i=0;i<=10;i+=2){
            //     myFile<<"Move: "<<i<<endl;
            //     string state(gameStates.front());
            //     string state1(gameStates.front());
            //     myFile<<"Next State:"<<endl;
            //     myFile<<state<<""<<state1<<endl;
            //     gameStates.pop();
            // }
            string resultState(abalone);
            roundsFile<<"Round "<<r+1<<": "<<abalone.whoWon()<<" won in "<<i+1<<" total moves."<<"\nLast board state:\n"<<resultState<<endl;

            if(pom)
                delete pom;
            delete w;
            delete b;
            sessionFile.close();

        }
        roundsFile.close();
        return 0;
}

