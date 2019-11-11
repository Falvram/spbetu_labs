#include "st_interf1.h"
#include "error.h"

using namespace std ;
namespace st_modul1
{
    struct Stack::node {
	base *hd;
	node *tl;
		node (){
            hd = nullptr;
            tl = nullptr;
		}
    };

    base Stack::pop(void)
    {
        node *oldTop = topOfStack;
        base r = *topOfStack->hd;
        topOfStack = topOfStack->tl;
        delete oldTop->hd;
        delete oldTop;
        return r;

	}

	void Stack::push (const base &x)
	{	node *p;
		p = topOfStack;
        topOfStack = new node;
        topOfStack->hd = new base;
        *topOfStack->hd = x;
        topOfStack->tl = p;
	}

	bool Stack::isNull(void)
    {	return (topOfStack == nullptr) ;
	}

	void Stack::destroy (void)
    {	while ( topOfStack != nullptr) {
			pop();	
		}
	}
}
