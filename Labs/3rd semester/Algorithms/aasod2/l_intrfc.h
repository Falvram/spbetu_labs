#include <strstream>
#define N 1000
using namespace std;
namespace h_list
{
    typedef char base;

	struct s_expr;
	struct  two_ptr
		{
		s_expr *hd;
		s_expr *tl;
    };

	struct s_expr {
		bool tag; // true: atom, false: pair
		union{
			base atom;
			two_ptr pair;	
        } node;
    };
	
	typedef s_expr *lisp;

    bool rep(lisp s, base x, base y);
	lisp head (const lisp s);
	lisp tail (const lisp s);
	lisp cons (const lisp h, const lisp t);
	lisp make_atom (const base x);
	bool isAtom (const lisp s);
	bool isNull (const lisp s);
	void destroy (lisp s);
	
	base getAtom (const lisp s);

    void read_lisp ( lisp& y, strstream &st);
    void read_s_expr (base prev, lisp& y, strstream &st);
    void read_seq ( lisp& y, strstream &st);
	
    void write_lisp (const lisp x);
    void write_seq (const lisp x);
    void Error(int a);

}
