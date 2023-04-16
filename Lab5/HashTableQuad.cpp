#include "HashTableQuad.h"
#include <iostream>
#include <cmath>
#include <random>
using namespace std;

HashTableQuad::HashTableQuad(int maxNum, double load)
{
    load_lim = load;
    t_sz = gen_next_prime(ceil(double(maxNum)/load));
    table = std::vector<int>(t_sz, 0);
}

int HashTableQuad::gen_next_prime(int start_pt){
    if (start_pt <= 3) return start_pt;
    start_pt+= (start_pt % 2) ? 0 : 1; //evens cant be prime, auto +1 if t_sz is even
    bool isnt_prime = true;
    while (isnt_prime){
        isnt_prime = false;
        for (int i = 2; i <= sqrt(start_pt); i++){
            if (start_pt % i == 0) {
                isnt_prime = true; 
                start_pt+=2; //only odds are prime, t_sz is gauranteed to be odd
                break; 
            }
        }
    }
    return start_pt;
}

void HashTableQuad::insert(int n)
{
    int t_ind = n % t_sz;
    int cnt = 0;
    while(table[(t_ind+cnt*cnt) % t_sz] != 0 && cnt < t_sz){
        if (table[(t_ind+cnt*cnt) % t_sz] == n) return;
        cnt++;
    }
    if(table[(t_ind+cnt*cnt) % t_sz] == 0){
        table[(t_ind+cnt*cnt) % t_sz] = n;
        num_k++;
    }
    if (load_lim < double(num_k)/double(t_sz)) rehash();

}

int HashTableQuad::insertCount(int n)
{
    int t_ind = n % t_sz;
    int ctr = 1;
    int cnt = 0;
    while(table[(t_ind+cnt*cnt) % t_sz] != 0 && cnt < t_sz){
        if (table[(t_ind+cnt*cnt) % t_sz] == n) return 0;
        cnt++;
        ctr++;
    }
    if(table[(t_ind+cnt*cnt) % t_sz] == 0){
        table[(t_ind+cnt*cnt) % t_sz] = n;
        num_k++;
    } else return 0;
   // if (load_lim < double(num_k)/double(t_sz)) rehash();
    return ctr;
}

void HashTableQuad::rehash()
{
    t_sz = gen_next_prime(t_sz*2);
    std::vector<int> tmp(t_sz, 0);
    for (int n : table){
        if (n == 0) continue;
        int t_ind = n % t_sz;
        int cnt = 0;
        for (cnt = 0; tmp[(t_ind+cnt*cnt) % t_sz] != 0 && cnt < t_sz; cnt++);
        if (tmp[(t_ind+cnt*cnt) % t_sz] != 0) num_k--;
        else tmp[(t_ind+cnt*cnt) % t_sz] = n;
    }
    table = tmp;
}

bool HashTableQuad::isIn(int n)
{
    int t_ind = n % t_sz;
    for (int cnt = 0; table[(t_ind+cnt*cnt) % t_sz] != 0 && cnt < t_sz; cnt++){
        if (table[(t_ind+cnt*cnt) % t_sz] == n) return true; 
    }
    return false;
}

void HashTableQuad::printKeys()
{
    for (int n : table){
        if (n != 0){
            std::cout << n << std::endl;
        }
    }
}

void HashTableQuad::printKeysAndIndexes()
{
    for (int i = 0; i < t_sz; i++){
        if (table[i] != 0){
            std::cout << i << " " << table[i] << std::endl;
        }
    }
}

int HashTableQuad::getNumKeys() {
	return num_k;
}

int HashTableQuad::getTableSize() {
    return t_sz;
}

double HashTableQuad::getMaxLoadFactor() {
	// TODO, change following code after completing this function
    return load_lim;
}


std::vector<double> HashTableQuad::simProbeSuccess()
{
    std::random_device rd;
    std::mt19937 ov(rd());
    vector<double> result(9);
    int tgt_cnt = 100000;
    int tries_count = 100;
    for (int i = 1; i < 10; i++){
        double o_avg = 0;
        for(int tries = 0; tries < tries_count; tries++){
            HashTableQuad tmp(tgt_cnt, double(i)*0.1);
            std::uniform_int_distribution<> gn(0,INT_MAX);
            //insert values
            for(int n = 0; n < tgt_cnt; n++){
                int r_turn = 0;
                int v = gn(ov);
                if ((r_turn = tmp.insertCount(v)) == 0){
                    n--;
                    continue;
                }
                //cout << " v" <<  v << " ";
                o_avg+= r_turn;
            }
            //  cout << "\n" << result[i-1] << "\n";

            o_avg /= tgt_cnt;
            result[i-1] += o_avg;
            //cout << "\n" << result[i-1] << "\n";
        }
        result[i-1] /= tries_count;
    }

    return result;
}

