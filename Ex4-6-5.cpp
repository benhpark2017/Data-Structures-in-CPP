/*********************************main.cpp*************************************/
#include <iostream>
#include "Polynomial.h"

int main() {
    Polynomial poly1, poly2, poly3;
    Polynomial sum, difference, product;

    std::cout << "Enter the first polynomial (format: number_of_terms coef1 expon1 coef2 expon2 ...): ";
    std::cin >> poly1;
    std::cout << "Polynomial 1: " << poly1 << std::endl;
    
    std::cout << "Enter the second polynomial (format: number_of_terms coef1 expon1 coef2 expon2 ...): ";
    std::cin >> poly2;
    std::cout << "Polynomial 2: " << poly2 << std::endl;
    
    poly3 = poly1;
    std::cout << "Testing assignment operator: " << poly3 << std::endl;
    
    sum = poly1 + poly2;
    std::cout << "Sum: " << sum << std::endl;
    
    difference = poly1 - poly2;
    std::cout << "Difference: " << difference << std::endl;
    
    product = poly1 * poly2;
    std::cout << "Product: " << product << std::endl;    
    
    float x = 2.0;  // Example value for x
    std::cout << "Value of Polynomial 1 at x = " << x << " is: " << poly1.Evaluate(x) << std::endl;
    std::cout << "Value of Polynomial 2 at x = " << x << " is: " << poly2.Evaluate(x) << std::endl;
    std::cout << "Value of Sum Polynomial at x = " << x << " is: " << sum.Evaluate(x) << std::endl;
    std::cout << "Value of Difference Polynomial at x = " << x << " is: " << difference.Evaluate(x) << std::endl;
    std::cout << "Value of Product Polynomial at x = " << x << " is: " << product.Evaluate(x) << std::endl;

    return 0;
}



/********************************ListNode.h************************************/
#ifndef LISTNODE_H
#define LISTNODE_H

template <class Type> class CircList;
template <class Type> class CircListIterator;

template <class Type>
class ListNode {
    friend class CircList<Type>;
    friend class CircListIterator<Type>;

    template <class T>
    friend std::ostream& operator<<(std::ostream& os, const CircList<T>& list);

private:
    Type data;
    ListNode *link;

public:
    ListNode(Type element = 0, ListNode *next = nullptr) : data(element), link(next) {}
};

#endif // LISTNODE_H



/*********************************CircList.h*************************************/
#ifndef CIRCLIST_H
#define CIRCLIST_H

#include <iostream>
#include "ListNode.h"

template <class Type> class CircListIterator;

template <class Type>
class CircList {
    friend class CircListIterator<Type>;

  private:
    ListNode<Type> *last;

  public:
    static ListNode<Type>* av;
    
    CircList() : last(nullptr) {}
    
    ~CircList() {
        if (last != nullptr) {
            ListNode<Type>* current = last->link;
            while (current != last) {
                ListNode<Type>* temp = current;
                current = current->link;
                delete temp;
            }
            delete last;
        }
    }

    void RetNode(ListNode<Type>* x) {
        x->link = av;
        av = x;
    }
    
    ListNode<Type> *GetNode() {
        ListNode<Type>* x;
        if (!av && (av == last) ) x = new ListNode<Type>;
        else if (av == last) { x = av; av = av->link; }
        return x;
    }
    
    ListNode<Type> *getFirst() const {
        if (last == nullptr) return nullptr;
        else return last->link;
    }
    
    ListNode<Type> *getLast() const { return last; }
    
    
    ListNode<Type> *Insert(ListNode<Type> *x, const Type& item) {
        ListNode<Type>* newNode = new ListNode<Type>(item);
        if (last == nullptr) {
            // List is empty
            last = newNode;
            last->link = last;
        } else if (x == nullptr) {
            // Insert at the beginning
            newNode->link = last->link;
            last->link = newNode;
        } else {
            // Insert after x
            newNode->link = x->link;
            x->link = newNode;
            if (x == last) {
                last = newNode;
            }
        }
        return newNode;
    }
    
    void Delete(ListNode<Type> *x) {
        if (last == nullptr) {
            std::cout << "The list is empty" << std::endl;
            return;
        }
        if (last->link == last && last == x) {
            delete last;
            last = nullptr;
            return;
        }
        ListNode<Type> *temp = last->link;
        ListNode<Type> *prev = last;
        do {
            if (temp == x) {
                prev->link = temp->link;
                if (x == last) {
                    last = prev;
                }
                if (x == last->link) {
                    last->link = x->link;
                }
                delete x;
                return;
            }
            prev = temp;
            temp = temp->link;
        } while (temp != last->link);
        std::cout << "Node not found" << std::endl;
    }
    
    void Attach(const Type &item) {
        ListNode<Type>* newNode = new ListNode<Type>(item);
        if (last == nullptr) {
            last = newNode;
            last->link = last;
        } else {
            newNode->link = last->link;
            last->link = newNode;
            last = newNode;
        }
    }

}; //end class CircList

template <class Type>
ListNode<Type>* CircList<Type>::av = nullptr;

template <class Type>
std::ostream& operator<<(std::ostream& os, const CircList<Type>& list);

#endif // CIRCLIST_H



/******************************CircListIterator.h**********************************/
#ifndef CIRCLISTITERATOR_H
#define CIRCLISTITERATOR_H

#include "CircList.h"

template <class Type>
class CircListIterator {
  public:

    CircListIterator(const CircList<Type> &l) : list(l), current(l.getFirst()){}
    
    Type getData() { return current->data; }
    
    bool NotNull() const { return current != nullptr ? true : false; }
    
    bool NextNotNull() const {
        return current != nullptr && current->link != nullptr ? true : false;
    }
    
    Type* First() {
        current = list.getFirst();
        if (current && current->data.expon == -1) {
            current = current->link;
            if (current == list.getFirst()) {
                current = nullptr;
            }
        }
        return current ? &current->data : nullptr;
    }
    
    Type* Next() {
        if (current) {
            current = current->link;
            if (current && 
               (current->data.expon == -1 || current == list.getFirst())) {
                current = nullptr;
            }
            return current ? &current->data : nullptr;
        }
        return nullptr;
    }
    
    bool IsEmpty() const {
        const ListNode<Type>* first = list.getFirst();
        return (first == nullptr) || (first->data.expon == -1 && first->link == first);
    }
    
    ListNode<Type>* CurrentPosition() const { return current; }

  private:
    const CircList<Type> &list;
    ListNode<Type> *current;
};

#endif // CIRCLISTITERATOR_H



/*********************************Polynomial.h*************************************/
#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "CircList.h"
#include "CircListIterator.h"
#include <iostream>
#include <cmath>

struct Term {
    int coef;
    int expon;
    void Init(int c, int e) { coef = c; expon = e; }
};

class Polynomial {
    
    friend std::istream& operator>>(std::istream& is, Polynomial& x) {
        int n, coef, expon;
        Term temp;
    
        x.poly = CircList<Term>();  // Clear existing polynomial

        temp.Init(0, -1);  // Create head node with exponent -1
        x.poly.Attach(temp);

        is >> n;  // Read number of terms

        for (int i = 0; i < n; i++) {
            is >> coef >> expon;
            if (coef != 0) {  // Ignore terms with zero coefficients
                temp.Init(coef, expon);
                x.poly.Attach(temp);
            }
        }
        return is;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        CircListIterator<Term> iter(p.poly);
        const Term* t = iter.First();
    
        if (t == nullptr) {
            os << "0";
            return os;
        }

        bool firstTerm = true;
        while (t != nullptr) {
            if (t->expon != -1) {
                if (!firstTerm) {
                    os << (t->coef > 0 ? " + " : " - ");
                } else {
                    if (t->coef < 0) {
                        os << "-";
                    }
                    firstTerm = false;
                }

                int absCoef = std::abs(t->coef);
                if (absCoef != 1 || t->expon == 0) {
                    os << absCoef;
                }

                if (t->expon != 0) {
                    os << "x";
                    if (t->expon != 1) {
                        os << "^" << t->expon;
                    }
                }
            }
            t = iter.Next();
        }
        return os;
    }
    
    friend Polynomial operator+(const Polynomial& a, const Polynomial& b) {
        CircListIterator<Term> Aiter(a.poly);
        CircListIterator<Term> Biter(b.poly);
        Polynomial c;
        Term *p = Aiter.First();
        Term *q = Biter.First();
        Term temp;

        while (p != nullptr || q != nullptr) {
            if (p == nullptr) {
                temp.Init(q->coef, q->expon);
                c.poly.Attach(temp);
                q = Biter.Next();
            } else if (q == nullptr) {
                temp.Init(p->coef, p->expon);
                c.poly.Attach(temp);
                p = Aiter.Next();
            } else if (p->expon == q->expon) {
                int sum = p->coef + q->coef;
                if (sum != 0) {
                    temp.Init(sum, p->expon);
                    c.poly.Attach(temp);
                }
                p = Aiter.Next();
                q = Biter.Next();
            } else if (p->expon > q->expon) {
                std::cout << "Attaching p: coef=" << p->coef << ", expon=" << p->expon << std::endl;
                temp.Init(p->coef, p->expon);
                c.poly.Attach(temp);
                p = Aiter.Next();
            } else {
                std::cout << "Attaching q: coef=" << q->coef << ", expon=" << q->expon << std::endl;
                temp.Init(q->coef, q->expon);
                c.poly.Attach(temp);
                q = Biter.Next();
            }
        }
        return c;
    }

    
    friend Polynomial operator-(const Polynomial& a, const Polynomial& b) {
        Term *p, *q;
        Term temp;
        CircListIterator<Term> Aiter(a.poly);
        CircListIterator<Term> Biter(b.poly);
        Polynomial c;

        p = Aiter.First();
        q = Biter.First();

        while (true) {
            if (p->expon == q->expon) {
                if (p->expon == -1) return c; // End of both polynomials
                int diff = p->coef - q->coef;
                if (diff != 0) {
                    temp.Init(diff, p->expon);
                    c.poly.Attach(temp);
                }
                p = Aiter.Next();
                q = Biter.Next();
            }
            else if (p->expon > q->expon) {
                temp.Init(p->coef, p->expon);
                c.poly.Attach(temp);
                p = Aiter.Next();
            }
            else { // p->expon < q->expon
                temp.Init(-q->coef, q->expon); // Note the negation here
                c.poly.Attach(temp);
                q = Biter.Next();
            }

            // Check for end of either polynomial
            if (p == nullptr || q == nullptr) break;
        }

        // If there are remaining terms in polynomial a
        while (p != nullptr && p->expon != -1) {
            temp.Init(p->coef, p->expon);
            c.poly.Attach(temp);
            p = Aiter.Next();
        }

        // If there are remaining terms in polynomial b
        while (q != nullptr && q->expon != -1) {
            temp.Init(-q->coef, q->expon); // Note the negation here
            c.poly.Attach(temp);
            q = Biter.Next();
        }
        return c;
    }
    
    friend Polynomial operator*(const Polynomial& a, const Polynomial& b) {
        Polynomial result;
        CircListIterator<Term> aIter(a.poly);
    
        for (Term* aTerm = aIter.First(); aTerm != nullptr && aTerm->expon != -1; aTerm = aIter.Next()) {
            CircListIterator<Term> bIter(b.poly);
        
            for (Term* bTerm = bIter.First(); bTerm != nullptr && bTerm->expon != -1; bTerm = bIter.Next()) {
                int newCoef = aTerm->coef * bTerm->coef;
                int newExpon = aTerm->expon + bTerm->expon;
            
                if (newCoef != 0) {
                    CircListIterator<Term> resultIter(result.poly);
                    Term* current = resultIter.First();
                    ListNode<Term>* prevNode = nullptr;

                    while (current != nullptr && current->expon >= newExpon) {
                        if (current->expon == newExpon) {
                            // Combine like terms
                            current->coef += newCoef;
                            if (current->coef == 0) {
                                result.poly.Delete(resultIter.CurrentPosition());
                            }
                            break;
                        }
                        prevNode = resultIter.CurrentPosition();
                        current = resultIter.Next();
                    }

                    if (current == nullptr || current->expon != newExpon) {
                        // Insert new term
                        Term temp;
                        temp.Init(newCoef, newExpon);
                        result.poly.Insert(prevNode, temp);
                    }
                }
            }
        }
        return result;
    }
    
private:
    CircList<Term> poly;

public:
    Polynomial() : poly() {}; //default constructor
    
    //copy constructor
    Polynomial(const Polynomial& other) {
        CircListIterator<Term> iter(other.poly);
        const Term* t = iter.First();
    
        while (t != nullptr) {
            if (t->expon != -1) {  // Skip the head node if it exists
                poly.Attach(*t);
            }
            t = iter.Next();
        }
    }
    
    const Polynomial& operator=(const Polynomial& a) {
        if (this != &a) {  // Check for self-assignment
            // Clear the existing polynomial
            while (poly.getFirst() != nullptr) {
                poly.Delete(poly.getFirst());
            }

            // Copy the polynomial 'a'
            CircListIterator<Term> aIter(a.poly);
            const Term* aTerm = aIter.First();
            while (aTerm != nullptr) {
                poly.Attach(*aTerm);
                aTerm = aIter.Next();
            }
        }
        return *this;
    }
    
    ~Polynomial() {
        while (poly.getFirst() != nullptr) {
            poly.Delete(poly.getFirst());
        }
    }
    
    float Evaluate(float x) {
        float result = 0.0;
        CircListIterator<Term> iter(poly);
        const Term* t = iter.First();
        while (t != nullptr) {
            result += t->coef * std::pow(x, t->expon);
            t = iter.Next();
        }
        return result;
    }
};

#endif // POLYNOMIAL_H
