#include "Poly.h"
#include <iostream>
Poly::Poly()
{	
	head = new PolyNode(-1,0,NULL);
}

Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	head = new PolyNode(-1,0,NULL);
	PolyNode* tgt_ptr = head;
	for (int i = 0; i < deg.size(); i++){
		size++;
		tgt_ptr->next = new PolyNode(deg[i], coeff[i], NULL);
		tgt_ptr = tgt_ptr->next;
	}
}

Poly::~Poly()
{
	size = 0;
	PolyNode* tgt_ptr = head->next;
	while(tgt_ptr != NULL){
		PolyNode* del = tgt_ptr;
		tgt_ptr = tgt_ptr->next;
		delete del;
	}
	head->next = NULL;
}

void Poly::addMono(int i, double c)
{
	if (c == 0) return;
	size++;
	PolyNode* tgt_ptr = head;
	while (tgt_ptr->next != NULL && tgt_ptr->next->deg > i){
		tgt_ptr = tgt_ptr->next;
	}
	if(tgt_ptr->next != NULL && tgt_ptr->next->deg == i) {
		tgt_ptr->next->coeff+=c;
		size--;
		if (tgt_ptr->next->coeff == 0){
			size--;
			PolyNode* del = tgt_ptr->next;
			tgt_ptr->next = tgt_ptr->next->next;
			delete del;
		}
	} else {
		PolyNode* n_pos = tgt_ptr->next;
		tgt_ptr->next = new PolyNode(i,c,n_pos);
	}
}

void Poly::addPoly(const Poly& p)
{
	for (PolyNode* i = p.head->next; i != NULL; i = i->next){
		addMono(i->deg, i->coeff);
	}
}

void Poly::multiplyMono(int i, double c)
{
	PolyNode* Lnode = head;
	for (PolyNode* k = head->next; k != NULL; k = k->next){
		k->coeff*=c;
		k->deg+=i;
		if (k->coeff == 0){
			size--;
			PolyNode* del = k;
			Lnode->next = k->next;
			delete del;
			k=Lnode; //backshift by one to ensure every term is covered after deletion
		}
		Lnode = k;
	}
}

void Poly::multiplyPoly(const Poly& p)
{
	Poly sum = Poly();
	Poly tmp = Poly();
	for (PolyNode* i = p.head->next; i != NULL; i = i->next){
		duplicate(tmp);
		tmp.multiplyMono(i->deg, i->coeff);
		sum.addPoly(tmp);
	}
	sum.duplicate(*this);
}

void Poly::duplicate(Poly& outputPoly)
{
	outputPoly.~Poly();
	outputPoly.addPoly(*this);

}

int Poly::getDegree()
{
	if (head->next == NULL) return -1;
	return head->next->deg;//change this after completing this function
}

int Poly::getTermsNo()
{
	return size;
}

double Poly::evaluate(double x)
{
	double output = 0;
	for (PolyNode* i = head->next; i != NULL; i = i->next){
		output += i->coeff * pow(x, double(i->deg));
	}
	return output;
}

std::string Poly::toString()
{
	if (size == 0) return "EMPTY";
	std::string output = "degree=" + std::to_string(getDegree()) + " ";
	for (PolyNode* i = head->next; i != NULL; i = i->next){
		output += std::to_string(i->coeff) + "x^" + std::to_string(i->deg) + " ";
	}
	output.pop_back();
	return output;
}
