#include <CtrlLib/CtrlLib.h>

using namespace Upp;

GUI_APP_MAIN
{
	String text = "Original text";
	if(EditText(text, "Please edit a text", "Text"))
		PromptOK(text);

	int number = 10;
	if(EditNumber(number, "Please enter a number between 1 and 100", "Number", 1, 100, true))
		PromptOK(AsString(number));
	
	Date dt = GetSysDate();
	if(EditDateDlg(dt, "Please enter a date", "Date"))
		PromptOK(AsString(dt));
}

