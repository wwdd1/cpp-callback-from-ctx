#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <exception>
#include <atomic>
#include <typeinfo>

#define CALLBACK_0(__fn__,__ctx__) (std::bind(__fn__,__ctx__))
#define CALLBACK_1(__fn__,__ctx__) (std::bind(__fn__,__ctx__,std::placeholders::_1))

using namespace std;

class Bye {
public:
	Bye(string s) { str = s; };
	void whaccadoin(std::string text) { cout << "Text : " + text << "," + str << " from callback." << endl; }
private:
	string str;
};

class Hellow {
public:
	Hellow() {};

	template < typename Tp >
	void addFieldToRequestData(const char *key, Tp *value) {
		if (typeid(const char) == typeid(Tp))
			cout << key << " : " << value << endl;
		else
			cout << key << " : " << *value << endl;
	}

	void setCallback(function<void(std::string)> fn) {
		callback_ = fn;
	}
	
	void poke() {
		done_.store(false);

		_thread = new thread([this] {
			cout << "giggle giggle" << endl;
			std::this_thread::sleep_for(2s);
			std::string x = "whassap";
			if (callback_) callback(x);
			done_.store(true);
			//_thread->join();
		});
	}

	bool isDone() {
		return done_.load();
	}

	void IsThreadNull() {
		if (_thread == NULL)
			cout << "thread obj is null" << endl;
		else
			cout << "thread obj is not null" << endl;
	}
	thread *_thread;
private:
	function<void(std::string)> callback_;
	atomic<bool> done_ = false;
};

int main() {
	
	shared_ptr<Bye> bye_one = make_shared<Bye>("Foo");
	shared_ptr<Bye> bye_two = make_shared<Bye>("Bar");
	unique_ptr<Hellow> hellow = make_unique<Hellow>();

	hellow->setCallback(CALLBACK_1(&Bye::whaccadoin, bye_one));
	hellow->poke();
	std::this_thread::sleep_for(4s);
	if (hellow->isDone()) {
		cout << "done" << endl;
		//delete hellow->_thread;
		hellow->_thread = nullptr;
		hellow->IsThreadNull();
	}
	else {
		cout << "running" << endl;
		hellow->IsThreadNull();
	}
	
	hellow->addFieldToRequestData< const char >( "whassup", "ma'man" );
	int x = 5;
	hellow->addFieldToRequestData< int >("whassup", &x);

	return 0;
}