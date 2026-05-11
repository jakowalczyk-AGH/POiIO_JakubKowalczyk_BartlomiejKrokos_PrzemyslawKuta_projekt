#include "MainWin.h"
#include "TGra.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]

int main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Familiada::MainWin form;
	Application::Run(% form);
	return 0;

}