#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <iterator> 
#include <algorithm> 
#include <queue> 
#include <memory>
using namespace std;

struct person {
	string surname, name;
	short age;
	size_t size() const
	{
		return (surname.capacity()+name.capacity()+ sizeof(age)+2*sizeof(" "));
	}
};

bool operator <(const person& s1, const person& s2)
{
	return (s1.name < s2.name);
}

bool operator >(const person& s1, const person& s2)
{
	return (s1.name > s2.name);
}

istream & operator>>(istream & in, person & s)
{
	in >> s.surname >> s.name >> s.age;
	return in;
}
ostream & operator<<(ostream & out, person const & s)
{
	out << s.surname << " " << s.name << " "  << s.age<<"\n";
	return out;
}



struct S {
public:
	ifstream *f;
	person s;
	S(const person& s_, ifstream* f_) : s(s_), f(f_) {}
};

bool operator < (const S& s1, const S& s2)
{
	return (s1.s > s2.s);
}


class Sort {
public:
	Sort (string name_main_file, string out_file, size_t buff_size);
	auto division()->void;
	auto make_file(string name_file)->void;
	auto file_sort()->void;
	~Sort();
private:
	string s_out, s_in;
	size_t count_of_files;
	vector<string> file_names;
	vector<person> pers;
	//uint_fast64_t buffer;
};



inline Sort::~Sort() 
{ 
	//file_names.clear();
}

inline Sort::Sort(string name_main_file, string out_file, size_t buff_size) :s_in(name_main_file), s_out(out_file), count_of_files(0), buffer(buff_size * 1024 * 1024*0.35) {
	pers.reserve(buffer);
	file_names.reserve(512);
	division();
};

inline auto Sort::make_file(string name_file)->void {
	file_names.push_back(name_file);
	std::sort(pers.begin(), pers.end());
	ofstream temp(name_file, ios::binary);
	for (auto i : pers) if (i.surname != "") temp << i ;
	temp.close();
	pers.clear();
}





inline auto Sort::file_sort()->void {
	priority_queue<S> end_sorting;

	for (int i = 0; i < count_of_files; ++i) {
		ifstream* f_ = new ifstream(file_names[i], ios::binary);
		person temp_s;
		*f_ >> temp_s;
		S ff(temp_s, f_);
		end_sorting.push(ff);
	}

	ofstream f12(s_out, ios::binary);
	while (!end_sorting.empty()) {
		S ff = end_sorting.top();
		end_sorting.pop();
		if (ff.s.surname != "") f12 << ff.s << endl;

		if (!(*ff.f).eof())
		{
			*ff.f >> ff.s;
			end_sorting.push(ff);
		}
		else {
			(*(ff.f)).close();
		}
	}
	f12.close();


	for (int i = 0; i < file_names.size(); ++i) {
		remove(file_names[i].c_str());
	}

}



inline auto Sort::division()->void {
size_t i(0);
person hmn;
ifstream file(s_in, ios::binary);
while (!file.eof()) 
{
file >> hmn;
i += hmn.size();
		if (i<buffer) 
		{
			pers.push_back(hmn);
		}
		else {
			count_of_files++;
			make_file(to_string(count_of_files));
			pers.push_back(hmn);
			i = hmn.size();
		}
	}
	file.close();
	if (!pers.empty()) {
		count_of_files++;
		make_file(to_string(count_of_files));
	}
	file_sort();
}
