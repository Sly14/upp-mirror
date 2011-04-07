#ifndef _PyCon_PyCon_h_
#define _PyCon_PyCon_h_

#include <Py/Py.h>

#include <Core/Core.h>
using namespace Upp;

#include <LogCtrl/LogCtrl.h>

class PyCon
{
public:
	static PyObject* ToStdOut(PyObject* self, PyObject* pArgs);
	static PyObject* ToStdErr(PyObject* self, PyObject* pArgs);

	static void Init();
	static void Enable(bool b = true);
	static void Disable() { Enable(false); }
	static bool IsEnabled() { return enabled; }

	static void SetStream(Stream& _ps) { ps = &_ps; }
	static Stream& GetStream() { return (ps)?(*ps):(StdLog()); }

	static PyMethodDef m[];

protected:
	static bool enabled;
	static Stream* ps;
};




#endif
