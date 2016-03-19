#include <iostream>
#include <string>
#include "bson.h"
#include "bsonobjbuilder.h"

using namespace std;

void go1() { 
    // example of the use of fromjson
//     mongo::BSONObjBuilder b;
//     stringstream s("{x:3,y:\"hello world\"}");
//     mongo::BSONObj o = mongo::fromjson(s, b);
//     cout << o.toString() << endl;
}
void go3( mongo::BSONObj & o)
{ 
	// example of using bsonobjbuilder and of nesting documents
	mongo::BSONObjBuilder  b;
	b.append("x", 3.14);
	b.append("y", true);
	mongo::BSONObjBuilder  a;
	a.append("x", 2);
	a.append("yy", b.obj());

	o = a.obj();
}

void go2() { 
   

	mongo::BSONObj  o;
	go3(o);
	cout << o.toString() << endl;
	std::string str(o.objdata() , o.objsize());
    // reading value of the "x" field from the bson object 
    cout << "x: " << o["x"].Int() << endl;

    // "dot notation" example
    cout << "yy.y: " << o.getFieldDotted("yy.y").Bool() << endl;
}

void go() { 
    go1();
    go2();
}

int main(int argc, char* argv[])
{ 
    try {
        go();
    }
    catch (std::exception& e) {
        cerr << "exception ";
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
	system("pause");
    return 0;
}

