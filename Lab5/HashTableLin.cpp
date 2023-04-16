#include "HashTableLin.h"
#include <iostream>
#include <cmath>
#include <random>
using namespace std;

HashTableLin::HashTableLin(int maxNum, double load)
{
    load_lim = load;
    t_sz = gen_next_prime(ceil(double(maxNum)/load));
    table = std::vector<int>(t_sz, 0);
}

int HashTableLin::gen_next_prime(int start_pt){
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

void HashTableLin::insert(int n)
{
    if (load_lim < double(num_k+1)/double(t_sz)) rehash();
    int t_ind = n % t_sz;
    while(table[t_ind] != 0){
        if (table[t_ind] == n) return;
        t_ind++;
        t_ind %= t_sz;
    }
    table[t_ind] = n;
    num_k++;
}

int HashTableLin::insertCount(int n)
{
    int ctr = 1;
    if (load_lim < double(num_k+1)/double(t_sz)) rehash();
    int t_ind = n % t_sz;
    while(table[t_ind] != 0){
        if (table[t_ind] == n) return 0;
        t_ind++;
        t_ind %= t_sz;
        ctr++;
    }
    table[t_ind] = n;
    num_k++;
    return ctr;
}

void HashTableLin::rehash()
{
    t_sz = gen_next_prime(t_sz*2);
    std::vector<int> tmp(t_sz, 0);
    for (int n : table){
        if (n == 0) continue;
        int t_ind;
        for (t_ind = n % t_sz; tmp[t_ind] != 0; t_ind = ++t_ind % t_sz);
        tmp[t_ind] = n;
    }
    table = tmp;
}

bool HashTableLin::isIn(int n)
{
    int cnt = 0;
    for (int t_ind = n % t_sz; table[t_ind] != 0 && cnt < t_sz; t_ind = ++t_ind % t_sz){
        cnt++;
        if (table[t_ind] == n) return true; 
    }
    return false;
}

int HashTableLin::isInCount(int n)
{
    int cnt = 1;
    for (int t_ind = n % t_sz; table[t_ind] != 0 && cnt < t_sz; t_ind = ++t_ind % t_sz){
        cnt++;
        if (table[t_ind] == n) return 0; 
    }
    return cnt;
}

void HashTableLin::printKeys()
{
    for (int n : table){
        if (n != 0){
            std::cout << n << std::endl;
        }
    }
}

void HashTableLin::printKeysAndIndexes()
{
    for (int i = 0; i < t_sz; i++){
        if (table[i] != 0){
            std::cout << i << " " << table[i] << std::endl;
        }
    }
}

int HashTableLin::getNumKeys() {
	return num_k;
}

int HashTableLin::getTableSize() {
    return t_sz;
}

double HashTableLin::getMaxLoadFactor() {
	// TODO, change following code after completing this function
    return load_lim;
}

std::vector<double> HashTableLin::simProbeSuccess()
{
    std::random_device rd;
    std::mt19937 ov(rd());
    vector<double> result(9);
    int tgt_cnt = 100000;
    int tries_count = 100;
    for (int i = 1; i < 10; i++){
        double o_avg = 0;
        for(int tries = 0; tries < tries_count; tries++){
            HashTableLin tmp(tgt_cnt, double(i)*0.1);
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

// for bonus
std::vector<double> HashTableLin::simProbeUnsuccess()
{
    std::random_device rd;
    std::mt19937 ov(rd());
    vector<double> result(9);
    int tgt_cnt = 100000;
    int tries_count = 100;
    for (int i = 1; i < 10; i++){
        double o_avg = 0;
        for(int tries = 0; tries < tries_count; tries++){
            HashTableLin tmp(tgt_cnt, double(i)*0.1);
            std::uniform_int_distribution<> gn(0,INT_MAX);
            //insert values
            for(int n = 0; n < tgt_cnt; n++){
                int v = gn(ov);
                if (tmp.insertCount(v) == 0){ //fail case: dupe
                    n--;
                    continue;
                }
            }

            //search values
            for(int k = 0; k < tgt_cnt; k++){
                int v = gn(ov);
                int r_turn = 0;
                if ((r_turn = tmp.isInCount(v)) == 0){ //fail case: found
                    k--;
                    continue;
                }
                o_avg += r_turn;
            }

            o_avg /= tgt_cnt;
            result[i-1] += o_avg;
        }
        result[i-1] /= tries_count;
    }

    return result;
}
