/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "agent.h"

movement::movement(const string& ll,const int& nn,const int& f,const int& m,const int& d,const int& mt,const bool& sm, const int& mvn):
                                                            l(ll),n(nn),fd(f),md(m),d2e(d),mtype(mt),scoreMove(sm),mn(mvn){} 
movement::movement(const movement& copy):l(copy.l),n(copy.n),fd(copy.fd),md(copy.md),d2e(copy.d2e),mtype(copy.mtype),scoreMove(copy.scoreMove),mn(copy.mn){} 

agent::agent(const char& c):symbol(c){
    for(int n=1;n<4;n++){  // number of marbles
        for(int fd=0;fd<6;fd++){// variations in following directions
            if(n==1){//will create 6 moves for n=1 (inline)
                vector<int> tv;
                tv.push_back(n);
                tv.push_back(fd);
                tv.push_back(getAdjacentIndex(fd));
                pmd.push_back(tv);
            }
            else{//will create a total of 60 moves for n=2 and n=3
                for(int md=0;md<6;md++){ // variations in moving directions
                    if(md!=fd){
                        vector<int> tv;
                        tv.push_back(n);
                        tv.push_back(fd);
                        tv.push_back(md);
                        pmd.push_back(tv);
                    }
                }
            }
        }
    }
}
vector<movement> agent::findMoves(const board& bd, const int& moveNumber) const{ //now accepts move number
    vector<movement> mv;
    map<string,cell*> cells = bd.getCells();
    for(map<string,cell*>::iterator it=cells.begin();it!=cells.end();it++){
        string l = it->first;
        cell* cp = it->second;
        if(cp!=nullptr && cp->getMarble()==symbol){ // only interested to cells that belong to us (will also cater for the #)
            set<int> mdSet;//directions in which neighbours either have a space or an opponent marble
            for(int md=0;md<6;md++){
                cell* np = cp->getAdjacent(md);
                if(np!=nullptr && np->getMarble()!=symbol && np->getMarble() != '#'){
                    mdSet.insert(md);
                }
            }
            for(vector<int> tv:pmd){ // finding matches from pmd that are also in mdset and are valid moves
                int md=tv[2];
                if(mdSet.find(md)!=mdSet.end()){ 
                    int mtype = 0;//default to in-line with no following marbles
                    bool scoreMove = false;
                    if(bd.validateMove(symbol,l,tv[0],tv[1],md,mtype,scoreMove))
                        mv.push_back(movement(l,tv[0],tv[1],md,cp->getDistanceToEdge(md),mtype,scoreMove, moveNumber)); //now accepts move number
                }
            }
        }
    }
    return mv;
}
double scoreOld(const movement& a){
    double maxScore = 0;
    if(a.mtype<0) // broadside
        maxScore = 0.6;
    else if(a.mtype>0) // a pushing inline
        maxScore = 0.2;
    else
        maxScore = 0.8; // just an inline
    if(a.mtype>0){ // if it is an attacking move we want to prio moves at the egde
        return maxScore*(1+(1-a.d2e)/9.0);  // [~0.11,1] = [far from the edge,close to the edge]   ---- not correct
    }
    else{ // broad side or none pushing inline
        return maxScore*(1-abs(5-a.d2e)/5.0); // prioritizing broadsides to the center of the board
    }
}

double score1(const movement& a){
     int intScore = 1;
    if(a.scoreMove){
        intScore = 0.1;
    }
    double maxScore = 0;
    if(a.mtype<0) // broadside
        maxScore = 0.6;
    else if(a.mtype>0) // a pushing inline
        maxScore = 0.2;
    else
        maxScore = 0.8; // just an inline
    if(a.mtype>0){ // if it is an attacking move we want to prio moves at the egde
        return (maxScore*(1+(a.d2e-9)/9.0)) * intScore;  // [~0.11,1] = [far from the edge,close to the edge] now correct
    }
    else{ // broad side or none pushing inline
        return maxScore*(1-abs(5-a.d2e)/5.0); // prioritizing broadsides to the center of the board
    }
}

double score(const movement& a){
    int intScore = 1;
    if(a.scoreMove){
        intScore = 0.01;
    }
    double maxScore = 0;
    if(a.mn<30){ // now takes into account the move number, ealry times move broadside and inline towards the center
        if(a.mtype<0) // broadside
        maxScore = 0.2;
        else if(a.mtype>0) // a pushing inline
            maxScore = 0.8;
        else
            maxScore = 0.4; // just an inline
    }
    else{
        if(a.mtype<0) // broadside
        maxScore = 0.6;
        else if(a.mtype>0) // a pushing inline
            maxScore = 0.1;
        else
            maxScore = 0.8; // just an inline
    }
    
    if(a.mtype>0){ // if it is an attacking move we want to prio moves at the egde
        return (maxScore*(1+(a.d2e-9)/9.0)) * intScore;  // [~0.11,1] = [far from the edge,close to the edge] takes into account that its a scoring move
    }
    else{ // broad side or none pushing inline
        return maxScore*(1-abs(5-a.d2e)/5.0); // prioritizing broadsides to the center of the board
    }
}

randAgent::randAgent(const char& c):agent(c){}
movement randAgent::move(const movement* const om, board bd, const int& moveNumber){ //now accepts move number
    vector<movement> mv = findMoves(bd, moveNumber);
    if(mv.size()==0){
        throw string("Error! There are no available moves!");
    }
    int i=rand()%mv.size();
    return mv[i];
}
distAgent::distAgent(const char& c):agent(c){}

movement distAgent::move(movement const* const om,board bd, const int& moveNumber){ //now accepts move number
    vector<movement> mv = findMoves(bd, moveNumber);
    sort(mv.begin(), mv.end(), moveTNDPriority());
    const int bm2c = 3;//consider only these top best moves.
    if(mv.size()==0){
        throw string("Error! There are no available moves!");
    }
    if(mv.size()<bm2c)
        return mv[0];
    else if(moveNumber>100){ // late in the game you want to play aggressively, so you favour only the best move which will most likey be a pushing move
        return mv[0];
    }
    else{ // randomly chooses the move from the best 3 moves in the vector
        int i=rand()%bm2c;
        return mv[i];
    }
}
