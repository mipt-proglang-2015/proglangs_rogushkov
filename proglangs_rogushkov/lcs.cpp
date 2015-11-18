#include <vector>
#include <map>

extern "C" {
#include <Python.h>
}

namespace lcstring {
	struct state {
		int len, link;
		std::map<char, int> next;
	};
	const int MAXLEN = 100000;
	state st[MAXLEN * 2];
	int sz, last;

	void sa_init() {
		sz = last = 0;
		st[0].len = 0;
		st[0].link = -1;
		++sz;
	}
	struct param {
		std::string T;
		std::string S;
	};

	void sa_extend(char c) {
		int cur = sz++;
		st[cur].len = st[last].len + 1;
		int p;
		for (p = last; p != -1 && !st[p].next.count(c); p = st[p].link)
			st[p].next[c] = cur;
		if (p == -1)
			st[cur].link = 0;
		else {
			int q = st[p].next[c];
			if (st[p].len + 1 == st[q].len)
				st[cur].link = q;
			else {
				int clone = sz++;
				st[clone].len = st[p].len + 1;
				st[clone].next = st[q].next;
				st[clone].link = st[q].link;
				for (; p != -1 && st[p].next[c] == q; p = st[p].link)
					st[p].next[c] = clone;
				st[q].link = st[cur].link = clone;
			}
		}
		last = cur;
	}

	std::string lcs(std::string s, std::string t) {
		sa_init();
		for (int i = 0; i < s.length(); ++i)
			sa_extend(s[i]);

		int v = 0, l = 0,
			best = 0, bestpos = 0;
		for (int i = 0; i < t.length(); ++i) {
			while (v && !st[v].next.count(t[i])) {
				v = st[v].link;
				l = st[v].len;
			}
			if (st[v].next.count(t[i])) {
				v = st[v].next[t[i]];
				++l;
			}
			if (l > best)
				best = l, bestpos = i;
		}
		
		return t.substr(bestpos - best + 1, best);
	}
}

namespace lcsWithOutSTL {
	struct state {
		int len, link;
		std::vector<std::pair<char, int> > next;
	};
	struct param {
		int sizeS, sizeT;
		char* T;
		char* S;
	};
	struct answer {
		char* ans;
		int size;
	};
	const int MAXLEN = 100000;
	state st[MAXLEN * 2];
	int sz, last;

	void sa_init() {
		sz = last = 0;
		st[0].len = 0;
		st[0].link = -1;
		++sz;
	}

	bool count(std::vector<std::pair<char, int> > map, char c) {
		for (int i = 0; i < map.size(); i++) {
			if(map[i].first == c) {
				return true;
			}
		}
		return false;
	}

	int find(std::vector<std::pair<char, int> > map, char c) {
		for (int i = 0; i < map.size(); i++) {
			if (map[i].first == c) {
				return i;
			}
		}
		return -1;
	}

	void sa_extend(char c) {
		int cur = sz++;
		st[cur].len = st[last].len + 1;
		int p;
		for (p = last; p != -1 && !count(st[p].next, c); p = st[p].link)
			st[p].next.push_back(std::make_pair(c, cur));
		if (p == -1)
			st[cur].link = 0;
		else {
			int q = find(st[p].next,c);
			if (st[p].len + 1 == st[q].len)
				st[cur].link = q;
			else {
				int clone = sz++;
				st[clone].len = st[p].len + 1;
				st[clone].next = st[q].next;
				st[clone].link = st[q].link;
				for (; p != -1 && find(st[p].next,c) == q; p = st[p].link)
					st[p].next.push_back(std::make_pair(c, clone) );
				st[q].link = st[cur].link = clone;
			}
		}
		last = cur;
	}

	answer lcs(param args) {
		sa_init();
		for (int i = 0; i < args.sizeS; ++i)
			sa_extend(args.S[i]);

		int v = 0, l = 0,
			best = 0, bestpos = 0;
		for (int i = 0; i < args.sizeT; ++i) {
			while (v && !count(st[v].next,args.T[i])) {
				v = st[v].link;
				l = st[v].len;
			}
			int temp= find(st[v].next, args.T[i]);
			if (temp != -1) {
				v = st[v].next[temp].second;
				++l;
			}
			if (l > best)
				best = l, bestpos = i;
		}
		char* sub = new char[best + 1];
		memset(sub, args.T[bestpos - best + 1], best);
		sub[best] = '\0';
		answer answers;
		answers.ans = sub;
		answers.size = best;
		return answers;
	}
}
// STL part
static lcstring::param pyobject_to_cxxSTL(PyObject * py_str)
{
	lcstring::param result;
	PyObject * py_first = PyList_GetItem(py_str, 0);
	PyObject * py_second = PyList_GetItem(py_str, 1);
	result.S = PyObject_Length(py_first);
	result.T = PyObject_Length(py_second);

	return result;
}

static PyObject * cxx_to_pyobjectSTL(const std::string ans)
{
	PyObject * result = PyBytes_FromString(ans.c_str());
	
	return result;
}

static PyObject* funcSTL(PyObject* mod, PyObject* args)
{
	PyObject* source_py = PyTuple_GetItem(args, 0);

	lcstring::param arg = pyobject_to_cxxSTL(source_py);

	std::string result = lcstring::lcs(arg.S, arg.T);

	PyObject* py_result = cxx_to_pyobjectSTL(result);
	return py_result;
}

//without map and string 
static lcsWithOutSTL::param pyobject_to_cxx(PyObject * py_str)
{
	lcsWithOutSTL::param result;
	PyObject * py_first = PyList_GetItem(py_str, 0);
	PyObject * py_second = PyList_GetItem(py_str, 1);
	result.sizeS = PyObject_Length(py_first);
	result.sizeT = PyObject_Length(py_second);
	result.S = PyBytes_AsString(PyUnicode_AsUTF8String(py_first));
	result.T = PyBytes_AsString(PyUnicode_AsUTF8String(py_second));
	//char* s;
	//PyArg_Parse(py_first, "s", &s)
	return result;
}

static PyObject * cxx_to_pyobject(const lcsWithOutSTL::answer ans)
{
	PyObject * result = PyBytes_FromStringAndSize(ans.ans, ans.size);
	return result;
}

static PyObject* func(PyObject* mod, PyObject* args)
{
	PyObject* source_py = PyTuple_GetItem(args, 0);

	lcsWithOutSTL::param arg = pyobject_to_cxx(source_py);

	lcsWithOutSTL::answer result = lcsWithOutSTL::lcs(arg);

	PyObject* py_result = cxx_to_pyobject(result);
	return py_result;
}


PyMODINIT_FUNC PyInit_lcs()
{
	static PyMethodDef ModuleMethods[] = {
		{ "lcs", func, METH_VARARGS, "lcs" },
		{ "lcs_map", funcSTL, METH_VARARGS, "lcs using map deprecated" },
		{ NULL, NULL, 0, NULL }
	};
	static PyModuleDef ModuleDef = {
		PyModuleDef_HEAD_INIT,
		"lcs",
		"My first python module",
		-1, ModuleMethods,
		NULL, NULL, NULL, NULL
	};
	PyObject * module = PyModule_Create(&ModuleDef);
	return module;
}