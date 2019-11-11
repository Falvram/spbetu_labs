#include <iostream>
#include <strstream>
#include "l_intrfc.h"
using namespace std;
namespace h_list
{
    bool rep(lisp s, base x, base y){
        if(isNull(s)) return false;
        if(isAtom(s)){
            if(getAtom(s) == x){  s->node.atom = y; return true; }
            return false;
        }
        else return  rep(head(s), x, y) + rep(tail(s), x, y);
    }

    lisp head (const lisp s)
    {
        return s->node.pair.hd;
	}

	bool isAtom (const lisp s)
	{	if(s == NULL) return false;
        else return (s -> tag);
        }
        bool isNull (const lisp s)
        { return s==NULL;
	}

    lisp tail (const lisp s)
    {
        return s->node.pair.tl;
    }

    base getAtom (const lisp s)
    {
        return s->node.atom;
    }

    lisp cons (const lisp h, const lisp t)
    {lisp p;

        p = new s_expr;
        p->tag = false;
        p->node.pair.hd = h;
        p->node.pair.tl = t;
        return p;

    }

	lisp make_atom (const base x)
	{	lisp s;
		s = new s_expr;
		s -> tag = true;
		s->node.atom = x;
		return s;
	}

	void destroy (lisp s) 
	{
        if ( s != NULL) {
            if (!isAtom(s)) {
                destroy ( head (s));
                destroy ( tail(s));
            }
            delete s;
        };
	}

    void read_lisp ( lisp& y, strstream &st)
    {	base x = 0;
        if(!(st >> x)) throw 3;
        while (x==' '){ st >> x; }
        if(x)
            read_s_expr (x, y, st);
    }

    void read_s_expr (base prev, lisp& y, strstream &st)
    {
        if(prev == ')') throw 1;
        if ( prev != '(' ) y = make_atom (prev);
        else read_seq (y, st);
    }

    void read_seq ( lisp& y, strstream &st)
	{	base x; 
		lisp p1, p2;

            if(!(st >> x)) throw 2;
            while  ( x==' ' ){ st >> x; }
			if ( x == ')' ) y = NULL;
			else {
                read_s_expr ( x, p1, st);
                read_seq ( p2, st);
				y = cons (p1, p2);
			} 
    }


	void write_lisp (const lisp x)
    {
        if (isAtom(x)) cout << ' ' << x->node.atom;
        else{
			cout << " (" ;
			write_seq(x);
			cout << " )";
		}
    }

    void write_seq (const lisp x)
    {
        if (!isNull(x)) {
            write_lisp(head (x));
            write_seq(tail (x));
        }
    }

    void Error(int a){
        switch (a) {
            case 1:{ cout << "no expression before ')'" << endl; break; }
            case 2:{ cout << "no ')'" << endl; break; }
            case 3:{ cout << "empty expression" << endl; break; }
        }
    }
}
