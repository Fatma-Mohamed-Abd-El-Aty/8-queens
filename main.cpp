#include <iostream>
#include <fstream>
#include "State.h"
#include <queue>
#include "vector"

state init_read_file(std::string file_name){
    std::string str;
    state s;
    int no_queens = 0;
    int no_lines = 0;
    std::ifstream myReadFile;
    myReadFile.open(file_name);
    if (myReadFile.is_open()) {
        std::cout << "Opening the file\n";
        while (!myReadFile.eof()) {
            getline(myReadFile,str);
            //std::cout << str;
            for(int i = 0; i < str.length(); i++){
                if('Q' == str.at(i)){
                    s.queens[no_queens][0] = no_lines;
                    s.queens[no_queens][1] = i -(i/2);
                    no_queens++;
                }
            }
            no_lines++;
        }
    }
    else {
        std::cout << "Can't open the file";
    }
    myReadFile.close();
    return s;
}

state assign (state s){
    state s1;
    for(int i = 0; i < 8; i++) {
        s1.queens[i][0] = s.queens[i][0];
        s1.queens[i][1] = s.queens[i][1];
    }
    s1.cost = s.cost;
    s1.cost_f = s.cost_f;
    return s1;
}


bool find (state s, int n, int m){
    for(int i = 0; i < 8; i++){
        if((s.queens[i][0] == n)&&(s.queens[i][1] == m)){
            return true;
        }
    }
    return false;
}



int calculate_cost_function(state s){
    int cost_f = 0;
    for(int i = 0; i < 8; i++){
        for(int j = i+1; j < 8; j++){
            if(s.queens[i][0] == s.queens[j][0]){
                cost_f ++;
            }
            if (s.queens[i][1] == s.queens[j][1]){
                cost_f ++;
            }
            if (abs(s.queens[i][0]-s.queens[j][0]) == abs(s.queens[i][1] - s.queens[j][1])){
                cost_f ++;
            }
        }
    }
    return cost_f;
}


state calculate_best_neighbour(state s){
    state neighbour;
    s.cost_f = calculate_cost_function(s);
    int min_cost = s.cost_f;
    bool found = false;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            state n;
            n = assign(s);
            n.cost = s.cost + 1;
            bool changed = false;
            if((j == 0)&&(n.queens[i][1] > 0)){
                n.queens[i][1]--;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if((j == 1)&&(n.queens[i][1] < 7)){
                n.queens[i][1]++;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if((j == 2)&&(n.queens[i][0] > 0)){
                n.queens[i][0]--;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if((j == 3)&&(n.queens[i][0] < 7)){
                n.queens[i][0]++;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if((j == 4)&&(n.queens[i][0] > 0)&&(n.queens[i][1] > 0)){
                n.queens[i][0]--;
                n.queens[i][1]--;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if((j == 5)&&(n.queens[i][0] < 7)&&(n.queens[i][1] < 7)){
                n.queens[i][0]++;
                n.queens[i][1]++;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if((j == 6)&&(n.queens[i][0] > 0)&&(n.queens[i][1] < 7)){
                n.queens[i][0]--;
                n.queens[i][1]++;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if((j == 7)&&(n.queens[i][0] < 7)&&(n.queens[i][1] > 0)){
                n.queens[i][0]++;
                n.queens[i][1]--;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if(changed){
                n.cost_f = calculate_cost_function(n);
                if(n.cost_f < min_cost){
                    min_cost = n.cost_f;
                    neighbour = assign(n);
                    found = true;
                }
                else if (n.cost_f == min_cost){
                    min_cost = n.cost_f;
                    neighbour = assign(n);
                    found = true;
                }
            }
        }
    }
    if(!found) {
        neighbour = assign(s);
    }
    return neighbour;
}

state generateRandomState() {
    state s;
    int q[8][2];
    for(int i=0;i<8;i++){
        for(int j=0;j<2;j++){
            s.queens[i][j] = std::rand() % 8;
        }
    }
    s.cost=0;
    s.cost_f=calculate_cost_function(s);
    //std::cout << s.cost_f<<std::endl;
    return s;
}

void hill_climbing(state initial_state){
    clock_t start = clock();
    state best_neighbour ;
    int expanded_nodes = 0;
    best_neighbour= calculate_best_neighbour(initial_state);
    while(calculate_cost_function(best_neighbour) !=0) {
        initial_state=generateRandomState();
        best_neighbour= calculate_best_neighbour(initial_state);
        while (1) {
            if (calculate_cost_function(best_neighbour) == calculate_cost_function(initial_state)) {
                break;
            }
            expanded_nodes++;
            //std::cout << calculate_cost_function(best_neighbour) << std::endl;
            initial_state = assign(best_neighbour);
            best_neighbour = calculate_best_neighbour(initial_state);
        }
    }
    clock_t end = clock();
    std::cout<<"time "<< (end - start)/( CLOCKS_PER_SEC / 1000 ) << " ms\n";
    std::cout<<"Cost : "<< best_neighbour.cost<<"\n";
    std::cout<< "no_expanded_nodes = " << expanded_nodes << "\n";
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(find(best_neighbour,i,j)){
                std::cout<<"Q ";
            }
            else {
                std::cout<<"# ";
            }
        }
        std::cout <<"\n";
    }

}

struct arr{
    int array[8];
    int cost = 0;
    int cost_f = 0;
};


int calculate_cost_f(arr arr1){
    int cost_f = 0;
    for(int i = 0; i < 8; i++){
        for(int j = i+1; j < 8; j++){
            if(arr1.array[i] == arr1.array[j]){
                cost_f++;
            }
            if(abs(i-j) == abs(arr1.array[i] - arr1.array[j])){
                cost_f++;
            }
        }
    }
    return cost_f;
}

struct Compare_cost_f{
    bool operator()(arr const& a1, arr const& a2)
    {
        return a1.cost_f > a2.cost_f;
    }
};



void GA() {
    clock_t start = clock();
    arr random_arr;
    int no_expanded_nodes = 0;
    std::priority_queue<arr, std::vector<arr>, Compare_cost_f> p_queue;
    int l = 0;
    while (l < 50) {
        for (int i = 0; i < 8; i++) {
            random_arr.array[i] = std::rand() % 8;
        }
        random_arr.cost = 1;
        random_arr.cost_f = calculate_cost_f(random_arr);
        p_queue.push(random_arr);
        l++;
    }

    do {
        no_expanded_nodes = no_expanded_nodes + 2;
        if(p_queue.size() == 600){
            while (!p_queue.empty()){
                p_queue.pop();
            }
            int l = 0;
            while (l < 50) {
                for (int i = 0; i < 8; i++) {
                    random_arr.array[i] = std::rand() % 8;
                }
                random_arr.cost = 1;
                random_arr.cost_f = calculate_cost_f(random_arr);
                p_queue.push(random_arr);
                l++;
            }
        }
        int random1 = (std::rand() % 50) / 10;
        int random2 = (std::rand() % 50) / 10;
        while (random2 == random1) {
            random2 = (std::rand() % 50) / 10;
        }
        //std::cout << random1 << "\n";
        //std::cout << random2 << "\n";
        int large_rand;
        std::vector<arr> temp;
        if (random1 > random2) {
            large_rand = random1;
        } else {
            large_rand = random2;
        }
        arr rand1;
        arr rand2;
        for (int i = 0; i < large_rand + 1; i++) {
            if (i == random1) {
                rand1 = p_queue.top();
                p_queue.pop();
            } else if (i == random2) {
                rand2 = p_queue.top();
                p_queue.pop();
            } else {
                temp.push_back(p_queue.top());
                p_queue.pop();
            }
        }
        while(!temp.empty()){
            p_queue.push(temp.back());
            temp.pop_back();
        }
        int random_point = std::rand() % 8;
        //std::cout << random_point;
        arr child_arr1;
        arr child_arr2;
        for (int i = 0; i < random_point; i++) {
            child_arr1.array[i] = rand2.array[i];
            child_arr2.array[i] = rand1.array[i];
        }
        for (int i = random_point; i < 8; i++) {
            child_arr1.array[i] = rand1.array[i];
            child_arr2.array[i] = rand2.array[i];
        }
        child_arr1.cost_f = calculate_cost_f(child_arr1);
        child_arr2.cost_f = calculate_cost_f(child_arr2);
        child_arr1.cost = rand1.cost + 1;
        child_arr2.cost = rand1.cost + 1;
        p_queue.push(child_arr1);
        p_queue.push(child_arr2);
        p_queue.push(rand1);
        p_queue.push(rand2);
//        std::cout<<p_queue.size()<<"\n";
//        std::cout<<"\n"<<p_queue.top().cost_f<<"\n";
    }while (p_queue.top().cost_f != 0);
    //std::cout<<"ennnnnnnnnnnnnnd";
    clock_t end = clock();
    arr final = p_queue.top();
    std::cout<<"time "<< (end - start)/( CLOCKS_PER_SEC / 1000 ) << " ms\n";
    std::cout<<"Cost : "<<final.cost<<"\n";
    std::cout<<"no_expanded_nodes = "<<no_expanded_nodes<<"\n";
    for(int n = 0; n < 8; n++){
        for(int m = 0; m < 8; m++){
            if(m == final.array[n]){
                std::cout<< "Q ";
            }
            else{
                std::cout<< "# ";
            }
        }
        std::cout<<"\n";
    }

}


struct Compare_beam_cost_f{
    bool operator()(state const& s1, state const& s2)
    {
        return s1.cost_f > s2.cost_f;
    }
};

std::vector<state> calculate_best_k_neighbours(state s, int k){
    s.cost_f = calculate_cost_function(s);
    std::priority_queue<state, std::vector<state>, Compare_beam_cost_f> p_queue;
    for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            state n;
            n = assign(s);
            n.cost = s.cost + 1;
            bool changed = false;
            if ((j == 0) && (n.queens[i][1] > 0)) {
                n.queens[i][1]--;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if ((j == 1) && (n.queens[i][1] < 7)) {
                n.queens[i][1]++;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if ((j == 2) && (n.queens[i][0] > 0)) {
                n.queens[i][0]--;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if ((j == 3) && (n.queens[i][0] < 7)) {
                n.queens[i][0]++;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if ((j == 4) && (n.queens[i][0] > 0) && (n.queens[i][1] > 0)) {
                n.queens[i][0]--;
                n.queens[i][1]--;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if ((j == 5) && (n.queens[i][0] < 7) && (n.queens[i][1] < 7)) {
                n.queens[i][0]++;
                n.queens[i][1]++;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if ((j == 6) && (n.queens[i][0] > 0) && (n.queens[i][1] < 7)) {
                n.queens[i][0]--;
                n.queens[i][1]++;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }
            if ((j == 7) && (n.queens[i][0] < 7) && (n.queens[i][1] > 0)) {
                n.queens[i][0]++;
                n.queens[i][1]--;
                changed = !find(s, n.queens[i][0], n.queens[i][1]);
            }if(changed){
                n.cost_f = calculate_cost_function(n);
                    p_queue.push(n);
            }
        }
    }
    std::vector<state> best_neighbours;
    for(int i = 0; ((i < k)&&(!p_queue.empty())); i++){
        best_neighbours.push_back(p_queue.top());
        p_queue.pop();
    }
    return best_neighbours;
}

void beam_search(state initial_state){
    clock_t start = clock();
    int no_expanded_nodes = 1;
    int k = 6;
    int min = 0;
    std::vector<state> neighbours;
    //while (k < 5){
        std::priority_queue<state, std::vector<state>, Compare_beam_cost_f> p_queue;
        state s = initial_state;
        std::vector<state> n;
        s.cost_f = calculate_cost_function(s);
        min = s.cost_f;
        n = calculate_best_k_neighbours(s,k);
        for(int i = 0; (!n.empty())&&(i < k); i++){
            p_queue.push(n.back());
            n.pop_back();
        }
        while ((0 != p_queue.top().cost_f)&&(!p_queue.empty())){
            no_expanded_nodes++;
            min = p_queue.top().cost_f;
            //std::cout<< "heeeeeeeeeeeeeere";
            //std::cout << min << "\n";
            for (int i = 0; i < p_queue.size(); i++) {
                std::vector<state> temp;
                state top = p_queue.top();
                p_queue.pop();
                temp = calculate_best_k_neighbours(top, k);
                while (!temp.empty()) {
                    neighbours.push_back(temp.back());
                    temp.pop_back();
                }
            }
            for (int i = 0;i < neighbours.size(); i++) {
                p_queue.push(neighbours.back());
                neighbours.pop_back();
            }
            neighbours.clear();
            std::vector<state> t ;
            for(int i = 0; (i < k)&&(!p_queue.empty()); i++) {
                t.push_back(p_queue.top());
                p_queue.pop();
            }
            while((!t.empty())&&(!p_queue.empty())){
                p_queue.pop();
            }
            while(!t.empty()){
                p_queue.push(t.back());
                t.pop_back();
            }
        }
        clock_t end = clock();
        state print = p_queue.top();
        std::cout<<"time "<< (end - start)/( CLOCKS_PER_SEC / 1000 ) << " ms\n";
        std::cout<<"Best_K : "<<k<<"\n";
        std::cout<<"Cost : "<<print.cost<<"\n";
        std::cout<<"no_expanded_nodes = "<<no_expanded_nodes<<"\n";
        //std::cout<< print.cost_f <<"\n";
        while(p_queue.top().cost_f == 0){
            p_queue.pop();
            for(int i = 0; i < 8; i++){
                for(int j = 0; j < 8; j++){
                    if(find(print,i,j)){
                        std::cout<<"Q ";
                    }
                    else {
                        std::cout<<"# ";
                    }
                }
                std::cout <<"\n";
            }
            print = p_queue.top();
        }
        //k++;
    //}
}

int calculate_attack(state s,int n_q){
    int attacks = 0;
    for(int i = 0; i < 8; i++){
        if((s.queens[n_q][0] == s.queens[i][0])&&(i != n_q)){
            attacks ++;
        } else if ((s.queens[n_q][1] == s.queens[i][1])&&(i != n_q)){
            attacks ++;
        } else if((abs(s.queens[n_q][1] - s.queens[i][1]) == abs(s.queens[n_q][0] - s.queens[i][0]))&&(i != n_q)){
            attacks ++;
        }
    }
    return attacks;
}


void CSP(state initial_state){
    state s = initial_state;
    s.cost_f = calculate_cost_function(s);
    std::vector<int> attacks;
    for(int i = 0; i < 8; i++){
        attacks.push_back(calculate_attack(s,i));
        //std::cout << attacks.back();
    }

}

int main(){
    srand(time(0));
    state initial_state = init_read_file("Sample_Input.txt");
    std::cout<<"Hill_climbing : \n";
    hill_climbing(initial_state);
    std::cout<<"\n";
    std::cout<<"Beam_Search : \n";
    beam_search(initial_state);
    std::cout<<"\n";
    std::cout<<"GA : \n";
    GA();
//    CSP(initial_state);
    return 0;
}
