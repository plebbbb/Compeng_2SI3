#include "HugeInteger.h"

HugeInteger::HugeInteger(const std::string& val) {
	if (val.length() == 0) throw "Empty input string";

	if (val[0] == '-'){
		negative = true;
	}

	bool zeroflag = true;
	short tmp = 0;


	u_digits = std::vector<short>(val.length()-negative);

	for(int i = negative; i < val.length(); i++){
		if ((tmp = val[i]-48) < 0 || tmp > 9) throw "invalid character";
		u_digits.emplace_back(tmp);
	}

	prune_L_zeros();
}

HugeInteger::HugeInteger(){
	u_digits = std::vector<short>();
}

HugeInteger::HugeInteger(int n) {
	if (n < 1) throw "out of bounds";
	negative = false;

	u_digits = std::vector<short>();
	u_digits.reserve(n);

	u_digits.emplace_back(1 + (rand() % 9));

	for (int i = 1; i < n; i++){
		u_digits.emplace_back(rand() % 10);
	}
}

void HugeInteger::prune_L_zeros(){
	int prune_len = 0;
	for (short a : u_digits){
		if (a != 0) break;
		prune_len++;
	}
	if (prune_len == u_digits.size()) negative = false;
	u_digits.erase(u_digits.begin(), u_digits.begin()+(prune_len - ((prune_len == u_digits.size()) ? 1 : 0)));
	u_digits.shrink_to_fit();
}

HugeInteger HugeInteger::add_same_sign(const HugeInteger& h) {
	HugeInteger output;

	unsigned int u_sz = u_digits.size() + b10_mag;
	unsigned int h_sz = h.u_digits.size() + h.b10_mag;

	unsigned int u_rsz = u_digits.size();
	unsigned int h_rsz = h.u_digits.size();

	unsigned int iter_count = ((u_sz > h_sz) ? u_sz : h_sz);

	output.u_digits.reserve(iter_count+1);

	short cur_d = 0;
	short carry = 0;

	int u_ind;
	int h_ind;
	for (int i = 0; i < iter_count; i++){
		u_ind = u_sz-i-1;
		h_ind = h_sz-i-1;


		cur_d = carry + (((u_ind < u_rsz) && (u_ind >= 0)) ? (u_digits.at(u_ind)) : 0) + (((h_ind < h_rsz) && (h_ind >= 0))? (h.u_digits.at(h_ind)) : 0);
	
		if (cur_d > 9){
			carry = 1;
		} else {
			//output.u_digits.emplace(output.u_digits.end()-i, abs(cur_d));
			output.u_digits.emplace(output.u_digits.end()-i, abs(cur_d));
			carry = 0;
			continue;
		}
		output.u_digits.emplace(output.u_digits.end()-i, abs(cur_d)-10);
	}

	switch(carry){
		case 0:
			output.u_digits.emplace(output.u_digits.begin(),0);
			break;
		case 1:
			output.u_digits.emplace(output.u_digits.begin(),1);
			//output.negative = false;
			break;
	}
	output.negative = negative;
	output.prune_L_zeros();
	return output;
}

HugeInteger HugeInteger::add_dif_sign(const HugeInteger& h) {
	HugeInteger output;

	const std::vector<short>* b_val;
	const std::vector<short>* l_val;

	unsigned int u_b10_mag;
	unsigned int h_b10_mag;


	bool digit_v_inv = false;

	if (compareToABS(h) < 0){
		b_val = &(h.u_digits);
		l_val = &(u_digits);
		u_b10_mag = h.b10_mag;
		h_b10_mag = b10_mag;
		digit_v_inv = true;
	} else {
		b_val = &(u_digits);
		l_val = &(h.u_digits);
		h_b10_mag = h.b10_mag;
		u_b10_mag = b10_mag;
	}

	unsigned int u_sz = b_val->size() + u_b10_mag;
	unsigned int h_sz = l_val->size() + h_b10_mag;

	unsigned int u_rsz = b_val->size();
	unsigned int h_rsz = l_val->size();

	short sub_flipside = (u_sz >= h_sz) ? 1 : -1;

	unsigned int iter_count = ((u_sz > h_sz) ? u_sz : h_sz);

	output.u_digits.reserve(iter_count+1);

	short cur_d = 0;
	short carry = 0;

	int u_ind;
	int h_ind;
	for (int i = 0; i < iter_count; i++){
		u_ind = u_sz-i-1;
		h_ind = h_sz-i-1;

		cur_d = carry + (((u_ind < u_rsz) && (u_ind >= 0)) ? (b_val->at(u_ind)) : 0) - (((h_ind < h_rsz) && (h_ind >= 0)) ? (l_val->at(h_ind)) : 0);
	
		if (sub_flipside*cur_d < 0){
			carry = -1;
			output.u_digits.emplace(output.u_digits.end()-i, 10-abs(cur_d));
		} else {
			//output.u_digits.emplace(output.u_digits.end()-i, abs(cur_d));
			output.u_digits.emplace(output.u_digits.end()-i, abs(cur_d));
			carry = 0;
		}
	}

	switch(carry){
		case 0:
			output.u_digits.emplace(output.u_digits.begin(),0);
			output.negative = (cur_d >= 0) ? false : true;
			break;
		case -1:
			output.u_digits.emplace(output.u_digits.begin(),1);
			output.negative = true;
			break;
	}
	if (negative) output.negative = !output.negative;
	if (digit_v_inv) output.negative = !output.negative;
	output.prune_L_zeros();

	return output;
}

HugeInteger HugeInteger::add(const HugeInteger& h) {
	HugeInteger output;
	if (negative == h.negative){
		output = add_same_sign(h);
	} else {
		output = add_dif_sign(h);
	}
	return output;
}

HugeInteger HugeInteger::subtract(const HugeInteger& h) {
	HugeInteger output;
	if (negative != h.negative){
		output = add_same_sign(h);
	} else {
		output = add_dif_sign(h);
	}
	return output;
}

HugeInteger HugeInteger::multiply_d10(unsigned short h, int digit_shift, bool neg) {
	HugeInteger output;

	unsigned int u_h = abs(h);

	unsigned int u_sz = u_digits.size();
	unsigned int cur_d;
	unsigned int carry = 0;
	unsigned int u_ind;
	output.negative = negative ^ neg;
	output.b10_mag = digit_shift+b10_mag;
	output.u_digits.reserve(u_sz+1);
	for (int i = 0; i < u_sz; i++){
		u_ind = u_sz-i-1;
		cur_d = carry + u_digits.at(u_ind) * u_h;
		carry = cur_d/10;
	//	std::cout << carry << "\n";
		output.u_digits.emplace(output.u_digits.end()-i, cur_d - carry*10);
	}
	output.u_digits.emplace(output.u_digits.begin(),carry);

	output.prune_L_zeros();
	//std::cout << output.toString() << "\n";
	return output;
}

HugeInteger HugeInteger::multiply(const HugeInteger& h) {
	HugeInteger output("0");
	unsigned int h_sz = h.u_digits.size();
	unsigned int h_ind;
	int count = 0;
	std::vector<HugeInteger> set(10);
	for (int i = 0; i < h_sz; i++){
		h_ind = h_sz-i-1;
		short dig = h.u_digits.at(h_ind);
		if (dig == 0) continue;
		if (set[dig-1].u_digits.size() == 0){
			set[dig-1] = multiply_d10(dig, 0, h.negative);
			//std::cout << "size: " << ++count << "\n";
		}
		set[dig-1].b10_mag = i;
		//std::cout << e.u_digits.size() << "\n";
		output = output.add(set[dig-1]);
	}
	//std::cout << "job done\n"; 
	return output;
}

int HugeInteger::compareTo(const HugeInteger& h) {
	unsigned int u_sz = u_digits.size() + b10_mag;
	unsigned int h_sz = h.u_digits.size() + h.b10_mag;

	if (!negative && h.negative) return 1;
	if (negative && !h.negative) return -1;

	if (u_sz > h_sz) return negative ? -1 : 1;
	if (u_sz < h_sz) return negative ? 1 : -1;

	if(u_sz == h_sz){
		for (int i = 0; i < u_sz; i++){
			if (u_digits.at(i) > h.u_digits.at(i)){
				return negative ? -1 : 1;
			}
			if (u_digits.at(i) < h.u_digits.at(i)){
				return negative ? 1 : -1;			
			} 
		}
	}

	return 0;
}

int HugeInteger::compareToABS(const HugeInteger& h) {
	unsigned int u_sz = u_digits.size() + b10_mag;
	unsigned int h_sz = h.u_digits.size() + h.b10_mag;

	if (u_sz > h_sz) return 1;
	if (u_sz < h_sz) return -1;

	if(u_sz == h_sz){
		for (int i = 0; i < u_sz; i++){
			if (u_digits.at(i) > h.u_digits.at(i)){
				return 1;
			}
			if (u_digits.at(i) < h.u_digits.at(i)){
				return -1;			
			} 
		}
	}

	return 0;
}

std::string HugeInteger::toString() {
	std::string output = negative ? "-" : ""; 
	for (int a : u_digits){
		output += char(a + 48);
	}
	return output;
}
